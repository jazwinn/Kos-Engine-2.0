// font.cpp
// Implementation of FontAtlas: uses FreeType to rasterize glyphs, pack them into an atlas,
// export a PNG (via stb_image_write), and write/read a simple binary format.
//
// Binary format (FNTC):
//  - 4 bytes: magic 'F','N','T','C'
//  - uint32_t version (1)
//  - uint32_t atlasWidth
//  - uint32_t atlasHeight
//  - uint32_t pngDataSize
//  - <pngDataSize> bytes: PNG encoded atlas (RGBA)
//  - uint32_t numChars
//  - for each char:
//      uint8_t charCode
//      int32_t sizeX, sizeY
//      int32_t bearingX, bearingY
//      uint32_t advance
//      float u0, v0, u1, v1
//
// Notes:
//  - Glyph bitmaps are rasterized as 8-bit gray; stored into atlas alpha channel. RGB left white (255,255,255).
//  - Advance is stored as pixel advance (unsigned int).
//
// Dependencies: FreeType2, stb_image_write (included below), glm (for ivec2/vec2).
//


#include "../Include/freetype/ft2build.h"
#include "../Include/freetype/freetype.h"
#include FT_FREETYPE_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <cassert>
#include <sstream>

// stb_image_write (single header) - used to write atlas PNG for inspection/extraction
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "FontCompiler.h"



// png encoding helper: we use stb_image_write's memory write to produce PNG bytes in memory.
static bool EncodePNGToMemory(const unsigned char* rgba, int w, int h, std::vector<unsigned char>& outPng)
{
    struct MemWriter {
        std::vector<unsigned char>* out;
        static void write(void* context, void* data, int size) {
            auto o = reinterpret_cast<std::vector<unsigned char>*>(context);
            unsigned char* d = (unsigned char*)data;
            o->insert(o->end(), d, d + size);
        }
    };

    outPng.clear();
    int stride = w * 4;
    // Use stb_image_write to write to memory
    stbi_write_func* func = [](void* context, void* data, int size) {
        MemWriter::write(context, data, size);
        };
    // Unfortunately stb_image_write provides a wrapper stbi_write_png_to_func
    stbi_write_png_to_func(func, &outPng, w, h, 4, rgba, stride);
    return !outPng.empty();
}

namespace text {

    FontAtlas::FontAtlas() {}
    FontAtlas::~FontAtlas() {}

    bool FontAtlas::GenerateFromTTF(const std::string& ttfPath, int pixelHeight, unsigned char firstChar, unsigned char lastChar)
    {
        m_characters.clear();
        m_atlasRGBA.clear();
        m_atlasWidth = m_atlasHeight = 0;

        FT_Library ft;
        if (FT_Init_FreeType(&ft)) {
            std::cerr << "ERROR::FREETYPE: Could not init FreeType Library\n";
            return false;
        }
        FT_Face face;
        if (FT_New_Face(ft, ttfPath.c_str(), 0, &face)) {
            std::cerr << "ERROR::FREETYPE: Failed to load font: " << ttfPath << "\n";
            FT_Done_FreeType(ft);
            return false;
        }

        FT_Set_Pixel_Sizes(face, 0, pixelHeight);

        // Rasterize each glyph into a temporary bitmap
        std::vector<GlyphBitmap> glyphs;
        glyphs.reserve(lastChar - firstChar + 1);

        int maxRowHeight = 0;
        int totalWidth = 0;
        for (unsigned char c = firstChar; c <= lastChar; ++c) {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                std::cerr << "WARN: Failed to load char " << int(c) << "\n";
                continue;
            }
            FT_GlyphSlot g = face->glyph;
            GlyphBitmap gb;
            gb.w = g->bitmap.width;
            gb.h = g->bitmap.rows;
            gb.pixels.assign(gb.w * gb.h, 0);
            if (gb.w && gb.h) {
                for (int row = 0; row < gb.h; ++row) {
                    for (int col = 0; col < gb.w; ++col) {
                        gb.pixels[row * gb.w + col] = g->bitmap.buffer[row * g->bitmap.pitch + col];
                    }
                }
            }
            gb.bearingX = g->bitmap_left;
            gb.bearingY = g->bitmap_top;
            // convert advance from 26.6 fixed point to pixels (rounded)
            gb.advance = static_cast<unsigned int>((g->advance.x + 32) >> 6);
            gb.ch = static_cast<char>(c);
            glyphs.push_back(std::move(gb));
            maxRowHeight = std::max(maxRowHeight, glyphs.back().h);
            totalWidth += glyphs.back().w;
        }

        // Simple shelf packing: place glyphs in rows, each row as wide as needed, then compute needed atlas width/height.
        // We set an atlas width limit (try to make almost square).
        const int maxAtlasWidth = 2048; // change as desired
        int curX = 0, curY = 0, rowHeight = 0;
        int atlasW = 0, atlasH = 0;
        struct Placed { char ch; int x, y, w, h; int bearingX, bearingY; unsigned int advance; };
        std::vector<Placed> placed;

        for (auto& g : glyphs) {
            if (curX + g.w > maxAtlasWidth) {
                curX = 0;
                curY += rowHeight;
                rowHeight = 0;
            }
            Placed p;
            p.ch = g.ch;
            p.x = curX;
            p.y = curY;
            p.w = g.w;
            p.h = g.h;
            p.bearingX = g.bearingX;
            p.bearingY = g.bearingY;
            p.advance = g.advance;
            placed.push_back(p);
            curX += g.w;
            rowHeight = std::max(rowHeight, g.h);
            atlasW = std::max(atlasW, curX);
            atlasH = std::max(atlasH, curY + rowHeight);
        }
        if (atlasW == 0) atlasW = 1;
        if (atlasH == 0) atlasH = 1;

        // Expand to next power-of-two optionally (not necessary), but convenient
        auto nextPow2 = [](int v) { int p = 1; while (p < v) p <<= 1; return p; };
        m_atlasWidth = nextPow2(atlasW);
        m_atlasHeight = nextPow2(atlasH);

        // allocate RGBA atlas and fill with white + zero alpha
        m_atlasRGBA.assign(m_atlasWidth * m_atlasHeight * 4, 255);
        for (size_t i = 0; i < m_atlasRGBA.size(); i += 4) {
            m_atlasRGBA[i + 0] = 255; // R
            m_atlasRGBA[i + 1] = 255; // G
            m_atlasRGBA[i + 2] = 255; // B
            m_atlasRGBA[i + 3] = 0;   // A initially 0
        }

        // copy glyph pixels into atlas' alpha
        for (size_t i = 0; i < placed.size(); ++i) {
            auto& p = placed[i];
            auto& g = glyphs[i];
            for (int row = 0; row < p.h; ++row) {
                for (int col = 0; col < p.w; ++col) {
                    int ax = p.x + col;
                    int ay = p.y + row;
                    if (ax < 0 || ay < 0 || ax >= m_atlasWidth || ay >= m_atlasHeight) continue;
                    unsigned char gray = 0;
                    if (col < g.w && row < g.h) gray = g.pixels[row * g.w + col];
                    size_t idx = (ay * m_atlasWidth + ax) * 4;
                    m_atlasRGBA[idx + 3] = gray; // alpha
                    // keep RGB as white (already 255)
                }
            }
            // compute uvs
            text::CharacterData cd;
            cd.m_textureID = 0;
            cd.m_size = glm::ivec2(p.w, p.h);
            cd.m_bearing = glm::ivec2(p.bearingX, p.bearingY);
            cd.m_advance = p.advance;
            cd.m_topLeftTexCoords = glm::vec2(static_cast<float>(p.x) / m_atlasWidth, static_cast<float>(p.y) / m_atlasHeight);
            cd.m_bottomRightTexCoords = glm::vec2(static_cast<float>(p.x + p.w) / m_atlasWidth, static_cast<float>(p.y + p.h) / m_atlasHeight);
            m_characters[p.ch] = cd;
        }

        FT_Done_Face(face);
        FT_Done_FreeType(ft);
        return true;
    }

    bool FontAtlas::ExportAtlasPNG(const std::string& pngPath) const
    {
        if (m_atlasRGBA.empty()) return false;
        // stb_image_write expects row-major RGBA bytes
        int w = m_atlasWidth, h = m_atlasHeight;
        if (stbi_write_png(pngPath.c_str(), w, h, 4, m_atlasRGBA.data(), w * 4) == 0) {
            std::cerr << "Failed to write PNG: " << pngPath << "\n";
            return false;
        }
        return true;
    }

    bool FontAtlas::SaveToFile(const std::string& outPath) const
    {
        // encode atlas to PNG bytes in memory
        std::vector<unsigned char> pngBytes;
        if (!EncodePNGToMemory(m_atlasRGBA.empty() ? nullptr : m_atlasRGBA.data(), m_atlasWidth, m_atlasHeight, pngBytes)) {
            std::cerr << "ERROR: failed to encode atlas PNG to memory\n";
            return false;
        }

        std::ofstream ofs(outPath, std::ios::binary);
        if (!ofs) {
            std::cerr << "ERROR: cannot open output file: " << outPath << "\n";
            return false;
        }

        // header
        ofs.write("FNTC", 4);
        uint32_t version = 1;
        auto write_u32 = [&](uint32_t v) { ofs.write(reinterpret_cast<const char*>(&v), sizeof(v)); };
        auto write_i32 = [&](int32_t v) { ofs.write(reinterpret_cast<const char*>(&v), sizeof(v)); };
        auto write_u8 = [&](uint8_t v) { ofs.write(reinterpret_cast<const char*>(&v), sizeof(v)); };
        auto write_f = [&](float f) { ofs.write(reinterpret_cast<const char*>(&f), sizeof(f)); };

        write_u32(version);
        write_u32(static_cast<uint32_t>(m_atlasWidth));
        write_u32(static_cast<uint32_t>(m_atlasHeight));
        write_u32(static_cast<uint32_t>(pngBytes.size()));
        if (!pngBytes.empty()) ofs.write(reinterpret_cast<const char*>(pngBytes.data()), pngBytes.size());

        // write number of characters
        uint32_t numChars = static_cast<uint32_t>(m_characters.size());
        write_u32(numChars);

        for (auto& kv : m_characters) {
            char ch = kv.first;
            const CharacterData& cd = kv.second;
            write_u8(static_cast<uint8_t>(ch));
            write_i32(cd.m_size.x);
            write_i32(cd.m_size.y);
            write_i32(cd.m_bearing.x);
            write_i32(cd.m_bearing.y);
            write_u32(cd.m_advance);
            write_f(cd.m_topLeftTexCoords.x);
            write_f(cd.m_topLeftTexCoords.y);
            write_f(cd.m_bottomRightTexCoords.x);
            write_f(cd.m_bottomRightTexCoords.y);
        }

        ofs.close();
        return true;
    }

    bool FontAtlas::LoadFromFile(const std::string& inPath)
    {
        m_atlasRGBA.clear();
        m_characters.clear();
        m_atlasWidth = m_atlasHeight = 0;

        std::ifstream ifs(inPath, std::ios::binary);
        if (!ifs) {
            std::cerr << "ERROR: cannot open file: " << inPath << "\n";
            return false;
        }

        char magic[4];
        ifs.read(magic, 4);
        if (ifs.gcount() != 4 || std::string(magic, 4) != "FNTC") {
            std::cerr << "ERROR: bad magic or not a FNTC file\n";
            return false;
        }

        uint32_t version = 0;
        auto read_u32 = [&]()->uint32_t { uint32_t v; ifs.read(reinterpret_cast<char*>(&v), sizeof(v)); return v; };
        auto read_i32 = [&]()->int32_t { int32_t v; ifs.read(reinterpret_cast<char*>(&v), sizeof(v)); return v; };
        auto read_u8 = [&]()->uint8_t { uint8_t v; ifs.read(reinterpret_cast<char*>(&v), sizeof(v)); return v; };
        auto read_f = [&]()->float { float v; ifs.read(reinterpret_cast<char*>(&v), sizeof(v)); return v; };

        version = read_u32();
        if (version != 1) {
            std::cerr << "WARNING: unknown version: " << version << " (expected 1)\n";
        }
        m_atlasWidth = static_cast<int>(read_u32());
        m_atlasHeight = static_cast<int>(read_u32());
        uint32_t pngSize = read_u32();
        std::vector<unsigned char> pngBytes;
        if (pngSize) {
            pngBytes.resize(pngSize);
            ifs.read(reinterpret_cast<char*>(pngBytes.data()), pngSize);
        }

        // decode PNG bytes into m_atlasRGBA
        if (!pngBytes.empty()) {
            // we use stb_image to decode
            int w, h, comp;
            unsigned char* img = stbi_load_from_memory(pngBytes.data(), (int)pngBytes.size(), &w, &h, &comp, 4);
            if (!img) {
                std::cerr << "ERROR: failed to decode PNG in file\n";
                return false;
            }
            m_atlasWidth = w; m_atlasHeight = h;
            m_atlasRGBA.assign(img, img + (w * h * 4));
            stbi_image_free(img);
        }
        else {
            // no png embedded; nothing to fill
            m_atlasRGBA.clear();
        }

        uint32_t numChars = read_u32();
        for (uint32_t i = 0; i < numChars; ++i) {
            uint8_t ch = read_u8();
            int32_t sx = read_i32();
            int32_t sy = read_i32();
            int32_t bx = read_i32();
            int32_t by = read_i32();
            uint32_t adv = read_u32();
            float u0 = read_f();
            float v0 = read_f();
            float u1 = read_f();
            float v1 = read_f();
            CharacterData cd;
            cd.m_textureID = 0;
            cd.m_size = glm::ivec2(sx, sy);
            cd.m_bearing = glm::ivec2(bx, by);
            cd.m_advance = adv;
            cd.m_topLeftTexCoords = glm::vec2(u0, v0);
            cd.m_bottomRightTexCoords = glm::vec2(u1, v1);
            m_characters[static_cast<char>(ch)] = cd;
        }

        return true;
    }

} // namespace text

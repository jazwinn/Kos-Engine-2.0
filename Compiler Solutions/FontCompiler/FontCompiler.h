#pragma once

#include <map>
#include <string>
#include <vector>
#include <cstdint>

#ifndef GL_TYPES_INCLUDED
// If OpenGL headers are not available on the build machine, we still want to compile.
// If you have GL available, include <GL/glew.h> or <GL/gl.h> before this file.
typedef unsigned int GLuint;
#endif
#include <glm/glm.hpp>


namespace text {

    class CharacterData
    {
    public:
        GLuint m_textureID{ 0 };       ///< ID handle of the glyph texture (for runtime; set to 0 in file)
        glm::ivec2 m_size{ 0,0 };      ///< Size of the glyph (width and height)
        glm::ivec2 m_bearing{ 0,0 };   ///< Offset from baseline to left/top of glyph
        GLuint m_advance{ 0 };         ///< Offset to advance to next glyph (in 1/64th pixel in FT convention; we store pixel advance)
        glm::vec2 m_topLeftTexCoords;     ///< uv top-left (0..1)
        glm::vec2 m_bottomRightTexCoords; ///< uv bottom-right (0..1)
    };

    // convenience typedef
    using FontMap = std::map<char, CharacterData>;

    /**
     * FontAtlas
     *
     * Responsibilities:
     *  - load a TTF (FreeType) and rasterize glyphs into an atlas
     *  - write and read a simple custom binary format containing:
     *      header, atlas size, atlas PNG bytes, per-character metadata (see font.cpp)
     *
     * Use GenerateFromTTF -> SaveToFile to produce a .fntc file.
     * Use LoadFromFile to parse one back into memory.
     */
    class FontAtlas
    {
    public:
        FontAtlas();
        ~FontAtlas();

        // Generate atlas from .ttf file (pixelHeight is font size in px)
        // returns true on success
        bool GenerateFromTTF(const std::string& ttfPath, int pixelHeight, unsigned char firstChar = 32, unsigned char lastChar = 126);

        // Save to custom .fntc file (binary)
        bool SaveToFile(const std::string& outPath) const;

        // Load from custom .fntc file (binary)
        bool LoadFromFile(const std::string& inPath);

        // optionally export the atlas to a PNG (uses stb_image_write)
        bool ExportAtlasPNG(const std::string& pngPath) const;

        // Accessors
        const FontMap& GetCharacters() const { return m_characters; }
        int GetAtlasWidth() const { return m_atlasWidth; }
        int GetAtlasHeight() const { return m_atlasHeight; }
        const std::vector<unsigned char>& GetAtlasRGBA() const { return m_atlasRGBA; } // RGBA bytes

    private:
        // packing helper
        struct GlyphBitmap { int w, h; std::vector<unsigned char> pixels; int bearingX, bearingY; unsigned int advance; char ch; };

        int m_atlasWidth{ 0 };
        int m_atlasHeight{ 0 };
        std::vector<unsigned char> m_atlasRGBA; // RGBA image bytes (width*height*4)
        FontMap m_characters;
    };
} // namespace text

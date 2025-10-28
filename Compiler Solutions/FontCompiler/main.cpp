#include "FontCompiler.h"
#include <iostream>
#include <filesystem>
#include <fstream>


int main(int argc, char** argv)
{
    if (argc < 4) {
        std::cout << "FontCompiler - pack TTF into a .fntc atlas format\n";
        std::cout << "Usage:\n";
        std::cout << "  FontCompiler.exe <font.ttf> <meta.json> <output.fntc>\n";
        return 0;
    }

    std::string fontPath = argv[1];
    std::string metaPath = argv[2];
    std::string outputPath = argv[3];

    // Ensure output directory exists
    std::filesystem::path outDir = std::filesystem::path(outputPath).parent_path();
    if (!outDir.empty() && !std::filesystem::exists(outDir)) {
        if (!std::filesystem::create_directories(outDir)) {
            std::cerr << "Failed to create directories: " << outDir << "\n";
            return 1;
        }
    }

    // Check extension of input
    std::string ext;
    {
        auto p = std::filesystem::path(fontPath);
        ext = p.extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    }

    if (ext == ".ttf" || ext == ".otf") {
        int pxSize = 48; // default size
        text::FontAtlas atlas;
        std::cout << "Generating atlas from: " << fontPath << " (size " << pxSize << "px)...\n";

        if (!atlas.GenerateFromTTF(fontPath, pxSize)) {
            std::cerr << "Failed to generate atlas\n";
            return 1;
        }

        // Save binary package (.fntc)
        if (!atlas.SaveToFile(outputPath)) {
            std::cerr << "Failed to save file: " << outputPath << "\n";
            return 1;
        }
        std::cout << "Wrote font package: " << outputPath << "\n";

        //Save atlas image next to output
        //
       /* std::filesystem::path pngPath = std::filesystem::path(outputPath).replace_extension(".atlas.png");
        if (!atlas.ExportAtlasPNG(pngPath.string())) {
            std::cerr << "Failed to export atlas PNG: " << pngPath.string() << "\n";
        }
        else {
            std::cout << "Wrote atlas PNG: " << pngPath.string() << "\n";
        }*/

        return 0;
    }
    else {
        std::cerr << "Unrecognized extension: " << ext << "\n";
        return 1;
    }
}
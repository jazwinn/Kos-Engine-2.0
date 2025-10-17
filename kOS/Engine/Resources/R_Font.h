#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Config/pch.h"
#include "Resource.h"

#define FT2_BUILD_LIBRARY
#include "freetype/ft2build.h"
#include FT_FREETYPE_H



class R_Font : public Resource {

public:
    using Resource::Resource;

	void Load() override;

	void Unload() override;

    ~R_Font() override {
        Unload(); // safe here because we're still in the derived destructor
    }

    /******************************************************************/
   /*!
   \class   CharacterData
   \brief   Stores information about individual glyphs used for rendering text.

            This class holds various properties of a character glyph,
            including its texture ID, size, bearing (offset from baseline),
            and advance (distance to the next glyph).
   */
   /******************************************************************/
    class CharacterData
    {
    public:
        GLuint m_textureID;       ///< ID handle of the glyph texture
        glm::ivec2 m_size;        ///< Size of the glyph (width and height)
        glm::ivec2 m_bearing;     ///< Offset from the baseline to the left/top of the glyph
        GLuint m_advance;         ///< Offset to advance to the next glyph
        glm::vec2 m_topLeftTexCoords;
        glm::vec2 m_bottomRightTexCoords;
    };

    /******************************************************************/
        /*!
        \typedef using FontMap = std::map<char, text::CharacterData>
        \brief   Defines a map type for storing character data by character.
        */
        /******************************************************************/
    using FontMap = std::map<char, CharacterData>;

    /******************************************************************/
    /*!
    \var     FontMap m_characters
    \brief   Stores character data for a font, indexed by character.
    */
    /******************************************************************/
    FontMap m_characters{};

    int m_atlasWidth{ 0 };
    int m_atlasHeight{ 0 };
    std::vector<unsigned char> m_atlasRGBA;

	REFLECTABLE(R_Font);

private:



};




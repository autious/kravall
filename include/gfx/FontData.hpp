#ifndef CORE_CONTENT_MANAGEMENT_ASSET_STRUCTS_FONT_DATA_HPP
#define CORE_CONTENT_MANAGEMENT_ASSET_STRUCTS_FONT_DATA_HPP

#include <GL/glew.h>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace GFX
{
    struct FontCharacter
    {
        float advanceX;
        float advanceY;

        float bitMapWidth;
        float bitMapHeight;

        float bitMapLeft;
        float bitMapTop;

        float uvOffSetX;
        float uvOffSetY;
    };

    struct FontData
    {
        static const int NUM_CHARACTERS = 128;
        static const int ATLAS_MAX_WIDTH = 256;
        FT_Face* fontFace;
        FontCharacter characters[NUM_CHARACTERS];

        GLuint fontAtlas;
        short atlasWidth;
        short atlasHeight;
    };
}


#endif

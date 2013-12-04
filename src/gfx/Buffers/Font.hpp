#ifndef SRC_GFX_BUFFERS_FONT_HPP
#define SRC_GFX_BUFFERS_FONT_HPP

#include <string>
#include <iostream>
#include <algorithm>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#define MAX_WIDTH 1024

namespace GFX
{
	class Text
	{
	public:
		float posX;
		float posY;
		float sizeX;
		float sizeY;
		glm::vec4 color;
		std::string text;

		Text(float _posx, float _posy, float _sizex, float _sizey, 
			glm::vec4 _color, std::string _text, int screenWidth, int screenHeight);
	};


	struct FontCharacter
	{
		float advanceX;
		float advanceY;

		float bitmapWidth;
		float bitmapHeight;

		float bitmapLeft;
		float bitmapTop;

		float uvOffsetX;
		float uvOffsetY;
	};

	class FontTextureAtlas
	{
	public:
		GLuint texture;
		GLuint textureUniform;

		int width;
		int height;

		FontCharacter characters[128];

		FontTextureAtlas(FT_Face face, int h, GLuint tUniform);

		~FontTextureAtlas();
	};
}

#endif
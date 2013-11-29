#ifndef SRC_GFX_BUFFERS_FONT_HPP
#define SRC_GFX_BUFFERS_FONT_HPP

#include <iostream>
#include <algorithm>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#define MAX_WIDTH 1024

namespace GFX
{
	struct Text
	{
		float posX;
		float posY;
		float sizeX;
		float sizeY;
		glm::vec4 color;
		std::string text;

		Text(float _posx, float _posy, float _sizex, float _sizey, glm::vec4 _color, std::string _text, int screenWidth, int screenHeight)
		{
			posX = ((2 * _posx) / screenWidth) - 1.0f;
			posY = -((2 * _posy) / screenHeight) + 1.0f;

			sizeX = _sizex / 15000.0f;
			sizeY = _sizey / 15000.0f;
			color = _color;
			text = _text;
		}
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

	struct FontTextureAtlas
	{
		GLuint texture;
		GLuint textureUniform;

		int width;
		int height;

		FontCharacter characters[128];

		FontTextureAtlas(FT_Face face, int h, GLuint tUniform)
		{
			FT_Set_Pixel_Sizes(face, 0, h);
			FT_GlyphSlot glyphSlot = face->glyph;

			int roww = 0;
			int rowh = 0;
			width = 0;
			height = 0;

			memset(characters, 0, sizeof(FontCharacter));

			for (int i = 32; i < 128; i++)
			{
				if (FT_Load_Char(face, i, FT_LOAD_RENDER))
				{
					std::cout << "Loading character %c failed\n", i;
					continue;
				}

				if (roww + glyphSlot->bitmap.width + 1 >= MAX_WIDTH)
				{
					width = std::max(width, roww);
					height += rowh;
					roww = 0;
					rowh = 0;
				}

				roww += glyphSlot->bitmap.width + 1;
				rowh = std::max(rowh, glyphSlot->bitmap.rows);
			}

			width = std::max(width, roww);
			height += rowh;

			

			glActiveTexture(GL_TEXTURE0);

			glGenTextures(1, &texture);

			if (glGetError() != GL_NO_ERROR)
			{
				std::cout << "glGenTextures failed\n";
			}

			if (glGetError() != GL_NO_ERROR)
			{
				std::cout << "glActiveTexture failed\n";
			}

			glBindTexture(GL_TEXTURE_2D, texture);

			if (glGetError() != GL_NO_ERROR)
			{
				std::cout << "glBindTexture failed\n";
			}

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			if (glGetError() != GL_NO_ERROR)
			{
				std::cout << "glPixelStorei failed\n";
			}

			glUniform1i(tUniform, 0);
			textureUniform = tUniform;

			glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

			if (glGetError() != GL_NO_ERROR)
			{
				std::cout << "glTexImage2D failed\n";
			}

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

			if (glGetError() != GL_NO_ERROR)
			{
				std::cout << "glTexParameteri failed\n";
			}

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			if (glGetError() != GL_NO_ERROR)
			{
				std::cout << "glTexParameteri failed\n";
			}

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			if (glGetError() != GL_NO_ERROR)
			{
				std::cout << "glTexParameteri failed\n";
			}

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			if (glGetError() != GL_NO_ERROR)
			{
				std::cout << "glTexParameteri failed\n";
			}

			
			int ox = 0;
			int oy = 0;

			rowh = 0;

			for (int i = 32; i < 128; i++)
			{
				if (FT_Load_Char(face, i, FT_LOAD_RENDER))
				{
					std::cout << "Loading character %c failed\n", i;
					continue;
				}

				if (ox + glyphSlot->bitmap.width + 1 >= MAX_WIDTH) 
				{
					oy += rowh;
					rowh = 0;
					ox = 0;
				}

				glTexSubImage2D(GL_TEXTURE_2D, 0, ox, oy, glyphSlot->bitmap.width, glyphSlot->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, glyphSlot->bitmap.buffer);

				if (glGetError() != GL_NO_ERROR)
				{
					std::cout << "BORKED AGAIN\n";
				}

				characters[i].advanceX = glyphSlot->advance.x >> 6;
				characters[i].advanceY = glyphSlot->advance.y >> 6;

				characters[i].bitmapWidth = glyphSlot->bitmap.width;
				characters[i].bitmapHeight = glyphSlot->bitmap.rows;

				characters[i].bitmapLeft = glyphSlot->bitmap_left;
				characters[i].bitmapTop = glyphSlot->bitmap_top;

				characters[i].uvOffsetX = ox / (float)width;
				characters[i].uvOffsetY = oy / (float)height;

				rowh = std::max(rowh, glyphSlot->bitmap.rows);
				ox += glyphSlot->bitmap.width + 1;
			}

			std::cout << "Generated a " << width << "x " << height << " (" << width * height / 1024 << " kb) texture atlas.\n";
		}

		~FontTextureAtlas()
		{
			glDeleteTextures(1, &texture);
		}
	};
}

#endif
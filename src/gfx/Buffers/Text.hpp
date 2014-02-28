#ifndef SRC_GFX_TEXT_HPP
#define SRC_GFX_TEXT_HPP

#include <string>
#include <iostream>
#include <algorithm>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include <FontData.hpp>

namespace GFX
{
	struct Text
	{
		float m_posX;
		float m_posY;
		float m_sizeX;
		float m_sizeY;
		glm::vec4 m_color;
		std::string m_text;
        GFX::FontData* m_fontData;

		Text(float posx, float posy, float sizex, float sizey, GFX::FontData* fontData,
			glm::vec4 color, std::string text);
	};
}

#endif

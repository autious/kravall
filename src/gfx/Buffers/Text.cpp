#include "Text.hpp"

namespace GFX
{

	Text::Text(float posx, float posy, float sizex, float sizey, GFX::FontData* fontData, glm::vec4 color, std::string text)
	{
		m_posX = posx;
		m_posY = posy;
        
		m_sizeX = sizex;
		m_sizeY = sizey;
        m_fontData = fontData;
		m_color = color;
		m_text = text;
	}
}

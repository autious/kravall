#include "Text.hpp"

namespace GFX
{

	Text::Text(float posx, float posy, float sizex, float sizey, GFX::FontData* fontData, glm::vec4 color, std::string text, int screenWidth, int screenHeight)
	{
		m_posX = ((2 * posx) / static_cast<float>(screenWidth)) - 1.0f;
		m_posY = ((2 * posy) / static_cast<float>(screenHeight)) - 1.0f;
        
		m_sizeX = sizex * (2 / static_cast<float>(screenWidth));
		m_sizeY = -sizey * (2 / static_cast<float>(screenHeight));
        m_fontData = fontData;
		m_color = color;
		m_text = text;
	}
}

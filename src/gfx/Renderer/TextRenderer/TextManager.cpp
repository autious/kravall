#include "TextManager.hpp"

namespace GFX
{
	TextManager& GetTextManager()
	{
		static TextManager t;
		return t;
	}

	TextManager::TextManager()
	{

	}

	TextManager::~TextManager()
	{

	}

	void TextManager::ClearText()
	{
		m_text.clear();
	}

	void TextManager::AddText(Text t)
	{
		m_text.push_back(t);
	}

	void TextManager::AddTextbox(GFX::FontData* fontData, glm::vec4 rectangle, float offset, float size, glm::vec4 color, const char* text)
	{
		std::string trim = " \n\r\t";
		std::string str = text;
		float l_off = offset;		
		float width = 0.0f;

		std::string::iterator start = str.begin();
		std::string::iterator it = str.begin();
		for (int i = 0; it < str.end(); it++, i++)
		{
			width += fontData->characters[static_cast<unsigned char>(*it)].advanceX * size;
			std::string line = std::string(start, it);
			if (width + 5.0f * size >= rectangle.z || line.find_last_of("\n") != line.npos)
			{
				unsigned int lineLength = line.length();
				std::size_t lastWS = line.find_last_of(trim);
				if (lastWS == line.npos) // Word is too long for textbox, wrap it!
				{
					str.insert(it, '\n');
					i++;
					it = str.begin() + i;
					start = it;
				}
				else // Last word doesnt fit, but all the others do!
				{
					*(start + lastWS) = '\n';
					i = i - (lineLength - lastWS) + 1;
					it = str.begin() + i;
					start = it;
				}
				width = 0.0f;
			}
		}

		start = str.begin();
		for (auto it = str.begin(); it < str.end(); it++)
		{
			if (*it == '\n')
			{

				l_off += 20.0f * size;
				if (l_off >= 20.0f && l_off < rectangle.w)
				{
					std::string line = std::string(start, it);

					// Remove leading whitespaces
					std::size_t pos;
					while (pos = line.find_first_of(trim) == 0)
						line.erase(line.begin());

					GFX::Text t(rectangle.x, rectangle.y + l_off, size, size, fontData, color, line.c_str());
					m_text.push_back(t);
				}
				start = it + 1;
			}
		}
		l_off += 20.0f * size;
		if (l_off >= 20.0f && l_off < rectangle.w)
		{
			std::string line = std::string(start, str.end());

			// Remove leading whitespaces
			std::size_t pos;
			while (pos = line.find_first_of(trim) == 0)
				line.erase(line.begin());

			GFX::Text t(rectangle.x, rectangle.y + l_off, size, size, fontData, color, line.c_str());
			m_text.push_back(t);
		}
	}
	void TextManager::GetTextboxSize(GFX::FontData* fontData, float rect_width, float size, const char* text, float& out_actualWidth, float& out_actualHeight)
	{
		std::string trim = " \n\r\t";
		std::string str = text;
		float l_off = 0.0f;		
		float width = 0.0f;
		float maxWidth = 0.0f;

		std::string::iterator start = str.begin();
		std::string::iterator it = str.begin();
		for (int i = 0; it < str.end(); it++, i++)
		{
			width += fontData->characters[static_cast<unsigned char>(*it)].advanceX * size;
			std::string line = std::string(start, it);
			if (width + 5.0f * size >= rect_width || line.find_last_of("\n") != line.npos)
			{
				unsigned int lineLength = line.length();
				std::size_t lastWS = line.find_last_of(trim);
				if (lastWS == line.npos) // Word is too long for textbox, wrap it!
				{
					str.insert(it, '\n');
					i++;
					it = str.begin() + i;
					start = it;
				}
				else // Last word doesnt fit, but all the others do!
				{
					*(start + lastWS) = '\n';
					i = i - (lineLength - lastWS) + 1;
					it = str.begin() + i;
					start = it;
				}
				width = 0.0f;
			}
			else
			{
				maxWidth = std::max(maxWidth, width + 5.0f * size);
			}
		}

		start = str.begin();
		for (auto it = str.begin(); it < str.end(); it++)
		{
			if (*it == '\n')
			{

				l_off += 20.0f * size;
				{
					std::string line = std::string(start, it);

					// Remove leading whitespaces
					std::size_t pos;
					while (pos = line.find_first_of(trim) == 0)
						line.erase(line.begin());
				}
				start = it + 1;
			}
		}
		l_off += 20.0f * size;
		{
			std::string line = std::string(start, str.end());

			// Remove leading whitespaces
			std::size_t pos;
			while (pos = line.find_first_of(trim) == 0)
				line.erase(line.begin());
		}

		out_actualHeight = l_off;
		out_actualWidth = maxWidth;
	}
}
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
}
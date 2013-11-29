#ifndef SRC_GFX_RENDERER_TEXT_RENDERER_TEXT_MANAGER_HPP
#define SRC_GFX_RENDERER_TEXT_RENDERER_TEXT_MANAGER_HPP

#include "Buffers/Font.hpp"
#include <vector>

namespace GFX
{
	class TextManager
	{
	public:
		friend TextManager& GetTextManager();

		void ClearText();
		void AddText(Text t);
		inline std::vector<Text>& GetText(){ return m_text; }

	private:
		std::vector<Text> m_text;

		TextManager();
		~TextManager();
	};

	TextManager& GetTextManager();
}


#endif
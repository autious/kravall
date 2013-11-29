#ifndef SRC_GFX_RENDERER_TEXT_RENDERER_TEXT_MANAGER_HPP
#define SRC_GFX_RENDERER_TEXT_RENDERER_TEXT_MANAGER_HPP

#include "Buffers/Font.hpp"
#include <vector>

namespace GFX
{
	class TextManager
	{
	public:
		
		/*!
		Friend function to get singleton
		\return TextManager instance
		*/
		friend TextManager& GetTextManager();

		/*!
		Clear the text list
		*/
		void ClearText();

		/*!
		Add text to the vector for rendering
		\param t Text to be added
		*/
		void AddText(Text t);
		
		/*!
		Return entire vector of text objects
		\return text vector
		*/
		inline std::vector<Text>& GetText(){ return m_text; }

	private:
		std::vector<Text> m_text;

		TextManager();
		~TextManager();
	};

	/*!
	Function to get TextManager singleton
	\return TextManager instance
	*/
	TextManager& GetTextManager();
}


#endif
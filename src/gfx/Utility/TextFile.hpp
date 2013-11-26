#ifndef SRC_GFX_UTILITY_TEXT_FILE_HPP
#define SRC_GFX_UTILITY_TEXT_FILE_HPP

#include <fstream>
#include <string>
#include <exception>
#include <cstring>

namespace GFX
{
	class TextFile
	{
	private:

		std::string m_text;
		char*		m_cText;
		bool		m_nonConstantChar;
		bool		m_loadedFile;

	public:

		TextFile();
		TextFile(bool enableNonConstantChar);

		~TextFile();

		void LoadFile(const char* filepath);

		std::string GetString();
		const char* GetCString();
		char* GetNonConstantChar();

		void Clear();
	};
}
#endif

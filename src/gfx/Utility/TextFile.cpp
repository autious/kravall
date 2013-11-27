#include "TextFile.hpp"
#include <cstring>

namespace GFX
{
	TextFile::TextFile()
	{
		//No char-Array is used by default.
		m_nonConstantChar = false;
		m_loadedFile = false;
	}

	TextFile::TextFile(bool enableNonConstantChar)
	{
		m_nonConstantChar = enableNonConstantChar;
		m_loadedFile = false;
	}

	TextFile::~TextFile()
	{
		//If _ctext points to an acquired memory block then release it.
		if (m_nonConstantChar && m_loadedFile)
		{
			delete[] m_cText;
		}
	}

	void TextFile::LoadFile(const char* filepath)
	{
		//Clear the string everytime a new file is loaded.
		m_text.clear();

		std::string temp;

		std::ifstream file(filepath);

		if (file.fail())
		{
			throw std::exception(); //std::exception doesn't take a string.
		}
		else
		{
			if (file.is_open())
			{
				while (file.good())
				{
					std::getline(file, temp);
					m_text += temp + "\n";
				}
			}
		}

		file.close();

		//Did the user enable saving in a char-Array?
		if (m_nonConstantChar)
		{

			//Does _ctext already point to an acquired memory block then
			//release it. 
			if (m_loadedFile)
				delete[] m_cText;

			unsigned int size = m_text.length() + 1;
			m_cText = new char[size];	//+1 for the null-termination
			strcpy(m_cText, m_text.c_str());
			m_loadedFile = true;
		}

	}

	std::string TextFile::GetString()
	{
		return m_text;
	}

	const char* TextFile::GetCString()
	{
		return m_text.c_str();
	}

	char* TextFile::GetNonConstantChar()
	{
		if (m_nonConstantChar && m_loadedFile)
		{
			return m_cText;
		}
		else
		{
			return "";
		}
	}

	void TextFile::Clear()
	{
		//If _ctext points to an acquired memory block then release it.
		if (m_nonConstantChar && m_loadedFile)
		{
			delete[] m_cText;
			m_loadedFile = false;
		}

		m_text.clear();

	}
}

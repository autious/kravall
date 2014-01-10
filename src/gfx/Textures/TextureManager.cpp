#include "TextureManager.hpp"

namespace GFX
{
	TextureManager::TextureManager()
	{
		m_idCounter = 0;
	}

	TextureManager::~TextureManager()
	{

	}

	void TextureManager::LoadTexture(unsigned int& id, unsigned char* data, int width, int height)
	{
        /*
		LoadTexture(id, data, GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_LINEAR, GL_LINEAR,
			GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, width, height);
        */

		LoadTexture(id, data, GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_LINEAR, GL_LINEAR,
			GL_REPEAT, GL_REPEAT, width, height);
	}
		
	void TextureManager::LoadTexture(unsigned int& id, unsigned char* data, const GLenum& target,
		const GLenum& internalFormat, const GLenum& format,
		const GLint& minFilter, const GLint& magFilter,
		const GLint& wrapS, const GLint& wrapT,
		int width, int height)
	{
		GLuint textureHandle;
		glGenTextures(1, &textureHandle);
		glBindTexture(GL_TEXTURE_2D, textureHandle);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);

		TextureData texture;
		texture.id = static_cast<unsigned int>(m_idCounter);
		m_idCounter++;
		texture.textureHandle = textureHandle;
		m_textures.push_back(texture);

		id = texture.id;
	}

	void TextureManager::DeleteTexture(unsigned long long int id)
	{
		unsigned int index = FindTexture(id);

		if (index != std::numeric_limits<unsigned int>::max())
		{
			glDeleteTextures(1, &m_textures[index].textureHandle);
			m_textures.erase(m_textures.begin() + index);
		}
	}

	///////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////
	//////////////////////////////
	//////////////////////
	//////////////HERE BE DRAGONS, BEWARE///////////////
	////////////////////////////////////////////////////
	/////////////////////////////////////////////////////
	/////////////HURR DURR
    //////////////////////I FEAR NOT DRAGONS////////////////
	GLuint TextureManager::LoadFromMemory(
		unsigned char* data, const GLenum& target,
		const GLenum& internalFormat, const GLenum& format,
		const GLint& minFilter, const GLint& magFilter,
		const GLint& wrapS, const GLint& wrapT,
		int width, int height)
	{
		GLuint textureHandle;
		glGenTextures(1, &textureHandle);
		glBindTexture(GL_TEXTURE_2D, textureHandle);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);

		return textureHandle;
	}

	GLuint TextureManager::LoadFromFile(
		const char* filepath, const GLenum& target,
		const GLenum& internalFormat, const GLenum& format,
		const GLint& minFilter, const GLint& magFilter,
		const GLint& wrapS, const GLint& wrapT)
	{
		unsigned char* data;
		GLuint textureHandle;
		int width, height;
		data = stbi_load(filepath, &width, &height, 0, 0);

		textureHandle = LoadFromMemory(data, target, internalFormat, format, minFilter, magFilter, wrapS, wrapT, width, height);

		stbi_image_free(data);

		return textureHandle;
	}

	void TextureManager::BindTexture(GLuint textureHandle, GLint uniform, GLuint position, GLenum target)
	{
		glUniform1i(uniform, position);
		glActiveTexture(GL_TEXTURE0 + position);
		glBindTexture(GL_TEXTURE_2D, textureHandle);
	}

	void TextureManager::UnbindTexture()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}


}

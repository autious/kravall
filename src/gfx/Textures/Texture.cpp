#include "Texture.hpp"

namespace GFX
{
	Texture::Texture()
	{
		m_textureHandle = 0;
		m_width = 0;
		m_height = 0;
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &m_textureHandle);
	}

	void Texture::LoadFromMemory(
		unsigned char* data, const GLenum& target,
		const GLenum& internalFormat, const GLenum& format,
		const GLint& minFilter, const GLint& magFilter,
		const GLint& wrapS, const GLint& wrapT)
	{

		glGenTextures(1, &m_textureHandle);
		glBindTexture(GL_TEXTURE_2D, m_textureHandle);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, data);

	}

	void Texture::LoadFromFile(
		const char* filepath, const GLenum& target,
		const GLenum& internalFormat, const GLenum& format,
		const GLint& minFilter, const GLint& magFilter,
		const GLint& wrapS, const GLint& wrapT)
	{
		unsigned char* data;
		data = stbi_load(filepath, &m_width, &m_height, 0, 0);
		LoadFromMemory(data, target, internalFormat, format, minFilter, magFilter, wrapS, wrapT);
		stbi_image_free(data);
	}

	void Texture::BindTexture(GLint uniform, GLuint position, GLenum target)
	{
		glUniform1i(uniform, position);
		glActiveTexture(GL_TEXTURE0 + position);
		glBindTexture(GL_TEXTURE_2D, m_textureHandle);
	}

	void Texture::UnbindTexture()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}
#include "FBOTexture.hpp"

namespace GFX
{
	
	FBOTexture::FBOTexture()
	{
		m_textureHandle = 0;
		m_textureTarget = 0;
		m_magFilter = 0;
		m_minFilter = 0;
		m_wrapS = 0;
		m_wrapT = 0;
		m_internalFormat = 0;
		m_format = 0;
	}

	//implement this shit
	FBOTexture::~FBOTexture()
	{
		glDeleteTextures(1, &m_textureHandle);
	}

	void FBOTexture::Initialize(GLenum textureTarget, GLint magFilter, GLint minFilter, GLint wrapModeS,
		GLint wrapModeT, GLint internalFormat, GLint format)
	{
		m_textureHandle = 0;
		m_textureTarget = textureTarget;
		
		m_magFilter = magFilter;
		m_minFilter = minFilter;
		m_wrapS = wrapModeS;
		m_wrapT = wrapModeT;
		
		m_internalFormat = internalFormat;
		m_format = format;

		this->GenerateTexture();
	}

	void FBOTexture::UpdateResolution(int width, int height)
	{
		//glActiveTexture(GL_TEXTURE0 + m_textureHandle);
		glBindTexture(m_textureTarget, m_textureHandle);

		glGetError();

		if (m_format == GL_DEPTH_STENCIL)
			glTexImage2D(m_textureTarget, 0, m_internalFormat, width, height, 0, m_format, GL_UNSIGNED_INT_24_8, nullptr);
		else
			glTexImage2D(m_textureTarget, 0, m_internalFormat, width, height, 0, m_format, GL_UNSIGNED_BYTE, nullptr);

		glGetError();
	}

	GLuint FBOTexture::GetTextureHandle()
	{
		return m_textureHandle;
	}

	//Is this correct?
	void FBOTexture::GenerateTexture()
	{
		glGenTextures(1, &m_textureHandle);
		//glActiveTexture(GL_TEXTURE0 + m_textureHandle);
		glBindTexture(m_textureTarget, m_textureHandle);

		glTexParameteri(m_textureTarget, GL_TEXTURE_MAG_FILTER, m_magFilter);
		glTexParameteri(m_textureTarget, GL_TEXTURE_MIN_FILTER, m_minFilter);
		glTexParameteri(m_textureTarget, GL_TEXTURE_WRAP_S, m_wrapS);
		glTexParameteri(m_textureTarget, GL_TEXTURE_WRAP_T, m_wrapT);
	}

}
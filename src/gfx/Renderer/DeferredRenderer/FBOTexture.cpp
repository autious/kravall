#include "FBOTexture.hpp"
#include <gfx/GFXSettings.hpp>
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

	void FBOTexture::CreateShadowmap(int resolution, int quality)
	{
		
		GLenum err;
		//Initialize Shadowmap texture, parameters should be configurable through settings
		glGenTextures(1, &m_textureHandle);
		glBindTexture(GL_TEXTURE_2D, m_textureHandle);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		if (quality == GFX_SHADOWS_VARIANCE) 
		{
			// Create 2 channel texture for variance shadowmapping
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, resolution, resolution, 0, GL_RG, GL_FLOAT, nullptr);
			GLfloat border[4] = { 1.0f, 1.0f, 0.0f, 0.0f };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
		}
		else if (quality == GFX_SHADOWS_BASIC) 
		{
			// Create 1 channel texture for basic shadowmapping
			glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, resolution, resolution, 0, GL_R, GL_FLOAT, nullptr);
			GLfloat border[4] = { 1.0f, 0.0f, 0.0f, 0.0f };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
		}
		else
		{

		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);
		glGenerateMipmap(GL_TEXTURE_2D);

		m_width = resolution;
		m_height = resolution;
	}

	void FBOTexture::UpdateResolution(int width, int height)
	{
		//glActiveTexture(GL_TEXTURE0 + m_textureHandle);
		glBindTexture(m_textureTarget, m_textureHandle);

		if (m_format == GL_DEPTH_STENCIL)
			glTexImage2D(m_textureTarget, 0, m_internalFormat, width, height, 0, m_format, GL_UNSIGNED_INT_24_8, nullptr);
		else
			glTexImage2D(m_textureTarget, 0, m_internalFormat, width, height, 0, m_format, GL_UNSIGNED_BYTE, nullptr);


		m_width = width;
		m_height = height;
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
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

	void TextureManager::LoadTexture(unsigned int& id, unsigned char* data, int width, int height, bool decal)
	{
        /*
		LoadTexture(id, data, GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_LINEAR, GL_LINEAR,
			GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, width, height);
        */

		if (decal)
		{
			LoadTexture(id, data, GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR,
				GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, width, height);
		}
		else
		{
			LoadTexture(id, data, GL_TEXTURE_2D, GL_RGBA, GL_RGBA, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR,
				GL_REPEAT, GL_REPEAT, width, height);
		}
	}
		
	void TextureManager::LoadTexture(unsigned int& id, unsigned char* data, const GLenum& target,
		const GLenum& internalFormat, const GLenum& format,
		const GLint& minFilter, const GLint& magFilter,
		const GLint& wrapS, const GLint& wrapT,
		int width, int height)
	{

		GLfloat maxAniso = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);

		GLuint textureHandle;
		glGenTextures(1, &textureHandle);
		glBindTexture(GL_TEXTURE_2D, textureHandle);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		TextureData texture;
		texture.id = static_cast<unsigned int>(m_idCounter);
		m_idCounter++;
		texture.textureHandle = textureHandle;
		m_textures.push_back(texture);

		id = texture.id;
	}

	void TextureManager::LoadCubemap(unsigned int& id, unsigned char* posX, unsigned char* negX, unsigned char* posY, unsigned char* negY, unsigned char* posZ, unsigned char* negZ, int width, int height)
	{
		GLfloat maxAniso = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);

		GLuint textureHandle;
		glGenTextures(1, &textureHandle);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureHandle);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, posX);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, negX);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, posY);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, negY);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, posZ);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, negZ);

		//glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

		TextureData texture;
		texture.id = static_cast<unsigned int>(m_idCounter);
		m_idCounter++;
		texture.textureHandle = textureHandle;
		m_textures.push_back(texture);

		id = texture.id;
	}

	void TextureManager::Load3DTexture(unsigned int& id, int width, int height, int depth, unsigned char* data)
	{
		GLuint textureHandle;
		glGenTextures(1, &textureHandle);
		glBindTexture(GL_TEXTURE_3D, textureHandle);

		GLfloat maxAniso = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);

		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);

		//glTexParameterf(GL_TEXTURE_3D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);

		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, width, height, depth, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		//glGenerateMipmap(GL_TEXTURE_3D);

		TextureData texture;
		texture.id = static_cast<unsigned int>(m_idCounter);
		m_idCounter++;
		texture.textureHandle = textureHandle;
		m_textures.push_back(texture);

		id = texture.id;
	}

	void TextureManager::Load3DTexture(unsigned int& id, int width, int height, int depth, char* filepath)
	{
		unsigned char* data;
		int w;
		int h;
		data = stbi_load(filepath, &w, &h, 0, 0);
		Load3DTexture(id, width, height, depth, data);
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
		glBindTexture(target, textureHandle);
	}

	void TextureManager::UnbindTexture()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}


}

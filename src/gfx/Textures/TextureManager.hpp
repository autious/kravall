#ifndef SRC_GFX_TEXTURES_TEXTURE_MANAGER_HPP
#define SRC_GFX_TEXTURES_TEXTURE_MANAGER_HPP

#define STBI_HEADER_FILE_ONLY
#include <GL/glew.h>
#include <vector>
#include <limits>

#include "stb_image.cpp"
#include "TextureData.hpp"

namespace GFX
{
	class TextureManager
	{
	public:
		static GLuint LoadFromMemory(
			unsigned char* data, const GLenum& target,
			const GLenum& internalFormat, const GLenum& format,
			const GLint& minFilter, const GLint& magFilter,
			const GLint& wrapS, const GLint& wrapT,
			int width, int height);

		static GLuint LoadFromFile(
			const char* filepath, const GLenum& target,
			const GLenum& internalFormat, const GLenum& format,
			const GLint& minFilter, const GLint& magFilter,
			const GLint& wrapS, const GLint& wrapT);

		static void BindTexture(GLuint textureHandle, GLint uniform, GLuint position, GLenum target);
		static void UnbindTexture();

		TextureManager();
		~TextureManager();

		void LoadTexture(unsigned int& id, unsigned char* data, int width, int height);
		
		void LoadTexture(unsigned int& id, unsigned char* data, const GLenum& target,
			const GLenum& internalFormat, const GLenum& format,
			const GLint& minFilter, const GLint& magFilter,
			const GLint& wrapS, const GLint& wrapT,
			int width, int height);

		void LoadCubemap(unsigned int& id, unsigned char* posX, unsigned char* negX, unsigned char* posY, unsigned char* negY, unsigned char* posZ, unsigned char* negZ, int width, int height);

		void Load3DTexture(unsigned int& id, int width, int height, int depth, unsigned char* data);

		void CUBEMAPTEST(unsigned int& id);
		void THREEDTEST(unsigned int& id);


		void DeleteTexture(unsigned long long int id);

		inline TextureData GetTexture(const unsigned long long int& textureID) 
		{ 
			unsigned int index = FindTexture(textureID);

			if (index != std::numeric_limits<unsigned int>::max())
				return m_textures[index]; 

			TextureData t;
			t.id = std::numeric_limits<unsigned int>::max();
			t.textureHandle = std::numeric_limits<unsigned int>::max();

			return t;
		}

	private:

		std::vector<TextureData> m_textures;
		unsigned long long int m_idCounter;

		inline const unsigned int FindTexture(unsigned long long id)
		{
			for (unsigned int i = 0; i < m_textures.size(); i++)
				if (m_textures[i].id == static_cast<unsigned int>(id))
					return i;

			return std::numeric_limits<unsigned int>::max();
		}
	};
}

#endif 

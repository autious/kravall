#include "Texture.hpp"

namespace GFX
{
	namespace Texture
	{
		GLuint LoadFromMemory(
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

		GLuint LoadFromFile(
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

		void BindTexture(GLuint textureHandle, GLint uniform, GLuint position, GLenum target)
		{
			glUniform1i(uniform, position);
			glActiveTexture(GL_TEXTURE0 + position);
			glBindTexture(GL_TEXTURE_2D, textureHandle);
		}

		void UnbindTexture()
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
}
#ifndef SRC_GFX_TEXTURES_TEXTURE_HPP
#define SRC_GFX_TEXTURES_TEXTURE_HPP

#define STBI_HEADER_FILE_ONLY
#include <GL/glew.h>
#include "stb_image.cpp"

namespace GFX
{
	namespace Texture
	{
		GLuint LoadFromMemory(
			unsigned char* data, const GLenum& target,
			const GLenum& internalFormat, const GLenum& format,
			const GLint& minFilter, const GLint& magFilter,
			const GLint& wrapS, const GLint& wrapT,
			int width, int height);

		GLuint LoadFromFile(
			const char* filepath, const GLenum& target,
			const GLenum& internalFormat, const GLenum& format,
			const GLint& minFilter, const GLint& magFilter,
			const GLint& wrapS, const GLint& wrapT);

		void BindTexture(GLuint textureHandle, GLint uniform, GLuint position, GLenum target);
		void UnbindTexture();

	}
}

#endif 
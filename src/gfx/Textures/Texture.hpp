#ifndef SRC_GFX_TEXTURES_TEXTURE_HPP
#define SRC_GFX_TEXTURES_TEXTURE_HPP

#include <GL/glew.h>
#include <stb_image.c>

namespace GFX
{
	class Texture
	{
	public:
		Texture();
		~Texture();

		void LoadFromMemory(
			unsigned char* data, const GLenum& target,
			const GLenum& internalFormat, const GLenum& format,
			const GLint& minFilter, const GLint& magFilter,
			const GLint& wrapS, const GLint& wrapT);

		void LoadFromFile(
			const char* filepath, const GLenum& target,
			const GLenum& internalFormat, const GLenum& format,
			const GLint& minFilter, const GLint& magFilter,
			const GLint& wrapS, const GLint& wrapT);

		void BindTexture(GLint uniform, GLuint position, GLenum target);
		void UnbindTexture();
		inline GLuint GetHandle() { return m_textureHandle; }

	private:
		GLuint m_textureHandle;
		int m_width;
		int m_height;
	};
}

#endif 
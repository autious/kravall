#ifndef SRC_GFX_RENDERER_FBO_TEXTURE_HPP
#define SRC_GFX_RENDERER_FBO_TEXTURE_HPP

#include "Utility/GLFWInclude.hpp"

namespace GFX
{
	class FBOTexture
	{
	private:
		GLuint m_textureHandle;
		GLenum m_textureTarget;
		GLint m_magFilter;
		GLint m_minFilter;
		GLint m_wrapS;
		GLint m_wrapT;
		GLint m_internalFormat;
		GLint m_format;

		void GenerateTexture();

	public:

		FBOTexture();
		~FBOTexture();

		void Initialize(GLenum textureTarget, GLint magFilter, GLint minFilter, GLint wrapModeS, 
			GLint wrapModeT, GLint internalFormat, GLint format);
		void UpdateResolution(int width, int height);
		GLuint GetTextureHandle();
	};
}

#endif
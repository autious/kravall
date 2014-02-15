#ifndef SRC_GFX_RENDERER_FBO_TEXTURE_HPP
#define SRC_GFX_RENDERER_FBO_TEXTURE_HPP

#include <GL/glew.h>

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

		int m_height;
		int m_width;

		void GenerateTexture();

	public:

		FBOTexture();
		~FBOTexture();
		
		/*!
		Initializes the FBO texture
		\param textureTarget What kind of texture the FBO texture is
		\param magFilter magfilter mode
		\param minFilter minfilter mode
		\param wrapModeS wrapmode
		\param wrapModeT wrapmode
		\param internalForm internal format of the texture
		\param format format of the texture
		*/
		void Initialize(GLenum textureTarget, GLint magFilter, GLint minFilter, GLint wrapModeS, 
			GLint wrapModeT, GLint internalFormat, GLint format);

		void CreateShadowmap(int resolution, int quality);
		
		/*!
		Updates the resolution of the FBO texture
		\param width New width
		\param height New height
		*/
		void UpdateResolution(int width, int height);

		/*!
		Get the handle of the texture
		\return id of texture
		*/
		GLuint GetTextureHandle();

		inline int GetWidth()
		{
			return m_width;
		}

		inline int GetHeight()
		{
			return m_height;
		}
	};
}

#endif

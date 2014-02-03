#ifndef SRC_GFX_BLUR_PAINTER_HPP
#define SRC_GFX_BLUR_PAINTER_HPP

#include "../BasePainter.hpp"
#include <Shaders/ShaderManager.hpp>
#include <Buffers/UniformBufferManager.hpp>
#include "../../Textures/TextureManager.hpp"
#include "../../Textures/LUTManager.hpp"
#include "../DeferredRenderer/FBOTexture.hpp"
#include "../RenderJobManager.hpp"

namespace GFX
{
	class BlurPainter : public BasePainter
	{
	public:
		BlurPainter(ShaderManager* shaderManager, UniformBufferManager* uniformBufferManager);

		~BlurPainter();

		/*!
		Initialization function which sets the dummyVAO and FBO for later use in the painter.
		Loads all shaders associated with deferred rendering.
		\param FBO ID of FBO used for rendertargets
		\param dummyVAO ID of an empty VAO used for screenspace rendering
		*/
		void Initialize(GLuint FBO, GLuint dummyVAO);

		/*!
		Gaussian blur pass
		*/
		void GaussianBlur(FBOTexture* texture);


	private:
		void InitFBO();

		FBOTexture* m_intermediateTexture;

		GLuint m_blurFBO;

		GLuint m_seedTexture;

		//SSDO uniforms
		GLint m_invProjUniform;
		GLint m_eyePosUniform;
		GLint m_screenSizeUniform;
		GLint m_occlusionRadiusUniform;
		GLint m_maxOcclusionDistanceUniform;

		GLint m_normalDepthUniform;
		GLint m_noiseUniform;

		float m_occlusionRadius;
		float m_maxOcclusionDistance;

	};
}

#endif
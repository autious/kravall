#ifndef SRC_GFX_GLOBAL_ILLUMINATION_RENDERER_GI_PAINTER_HPP
#define SRC_GFX_GLOBAL_ILLUMINATION_RENDERER_GI_PAINTER_HPP

#include "../BasePainter.hpp"
#include <Shaders/ShaderManager.hpp>
#include <Buffers/UniformBufferManager.hpp>
#include "../../Textures/TextureManager.hpp"
#include "../../Textures/LUTManager.hpp"
#include "../DeferredRenderer/FBOTexture.hpp"

namespace GFX
{
	class GIPainter : public BasePainter
	{
	public:
		GIPainter(ShaderManager* shaderManager, UniformBufferManager* uniformBufferManager);

		~GIPainter();

		/*!
		Initialization function which sets the dummyVAO and FBO for later use in the painter.
		Loads all shaders associated with deferred rendering.
		\param FBO ID of FBO used for rendertargets
		\param dummyVAO ID of an empty VAO used for screenspace rendering
		*/
		void Initialize(GLuint FBO, GLuint dummyVAO, int screenWidth, int screenHeight);

		/*!
		Main rendering loop
		*/
		void Render(const double& delta, FBOTexture* normalDepth, FBOTexture* diffuse, FBOTexture* specular );

		void Resize(int width, int height);

	private:
		GLuint m_radianceTexture;

		int m_screenWidth;
		int m_screenHeight;

	};
}

#endif
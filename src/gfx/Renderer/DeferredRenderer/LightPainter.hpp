#ifndef SRC_GFX_RENDERER_LIGHT_PAINTER_HPP
#define SRC_GFX_RENDERER_LIGHT_PAINTER_HPP

#include "../BasePainter.hpp"
#include <Shaders/ShaderManager.hpp>

#include "../RenderJobManager.hpp"
#include "../../Textures/TextureManager.hpp"

#include "FBOTexture.hpp"
#include "../../Buffers/UniformBufferManager.hpp"
#include <GL/glew.h>
#include <vector>
#include <gfx/Lights.hpp>

namespace GFX
{
	class LightPainter : public BasePainter
	{
	public:

		/*!
		Set ShaderManager and BufferManager pointers of the painter to point to the managers within the rendering core
		\param shaderManager Pointer to ShaderManager present in RenderCore
		\param bufferManager Pointer to BufferManager present in RenderCore
		*/
		LightPainter(ShaderManager* shaderManager, UniformBufferManager* uniformBufferManager, RenderJobManager* renderJobManager);

		~LightPainter();

		/*!
		Initialization function which sets the dummyVAO and FBO for later use in the painter.
		Loads all shaders associated with deferred rendering.
		\param FBO ID of FBO used for rendertargets
		\param dummyVAO ID of an empty VAO used for screenspace rendering
		*/
		void Initialize(GLuint FBO, GLuint dummyVAO);

		/*!
		Main deferred rendering loop
		\param normalDepth Rendertarget for normals and depth
		\param diffuse Rendertarget for diffuse
		\param specular Rendertarget for specular
		\param glowMatID Rendertarget for glow and materialID
		*/
		void Render(FBOTexture* depthBuffer, FBOTexture* normalDepth, FBOTexture* diffuse, FBOTexture* specular, FBOTexture* glowMatID, glm::mat4 viewMatrix, glm::mat4 projMatrix);

	private:
		RenderJobManager* m_renderJobManager;

		GLint alphaUniform;
		GLint matrixUniform;
		GLint textureUniform;
		GLuint textureHandle;

		GLuint m_pointLightBuffer;

		const unsigned int m_maximumLights = 4096;
		PointLight* m_pointLights;

	};
}

#endif

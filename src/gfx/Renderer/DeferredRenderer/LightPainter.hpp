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
#include <gfx/LightData.hpp>

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
		void Initialize(GLuint FBO, GLuint dummyVAO, int screenWidth, int screenHeight);

		/*!
		Main deferred rendering loop
		\param normalDepth Rendertarget for normals and depth
		\param diffuse Rendertarget for diffuse
		\param specular Rendertarget for specular
		\param glowMatID Rendertarget for glow and materialID
		*/
		void Render(unsigned int& renderIndex, FBOTexture* depthBuffer, FBOTexture* normalDepth, FBOTexture* diffuse, FBOTexture* specular, FBOTexture* glowMatID, FBOTexture* SSDOTexture,
			FBOTexture* shadowMap, glm::mat4 viewMatrix, glm::mat4 projMatrix, float exposure, float gamma, glm::vec3 whitePoint, GLuint& toneMappedTexture);

		void Resize(int screenWidth, int screenHeight);

	private:
		RenderJobManager* m_renderJobManager;

		GLint m_alphaUniform;
		GLint m_invProjViewUniform;
		GLint m_invProjUniform;
		GLint m_viewUniform;
		GLint m_projUniform;
		GLint m_eyePosUniform;

		GLint m_gammaUniform;
		GLint m_exposureUniform;
		GLint m_whitePointUniform;

		GLint m_frambufferSizeUniform;
		GLint m_numActiveLightsUniform;
		GLint m_textureUniform;
		GLint m_shadowMapUniform;

		GLuint m_textureHandle;

		GLint m_numPointLightsUniform;
		GLint m_numSpotLightsUniform;
		GLint m_numDirLightsUniform;
		GLint m_numAmbientLightsUniform;

		GLint m_numPointShadowsUniform;
		GLint m_numSpotShadowsUniform;
		GLint m_numDirShadowsUniform;


		GLuint m_pointLightBuffer;
		GLuint m_shadowDataBuffer;

		const unsigned int MAXIMUM_LIGHTS = 1024;
		const unsigned int WORK_GROUP_SIZE = 16;
		LightData* m_lights;

		int m_screenWidth;
		int m_screenHeight;
	};
}

#endif

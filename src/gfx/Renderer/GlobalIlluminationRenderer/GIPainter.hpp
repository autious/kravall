#ifndef SRC_GFX_GLOBAL_ILLUMINATION_RENDERER_GI_PAINTER_HPP
#define SRC_GFX_GLOBAL_ILLUMINATION_RENDERER_GI_PAINTER_HPP

#include "../BasePainter.hpp"
#include <Shaders/ShaderManager.hpp>
#include <Buffers/UniformBufferManager.hpp>
#include "../../Textures/TextureManager.hpp"
#include "../../Textures/LUTManager.hpp"
#include "../DeferredRenderer/FBOTexture.hpp"
#include "../RenderJobManager.hpp"

namespace GFX
{
	class GIPainter : public BasePainter
	{
	public:
		GIPainter(ShaderManager* shaderManager, UniformBufferManager* uniformBufferManager, RenderJobManager* renderJobManager);

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
		void Render(const double& delta, FBOTexture* normalDepth, FBOTexture* diffuse, glm::mat4 viewMatrix, glm::mat4 projMatrix);

		void Resize(int width, int height);

		FBOTexture* m_SSDOTexture;
	private:
		void InitFBO();

		void BindSSDOFBO();

		void RenderSSDO(FBOTexture* normalDepth, FBOTexture* diffuse, glm::mat4 invViewProj, glm::mat4 viewMatrix, glm::mat4 projMatrix);

		RenderJobManager* m_renderJobManager;

		int m_screenWidth;
		int m_screenHeight;

		GLuint m_SSDOFBO;

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
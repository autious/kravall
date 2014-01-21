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

		FBOTexture* m_radianceTexture;
		FBOTexture* m_SSDOTexture;


	private:
		void InitFBO();

		void GenerateSeedTexture();
		float HaltonNumber(const int base, int index);

		void BindRadianceFBO();
		void BindSSDOFBO();

		void RenderRadiance(FBOTexture* normalDepth, FBOTexture* diffuse, glm::mat4 invViewProj);
		void RenderSSDO(FBOTexture* normalDepth, FBOTexture* diffuse, glm::mat4 invViewProj, glm::mat4 viewMatrix, glm::mat4 projMatrix);

		RenderJobManager* m_renderJobManager;

		int m_screenWidth;
		int m_screenHeight;

		GLuint m_radianceFBO;
		GLuint m_SSDOFBO;

		GLuint m_seedTexture;

	
		//Radiance uniforms
		GLint m_radDiffuseUniform;
		GLint m_radNormalsDepthUniform;
		GLint m_lightDirUniform;
		GLint m_lightDiffuseUniform;
		GLint m_invViewProjUniform;

		//SSDO variables
		float m_strength;
		float m_singularity;
		float m_depthBias;
		float m_bounceStrength;
		float m_bounceSingularity;
		float m_sampleRadius;
		float m_lightRotationAngle;
		float m_sampleCount;
		float m_maxSampleCount;
		float m_patternSize;
		float m_kernelSize;
		float m_positionThreshold;
		float m_normalThreshold;
		float m_maxRadiance;

		//SSDO uniforms
		GLint m_ssdoDiffuseUniform;
		GLint m_ssdoNormals_depthUniform;
		GLint m_radianceUniform;

		GLint m_sampleRadiusUniform;

		GLint m_viewMatrixUniform;
		GLint m_projMatrixUniform;
		GLint m_ssdoInvViewProjUniform;

		GLint m_strengthUniform;
		GLint m_singularityUniform;
		GLint m_depthBiasUniform;
		GLint m_bounceStrengthUniform;
		GLint m_bounceSingularityUniform;

		GLint m_sampleCountUniform;
		GLint m_patternSizeUniform;
		GLint m_seedTextureUniform;
		GLint m_lightRotationUniform;

	};
}

#endif
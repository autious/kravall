#ifndef SRC_GFX_RENDERER_DEFERRED_PAINTER_HPP
#define SRC_GFX_RENDERER_DEFERRED_PAINTER_HPP

#include "BasePainter.hpp"
#include <Shaders/ShaderManager.hpp>

#include "FBOTexture.hpp"
#include "../Buffers/UniformBufferManager.hpp"
#include <GL/glew.h>
#include <vector>
#include <gfx/Material.hpp>
#include "../Textures/Texture.hpp"
namespace GFX
{
	class DeferredPainter : public BasePainter
	{
	public:

		/*!
		Set ShaderManager and BufferManager pointers of the painter to point to the managers within the rendering core
		\param shaderManager Pointer to ShaderManager present in RenderCore
		\param bufferManager Pointer to BufferManager present in RenderCore
		*/
		DeferredPainter(ShaderManager* shaderManager, UniformBufferManager* uniformBufferManager);

		~DeferredPainter();

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
		void Render(FBOTexture* normalDepth, FBOTexture* diffuse, FBOTexture* specular, FBOTexture* glowMatID, glm::mat4 viewMatrix, glm::mat4 projMatrix);
		
		void AddRenderJob(const GLuint& ibo, const GLuint& vao, const int& size, const unsigned int& shader, glm::mat4* mMatrix, Material* mat);

	private:
		struct RenderJob
		{
			GLuint VAO;
			GLuint IBO;
			GLuint IBOSize;
			unsigned int shaderID;
			glm::mat4* modelMatrix;
			Material* material;
		};
		/*!
		Binds all the rendertargets to the FBO and GPU
		\param normalDepth Rendertarget for normals and depth
		\param diffuse Rendertarget for diffuse
		\param specular Rendertarget for specular
		\param glowMatID Rendertarget for glow and materialID
		*/
		void BindGBuffer(FBOTexture* normalDepth, FBOTexture* diffuse, FBOTexture* specular, FBOTexture* glowMatID);

		GLuint m_diffuseUniform;
		GLuint m_normalUniform;
		GLuint m_specularUniform;
		GLuint m_glowUniform;

		std::vector<RenderJob> m_renderJobs;

	
	};
}

#endif

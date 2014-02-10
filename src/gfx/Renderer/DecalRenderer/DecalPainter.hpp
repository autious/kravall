#ifndef SRC_GFX_RENDERER_DECAL_PAINTER_HPP
#define SRC_GFX_RENDERER_DECAL_PAINTER_HPP

#include "../BasePainter.hpp"
#include "../DeferredRenderer/FBOTexture.hpp"
#include <GL/glew.h>
#include <vector>

#include <Animation/AnimationManagerGFX.hpp>

namespace GFX
{
	class InstanceData;
	class MeshManager;
	class TextureManager;
	class MaterialManager;
	class UniformBufferManager;
	class RenderJobManager;
	class ShaderManager;
}

namespace GFX
{
	class DecalPainter : public BasePainter
	{
	public:

		/*!
		Set ShaderManager and BufferManager pointers of the painter to point to the managers within the rendering core
		\param shaderManager Pointer to ShaderManager present in RenderCore
		\param bufferManager Pointer to BufferManager present in RenderCore
		*/
		DecalPainter(ShaderManager* shaderManager, UniformBufferManager* uniformBufferManager, RenderJobManager* renderJobManager,
			MeshManager* meshManager, TextureManager* textureManager, MaterialManager* materialManager);

		~DecalPainter();

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
		void Render(AnimationManagerGFX* animationManager, unsigned int& renderIndex, FBOTexture* depthBuffer, FBOTexture* normalDepth,
			FBOTexture* diffuse, FBOTexture* specular, FBOTexture* glowMatID, glm::mat4 viewMatrix, glm::mat4 projMatrix, const float& gamma);

	private:

		/*!
		Binds all the rendertargets to the FBO and GPU
		\param normalDepth Rendertarget for normals and depth
		\param diffuse Rendertarget for diffuse
		\param specular Rendertarget for specular
		\param glowMatID Rendertarget for glow and materialID
		*/
		void BindGBuffer(FBOTexture* depthBuffer, FBOTexture* normalDepth, FBOTexture* diffuse, FBOTexture* specular, FBOTexture* glowMatID);


		RenderJobManager* m_renderJobManager;
		MeshManager* m_meshManager;
		MaterialManager* m_materialManager;
		TextureManager* m_textureManager;

		InstanceData* m_staticInstances;
		const unsigned int MAX_INSTANCES = 1024;
		GLuint m_instanceBuffer;
	};
}

#endif

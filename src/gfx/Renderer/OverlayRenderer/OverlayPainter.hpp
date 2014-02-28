#ifndef SRC_GFX_RENDERER_OVERLAYRENDERER_OVERLAYPAINTER_H
#define SRC_GFX_RENDERER_OVERLAYRENDERER_OVERLAYPAINTER_H

#include "../BasePainter.hpp"
#include <Shaders/ShaderManager.hpp>

#include "../RenderJobManager.hpp"

#include "../../Buffers/UniformBufferManager.hpp"
#include <GL/glew.h>
#include <vector>
#include "../../Textures/TextureManager.hpp"
#include "../../Buffers/MeshManager.hpp"
#include "../../Material/MaterialManager.hpp"

namespace GFX
{
	class OverlayPainter : public BasePainter
	{
	public:

		/*!
		Set ShaderManager and BufferManager pointers of the painter to point to the managers within the rendering core
		\param shaderManager Pointer to ShaderManager present in RenderCore
		\param uniformBufferManager Pointer to uniformBufferManager present in RenderCore
		*/
	OverlayPainter(ShaderManager* shaderManager, UniformBufferManager* uniformBufferManager, RenderJobManager* renderJobManager, MeshManager* meshManager, TextureManager* textureManager, MaterialManager* materialManager);

		~OverlayPainter();

		/*!
		Initialization function which sets the dummyVAO and FBO for later use in the painter.
		Loads all shaders associated with debug rendering.
		\param FBO ID of FBO used for rendertargets
		\param dummyVAO ID of an empty VAO used for screenspace rendering
		*/
		void Initialize(GLuint FBO, GLuint dummyVAO);

		/*!
		Main console rendering loop
		*/
        void Render( unsigned int& renderIndex, glm::mat4& viewMatrix, glm::mat4& projectionMatrix, RenderInfo& out_RenderInfo );

	private:

		GLuint m_uniformTexture0;
        GLuint m_modelMatrixUniform;
        GLuint m_viewMatrixUniform;
        GLuint m_projectionMatrixUniform;

		RenderJobManager* m_renderJobManager;
		MeshManager* m_meshManager;
		MaterialManager* m_materialManager;
		TextureManager* m_textureManager;
    };
}
#endif

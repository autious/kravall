#ifndef SRC_GFX_RENDERER_OVERLAYRENDERER_BOXPAINTER_H
#define SRC_GFX_RENDERER_OVERLAYRENDERER_BOXPAINTER_H

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
	class BoxPainter : public BasePainter
	{
	public:

		/*!
		Set ShaderManager and BufferManager pointers of the painter to point to the managers within the rendering core
		\param shaderManager Pointer to ShaderManager present in RenderCore
		\param uniformBufferManager Pointer to uniformBufferManager present in RenderCore
		*/
		BoxPainter(ShaderManager* shaderManager, UniformBufferManager* uniformBufferManager, RenderJobManager* renderJobManager, MeshManager* meshManager);

		~BoxPainter();

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
        void Render( glm::vec4& posdim, glm::vec4& color );

	private:

        GLuint m_pointUniform;
		GLuint m_colorUniform;
        GLuint m_viewMatrixUniform;
        GLuint m_projectionMatrixUniform;

		RenderJobManager* m_renderJobManager;
		MeshManager* m_meshManager;
    };
}
#endif

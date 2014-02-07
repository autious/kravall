#ifndef SRC_GFX_RENDERER_SHADOW_PAINTER_HPP
#define SRC_GFX_RENDERER_SHADOW_PAINTER_HPP

#include "../BasePainter.hpp"
#include <Shaders/ShaderManager.hpp>
#include "../RenderJobManager.hpp"

#include "FBOTexture.hpp"
#include "../../Buffers/UniformBufferManager.hpp"
#include <GL/glew.h>
#include <vector>
#include "../../Buffers/MeshManager.hpp"
#include "../PostProcessing/BlurPainter.hpp"
#include <gfx/InstanceData.hpp>
#include <Animation/AnimationManager.hpp>
#include "../Defines.hpp"

namespace GFX
{
	class ShadowPainter : public BasePainter
	{
	public:

		/*!
		Set ShaderManager and BufferManager pointers of the painter to point to the managers within the rendering core
		\param shaderManager Pointer to ShaderManager present in RenderCore
		\param bufferManager Pointer to BufferManager present in RenderCore
		*/
		ShadowPainter(ShaderManager* shaderManager, UniformBufferManager* uniformBufferManager, RenderJobManager* renderJobManager, 
			MeshManager* meshManager);

		~ShadowPainter();

		/*!
		Initialization function which sets the dummyVAO and FBO for later use in the painter.
		Loads all shaders associated with deferred rendering.
		\param FBO ID of FBO used for rendertargets
		\param dummyVAO ID of an empty VAO used for screenspace rendering
		*/
		void Initialize(GLuint FBO, GLuint dummyVAO, BlurPainter* blurPainter);

		/*!
		Main rendering function
		*/
		void Render(AnimationManager* animationManager, const unsigned int& renderIndex, glm::mat4 viewMatrix, glm::mat4 projMatrix,
			const unsigned int& geometryStartIndex, const unsigned int& geometryEndIndex, FBOTexture* shadowMap, const unsigned int& width, const unsigned int& height);

	private:

		GLuint m_viewProjUniform;

		RenderJobManager* m_renderJobManager;
		MeshManager* m_meshManager;

		InstanceData* m_staticInstances;
		GLuint m_instanceBuffer;

		GLuint m_matricesUniform;
	};
}

#endif

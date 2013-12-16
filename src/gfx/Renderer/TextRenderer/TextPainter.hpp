#ifndef SRC_GFX_RENDERER_TEXT_RENDERER_TEXT_PAINTER_HPP
#define SRC_GFX_RENDERER_TEXT_RENDERER_TEXT_PAINTER_HPP

#include "../BasePainter.hpp"
#include <Shaders/ShaderManager.hpp>
#include "TextManager.hpp"
#include "../DeferredRenderer/FBOTexture.hpp"
#include <Buffers/UniformBufferManager.hpp>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <Buffers/Text.hpp>
#include <FontData.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace GFX
{
	class TextPainter : public BasePainter
	{
	public:

		/*!
		Set ShaderManager and BufferManager pointers of the painter to point to the managers within the rendering core
		\param shaderManager Pointer to ShaderManager present in RenderCore
		\param bufferManager Pointer to BufferManager present in RenderCore
		*/
		TextPainter(ShaderManager* shaderManager, UniformBufferManager* uniformBufferManager);

		~TextPainter();

		/*!
		Initialization function which sets the dummyVAO and FBO for later use in the painter.
		Loads all shaders associated with deferred rendering.
		\param FBO ID of FBO used for rendertargets
		\param dummyVAO ID of an empty VAO used for screenspace rendering
		*/
		void Initialize(GLuint FBO, GLuint dummyVAO);

		/*!
		Main text rendering loop
		*/
		void Render();

	private:

		struct TextVertex
		{
			glm::vec4 pos_dim;
			glm::vec4 uv;
		};
		GLuint m_textVBO;
		GLuint m_textVAO;
		
		GLuint m_colorUniform;
		GLuint m_textureUniform;

		void RenderText(const char* text, GFX::FontData* fontData, float x, float y, float sx, float sy, glm::vec4 color);

		std::vector<Text> m_text;
	};
}

#endif

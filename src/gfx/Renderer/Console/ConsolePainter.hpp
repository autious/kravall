#ifndef SRC_GFX_RENDERER_CONSOLE_PAINTER_HPP
#define SRC_GFX_RENDERER_CONSOLE_PAINTER_HPP

#include "../BasePainter.hpp"
#include <Shaders/ShaderManager.hpp>
#include "../DebugRenderer/DebugManager.hpp"

#include "../FBOTexture.hpp"
#include "../../Buffers/UniformBufferManager.hpp"
#include <GL/glew.h>

namespace GFX
{
	class ConsolePainter : public BasePainter
	{
	public:

		/*!
		Set ShaderManager and BufferManager pointers of the painter to point to the managers within the rendering core
		\param shaderManager Pointer to ShaderManager present in RenderCore
		\param bufferManager Pointer to BufferManager present in RenderCore
		\param uniformBufferManager Pointer to uniformBufferManager present in RenderCore
		*/
		ConsolePainter(ShaderManager* shaderManager, BufferManager* bufferManager, UniformBufferManager* uniformBufferManager);

		~ConsolePainter();

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
		void Render();

		void SetConsoleVisible(bool visible);
		bool GetConsoleVisible();
		DebugRect GetConsoleRect();

		void SetConsoleHeight(int height);

	private:

		// Uniforms
		GLuint m_rectPosUniform;
		GLuint m_rectDimUniform;
		GLuint m_rectColorUniform;

		GLuint m_screenSizeUniform;

		bool m_showConsole;

		DebugRect m_consoleRect;
		DebugRect m_consoleInputRect;
		int m_consoleHeight;
		glm::vec4 m_consoleColor;
	};
}

#endif

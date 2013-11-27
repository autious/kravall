#include <iostream>

#include <Renderer/RenderCore.hpp>
#include <Shaders/ShaderManager.hpp>

#include <GFXInterface.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

void APIENTRY glErrorCallback(GLenum _source, GLenum _type, GLuint _id, GLenum _severity, GLsizei _length, const char* _message, void* _userParam)
{
	const char* source = "";

	if ( _source == GL_DEBUG_SOURCE_API_ARB )
    {
		source = "The GL";
    }
	else if (_source == GL_DEBUG_SOURCE_SHADER_COMPILER_ARB)
    {
		source = "The GLSL Shader compiler";
    }
	else if (_source == GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB)
    {
		source = "The source window system arb";
    }

	std::cout << "OPENGL_ERROR: id:" << source << ":" << _type << ":" << _message << std::endl;
}

namespace GFX
{

	int Init(int windowWidth, int windowHeight)
	{
		// TODO: Init graphics engine
		GLenum err;

		err = glGetError();
		glClearColor(100.0f / 255.0f, 149.0f / 255.0f, 237.0f / 255.0f, 1.0f);

		// assign callback functions
		glDebugMessageCallbackARB(glErrorCallback, NULL);
		err = glGetError();

		glEnable(GL_DEPTH_TEST);
		err = glGetError();
		glDepthFunc(GL_LESS);
		err = glGetError();
		glEnable(GL_CULL_FACE);
		err = glGetError();

		glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

		glCullFace(GL_BACK);
		err = glGetError();
		glFrontFace(GL_CW);
		err = glGetError();

		//Initialize RenderCore
		Renderer().Initialize(windowWidth, windowHeight);

		return GFX_SUCCESS;
	}

	void Resize(int width, int height)
	{
		Renderer().Resize(width, height);
	}

	void Render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Renderer().Render();
	}

	void SetViewMatrix(GFXMat4x4 matrix)
	{
		Renderer().SetViewMatrix(matrix);
	}

	void SetProjectionMatrix(GFXMat4x4 matrix)
	{
		Renderer().SetProjMatrix(matrix);
	}

}

// Separated for debug interface
namespace GFX
{
	void Debug::DrawPoint(GFXVec2 point, GFXColor color)
	{
		Debug::DrawPoint(point, color, 1.0f);
	}
	void Debug::DrawPoint(GFXVec2 point, GFXColor color, float size)
	{
		DebugPoint p;
		p.position = glm::vec3(
			point.x / float(Renderer().GetWindowWidth()/2) - 1.0f,
			1.0f - point.y / float(Renderer().GetWindowHeight()/2),
			0.0);
		p.size = size;
		p.color = color;
		DebugDrawing().AddPoint(p);
	}
}

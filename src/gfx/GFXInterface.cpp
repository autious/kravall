#include <iostream>


#include <Renderer/RenderCore.hpp>
#include <Shaders/ShaderManager.hpp>
#include <Buffers/MeshManager.hpp>
#include <gfx/Vertex.hpp>
#include <GFXInterface.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <logger/Logger.hpp>

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
		GLenum err = 0;

		err = glGetError();

        if( err )
        {
            LOG_ERROR << "Got GL error:" << err << std::endl;
            err = 0;
        }
		//glClearColor(100.0f / 255.0f, 149.0f / 255.0f, 237.0f / 255.0f, 1.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		// assign callback functions
		glDebugMessageCallbackARB(glErrorCallback, NULL);
		err = glGetError();
        if( err )
        {
            LOG_ERROR << "Got GL error:" << err << std::endl;
            err = 0;
        }

		glEnable(GL_DEPTH_TEST);
		err = glGetError();
        if( err )
        {
            LOG_ERROR << "Got GL error:" << err << std::endl;
            err = 0;
        }

		glDepthFunc(GL_LESS);
		err = glGetError();
        if( err )
        {
            LOG_ERROR << "Got GL error:" << err << std::endl;
            err = 0;
        }

		glEnable(GL_CULL_FACE);
		err = glGetError();
        if( err )
        {
            LOG_ERROR << "Got GL error:" << err << std::endl;
            err = 0;
        }

		glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

		glCullFace(GL_BACK);
		err = glGetError();
        if( err )
        {
            LOG_ERROR << "Got GL error:" << err << std::endl;
            err = 0;
        }

		glFrontFace(GL_CCW);
		err = glGetError();
        if( err )
        {
            LOG_ERROR << "Got GL error:" << err << std::endl;
            err = 0;
        }

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

	void RenderText(GFX::FontData* fontData, GFXVec2 position, float size, GFXVec4 color, const char* text)
	{
		Text t(position.x, position.y, size, size, fontData, color, text, Renderer().GetWindowWidth(), Renderer().GetWindowHeight());
		GetTextManager().AddText(t);
	}
	void ShowConsole()
	{
		Renderer().SetConsoleVisible(true);
	}
	void HideConsole()
	{
		Renderer().SetConsoleVisible(false);
	}
	void ToggleConsole()
	{
		Renderer().SetConsoleVisible(!Renderer().GetConsoleVisible());
	}

	void RenderSplash(bool renderSplash)
	{
		Renderer().SetSplash(renderSplash);
	}

	void DeleteGFX()
	{
		Renderer().Delete();
	}

	void Draw(GFXBitmask bitmask, void* value)
	{
		Renderer().AddRenderJob(bitmask, value);
	}
	
	int GetScreenWidth()
	{
		return Renderer().GetWindowWidth();
	}
	int GetScreenHeight()
	{
		return Renderer().GetWindowHeight();
	}

}

namespace GFX
{
	namespace Content
	{
		void LoadTexture2DFromMemory(unsigned int& id, unsigned char* data, int width, int height)
		{
			Renderer().LoadTexture(id, data, width, height);
		}

		void DeleteTexture(unsigned int id)
		{
			Renderer().DeleteTexture(id);
		}

		void LoadStaticMesh(unsigned int& meshID, int& sizeVerts, int& sizeIndices, GFX::StaticVertex* verts, int* indices)
		{
			Renderer().LoadStaticMesh(meshID, sizeVerts, sizeIndices, verts, indices);
		}

		void DeleteStaticMesh(unsigned int& meshID)
		{
			Renderer().DeleteMesh(meshID);
		}

		void CreateMaterial(unsigned long long int& out_id)
		{
			Renderer().CreateMaterial(out_id);
		}

		void DeleteMaterial(const unsigned long long int& id)
		{
			Renderer().DeleteMaterial(id);
		}

		void AddTextureToMaterial(const unsigned long long int& materialID, const unsigned long long int& textureID)
		{
			Renderer().AddTextureToMaterial(materialID, textureID);
		}

		void RemoveTextureFromMaterial(const unsigned long long int& materialID, const unsigned long long int& textureID)
		{
			Renderer().RemoveTextureFromMaterial(materialID, textureID);
		}

		void AttachShaderToMaterial(const unsigned long long int& materialID, const unsigned int& shaderID)
		{
			Renderer().SetShaderToMaterial(materialID, shaderID);
		}
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

	void Debug::DrawLine(GFXVec3 p1, GFXVec3 p2, GFXColor color, bool useDepth) // TODO: IMPLEMENT DEPTH
	{
		Debug::DrawLine(p1, p2, color, 1.0f, useDepth);
	}

	void Debug::DrawLine(GFXVec3 p1, GFXVec3 p2, GFXColor color, float thickness, bool useDepth) // TODO: IMPLEMENT DEPTH
	{
		DebugLine l;
		l.color = color;
		l.start = p1;
		l.end = p2;
		l.thickness = thickness;
		l.useDepth = useDepth;
		DebugDrawing().AddLineWorld(l);
	}

	void Debug::DrawLine(GFXVec2 p1, GFXVec2 p2, GFXColor color)
	{
		Debug::DrawLine(p1, p2, color, 1.0f);
	}

	void Debug::DrawLine(GFXVec2 p1, GFXVec2 p2, GFXColor color, float thickness)
	{
		DebugLine l;
		l.color = color;
		l.start = glm::vec3(
			p1.x / float(Renderer().GetWindowWidth() / 2) - 1.0f,
			1.0f - p1.y / float(Renderer().GetWindowHeight() / 2),
			0.0);
		l.end = glm::vec3(
			p2.x / float(Renderer().GetWindowWidth() / 2) - 1.0f,
			1.0f - p2.y / float(Renderer().GetWindowHeight() / 2),
			0.0);
		l.thickness = thickness;
		DebugDrawing().AddLine(l);
	}

	void Debug::DrawRectangle(GFXVec2 position, GFXVec2 dimensions, 
		bool solid, GFXColor color)
	{
		DebugRect r;
		r.color = color;
		r.position = glm::vec3(
			position.x / float(Renderer().GetWindowWidth() / 2) - 1.0f,
			1.0f - position.y / float(Renderer().GetWindowHeight() / 2), 0.0f);
		r.dimensions = glm::vec3(
			dimensions.x / float(Renderer().GetWindowWidth())*2,
			dimensions.y / float(Renderer().GetWindowHeight())*2, 0.0f);
		DebugDrawing().AddRect(r, solid);
	}

	void Debug::DrawBox(GFXVec3 position, GFXVec3 dimensions, bool solid, GFXColor color, bool useDepth) // TODO: IMPLEMENT DEPTH
	{
		DebugBox b;
		b.color = color;
		b.position = position;
		b.dimensions = dimensions;
		b.useDepth = useDepth;
		DebugDrawing().AddBox(b, solid);
	}
	void Debug::DrawCircle(GFXVec2 position, float radius, unsigned int lineWidth, GFXColor color)
	{
		DebugRect c;
		c.position = glm::vec3(
			(position.x-radius) / float(Renderer().GetWindowWidth() / 2) - 1.0f,
			1.0f - (position.y+radius) / float(Renderer().GetWindowHeight() / 2), 0.0f);
		c.color = color;
		c.dimensions = glm::vec3(
			radius * 2 / float(Renderer().GetWindowWidth() / 2),
			radius * 2 / float(Renderer().GetWindowHeight() / 2), 
			radius);
		c.lineWidth = (lineWidth == 0) ? -1.0f : (float)lineWidth;
		DebugDrawing().AddCircle(c);
	}

	void Debug::DrawSphere(GFXVec3 position, float radius, GFXColor color, bool useDepth) // TODO: IMPLEMENT DEPTH
	{
		DebugSphere s;
		s.position = position;
		s.radius = radius;
		s.color = color;
		s.useDepth = useDepth;
		DebugDrawing().AddSphere(s);
	}

    void Debug::SetStatisticsFont(GFX::FontData* font)
    {
        Renderer().SetStatisticsFont(font);
    }

	void Debug::DisplaySystemInfo(bool enabled)
	{
		Renderer().ShowStatistics(enabled);
	}

	void Debug::DisplayFBO(int which)
	{
		Renderer().ShowFBO(which);
	}
}

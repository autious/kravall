#include "BoxPainter.hpp"

#include "../RenderCore.hpp"
#include <logger/Logger.hpp>
#include <utility/GLMStreams.hpp>

#include <InstanceData.hpp>

namespace GFX
{

	BoxPainter::BoxPainter(ShaderManager* shaderManager, UniformBufferManager* uniformBufferManager, RenderJobManager* renderJobManager, MeshManager* meshManager) : BasePainter(shaderManager, uniformBufferManager)
    {
		m_renderJobManager = renderJobManager;
		m_meshManager = meshManager;
    }

    BoxPainter::~BoxPainter()
    {

    }

    void BoxPainter::Initialize(GLuint FBO, GLuint dummyVAO)
    {
		BasePainter::Initialize(FBO, dummyVAO);

		// Load shared shaders (vertex and fragment)
		m_shaderManager->LoadShader("shaders/overlay/BoxVS.glsl", "BoxSelectVS", GL_VERTEX_SHADER);
		m_shaderManager->LoadShader("shaders/overlay/BoxGS.glsl", "BoxSelectGS", GL_GEOMETRY_SHADER);
		m_shaderManager->LoadShader("shaders/overlay/BoxFS.glsl", "BoxSelectFS", GL_FRAGMENT_SHADER);

		// Create and attach rect debug shaders
		m_shaderManager->CreateProgram("BoxSelect");
		m_shaderManager->AttachShader("BoxSelectVS", "BoxSelect");
		m_shaderManager->AttachShader("BoxSelectGS", "BoxSelect");
		m_shaderManager->AttachShader("BoxSelectFS", "BoxSelect");
		m_shaderManager->LinkProgram("BoxSelect");

		m_pointUniform = m_shaderManager->GetUniformLocation("BoxSelect", "point");
		m_colorUniform = m_shaderManager->GetUniformLocation("BoxSelect", "color");


		// Load shared shaders (vertex and fragment)
		m_shaderManager->LoadShader("shaders/debug_shaders/Debug.vertex", "FilledRectVS", GL_VERTEX_SHADER);
		m_shaderManager->LoadShader("shaders/debug_shaders/Debug.fragment", "FilledRectFS", GL_FRAGMENT_SHADER);

		// Load specific geometry shaders
		m_shaderManager->LoadShader("shaders/debug_shaders/DebugRect.geometry", "FilledRectGS", GL_GEOMETRY_SHADER);

		m_shaderManager->CreateProgram("FilledRect");
		m_shaderManager->AttachShader("FilledRectVS", "FilledRect");
		m_shaderManager->AttachShader("FilledRectGS", "FilledRect");
		m_shaderManager->AttachShader("FilledRectFS", "FilledRect");
		m_shaderManager->LinkProgram("FilledRect");

		m_rectPosUniform = m_shaderManager->GetUniformLocation("FilledRect", "pointPosition");
		m_rectDimUniform = m_shaderManager->GetUniformLocation("FilledRect", "pointPosition2");
		m_rectColorUniform = m_shaderManager->GetUniformLocation("FilledRect", "inColor");
    }

    void BoxPainter::Render( glm::vec4& posdim, glm::vec4& color, bool renderSelection )
    {
		BasePainter::Render();

		if (renderSelection)
		{
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glLineWidth(1);
			m_shaderManager->UseProgram("BoxSelect");
			m_shaderManager->SetUniform(1, posdim, m_pointUniform);
			m_shaderManager->SetUniform(1, color, m_colorUniform);

			glBindVertexArray(m_dummyVAO);
			glDrawArrays(GL_POINTS, 0, 1);

			m_shaderManager->ResetProgram();
			glEnable(GL_DEPTH_TEST);
			glDisable(GL_BLEND);
		}
		

		std::vector<FilledRect> rects = m_renderJobManager->GetFilledRects();

		BasicCamera bc;
		glDisable(GL_DEPTH_TEST);
		bc.viewMatrix = glm::mat4x4(1.0f);
		bc.projMatrix = glm::mat4x4(1.0f);
		m_uniformBufferManager->SetBasicCameraUBO(bc);

		BasePainter::ClearFBO();

		// Draw filled rectangles
		m_shaderManager->UseProgram("FilledRect");
		for (unsigned int i = 0; i < rects.size(); ++i)
		{
			FilledRect r = rects[i];

			m_shaderManager->SetUniform(1, r.color, m_rectColorUniform);
			m_shaderManager->SetUniform(1, r.position, m_rectPosUniform);
			m_shaderManager->SetUniform(1, r.dimensions, m_rectDimUniform);

			glDrawArrays(GL_POINTS, 0, 1);
		}
		m_shaderManager->ResetProgram();


    }
}

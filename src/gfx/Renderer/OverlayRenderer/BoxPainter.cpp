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
    }

    void BoxPainter::Render( glm::vec4& posdim, glm::vec4& color )
    {
		BasePainter::Render();

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

		m_shaderManager->UseProgram("BoxSelect");
		m_shaderManager->SetUniform(1, posdim, m_pointUniform);
		m_shaderManager->SetUniform(1, color, m_colorUniform);
		
		glBindVertexArray(m_dummyVAO);
		glDrawArrays(GL_POINTS, 0, 1);

		m_shaderManager->ResetProgram();
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
    }
}

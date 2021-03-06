#include "DebugPainter.hpp"
#include "DebugManager.hpp"

#include "../RenderCore.hpp"
#include <logger/Logger.hpp>
#include <Textures/TextureManager.hpp>

namespace GFX
{
	DebugPainter::DebugPainter(ShaderManager* shaderManager, UniformBufferManager* uniformBufferManager)
		: BasePainter(shaderManager, uniformBufferManager)
	{
	}

	DebugPainter::~DebugPainter()
	{
	}

	void DebugPainter::Initialize(GLuint FBO, GLuint dummyVAO)
	{
		BasePainter::Initialize(FBO, dummyVAO);


		// Load shared shaders (vertex and fragment)
		m_shaderManager->LoadShader("shaders/debug_shaders/Debug.vertex", "DebugVS", GL_VERTEX_SHADER);
		m_shaderManager->LoadShader("shaders/debug_shaders/Debug.fragment", "DebugFS", GL_FRAGMENT_SHADER);
		m_shaderManager->LoadShader("shaders/debug_shaders/DebugCircle.fragment", "DebugCircleFS", GL_FRAGMENT_SHADER);

		// Load specific geometry shaders
		m_shaderManager->LoadShader("shaders/debug_shaders/DebugLine.geometry", "DebugLineGS", GL_GEOMETRY_SHADER);
		m_shaderManager->LoadShader("shaders/debug_shaders/DebugRect.geometry", "DebugRectGS", GL_GEOMETRY_SHADER);
		m_shaderManager->LoadShader("shaders/debug_shaders/DebugBox.geometry", "DebugBoxGS", GL_GEOMETRY_SHADER);

		// Create and attach point debug shaders
		m_shaderManager->CreateProgram("DebugPoint");
		m_shaderManager->AttachShader("DebugVS", "DebugPoint");
		m_shaderManager->AttachShader("DebugFS", "DebugPoint");
		m_shaderManager->LinkProgram("DebugPoint");

		m_pointPositionUniform = m_shaderManager->GetUniformLocation("DebugPoint", "pointPosition");
		m_pointColorUniform = m_shaderManager->GetUniformLocation("DebugPoint", "inColor");
		m_pointSizeUniform = m_shaderManager->GetUniformLocation("DebugPoint", "pointSize");

		// Create and attach circle debug shaders
		m_shaderManager->CreateProgram("DebugCircle");
		m_shaderManager->AttachShader("DebugVS", "DebugCircle");
		m_shaderManager->AttachShader("DebugRectGS", "DebugCircle");
		m_shaderManager->AttachShader("DebugCircleFS", "DebugCircle");
		m_shaderManager->LinkProgram("DebugCircle");

		m_circlePosUniform = m_shaderManager->GetUniformLocation("DebugCircle", "pointPosition");
		m_circleDimUniform = m_shaderManager->GetUniformLocation("DebugCircle", "pointPosition2");
		m_circleColorUniform = m_shaderManager->GetUniformLocation("DebugCircle", "inColor");
		m_screenSizeUniform = m_shaderManager->GetUniformLocation("DebugCircle", "screenSizeAndLinewidth");

		// Create and attach line debug shaders
		m_shaderManager->CreateProgram("DebugLine");
		m_shaderManager->AttachShader("DebugVS", "DebugLine");
		m_shaderManager->AttachShader("DebugLineGS", "DebugLine");
		m_shaderManager->AttachShader("DebugFS", "DebugLine");
		m_shaderManager->LinkProgram("DebugLine");

		m_lineStartUniform = m_shaderManager->GetUniformLocation("DebugLine", "pointPosition");
		m_lineEndUniform = m_shaderManager->GetUniformLocation("DebugLine", "pointPosition2");
		m_lineColorUniform = m_shaderManager->GetUniformLocation("DebugLine", "inColor");

		// Create and attach rect debug shaders
		m_shaderManager->CreateProgram("DebugRect");
		m_shaderManager->AttachShader("DebugVS", "DebugRect");
		m_shaderManager->AttachShader("DebugRectGS", "DebugRect");
		m_shaderManager->AttachShader("DebugFS", "DebugRect");
		m_shaderManager->LinkProgram("DebugRect");

		m_rectPosUniform = m_shaderManager->GetUniformLocation("DebugRect", "pointPosition");
		m_rectDimUniform = m_shaderManager->GetUniformLocation("DebugRect", "pointPosition2");
		m_rectColorUniform = m_shaderManager->GetUniformLocation("DebugRect", "inColor");

		// Create and attach box debug shaders
		m_shaderManager->CreateProgram("DebugBox");
		m_shaderManager->AttachShader("DebugVS", "DebugBox");
		m_shaderManager->AttachShader("DebugBoxGS", "DebugBox");
		m_shaderManager->AttachShader("DebugFS", "DebugBox");
		m_shaderManager->LinkProgram("DebugBox");
		
		m_boxPosUniform = m_shaderManager->GetUniformLocation("DebugBox", "pointPosition");
		m_boxDimUniform = m_shaderManager->GetUniformLocation("DebugBox", "pointPosition2");
		m_boxColorUniform = m_shaderManager->GetUniformLocation("DebugBox", "inColor");

        m_uniformBufferManager->SetUniformBlockBindingIndex(m_shaderManager->GetShaderProgramID("DebugBox"), "PerFrameBlock", UniformBufferManager::CAMERA_BINDING_INDEX);
	}

	void DebugPainter::Render(FBOTexture* depthBuffer, FBOTexture* color, glm::mat4 viewMatrix, glm::mat4 projMatrix, bool draw)
	{
		BasePainter::Render();

		if (DebugDrawing().ShouldRender() && draw)
		{

			glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);


			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer->GetTextureHandle(), 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color->GetTextureHandle(), 0);

			GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
			glDrawBuffers(1, drawBuffers);

			glClear(GL_COLOR_BUFFER_BIT);

			//DetachTextures();
        

			glBindVertexArray(m_dummyVAO);
			glDisable(GL_PROGRAM_POINT_SIZE);
			glEnable(GL_BLEND);
            

			// Draw 3D
			//Disable depth for debug
			//glDisable(GL_DEPTH_TEST);

			BasicCamera bc;
			bc.viewMatrix = viewMatrix;
			bc.projMatrix = projMatrix;

			m_uniformBufferManager->SetBasicCameraUBO(bc);

			// Draw boxes
			m_shaderManager->UseProgram("DebugBox");
			for (unsigned int i = 0; i < DebugDrawing().GetFilledBoxes().size(); ++i)
			{
				DebugBox b = DebugDrawing().GetFilledBoxes()[i];

				m_shaderManager->SetUniform(1, b.color, m_boxColorUniform);
				m_shaderManager->SetUniform(1, b.position, m_boxPosUniform);
				m_shaderManager->SetUniform(1, b.dimensions, m_boxDimUniform);
				
				if (b.useDepth)
					glEnable(GL_DEPTH_TEST);
				else
					glDisable(GL_DEPTH_TEST); 

				glDrawArrays(GL_POINTS, 0, 1);
			}
			m_shaderManager->ResetProgram();
			

			// Draw lines3D
			m_shaderManager->UseProgram("DebugLine");
			for (unsigned int i = 0; i < DebugDrawing().GetLinesWorld().size(); ++i)
			{
				DebugLine dl = DebugDrawing().GetLinesWorld()[i];

				m_shaderManager->SetUniform(1, dl.color, m_lineColorUniform);
				m_shaderManager->SetUniform(1, dl.start, m_lineStartUniform);
				m_shaderManager->SetUniform(1, dl.end, m_lineEndUniform);

				glLineWidth(dl.thickness);

				if (dl.useDepth)
					glEnable(GL_DEPTH_TEST);
				else
					glDisable(GL_DEPTH_TEST); 

				glDrawArrays(GL_POINTS, 0, 1);
			}
			glLineWidth(1.0f);
			m_shaderManager->ResetProgram();

			// Draw 2D
			glDisable(GL_DEPTH_TEST); 
			bc.viewMatrix = glm::mat4x4(1.0f);
			bc.projMatrix = glm::mat4x4(1.0f);
			m_uniformBufferManager->SetBasicCameraUBO(bc);
			
			// Draw lines
			m_shaderManager->UseProgram("DebugLine");

			for (unsigned int i = 0; i < DebugDrawing().GetLines().size(); ++i)
			{
				DebugLine dl = DebugDrawing().GetLines()[i];

				m_shaderManager->SetUniform(1, dl.color, m_lineColorUniform);
				m_shaderManager->SetUniform(1, dl.start, m_lineStartUniform);
				m_shaderManager->SetUniform(1, dl.end, m_lineEndUniform);

				glLineWidth(dl.thickness);
				glDrawArrays(GL_POINTS, 0, 1);
			}
			glLineWidth(1.0f);
			m_shaderManager->ResetProgram();

			BasePainter::ClearFBO();

			// Draw filled rectangles
			m_shaderManager->UseProgram("DebugRect");
			for (unsigned int i = 0; i < DebugDrawing().GetFilledRects().size(); ++i)
			{
				DebugRect r = DebugDrawing().GetFilledRects()[i];

				m_shaderManager->SetUniform(1, r.color, m_rectColorUniform);
				m_shaderManager->SetUniform(1, r.position, m_rectPosUniform);
				m_shaderManager->SetUniform(1, r.dimensions, m_rectDimUniform);

				glDrawArrays(GL_POINTS, 0, 1);
			}
			m_shaderManager->ResetProgram();

			// Draw filled circles
			m_shaderManager->UseProgram("DebugCircle");
			for (unsigned int i = 0; i < DebugDrawing().GetFilledCircles().size(); ++i)
			{
				DebugRect r = DebugDrawing().GetFilledCircles()[i];

				m_shaderManager->SetUniform(1, r.color, m_circleColorUniform);
				m_shaderManager->SetUniform(1, r.position, m_circlePosUniform);
				m_shaderManager->SetUniform(1, r.dimensions, m_circleDimUniform);
				m_shaderManager->SetUniform(1,
					glm::vec3(Renderer().GetWindowWidth(), Renderer().GetWindowHeight(), r.lineWidth),
					m_screenSizeUniform);


				glDrawArrays(GL_POINTS, 0, 1);
			}
			m_shaderManager->ResetProgram();

			// Draw points
			m_shaderManager->UseProgram("DebugPoint");
			for (unsigned int i = 0; i < DebugDrawing().GetPoints().size(); ++i)
			{
				DebugPoint dp = DebugDrawing().GetPoints()[i];

				m_shaderManager->SetUniform(1, dp.color, m_pointColorUniform);
				m_shaderManager->SetUniform(1, dp.position, m_pointPositionUniform);
				m_shaderManager->SetUniform(dp.size, m_pointSizeUniform);

				glPointSize(dp.size);
				glDrawArrays(GL_POINTS, 0, 1);
			}
			glPointSize(1.0f);
			m_shaderManager->ResetProgram();


			m_shaderManager->UseProgram("TQ");

			glDisable(GL_DEPTH_TEST);
			glDepthMask(GL_FALSE);

			m_shaderManager->SetUniform(1.0f, m_shaderManager->GetUniformLocation("TQ", "alphaIN"));
			TextureManager::BindTexture(color->GetTextureHandle(), m_shaderManager->GetUniformLocation("TQ", "textureIN"), 0, GL_TEXTURE_2D);

			glBindVertexArray(m_dummyVAO);
			glDrawArrays(GL_POINTS, 0, 1);

			m_shaderManager->ResetProgram();

			glEnable(GL_DEPTH_TEST);
			glDepthMask(GL_TRUE);
			glDisable(GL_BLEND);

			BasePainter::ClearFBO();

			TextureManager::UnbindTexture();

			//DebugDrawing().Clear();
		}
		
		DebugDrawing().Clear();
		

	}


	void DebugPainter::DetachTextures()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, 0, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, 0, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, 0, 0);

		// define outputs
		GLenum drawBuffers[] = { GL_NONE, GL_NONE, GL_NONE, GL_NONE };
		glDrawBuffers(4, drawBuffers);
	}
}

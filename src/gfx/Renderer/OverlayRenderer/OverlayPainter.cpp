#include "OverlayPainter.hpp"

#include "../RenderCore.hpp"
#include <logger/Logger.hpp>

namespace GFX
{

	OverlayPainter::OverlayPainter(ShaderManager* shaderManager, UniformBufferManager* uniformBufferManager, RenderJobManager* renderJobManager, MeshManager* meshManager, TextureManager* textureManager, MaterialManager* materialManager) : BasePainter(shaderManager, uniformBufferManager)
    {
		m_renderJobManager = renderJobManager;
		m_meshManager = meshManager;
		m_textureManager = textureManager;
		m_materialManager = materialManager;
    }

    OverlayPainter::~OverlayPainter()
    {

    }

    void OverlayPainter::Initialize(GLuint FBO, GLuint dummyVAO)
    {
		BasePainter::Initialize(FBO, dummyVAO);

		// Load shared shaders (vertex and fragment)
		m_shaderManager->LoadShader("shaders/Overlay/Overlay_vs.glsl", "OverlayVS", GL_VERTEX_SHADER);
		m_shaderManager->LoadShader("shaders/Overlay/Overlay_fs.glsl", "OverlayFS", GL_FRAGMENT_SHADER);

		// Create and attach rect debug shaders
		m_shaderManager->CreateProgram("Overlay");
		m_shaderManager->AttachShader("OverlayVS", "Overlay");
		m_shaderManager->AttachShader("OverlayFS", "Overlay");
		m_shaderManager->LinkProgram("Overlay");

		m_uniformTexture0 = m_shaderManager->GetUniformLocation("Overlay", "diffuseMap");
		m_modelMatrixUniform = m_shaderManager->GetUniformLocation("Overlay", "modelMatrix");
		m_viewMatrixUniform = m_shaderManager->GetUniformLocation("Overlay", "viewMatrix");
        LOG_INFO << "VIEW: " << m_viewMatrixUniform << std::endl;
		m_projectionMatrixUniform = m_shaderManager->GetUniformLocation("Overlay", "projectionMatrix");
        LOG_INFO << "PROJ: " << m_projectionMatrixUniform << std::endl;
    }

    void OverlayPainter::Render( glm::mat4& viewMatrix, glm::mat4& projectionMatrix )
    {
		BasePainter::Render();

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);

		m_shaderManager->UseProgram("Overlay");
		
		std::vector<RenderJobManager::RenderJob> renderJobs = m_renderJobManager->GetJobs();

		unsigned int currentShader = std::numeric_limits<decltype(currentShader)>::max();
		unsigned int currentMaterial = std::numeric_limits<decltype(currentMaterial)>::max();
		unsigned int currentMesh = std::numeric_limits<decltype(currentMesh)>::max();

		unsigned int objType = std::numeric_limits<decltype(objType)>::max();
		unsigned int viewport = std::numeric_limits<decltype(viewport)>::max();
		unsigned int layer = std::numeric_limits<decltype(layer)>::max();
		unsigned int translucency = std::numeric_limits<decltype(translucency)>::max();
		unsigned int meshID = std::numeric_limits<decltype(meshID)>::max();
		unsigned int material = std::numeric_limits<decltype(material)>::max();
		unsigned int depth = std::numeric_limits<decltype(depth)>::max();

		GLenum error;

		Material mat;
		Mesh mesh;
		GFXBitmask bitmask;

        m_shaderManager->SetUniform(1, viewMatrix, m_viewMatrixUniform);
        m_shaderManager->SetUniform(1, projectionMatrix, m_projectionMatrixUniform);

		for (unsigned int i = 0; i < renderJobs.size(); i++)
		{
			bitmask = renderJobs[i].bitmask;

			objType = GetBitmaskValue(bitmask, BITMASK::TYPE);

			// Break if no opaque object
			if (objType != GFX::OBJECT_TYPES::OVERLAY_GEOMETRY)
			{
				break;
			}

			viewport = GetBitmaskValue(bitmask, BITMASK::VIEWPORT_ID);
			layer = GetBitmaskValue(bitmask, BITMASK::LAYER);
			translucency = GetBitmaskValue(bitmask, BITMASK::TRANSLUCENCY_TYPE);
			meshID = GetBitmaskValue(bitmask, BITMASK::MESH_ID);
			material = GetBitmaskValue(bitmask, BITMASK::MATERIAL_ID);
			depth = GetBitmaskValue(bitmask, BITMASK::DEPTH);

			if (material != currentMaterial)
			{
				mat = m_materialManager->GetMaterial(material);

                if( mat.textures.size() < 1 )
                {
                    LOG_ERROR << "Trying to render object with invalid material" << std::endl;
                    continue;
                }

				//set textures
				m_textureManager->BindTexture(m_textureManager->GetTexture(mat.textures[0]).textureHandle, m_uniformTexture0, 0, GL_TEXTURE_2D);
			}

			if (meshID != currentMesh)
			{
				mesh = m_meshManager->GetMesh(meshID);
				currentMesh = meshID;

				glBindVertexArray(mesh.VAO);
			}

			m_shaderManager->SetUniform(1, *(glm::mat4*)renderJobs.at(i).value, m_modelMatrixUniform);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.IBO);
			glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, (GLvoid*)0);
		}

		m_shaderManager->ResetProgram();
    }
}

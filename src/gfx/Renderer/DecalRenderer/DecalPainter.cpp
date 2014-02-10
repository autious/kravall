#include "DecalPainter.hpp"
#include <gfx/BitmaskDefinitions.hpp>

#include <logger/Logger.hpp>

#include <gfx/InstanceData.hpp>
#include "../../Buffers/MeshManager.hpp"
#include "../../Textures/TextureManager.hpp"
#include "../../Material/MaterialManager.hpp"
#include <Animation/AnimationManagerGFX.hpp>
#include "../../Buffers/UniformBufferManager.hpp"
#include "../RenderJobManager.hpp"
#include <Shaders/ShaderManager.hpp>


//Ugly fix for something that messed up OpenGL on GCC.
// causes no problems since there is only one instance of DeferredPainter anyway.
extern "C"
{
	static GLuint m_uniformTexture0;
	static GLuint m_normalDepthUniform;
	static GLuint m_gammaUniform;

	static GLuint m_modelMatrixUniform;

	static const unsigned int MAX_INSTANCES = 1024;
	static GLuint m_instanceBuffer;
}
namespace GFX
{

	DecalPainter::DecalPainter(ShaderManager* shaderManager, UniformBufferManager* uniformBufferManager, RenderJobManager* renderJobManager,
		MeshManager* meshManager, TextureManager* textureManager, MaterialManager* materialManager)
		: BasePainter(shaderManager, uniformBufferManager)
	{
		m_renderJobManager = renderJobManager;
		m_meshManager = meshManager;
		m_textureManager = textureManager;
		m_materialManager = materialManager;

	}

	DecalPainter::~DecalPainter()
	{
	}

	void DecalPainter::Initialize(GLuint FBO, GLuint dummyVAO)
	{
		BasePainter::Initialize(FBO, dummyVAO);
	}

	void DecalPainter::Render(AnimationManagerGFX* animationManager, unsigned int& renderIndex,
		FBOTexture* depthBuffer, FBOTexture* normalDepth, FBOTexture* diffuse, FBOTexture* specular, FBOTexture* glowMatID, glm::mat4 viewMatrix, glm::mat4 projMatrix, const float& gamma)
	{
		//BasePainter::Render();
		/*
		BindGBuffer(depthBuffer, normalDepth, diffuse, specular, glowMatID);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Clear depth RT
		float c[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 1, &glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)[0]);

		m_shaderManager->UseProgram("StaticNormal");

		BasicCamera bc;
		bc.viewMatrix = viewMatrix;
		bc.projMatrix = projMatrix;

		m_uniformBufferManager->SetBasicCameraUBO(bc);

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

		unsigned int i;
		for (i = 0; i < renderJobs.size(); i++)
		{
			bitmask = renderJobs[i].bitmask;

			objType = GetBitmaskValue(bitmask, BITMASK::TYPE);

			// Break if no opaque object
			if (objType != GFX::OBJECT_TYPES::OPAQUE_GEOMETRY)
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

				
				currentMaterial = material;

				//compare shader
				if (mat.shaderProgramID != currentShader)
				{
					glUseProgram(mat.shaderProgramID);
					currentShader = mat.shaderProgramID;
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
			//glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, (GLvoid*)0);
		}

		m_shaderManager->ResetProgram();
		*/
		//ClearFBO();
	}

	void DecalPainter::BindGBuffer(FBOTexture* depthBuffer, FBOTexture* normalDepth, FBOTexture* diffuse, FBOTexture* specular, FBOTexture* glowMatID)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthBuffer->GetTextureHandle(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, diffuse->GetTextureHandle(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, specular->GetTextureHandle(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, glowMatID->GetTextureHandle(), 0);

		// define outputs
		GLenum drawBuffers[] = { GL_NONE, GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(4, drawBuffers);

	}
}

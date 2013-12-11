#include "DeferredPainter.hpp"
#include <gfx/BitmaskDefinitions.hpp>

namespace GFX
{
	DeferredPainter::DeferredPainter(ShaderManager* shaderManager, UniformBufferManager* uniformBufferManager, RenderJobManager* renderJobManager,
		MeshManager* meshManager, TextureManager* textureManager, MaterialManager* materialManager)
		: BasePainter(shaderManager, uniformBufferManager)
	{
		m_renderJobManager = renderJobManager;
		m_meshManager = meshManager;
		m_textureManager = textureManager;
		m_materialManager = materialManager;
	}

	DeferredPainter::~DeferredPainter()
	{
	}
	
	void DeferredPainter::Initialize(GLuint FBO, GLuint dummyVAO)
	{
		BasePainter::Initialize(FBO, dummyVAO);

		m_shaderManager->CreateProgram("StaticMesh");

		m_shaderManager->LoadShader("shaders/SimpleGeometry.vertex", "StaticMeshVS", GL_VERTEX_SHADER);
		m_shaderManager->LoadShader("shaders/SimpleGeometry.fragment", "StaticMeshFS", GL_FRAGMENT_SHADER);
		
		m_shaderManager->AttachShader("StaticMeshVS", "StaticMesh");
		m_shaderManager->AttachShader("StaticMeshFS", "StaticMesh");

		m_shaderManager->LinkProgram("StaticMesh");

		// TODO: Change texture names in shader
		m_uniformTexture0 = m_shaderManager->GetUniformLocation("StaticMesh", "diffuseMap");
		m_uniformTexture1 = m_shaderManager->GetUniformLocation("StaticMesh", "normalMap");
		m_uniformTexture2 = m_shaderManager->GetUniformLocation("StaticMesh", "specularMap");
		m_uniformTexture3 = m_shaderManager->GetUniformLocation("StaticMesh", "glowMap");

		m_modelMatrixUniform = m_shaderManager->GetUniformLocation("StaticMesh", "modelMatrix");

		m_uniformBufferManager->CreateBasicCameraUBO(m_shaderManager->GetShaderProgramID("StaticMesh"));
	}

	void DeferredPainter::Render(FBOTexture* depthBuffer, FBOTexture* normalDepth, FBOTexture* diffuse, FBOTexture* specular, FBOTexture* glowMatID, glm::mat4 viewMatrix, glm::mat4 projMatrix)
	{
		BasePainter::Render();

		BindGBuffer(depthBuffer, normalDepth, diffuse, specular, glowMatID);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_shaderManager->UseProgram("StaticMesh");
		
		BasicCamera bc;
		bc.viewMatrix = viewMatrix;
		bc.projMatrix = projMatrix;
		
		m_uniformBufferManager->SetBasicCameraUBO(bc);

		std::vector<RenderJobManager::RenderJob> renderJobs = m_renderJobManager->GetJobs();

		unsigned int currentShader = UINT_MAX;
		unsigned int currentMaterial = UINT_MAX;
		unsigned int currentMesh = UINT_MAX;

		unsigned int objType = UINT_MAX;
		unsigned int viewport = UINT_MAX;
		unsigned int layer = UINT_MAX;
		unsigned int translucency = UINT_MAX;
		unsigned int meshID = UINT_MAX;
		unsigned int material = UINT_MAX;
		unsigned int depth = UINT_MAX;

		Material mat;
		Mesh mesh;
		GFXBitmask bitmask;

		for (unsigned int i = 0; i < renderJobs.size(); i++)
		{
			bitmask = renderJobs[i].bitmask;

			objType = GetBitmaskValue(bitmask, BT_OBJECT_TYPE);

			viewport = GetBitmaskValue(bitmask, BT_VIEWPORT_ID);
			layer = GetBitmaskValue(bitmask, BT_LAYER);
			translucency = GetBitmaskValue(bitmask, BT_TRANSLUCENCY_TYPE);
			meshID = GetBitmaskValue(bitmask, BT_MESH_ID);
			material = GetBitmaskValue(bitmask, BT_MATERIAL_ID);
			depth = GetBitmaskValue(bitmask, BT_DEPTH);
			
			if (material != currentMaterial)
			{
				mat = m_materialManager->GetMaterial(material);

				currentMaterial = material;
				
				//compare shader
				if (mat.shaderProgramID != currentShader)
				{
					//glUseProgram(mat.shaderProgramID);
					currentShader = mat.shaderProgramID;
				}
				
				//set textures
				//assert(mat.textures.size() == 4);
				//m_textureManager->BindTexture(m_textureManager->GetTexture(mat.textures[0]).textureHandle, m_uniformTexture0, 0, GL_TEXTURE_2D);
				//m_textureManager->BindTexture(m_textureManager->GetTexture(mat.textures[1]).textureHandle, m_uniformTexture1, 1, GL_TEXTURE_2D);
				//m_textureManager->BindTexture(m_textureManager->GetTexture(mat.textures[2]).textureHandle, m_uniformTexture2, 2, GL_TEXTURE_2D);
				//m_textureManager->BindTexture(m_textureManager->GetTexture(mat.textures[3]).textureHandle, m_uniformTexture3, 3, GL_TEXTURE_2D);


			}

			if (meshID != currentMesh)
			{
				mesh = m_meshManager->GetMesh(meshID);
				currentMesh = meshID;

				glBindVertexArray(mesh.VAO);
				//set mesh
			}

			m_shaderManager->SetUniform(1, *(glm::mat4*)renderJobs.at(i).value, m_modelMatrixUniform);
			glDrawArrays(GL_TRIANGLES, 0, 8127);
			//glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, (GLvoid*)0);
		}

		//struct mesh
		//id
		//vao
		//index count

		//struct material
		//shader program id
		//texture array
		//texture count

		//graphics needs to hold a list of this shit

		//Get all renderjobs

		//for each renderjob
		//decode material id
		//decode mesh id == struct mesh id
		//check against current shader
		//Check against current mesh
		//if different, fetch new mesh (WHERE?)
		//VAO
		//index count
		//check against current material
		//if different, fetch new material (WHERE?)
		//Set uniforms
		//do rendering
		
		//for (unsigned int i = 0; i < m_renderJobs.size(); i++)
		//{
		//	Texture::BindTexture(m_renderJobs.at(i).material->diffuse, m_diffuseUniform, 0, GL_TEXTURE_2D);
		//	Texture::BindTexture(m_renderJobs.at(i).material->normal, m_normalUniform, 1, GL_TEXTURE_2D);
		//	Texture::BindTexture(m_renderJobs.at(i).material->specular, m_specularUniform, 2, GL_TEXTURE_2D);
		//	Texture::BindTexture(m_renderJobs.at(i).material->glow, m_glowUniform, 3, GL_TEXTURE_2D);
		//
		//	m_shaderManager->SetUniform(1, *m_renderJobs.at(i).modelMatrix, m_modelMatrixUniform);
		//
		//	glBindVertexArray(m_renderJobs.at(i).VAO);
		//	glDrawArrays(GL_TRIANGLES, 0, m_renderJobs.at(i).IBOSize);
		//}
		//
		//m_renderJobs.clear();
		//glBindVertexArray(m_dummyVAO);
		//glDrawArrays(GL_TRIANGLES, 0, 8127);

		m_shaderManager->ResetProgram();

		ClearFBO();
	}


	void DeferredPainter::BindGBuffer(FBOTexture* depthBuffer, FBOTexture* normalDepth, FBOTexture* diffuse, FBOTexture* specular, FBOTexture* glowMatID)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);


		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer->GetTextureHandle(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, normalDepth->GetTextureHandle(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, diffuse->GetTextureHandle(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, specular->GetTextureHandle(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, glowMatID->GetTextureHandle(), 0);

		// define outputs
		GLenum drawBuffers[] = { GL_NONE, GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(5, drawBuffers);
	}
}

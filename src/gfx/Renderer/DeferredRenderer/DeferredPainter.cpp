#include "DeferredPainter.hpp"
#include <gfx/BitmaskDefinitions.hpp>

#include <logger/Logger.hpp>

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

		glGenBuffers(1, &buffer);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer);
		glBufferData(GL_SHADER_STORAGE_BUFFER, 1024 * sizeof(StaticMeshInstanceData), NULL, GL_STREAM_COPY);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, buffer);
	}

	void DeferredPainter::Render(unsigned int& renderIndex, FBOTexture* depthBuffer, FBOTexture* normalDepth, FBOTexture* diffuse, FBOTexture* specular, FBOTexture* glowMatID, glm::mat4 viewMatrix, glm::mat4 projMatrix)
	{
		BasePainter::Render();

		BindGBuffer(depthBuffer, normalDepth, diffuse, specular, glowMatID);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Clear depth RT
		float c[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 1, &glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)[0]);

		m_shaderManager->UseProgram("StaticMesh");

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
		int instanceCount = 0;
		unsigned int i;

		std::vector<StaticMeshInstanceData> currentInstances;
		StaticMeshInstanceData* instances = new StaticMeshInstanceData[1024];
		StaticMeshInstanceData* pData;

		bool endMe = false;
		for (i = renderIndex; i < renderJobs.size(); i++)
		{
			bitmask = renderJobs[i].bitmask;

			objType = GetBitmaskValue(bitmask, BITMASK::TYPE);

			// Break if no opaque object
			if (objType != GFX::OBJECT_TYPES::OPAQUE_GEOMETRY)
			{
				endMe = true;
			}

			if (!endMe)
			{
				viewport = GetBitmaskValue(bitmask, BITMASK::VIEWPORT_ID);
				layer = GetBitmaskValue(bitmask, BITMASK::LAYER);
				translucency = GetBitmaskValue(bitmask, BITMASK::TRANSLUCENCY_TYPE);
				meshID = GetBitmaskValue(bitmask, BITMASK::MESH_ID);
				material = GetBitmaskValue(bitmask, BITMASK::MATERIAL_ID);
				depth = GetBitmaskValue(bitmask, BITMASK::DEPTH);
			}
			

			if (material == currentMaterial && meshID == currentMesh && !endMe && instanceCount < 1024)
			{
				StaticMeshInstanceData smid;
				smid.modelMatrix = *(glm::mat4*)renderJobs.at(i).value;
				instances[instanceCount++] = smid;
				
				//currentInstances.push_back(smid);
			}
			else 
			{
				if (i > 0)
				{
					pData = (StaticMeshInstanceData*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, 1024 * sizeof(StaticMeshInstanceData), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
					//
					//glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, 1024 * sizeof(StaticMeshInstanceData), (GLvoid*)instances);
					//for (unsigned int j = 0; j < currentInstances.size(); j++)
					//{
					//	pData[j] = currentInstances[j];
					//}
					//
					for (unsigned int j = 0; j < instanceCount; j++)
					{
						pData[j] = instances[j];
					}
					//
					glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.IBO);
					glDrawElementsInstanced(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, (GLvoid*)0, instanceCount);
					instanceCount = 0;
				}
				
				if (endMe)
					break;

				if (material != currentMaterial)
				{
					mat = m_materialManager->GetMaterial(material);

					//It's possible that a material is removed before an entity. Should this be ok, do we need to be
					// more rigorous from the outside?
					if (mat.textures.size() != 4)
					{
						LOG_ERROR << "Trying to render object with invalid material" << std::endl;
						continue;
					}
					//alt
					//assert(mat.textures.size() == 4); 

					currentMaterial = material;

					//compare shader
					if (mat.shaderProgramID != currentShader)
					{
						glUseProgram(mat.shaderProgramID);
						error = glGetError();
						currentShader = mat.shaderProgramID;
					}

					//set textures
					m_textureManager->BindTexture(m_textureManager->GetTexture(mat.textures[0]).textureHandle, m_uniformTexture0, 0, GL_TEXTURE_2D);
					m_textureManager->BindTexture(m_textureManager->GetTexture(mat.textures[1]).textureHandle, m_uniformTexture1, 1, GL_TEXTURE_2D);
					m_textureManager->BindTexture(m_textureManager->GetTexture(mat.textures[2]).textureHandle, m_uniformTexture2, 2, GL_TEXTURE_2D);
					m_textureManager->BindTexture(m_textureManager->GetTexture(mat.textures[3]).textureHandle, m_uniformTexture3, 3, GL_TEXTURE_2D);
				}

				if (meshID != currentMesh)
				{
					mesh = m_meshManager->GetMesh(meshID);
					currentMesh = meshID;

					glBindVertexArray(mesh.VAO);
					error = glGetError();
				}

				StaticMeshInstanceData smid;
				smid.modelMatrix = *(glm::mat4*)renderJobs.at(i).value;
				instances[instanceCount++] = smid;
				
				//currentInstances.push_back(smid);
			}


		}

		m_shaderManager->ResetProgram();

		ClearFBO();

		renderIndex = i;

	}


	/*
	void DeferredPainter::Render(unsigned int& renderIndex, FBOTexture* depthBuffer, FBOTexture* normalDepth, FBOTexture* diffuse, FBOTexture* specular, FBOTexture* glowMatID, glm::mat4 viewMatrix, glm::mat4 projMatrix)
	{
		BasePainter::Render();

		BindGBuffer(depthBuffer, normalDepth, diffuse, specular, glowMatID);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Clear depth RT
		float c[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 1, &glm::vec4(0.0f, 0.0f, 0.0f,1.0f)[0]);

		m_shaderManager->UseProgram("StaticMesh");
		
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
		for (i = renderIndex; i < renderJobs.size(); i++)
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

			if (material == currentMaterial && meshID == currentMesh)
			{

			}

			if (material != currentMaterial)
			{
				mat = m_materialManager->GetMaterial(material);

                //It's possible that a material is removed before an entity. Should this be ok, do we need to be
                // more rigorous from the outside?
                if( mat.textures.size() != 4 )
                {
                    LOG_ERROR << "Trying to render object with invalid material" << std::endl;
                    continue;
                }
                //alt
				//assert(mat.textures.size() == 4); 

				currentMaterial = material;
				
				//compare shader
				if (mat.shaderProgramID != currentShader)
				{
					glUseProgram(mat.shaderProgramID);
					error = glGetError();
					currentShader = mat.shaderProgramID;
				}
				
				//set textures
				m_textureManager->BindTexture(m_textureManager->GetTexture(mat.textures[0]).textureHandle, m_uniformTexture0, 0, GL_TEXTURE_2D);
				m_textureManager->BindTexture(m_textureManager->GetTexture(mat.textures[1]).textureHandle, m_uniformTexture1, 1, GL_TEXTURE_2D);
				m_textureManager->BindTexture(m_textureManager->GetTexture(mat.textures[2]).textureHandle, m_uniformTexture2, 2, GL_TEXTURE_2D);
				m_textureManager->BindTexture(m_textureManager->GetTexture(mat.textures[3]).textureHandle, m_uniformTexture3, 3, GL_TEXTURE_2D);
			}

			if (meshID != currentMesh)
			{
				mesh = m_meshManager->GetMesh(meshID);
				currentMesh = meshID;

				glBindVertexArray(mesh.VAO);
					error = glGetError();
				//set mesh
			}

			StaticMeshInstanceData* pData = (StaticMeshInstanceData*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, 1024 * sizeof(StaticMeshInstanceData), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
			
			for (unsigned int j = 0; j < 1024; j++)
			{
				pData[j].modelMatrix = *(glm::mat4*)renderJobs.at(i).value;
			}

			glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

			m_shaderManager->SetUniform(1, *(glm::mat4*)renderJobs.at(i).value, m_modelMatrixUniform);
					error = glGetError();
			//glDrawArrays(GL_TRIANGLES, 0, 3559);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.IBO);
					error = glGetError();
			glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, (GLvoid*)0);
					error = glGetError();
		}

		m_shaderManager->ResetProgram();

		ClearFBO();

		renderIndex = i;

	}*/


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

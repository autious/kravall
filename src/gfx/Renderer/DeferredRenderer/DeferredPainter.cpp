#include "DeferredPainter.hpp"
#include <gfx/BitmaskDefinitions.hpp>

#include <logger/Logger.hpp>

#define INSTANCED_DRAWING

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


		//TODO: CHANGE THIS INTO A PROPER STATIC SHADER
		m_shaderManager->CreateProgram("StaticMesh");
		m_shaderManager->LoadShader("shaders/SimpleGeometryVS.glsl", "StaticMeshVS", GL_VERTEX_SHADER);
		m_shaderManager->LoadShader("shaders/SimpleGeometryFS.glsl", "StaticMeshFS", GL_FRAGMENT_SHADER);
		m_shaderManager->AttachShader("StaticMeshVS", "StaticMesh");
		m_shaderManager->AttachShader("StaticMeshFS", "StaticMesh");
		m_shaderManager->LinkProgram("StaticMesh");

		//Normal mapped, non-animated shader
		m_shaderManager->CreateProgram("NormalMappedStatic");
		m_shaderManager->LoadShader("shaders/SimpleGeometryVS.glsl", "NormalMappedStaticVS", GL_VERTEX_SHADER);
		m_shaderManager->LoadShader("shaders/NormalMappedFS.glsl", "NormalMappedStaticFS", GL_FRAGMENT_SHADER);
		m_shaderManager->AttachShader("NormalMappedStaticVS", "NormalMappedStatic");
		m_shaderManager->AttachShader("NormalMappedStaticFS", "NormalMappedStatic");
		m_shaderManager->LinkProgram("NormalMappedStatic");
		
		m_shaderManager->CreateProgram("AnimatedMesh");
		m_shaderManager->LoadShader("shaders/AnimatedMeshVS.glsl", "AnimatedMeshVS", GL_VERTEX_SHADER);
		m_shaderManager->LoadShader("shaders/SimpleGeometryFS.glsl", "AnimatedMeshFS", GL_FRAGMENT_SHADER);
		m_shaderManager->AttachShader("AnimatedMeshVS", "AnimatedMesh");
		m_shaderManager->AttachShader("AnimatedMeshFS", "AnimatedMesh");
		m_shaderManager->LinkProgram("AnimatedMesh");

		m_modelMatrixUniform = m_shaderManager->GetUniformLocation("StaticMesh", "modelMatrix");

		m_uniformBufferManager->CreateBasicCameraUBO(m_shaderManager->GetShaderProgramID("StaticMesh"));

#ifdef INSTANCED_DRAWING

		m_staticInstances = new InstanceData[1024];

		glGenBuffers(1, &m_instanceBuffer);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_instanceBuffer);
		glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_INSTANCES * sizeof(InstanceData), NULL, GL_STREAM_COPY);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, m_instanceBuffer);

#endif
	}
#ifdef INSTANCED_DRAWING
	void DeferredPainter::Render(unsigned int& renderIndex, FBOTexture* depthBuffer, FBOTexture* normalDepth, FBOTexture* diffuse, FBOTexture* specular, FBOTexture* glowMatID, glm::mat4 viewMatrix, glm::mat4 projMatrix, const float& gamma)
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
			

			if (material == currentMaterial && meshID == currentMesh && !endMe && instanceCount < MAX_INSTANCES)
			{
				InstanceData smid = *(InstanceData*)renderJobs.at(i).value;
				m_staticInstances[instanceCount++] = smid;
			}
			else 
			{
				if (i > 0)
				{
					glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_instanceBuffer);
					InstanceData* pData = (InstanceData*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, MAX_INSTANCES * sizeof(InstanceData), 

						GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
					memcpy(pData, m_staticInstances, instanceCount * sizeof(InstanceData));

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

						m_uniformTexture0 = m_shaderManager->GetUniformLocation(currentShader, "gDiffuse");
						m_uniformTexture1 = m_shaderManager->GetUniformLocation(currentShader, "gNormal");
						m_uniformTexture2 = m_shaderManager->GetUniformLocation(currentShader, "gSpecular");
						m_uniformTexture3 = m_shaderManager->GetUniformLocation(currentShader, "gGlow");

						m_gammaUniform = m_shaderManager->GetUniformLocation(currentShader, "gGamma");
					}

					//set textures
					m_textureManager->BindTexture(m_textureManager->GetTexture(mat.textures[0]).textureHandle, m_uniformTexture0, 0, GL_TEXTURE_2D);
					m_textureManager->BindTexture(m_textureManager->GetTexture(mat.textures[2]).textureHandle, m_uniformTexture1, 1, GL_TEXTURE_2D);
					m_textureManager->BindTexture(m_textureManager->GetTexture(mat.textures[1]).textureHandle, m_uniformTexture2, 2, GL_TEXTURE_2D);
					m_textureManager->BindTexture(m_textureManager->GetTexture(mat.textures[3]).textureHandle, m_uniformTexture3, 3, GL_TEXTURE_2D);
					
					//Set gamma
					m_shaderManager->SetUniform(gamma, m_gammaUniform);

					
				}

					if (meshID != currentMesh)
				{
					mesh = m_meshManager->GetMesh(meshID);
					currentMesh = meshID;

					glBindVertexArray(mesh.VAO);
					error = glGetError();
				}
					 
				InstanceData smid = *(InstanceData*)renderJobs.at(i).value;
				m_staticInstances[instanceCount++] = smid;
			}


		}

		m_shaderManager->ResetProgram();

		ClearFBO();

		renderIndex = i;

	}
	
#else
	
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

	}
	
#endif

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

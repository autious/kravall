#include "ShadowPainter.hpp"
#include <gfx/BitmaskDefinitions.hpp>

#include <logger/Logger.hpp>

namespace GFX
{
	ShadowPainter::ShadowPainter(ShaderManager* shaderManager, UniformBufferManager* uniformBufferManager, RenderJobManager* renderJobManager, 
			MeshManager* meshManager, MaterialManager* materialManager)
		: BasePainter(shaderManager, uniformBufferManager)
	{
		m_renderJobManager = renderJobManager;
		m_meshManager = meshManager;
		m_materialManager = materialManager;
	}

	ShadowPainter::~ShadowPainter()
	{
	}
	
	void ShadowPainter::Initialize(GLuint FBO, GLuint dummyVAO, BlurPainter* blurPainter)
	{
		BasePainter::Initialize(FBO, dummyVAO);

		// Normal map shaders
		m_shaderManager->CreateProgram("SMSV"); // Shadowmap Static Variance
		m_shaderManager->LoadShader("shaders/shadow/StaticVS.glsl", "SMSV_VS", GL_VERTEX_SHADER);
		m_shaderManager->LoadShader("shaders/shadow/VarianceFS.glsl", "SMSV_FS", GL_FRAGMENT_SHADER);
		m_shaderManager->AttachShader("SMSV_VS", "SMSV");
		m_shaderManager->AttachShader("SMSV_FS", "SMSV");
		m_shaderManager->LinkProgram("SMSV");
		
		m_shaderManager->CreateProgram("SMAV"); // Shadowmap Animated Variance
		m_shaderManager->LoadShader("shaders/shadow/AnimatedFS.glsl", "SMAV_VS", GL_VERTEX_SHADER);
		m_shaderManager->LoadShader("shaders/shadow/VarianceFS.glsl",   "SMAV_FS", GL_FRAGMENT_SHADER);
		m_shaderManager->AttachShader("SMAV_VS", "SMAV");
		m_shaderManager->AttachShader("SMAV_FS", "SMAV");
		m_shaderManager->LinkProgram("SMAV");

		m_staticInstances = new InstanceData[1024];

		glGenBuffers(1, &m_instanceBuffer);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_instanceBuffer);
		glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_INSTANCES * sizeof(InstanceData), NULL, GL_STREAM_COPY);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, m_instanceBuffer);
	}

	void ShadowPainter::Render(AnimationManager* animationManager, unsigned int& renderIndex, glm::mat4 viewMatrix, glm::mat4 projMatrix,
			const unsigned int& geometryStartIndex, const unsigned int& geometryEndIndex, FBOTexture* shadowMap)
	{
		BasePainter::Render();

		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);

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
					glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, m_instanceBuffer);
					InstanceData* pData = (InstanceData*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, MAX_INSTANCES * sizeof(InstanceData), 
						GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT);

					memcpy(pData, m_staticInstances, instanceCount * sizeof(InstanceData));

					glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.IBO);
					
					if (mesh.skeletonID >= 0)
						animationManager->BindSkeleton(mesh.skeletonID);

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
					currentMaterial = material;
					
					
					//compare shader
					if (mat.shaderProgramID != currentShader)
					{
						glUseProgram(mat.shaderProgramID);
						currentShader = mat.shaderProgramID;
					}
				}

					if (meshID != currentMesh)
				{
					mesh = m_meshManager->GetMesh(meshID);
					currentMesh = meshID;

					glBindVertexArray(mesh.VAO);
					error = glGetError();
					
					
					if (mesh.skeletonID >= 0)
						animationManager->BindSkeletonData(mesh.skeletonID);
				}
					 
				InstanceData smid = *(InstanceData*)renderJobs.at(i).value;
				m_staticInstances[instanceCount++] = smid;
			}


		}

		m_shaderManager->ResetProgram();

		ClearFBO();

		renderIndex = i;

	}
}

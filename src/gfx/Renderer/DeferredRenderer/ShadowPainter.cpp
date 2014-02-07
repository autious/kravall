#include "ShadowPainter.hpp"
#include <gfx/BitmaskDefinitions.hpp>

#include <logger/Logger.hpp>
#include <glm/ext.hpp>
#include <gfx/LightData.hpp>

namespace GFX
{
	ShadowPainter::ShadowPainter(ShaderManager* shaderManager, UniformBufferManager* uniformBufferManager, RenderJobManager* renderJobManager, 
			MeshManager* meshManager)
		: BasePainter(shaderManager, uniformBufferManager)
	{
		m_renderJobManager = renderJobManager;
		m_meshManager = meshManager;

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
		m_shaderManager->LoadShader("shaders/shadow/AnimatedVS.glsl", "SMAV_VS", GL_VERTEX_SHADER);
		m_shaderManager->LoadShader("shaders/shadow/VarianceFS.glsl",   "SMAV_FS", GL_FRAGMENT_SHADER);
		m_shaderManager->AttachShader("SMAV_VS", "SMAV");
		m_shaderManager->AttachShader("SMAV_FS", "SMAV");
		m_shaderManager->LinkProgram("SMAV");

		glGenBuffers(1, &m_matricesUniform);
		glBindBuffer(GL_UNIFORM_BUFFER, m_matricesUniform);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4x4) * 2, NULL, GL_STREAM_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);



		m_staticInstances = new InstanceData[1024];

		glGenBuffers(1, &m_instanceBuffer);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_instanceBuffer);
		glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_INSTANCES * sizeof(InstanceData), NULL, GL_STREAM_COPY);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, m_instanceBuffer);
	}

	void ShadowPainter::Render(AnimationManager* animationManager, const unsigned int& renderIndex, glm::mat4 viewMatrix, glm::mat4 projMatrix,
			const unsigned int& geometryStartIndex, const unsigned int& geometryEndIndex, FBOTexture* shadowMap, const unsigned int& width, const unsigned int& height)
	{
		BasePainter::Render();
		unsigned int startIndex = geometryStartIndex;
		unsigned int endIndex = geometryEndIndex;
		//GLenum error;

		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Set framebuffers for shadowmapping
		m_shaderManager->UseProgram("SMSV");
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
		glViewport(0, 0, shadowMap->GetWidth(), shadowMap->GetHeight());
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, shadowMap->GetTextureHandle(), 0);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		

		std::vector<RenderJobManager::RenderJob> renderJobs = m_renderJobManager->GetJobs();

		unsigned int currentMesh =	std::numeric_limits<decltype(currentMesh)>::max();
		unsigned int objType =		std::numeric_limits<decltype(objType	)>::max();
		unsigned int lightType =	std::numeric_limits<decltype(objType	)>::max();
		unsigned int viewport =		std::numeric_limits<decltype(viewport	)>::max();
		unsigned int layer =		std::numeric_limits<decltype(layer		)>::max();
		unsigned int translucency = std::numeric_limits<decltype(translucency)>::max();
		unsigned int meshID =		std::numeric_limits<decltype(meshID		)>::max();
		unsigned int depth =		std::numeric_limits<decltype(depth		)>::max();
		
		bool endLights = false;
		
		BasicCamera bc;
		GFXBitmask lightBitmask;

		// Loop through each light
		unsigned int l = renderIndex;
		for (l = renderIndex; l < renderJobs.size(); l++)
		{
			lightBitmask = renderJobs[l].bitmask;
			lightType = GetBitmaskValue(lightBitmask, BITMASK::LIGHT_TYPE);
			
			if (lightType == GFX::LIGHT_TYPES::DIR_SHADOW)
			{
				LightData lightData = *(LightData*)renderJobs.at(l).value;
				// Create matrices for the lights
				// TODO: Make light frustum fit the camera frustum
				bc.viewMatrix = viewMatrix;// glm::lookAt<float>(-lightData.orientation, lightData.orientation, glm::vec3(0.0f, 1.0f, 0.0f));
				bc.projMatrix = projMatrix;// glm::ortho<float>(-10.0f, 10.0f, -10.0f, 10.0f);
				
			}
			else if (lightType == GFX::LIGHT_TYPES::SPOT_SHADOW)
			{
				LightData lightData = *(LightData*)renderJobs.at(l).value;
				// Create matrices for the lights
				bc.viewMatrix = glm::lookAt<float>(lightData.position, lightData.orientation, glm::vec3(0.0f, 1.0f, 0.0f));
				bc.projMatrix = glm::perspective<float>(glm::degrees(lightData.spot_angle), 1.0f, 0.001f, lightData.radius_length);
			}
			else if (lightType == GFX::LIGHT_TYPES::POINT_SHADOW)
			{
				break; // TODO: Implement point shadowmapping
			}
			else
			{
				break; // Break if no shadowcasting light
			}
			m_shaderManager->UseProgram("SMSV");
			glBindBuffer(GL_UNIFORM_BUFFER, m_matricesUniform);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4x4), &bc.viewMatrix);
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4x4), sizeof(glm::mat4x4), &bc.projMatrix);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);

			Mesh mesh;
			GFXBitmask geometryBitmask;
			int instanceCount = 0;
			bool endGeometry = false;

			// Loop through all the geometry
			unsigned int i;
			for (i = startIndex; i < endIndex; i++)
			{
				geometryBitmask = renderJobs[i].bitmask;

				objType = GetBitmaskValue(geometryBitmask, BITMASK::TYPE);

				// Break if no opaque object
				if (objType != GFX::OBJECT_TYPES::OPAQUE_GEOMETRY)
				{
					endGeometry = true;
				}

				if (!endGeometry)
				{
					viewport = GetBitmaskValue(geometryBitmask, BITMASK::VIEWPORT_ID);
					layer = GetBitmaskValue(geometryBitmask, BITMASK::LAYER);
					translucency = GetBitmaskValue(geometryBitmask, BITMASK::TRANSLUCENCY_TYPE);
					meshID = GetBitmaskValue(geometryBitmask, BITMASK::MESH_ID);
					depth = GetBitmaskValue(geometryBitmask, BITMASK::DEPTH);
				}


				if (meshID == currentMesh && !endGeometry && instanceCount < MAX_INSTANCES)
				{
					InstanceData smid = *(InstanceData*)renderJobs.at(i).value;
					m_staticInstances[instanceCount++] = smid;
				}
				else
				{
					if (i > 0)
					{
						
						if (mesh.skeletonID >= 0)
						{
							animationManager->BindSkeleton(mesh.skeletonID);
							m_shaderManager->UseProgram("SMAV");
						}
						else
						{
							m_shaderManager->UseProgram("SMSV");
						}


						glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, m_instanceBuffer);
						InstanceData* pData = (InstanceData*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, MAX_INSTANCES * sizeof(InstanceData),
							GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT);

						memcpy(pData, m_staticInstances, instanceCount * sizeof(InstanceData));

						glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.IBO);


						glDrawElementsInstanced(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, (GLvoid*)0, instanceCount);

						instanceCount = 0;

					}

					if (endGeometry)
						break;

					if (meshID != currentMesh)
					{
						mesh = m_meshManager->GetMesh(meshID);
						currentMesh = meshID;
						glBindVertexArray(mesh.VAO);
						if (mesh.skeletonID >= 0)
							animationManager->BindSkeletonData(mesh.skeletonID);
					}

					InstanceData smid = *(InstanceData*)renderJobs.at(i).value;
					m_staticInstances[instanceCount++] = smid;
				}


			}
		}

		m_shaderManager->ResetProgram();

		ClearFBO();

		glViewport(0, 0, width, height);

	}
}

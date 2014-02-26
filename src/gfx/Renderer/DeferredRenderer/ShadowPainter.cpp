#include "ShadowPainter.hpp"
#include <gfx/BitmaskDefinitions.hpp>

#include <logger/Logger.hpp>
#include <glm/ext.hpp>
#include <gfx/LightData.hpp>
#include "../ShadowData.hpp"
#include <algorithm>

#include "../DebugRenderer/DebugManager.hpp"
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
	
	void ShadowPainter::Resize(const unsigned int& shadowmapResolution)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

		for (int i = 0; i < 4; i++)
		{
			glBindRenderbuffer(GL_RENDERBUFFER, m_RBO[i]);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, shadowmapResolution, shadowmapResolution);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RBO[i]);
		}
	}

	void ShadowPainter::Initialize(GLuint FBO, GLuint dummyVAO, BlurPainter* blurPainter, const unsigned int& shadowmapResolution)
	{
		//BasePainter::Initialize(FBO, dummyVAO);

		m_dummyVAO = dummyVAO;
		m_blurPainter = blurPainter;

		// Init the fbo
		m_RBO = new GLuint[4];
		glGenFramebuffers(1, &m_FBO);
		glGenRenderbuffers(4, m_RBO);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

		Resize(shadowmapResolution);

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

		m_staticInstances = new InstanceData[1024];

		glGenBuffers(1, &m_instanceBuffer);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_instanceBuffer);
		glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_INSTANCES * sizeof(InstanceData), NULL, GL_STREAM_COPY);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, m_instanceBuffer);



	}

	glm::mat4x4 FitFrustum(const glm::mat4x4& camera, const glm::mat4x4& lightViewMat, int shadowmapRes)
	{
		// Get view frustum corner points
		glm::mat4x4 invViewProj = glm::inverse(camera);
		glm::vec3 corners[8];
		corners[0] = glm::vec3(-1.0f, -1.0f, 1.0f);
		corners[1] = glm::vec3(-1.0f, -1.0f, -1.0f);
		corners[2] = glm::vec3(-1.0f, 1.0f, 1.0f);
		corners[3] = glm::vec3(-1.0f, 1.0f, -1.0f);
		corners[4] = glm::vec3(1.0f, -1.0f, 1.0f);
		corners[5] = glm::vec3(1.0f, -1.0f, -1.0f);
		corners[6] = glm::vec3(1.0f, 1.0f, 1.0f);
		corners[7] = glm::vec3(1.0f, 1.0f, -1.0f);
		for (unsigned int i = 0; i < 8; i++)
		{
			glm::vec4 worldPos = invViewProj * glm::vec4(corners[i], 1.0f);
			worldPos.x /= worldPos.w;
			worldPos.y /= worldPos.w;
			worldPos.z /= worldPos.w;
			worldPos.w = 1.0f;
			worldPos = lightViewMat * worldPos;

			//eyePos = m_debugLightFrustum * eyePos;
			//corners[i] = glm::vec3(eyePos);
			// Convert view frustum corners to light space

			corners[i] = glm::vec3(worldPos);
		}

		// Chack corners to finx min/max x/y
		float minX = 1000000.0f, maxX = -1000000.0f;
		float minY = 1000000.0f, maxY = -1000000.0f;
		float minZ = 1000000.0f, maxZ = -1000000.0f;
		for (unsigned int i = 0; i < 8; i++)
		{
			minX = std::min(minX, corners[i].x);
			minY = std::min(minY, corners[i].y);
			minZ = std::min(minZ, corners[i].z);

			maxX = std::max(maxX, corners[i].x);
			maxY = std::max(maxY, corners[i].y);
			maxZ = std::max(maxZ, corners[i].z);
		}

		// Clamp minmax to texels to avoid swimming shadowmaps
		glm::vec2 min = glm::vec2(minX, minY);
		glm::vec2 max = glm::vec2(maxX, maxY);
		glm::vec2 unitsPerTexel = glm::vec2(max-min) / float(shadowmapRes);
		
		min /= unitsPerTexel;
		min = glm::floor(min);
		min *= unitsPerTexel;
		
		max /= unitsPerTexel;
		max = glm::floor(max);
		max *= unitsPerTexel;

		// Set ortho matrix to use the calculated corner points
		return glm::ortho<float>(min.x, max.x, min.y, max.y, -225.0f, -minZ);
		//return glm::ortho<float>(minX, maxX, minY, maxY, -maxZ, -minZ);

	}
//#ifdef _WIN32
//#include <Windows.h>
//#endif
	void ShadowPainter::Render(AnimationManagerGFX* animationManager, const unsigned int& renderIndex, FBOTexture* depthBuffer, glm::mat4 viewMatrix, glm::mat4 projMatrix,
			const unsigned int& geometryStartIndex, const unsigned int& geometryEndIndex, FBOTexture** shadowMaps, const unsigned int& width, const unsigned int& height,
			const glm::vec2& nearFar, RenderInfo& out_RenderInfo)
	{
		BasePainter::Render();

		unsigned int numDrawCalls = 0;
		unsigned int numTris = 0;

		unsigned int startIndex = geometryStartIndex;
		unsigned int endIndex = geometryEndIndex;
		//GLenum error;

		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		int shadowmapRes = shadowMaps[0]->GetWidth();
		glViewport(0, 0, shadowmapRes, shadowmapRes);

		// Set framebuffers for shadowmapping

		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, shadowMaps[0]->GetTextureHandle());
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, shadowMaps[1]->GetTextureHandle());
		//glActiveTexture(GL_TEXTURE2);
		//glBindTexture(GL_TEXTURE_2D, shadowMaps[2]->GetTextureHandle());
		//glActiveTexture(GL_TEXTURE3);
		//glBindTexture(GL_TEXTURE_2D, shadowMaps[3]->GetTextureHandle());
		
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, shadowMaps[0]->GetTextureHandle(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, shadowMaps[1]->GetTextureHandle(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, shadowMaps[2]->GetTextureHandle(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, shadowMaps[3]->GetTextureHandle(), 0);
		
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RBO[0]);
		glClear(GL_DEPTH_BUFFER_BIT);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RBO[1]);
		glClear(GL_DEPTH_BUFFER_BIT);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RBO[2]);
		glClear(GL_DEPTH_BUFFER_BIT);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RBO[3]);
		glClear(GL_DEPTH_BUFFER_BIT);
		
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glClearBufferfv(GL_COLOR, 0, &glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)[0]);
		glDrawBuffer(GL_COLOR_ATTACHMENT1);
		glClearBufferfv(GL_COLOR, 0, &glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)[0]);
		glDrawBuffer(GL_COLOR_ATTACHMENT2);
		glClearBufferfv(GL_COLOR, 0, &glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)[0]);
		glDrawBuffer(GL_COLOR_ATTACHMENT3);
		glClearBufferfv(GL_COLOR, 0, &glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)[0]);



		

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

			glm::mat4x4 matrices[8];
			ShadowData shadowData;

			unsigned int totalShadowcasters = ShadowDataContainer::numPointLights + ShadowDataContainer::numSpotLights + ShadowDataContainer::numDirLights;

			if (totalShadowcasters >= MAXIMUM_SHADOWCASTERS)
			{
				LOG_WARNING << "Number of shadowcasters is too high (" << totalShadowcasters << "), maximum shadowcasters allowed: " << MAXIMUM_SHADOWCASTERS;
				break;
			}

			if (lightType == GFX::LIGHT_TYPES::DIR_SHADOW)
			{
				LightData lightData = *(LightData*)renderJobs.at(l).value;
				if (lightData.orientation.x == 0.0f)
				{
					lightData.orientation.x = 0.0001f;
				}
				// Create matrices for the lights
				bc.viewMatrix = glm::lookAt<float>(-glm::normalize(lightData.orientation) * 50.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				//bc.projMatrix = projMatrix;


				
				float nz = nearFar.x;
				float fz = nearFar.y;
				matrices[0] = glm::perspective<float>(45.0f, 1280.0f/720.0f, 5.0f,	 fz/10.0f);
				matrices[1] = glm::perspective<float>(45.0f, 1280.0f/720.0f, fz/10.0f, fz/5.0f);
				matrices[2] = glm::perspective<float>(45.0f, 1280.0f/720.0f, fz/5.0f, fz/2.0f);
				matrices[3] = glm::perspective<float>(45.0f, 1280.0f/720.0f, fz/2.0f, fz);


				matrices[4] = FitFrustum(matrices[0] * viewMatrix, bc.viewMatrix, shadowmapRes);
				matrices[5] = FitFrustum(matrices[1] * viewMatrix, bc.viewMatrix, shadowmapRes);
				matrices[6] = FitFrustum(matrices[2] * viewMatrix, bc.viewMatrix, shadowmapRes);
				matrices[7] = FitFrustum(matrices[3] * viewMatrix, bc.viewMatrix, shadowmapRes);

				
				glm::mat4x4 dummyProjMat = glm::perspective<float>(45.0f, 1280.0f/720.0f, 5.0f, 125.0f);
				bc.projMatrix = FitFrustum(dummyProjMat * viewMatrix, bc.viewMatrix, shadowmapRes);
				//bc.projMatrix = glm::perspective<float>(45.0f, 1.0f, 20.0f, 100.0f);

				// Add the data to the global array of shadow data for use in LightPainter
				
				shadowData.lightMatrix1 = matrices[4] * bc.viewMatrix;
				shadowData.lightMatrix2 = matrices[5] * bc.viewMatrix;
				shadowData.lightMatrix3 = matrices[6] * bc.viewMatrix;
				shadowData.lightMatrix4 = matrices[7] * bc.viewMatrix;
				//shadowData.lightMatrix = bc.projMatrix * bc.viewMatrix;
				shadowData.atlasCoords = glm::vec4(0.0f);
				ShadowDataContainer::data[totalShadowcasters] = shadowData;
				ShadowDataContainer::numDirLights++;

				
//#ifdef _WIN32
//				if (GetAsyncKeyState(VK_F1))
//				{
//					m_dbgmat1 = matrices[0] * viewMatrix;
//					m_dbgmat2 = matrices[1] * viewMatrix;
//					m_dbgmat3 = matrices[2] * viewMatrix;
//					m_dbgmat4 = matrices[3] * viewMatrix;
//
//					m_dbgmat5 = shadowData.lightMatrix1;
//					m_dbgmat6 = shadowData.lightMatrix2;
//					m_dbgmat7 = shadowData.lightMatrix3;
//					m_dbgmat8 = shadowData.lightMatrix4;
//				}
//#endif
//				
//				DebugDrawing().AddFrustum(m_dbgmat1, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), true);
//				DebugDrawing().AddFrustum(m_dbgmat2, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), true);
//				DebugDrawing().AddFrustum(m_dbgmat3, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), true);
//				DebugDrawing().AddFrustum(m_dbgmat4, glm::vec4(0.0f, 1.0f, 1.0f, 1.0f), true);
//				
//				DebugDrawing().AddFrustum(m_dbgmat5, glm::vec4(1.0f, 0.5f, 0.5f, 1.0f), true);
//				DebugDrawing().AddFrustum(m_dbgmat6, glm::vec4(0.5f, 1.0f, 0.5f, 1.0f), true);
//				DebugDrawing().AddFrustum(m_dbgmat7, glm::vec4(0.5f, 0.5f, 1.0f, 1.0f), true);
//				DebugDrawing().AddFrustum(m_dbgmat8, glm::vec4(0.5f, 1.0f, 1.0f, 1.0f), true);
				
			}
			else if (lightType == GFX::LIGHT_TYPES::SPOT_SHADOW)
			{
				break; // TODO: Implement spot shadowmapping
				//LightData lightData = *(LightData*)renderJobs.at(l).value;
				//if (lightData.orientation.x == 0.0f)
				//{
				//	lightData.orientation.x = 0.0001f;
				//}
				//// Create matrices for the lights
				//bc.viewMatrix = glm::lookAt<float>(lightData.position, lightData.orientation, glm::vec3(0.0f, 1.0f, 0.0f));
				//bc.projMatrix = glm::perspective<float>(glm::degrees(lightData.spot_angle), 1.0f, 0.001f, lightData.radius_length);
				//// Add the data to the global array of shadow data for use in LightPainter
				//ShadowData shadowData;
				//shadowData.lightMatrix = bc.projMatrix * bc.viewMatrix;
				//shadowData.atlasCoords = glm::vec4(1.0f);
				//ShadowDataContainer::data[totalShadowcasters] = shadowData;
				//ShadowDataContainer::numSpotLights++;
			}
			else if (lightType == GFX::LIGHT_TYPES::POINT_SHADOW)
			{
				//// Add the data to the global array of shadow data for use in LightPainter
				//ShadowData shadowData;
				//shadowData.lightMatrix = bc.projMatrix * bc.viewMatrix;
				//shadowData.atlasCoords = glm::vec4(1.0f);
				//ShadowDataContainer::data[totalShadowcasters] = shadowData;
				//ShadowDataContainer::numPointLights+=6;
				break; // TODO: Implement point shadowmapping
			}
			else
			{
				break; // Break if no shadowcasting light
			}

			
			// Loop through all the geometry
			for (unsigned int i = startIndex; i < endIndex;)
			{
				GFXBitmask geometryBitmask = renderJobs[i].bitmask;
				objType = GetBitmaskValue(geometryBitmask, BITMASK::TYPE);
				meshID = GetBitmaskValue(geometryBitmask, BITMASK::MESH_ID);
				currentMesh = meshID;

				if (objType != GFX::OBJECT_TYPES::OPAQUE_GEOMETRY)
					break;
				
				unsigned int instanceCount = 0;
				do
				{
					InstanceData smid = *(InstanceData*)renderJobs.at(i).value;
					m_staticInstances[instanceCount] = smid;
					instanceCount++;
					i++;
					geometryBitmask = renderJobs[i].bitmask;
					meshID = GetBitmaskValue(geometryBitmask, BITMASK::MESH_ID);
				} while (meshID == currentMesh && i < endIndex);

				// Bind mesh for drawing
				Mesh mesh = m_meshManager->GetMesh(currentMesh);
				glBindVertexArray(mesh.VAO);

				if (mesh.skeletonID >= 0)
				{
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

				// Draw each cascade
				bc.projMatrix = matrices[4];
				m_uniformBufferManager->SetBasicCameraUBO(bc);
				glDrawBuffer(GL_COLOR_ATTACHMENT0);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RBO[0]);
				glDrawElementsInstanced(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, (GLvoid*)0, instanceCount);
				
				bc.projMatrix = matrices[5];
				m_uniformBufferManager->SetBasicCameraUBO(bc);
				glDrawBuffer(GL_COLOR_ATTACHMENT1);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RBO[1]);
				glDrawElementsInstanced(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, (GLvoid*)0, instanceCount);
				
				bc.projMatrix = matrices[6];
				m_uniformBufferManager->SetBasicCameraUBO(bc);
				glDrawBuffer(GL_COLOR_ATTACHMENT2);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RBO[2]);
				glDrawElementsInstanced(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, (GLvoid*)0, instanceCount);
				
				bc.projMatrix = matrices[7];
				m_uniformBufferManager->SetBasicCameraUBO(bc);
				glDrawBuffer(GL_COLOR_ATTACHMENT3);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RBO[3]);
				glDrawElementsInstanced(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, (GLvoid*)0, instanceCount);

				numDrawCalls += 4;
				numTris += (mesh.indexCount / 3) * instanceCount * 4;

				//instanceCount = 0;

			}
		}

		m_shaderManager->ResetProgram();
		
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		ClearFBO();

		// Apply gaussain blur to the shadowmap
		m_blurPainter->GaussianBlur(shadowMaps[0], 0.7);
		m_blurPainter->GaussianBlur(shadowMaps[1], 0.5);
		m_blurPainter->GaussianBlur(shadowMaps[2], 0.3);
		m_blurPainter->GaussianBlur(shadowMaps[3], 0.1);

		glViewport(0, 0, width, height);
		
		out_RenderInfo.numDrawCalls = numDrawCalls;
		out_RenderInfo.numTris = numTris;
	}
}

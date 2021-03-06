#include "DeferredPainter.hpp"
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

#define INSTANCED_DRAWING

//Ugly fix for something that messed up OpenGL on GCC.
// causes no problems since there is only one instance of DeferredPainter anyway.
extern "C"
{
    static GLuint m_uniformTexture0;
    static GLuint m_uniformTexture1;
    static GLuint m_uniformTexture2;
    static GLuint m_uniformTexture3;

    static GLuint m_modelMatrixUniform;

    static GLuint m_gammaUniform;

    static GLuint m_instanceBuffer;

    static GLint m_animatedBlend;
	static GLint m_animatedNormal;

	static GLint m_staticBlend;
	static GLint m_staticNormal;

	static unsigned int m_outlineThickness = 0;
}

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
        
        m_animatedBlend = 0;

	}

	DeferredPainter::~DeferredPainter()
	{
	}

	void DeferredPainter::Initialize(GLuint FBO, GLuint dummyVAO)
	{
		BasePainter::Initialize(FBO, dummyVAO);

		//TODO: CHANGE THIS INTO A PROPER STATIC SHADER
		m_shaderManager->CreateProgram("StaticMesh");
		m_shaderManager->LoadShader("shaders/geometry/SimpleGeometryVS.glsl", "StaticMeshVS", GL_VERTEX_SHADER);
		m_shaderManager->LoadShader("shaders/geometry/SimpleGeometryFS.glsl", "StaticMeshFS", GL_FRAGMENT_SHADER);
		m_shaderManager->AttachShader("StaticMeshVS", "StaticMesh");
		m_shaderManager->AttachShader("StaticMeshFS", "StaticMesh");
		m_shaderManager->LinkProgram("StaticMesh");

		//Normal mapped, non-animated shader
		m_shaderManager->CreateProgram("NormalMappedStatic");
		m_shaderManager->LoadShader("shaders/geometry/StaticNormalVS.glsl", "NormalMappedStaticVS", GL_VERTEX_SHADER);
		m_shaderManager->LoadShader("shaders/geometry/NormalMappedFS.glsl", "NormalMappedStaticFS", GL_FRAGMENT_SHADER);
		m_shaderManager->AttachShader("NormalMappedStaticVS", "NormalMappedStatic");
		m_shaderManager->AttachShader("NormalMappedStaticFS", "NormalMappedStatic");
		m_shaderManager->LinkProgram("NormalMappedStatic");

		// Normal map shaders
		m_shaderManager->CreateProgram("StaticNormal");
		m_shaderManager->LoadShader("shaders/geometry/StaticNormalVS.glsl", "StaticNormalVS", GL_VERTEX_SHADER);
		m_shaderManager->LoadShader("shaders/geometry/StaticNormalFS.glsl", "StaticNormalFS", GL_FRAGMENT_SHADER);
		m_shaderManager->AttachShader("StaticNormalVS", "StaticNormal");
		m_shaderManager->AttachShader("StaticNormalFS", "StaticNormal");
		m_shaderManager->LinkProgram("StaticNormal");

		m_staticNormal = m_shaderManager->GetShaderProgramID("StaticNormal");

		m_shaderManager->CreateProgram("AnimatedNormal");
		m_shaderManager->LoadShader("shaders/geometry/AnimatedNormalVS.glsl", "AnimatedNormalVS", GL_VERTEX_SHADER);
		m_shaderManager->LoadShader("shaders/geometry/StaticNormalFS.glsl", "AnimatedNormalFS", GL_FRAGMENT_SHADER);
		m_shaderManager->AttachShader("AnimatedNormalVS", "AnimatedNormal");
		m_shaderManager->AttachShader("AnimatedNormalFS", "AnimatedNormal");
		m_shaderManager->LinkProgram("AnimatedNormal");

		m_animatedNormal = m_shaderManager->GetShaderProgramID("AnimatedNormal");

		// Blend map shaders
		m_shaderManager->CreateProgram("StaticBlend");
		m_shaderManager->LoadShader("shaders/geometry/StaticBlendVS.glsl", "StaticBlendVS", GL_VERTEX_SHADER);
		m_shaderManager->LoadShader("shaders/geometry/StaticBlendFS.glsl", "StaticBlendFS", GL_FRAGMENT_SHADER);
		m_shaderManager->AttachShader("StaticBlendVS", "StaticBlend");
		m_shaderManager->AttachShader("StaticBlendFS", "StaticBlend");
		m_shaderManager->LinkProgram("StaticBlend");

		m_staticBlend = m_shaderManager->GetShaderProgramID("StaticBlend");

		m_shaderManager->CreateProgram("AnimatedBlend");
		m_shaderManager->LoadShader("shaders/geometry/AnimatedBlendVS.glsl", "AnimatedBlendVS", GL_VERTEX_SHADER);
		m_shaderManager->LoadShader("shaders/geometry/StaticBlendFS.glsl", "AnimatedBlendFS", GL_FRAGMENT_SHADER);
		m_shaderManager->AttachShader("AnimatedBlendVS", "AnimatedBlend");
		m_shaderManager->AttachShader("AnimatedBlendFS", "AnimatedBlend");
		m_shaderManager->LinkProgram("AnimatedBlend");

		m_animatedBlend = m_shaderManager->GetShaderProgramID("AnimatedBlend");

		m_shaderManager->CreateProgram("AnimatedOutline");
		m_shaderManager->LoadShader("shaders/Outline/AnimatedOutlineVS.glsl", "AnimatedOutlineVS", GL_VERTEX_SHADER);
		m_shaderManager->LoadShader("shaders/Outline/OutlineFS.glsl", "AnimatedOutlineFS", GL_FRAGMENT_SHADER);
		m_shaderManager->AttachShader("AnimatedOutlineVS", "AnimatedOutline");
		m_shaderManager->AttachShader("AnimatedOutlineFS", "AnimatedOutline");
		m_shaderManager->LinkProgram("AnimatedOutline");

		m_shaderManager->CreateProgram("StaticOutline");
		m_shaderManager->LoadShader("shaders/Outline/StaticOutlineVS.glsl", "StaticOutlineVS", GL_VERTEX_SHADER);
		m_shaderManager->LoadShader("shaders/Outline/OutlineFS.glsl", "StaticOutlineFS", GL_FRAGMENT_SHADER);
		m_shaderManager->AttachShader("StaticOutlineVS", "StaticOutline");
		m_shaderManager->AttachShader("StaticOutlineFS", "StaticOutline");
		m_shaderManager->LinkProgram("StaticOutline");
		
        m_uniformBufferManager->SetUniformBlockBindingIndex(m_shaderManager->GetShaderProgramID("StaticMesh"),			"PerFrameBlock", UniformBufferManager::CAMERA_BINDING_INDEX);
        m_uniformBufferManager->SetUniformBlockBindingIndex(m_shaderManager->GetShaderProgramID("NormalMappedStatic"),	"PerFrameBlock", UniformBufferManager::CAMERA_BINDING_INDEX);
        m_uniformBufferManager->SetUniformBlockBindingIndex(m_shaderManager->GetShaderProgramID("StaticNormal"),		"PerFrameBlock", UniformBufferManager::CAMERA_BINDING_INDEX);
        m_uniformBufferManager->SetUniformBlockBindingIndex(m_shaderManager->GetShaderProgramID("AnimatedNormal"),		"PerFrameBlock", UniformBufferManager::CAMERA_BINDING_INDEX);
        m_uniformBufferManager->SetUniformBlockBindingIndex(m_shaderManager->GetShaderProgramID("StaticBlend"),			"PerFrameBlock", UniformBufferManager::CAMERA_BINDING_INDEX);
        m_uniformBufferManager->SetUniformBlockBindingIndex(m_shaderManager->GetShaderProgramID("AnimatedBlend"),		"PerFrameBlock", UniformBufferManager::CAMERA_BINDING_INDEX);
        m_uniformBufferManager->SetUniformBlockBindingIndex(m_shaderManager->GetShaderProgramID("AnimatedOutline"),		"PerFrameBlock", UniformBufferManager::CAMERA_BINDING_INDEX);
        m_uniformBufferManager->SetUniformBlockBindingIndex(m_shaderManager->GetShaderProgramID("StaticOutline"),		"PerFrameBlock", UniformBufferManager::CAMERA_BINDING_INDEX);
		
        m_uniformBufferManager->SetUniformBlockBindingIndex(m_shaderManager->GetShaderProgramID("StaticMesh"),			"instanceBufferOffset", UniformBufferManager::INSTANCE_ID_OFFSET_INDEX);
        m_uniformBufferManager->SetUniformBlockBindingIndex(m_shaderManager->GetShaderProgramID("NormalMappedStatic"),	"instanceBufferOffset", UniformBufferManager::INSTANCE_ID_OFFSET_INDEX);
        m_uniformBufferManager->SetUniformBlockBindingIndex(m_shaderManager->GetShaderProgramID("StaticNormal"),		"instanceBufferOffset", UniformBufferManager::INSTANCE_ID_OFFSET_INDEX);
        m_uniformBufferManager->SetUniformBlockBindingIndex(m_shaderManager->GetShaderProgramID("AnimatedNormal"),		"instanceBufferOffset", UniformBufferManager::INSTANCE_ID_OFFSET_INDEX);
        m_uniformBufferManager->SetUniformBlockBindingIndex(m_shaderManager->GetShaderProgramID("StaticBlend"),			"instanceBufferOffset", UniformBufferManager::INSTANCE_ID_OFFSET_INDEX);
        m_uniformBufferManager->SetUniformBlockBindingIndex(m_shaderManager->GetShaderProgramID("AnimatedBlend"),		"instanceBufferOffset", UniformBufferManager::INSTANCE_ID_OFFSET_INDEX);
        m_uniformBufferManager->SetUniformBlockBindingIndex(m_shaderManager->GetShaderProgramID("AnimatedOutline"),		"instanceBufferOffset", UniformBufferManager::INSTANCE_ID_OFFSET_INDEX);
        m_uniformBufferManager->SetUniformBlockBindingIndex(m_shaderManager->GetShaderProgramID("StaticOutline"),		"instanceBufferOffset", UniformBufferManager::INSTANCE_ID_OFFSET_INDEX);

		m_outlineThickness = 2;
		m_staticInstances = new InstanceData[MAX_INSTANCES];
		

		glGenBuffers(1, &m_instanceBuffer);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, m_instanceBuffer);
		glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_INSTANCES * sizeof(InstanceData), NULL, GL_STREAM_COPY);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, 0);

		glGenBuffers(1, &m_instanceOffsetBuffer);
		glBindBuffer(GL_UNIFORM_BUFFER, m_instanceOffsetBuffer);
		glBufferData(GL_UNIFORM_BUFFER, 4*sizeof(unsigned int), NULL, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, UniformBufferManager::INSTANCE_ID_OFFSET_INDEX, 0);

	}

	void DeferredPainter::Render(AnimationManagerGFX* animationManager, unsigned int& renderIndex,
		FBOTexture* depthBuffer, FBOTexture* normalDepth, FBOTexture* diffuse, FBOTexture* specular,
		FBOTexture* glowMatID, glm::mat4 viewMatrix, glm::mat4 projMatrix, const float& gamma, RenderInfo& out_RenderInfo)
	{
		BasePainter::Render();
		unsigned int numDrawCalls = 0;
		unsigned int numTris = 0;

		BindGBuffer(depthBuffer, normalDepth, diffuse, specular, glowMatID);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		glClearStencil(0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// Clear depth RT
		float c[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 1, &glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)[0]);
		m_shaderManager->UseProgram("StaticBlend");

		BasicCamera bc;
		bc.viewMatrix = viewMatrix;
		bc.projMatrix = projMatrix;

		m_uniformBufferManager->SetBasicCameraUBO(bc);

		std::vector<RenderJobManager::RenderJob> renderJobs = m_renderJobManager->GetJobs();

		unsigned int currentShader = std::numeric_limits<decltype(currentShader)>::max();
		unsigned int currentMaterial = std::numeric_limits<decltype(currentMaterial)>::max();
		unsigned int currentMesh = std::numeric_limits<decltype(currentMesh)>::max();
		unsigned int currentLayer = std::numeric_limits<decltype(currentMesh)>::max();


		unsigned int objType = std::numeric_limits<decltype(objType)>::max();
		unsigned int viewport = std::numeric_limits<decltype(viewport)>::max();
		unsigned int layer = std::numeric_limits<decltype(layer)>::max();
		unsigned int translucency = std::numeric_limits<decltype(translucency)>::max();
		unsigned int meshID = std::numeric_limits<decltype(meshID)>::max();
		unsigned int material = std::numeric_limits<decltype(material)>::max();
		unsigned int depth = std::numeric_limits<decltype(depth)>::max();

		int instanceBufferSize = 0;

		{
			Material mat;
			Mesh mesh;
			GFXBitmask bitmask;
			int instanceCount = 0;
			unsigned int instanceOffset = 0;
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

				if (material == currentMaterial && meshID == currentMesh && !endMe && instanceCount < MAX_INSTANCES && layer == currentLayer)
				{
					InstanceData smid = *(InstanceData*)renderJobs.at(i).value;
					m_staticInstances[instanceOffset + instanceCount] = smid;
					instanceCount++;
					instanceBufferSize++;
				}
				else
				{
					if (i > 0)
					{
						instanceOffset += instanceCount;
						instanceCount = 0;
					}

					if (endMe)
						break;

					if (material != currentMaterial)
					{
						mat = m_materialManager->GetMaterial(material);

						currentMaterial = material;

						//compare shader
						if (mat.shaderProgramID != currentShader)
						{
							currentShader = mat.shaderProgramID;
						}
					}

					if (meshID != currentMesh)
					{
						mesh = m_meshManager->GetMesh(meshID);
						currentMesh = meshID;
					}

					if (layer != currentLayer)
					{
						currentLayer = layer;
					}
					InstanceData smid = *(InstanceData*)renderJobs.at(i).value;
					m_staticInstances[instanceOffset + instanceCount] = smid;
					instanceCount++;
					instanceBufferSize++;
				}
			}
		}
		
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, m_instanceBuffer);
		InstanceData* pData = (InstanceData*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, MAX_INSTANCES * sizeof(InstanceData),
			GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT);

		memcpy(pData, m_staticInstances, instanceBufferSize * sizeof(InstanceData));

		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

		
		currentShader = std::numeric_limits<decltype(currentShader)>::max();
		currentMaterial = std::numeric_limits<decltype(currentMaterial)>::max();
		currentMesh = std::numeric_limits<decltype(currentMesh)>::max();
		currentLayer = std::numeric_limits<decltype(currentMesh)>::max();
		objType = std::numeric_limits<decltype(objType)>::max();
		viewport = std::numeric_limits<decltype(viewport)>::max();
		layer = std::numeric_limits<decltype(layer)>::max();
		translucency = std::numeric_limits<decltype(translucency)>::max();
		meshID = std::numeric_limits<decltype(meshID)>::max();
		material = std::numeric_limits<decltype(material)>::max();
		depth = std::numeric_limits<decltype(depth)>::max();

		Material mat;
		Mesh mesh;
		GFXBitmask bitmask;
		int instanceCount = 0;
		unsigned int instanceOffset = 0;
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

			if (material == currentMaterial && meshID == currentMesh && !endMe && instanceCount < MAX_INSTANCES && layer == currentLayer)
			{
				//InstanceData smid = *(InstanceData*)renderJobs.at(i).value;
				//m_staticInstances[instanceOffset+instanceCount] = smid;
				instanceCount++;
			}
			else
			{
				if (i > 0)
				{
						
					unsigned int asd[4] = {instanceOffset, 0U, 0U, 0U};
					glBindBufferBase(GL_UNIFORM_BUFFER, UniformBufferManager::INSTANCE_ID_OFFSET_INDEX, m_instanceOffsetBuffer);
					glBufferSubData(GL_UNIFORM_BUFFER, 0, 4*sizeof(unsigned int), asd);

					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.IBO);

					if (currentLayer == LAYER_TYPES::OUTLINE_LAYER)
					{
						glEnable(GL_STENCIL_TEST);
						glStencilFunc(GL_ALWAYS, 1, -1);
						glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
						glEnable(GL_DEPTH_TEST);
					}

					glDrawElementsInstanced(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, (GLvoid*)0, instanceCount);
					numDrawCalls++;
					numTris += (mesh.indexCount / 3) * instanceCount;

					if (currentLayer == LAYER_TYPES::OUTLINE_LAYER)
					{
						glDisable(GL_DEPTH_TEST);
						glStencilFunc(GL_NOTEQUAL, 1, -1);
						glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
						glLineWidth(m_staticInstances[instanceOffset + instanceCount - 1].outlineColor[3]);
						glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

						if (currentShader == m_staticBlend || currentShader == m_staticNormal)
							m_shaderManager->UseProgram("StaticOutline");
						else if (currentShader == m_animatedBlend || currentShader == m_animatedNormal)
							m_shaderManager->UseProgram("AnimatedOutline");

						glDrawElementsInstanced(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, (GLvoid*)0, instanceCount);
						numDrawCalls++;
						numTris += (mesh.indexCount / 3) * instanceCount;

						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


						glUseProgram(currentShader);
						glEnable(GL_DEPTH_TEST);
						glDisable(GL_STENCIL_TEST);
					}
					
					instanceOffset += instanceCount;
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
						currentShader = mat.shaderProgramID;

						m_uniformTexture0 = m_shaderManager->GetUniformLocation(currentShader, "gDiffuse");
						m_uniformTexture1 = m_shaderManager->GetUniformLocation(currentShader, "gNormal");
						m_uniformTexture2 = m_shaderManager->GetUniformLocation(currentShader, "gSpecular");
						m_uniformTexture3 = m_shaderManager->GetUniformLocation(currentShader, "gGlow");

						m_gammaUniform = m_shaderManager->GetUniformLocation(currentShader, "gGamma");
					}

					//set textures
					m_textureManager->BindTexture(m_textureManager->GetTexture(mat.textures[0]).textureHandle, m_uniformTexture0, 0, GL_TEXTURE_2D);
					m_textureManager->BindTexture(m_textureManager->GetTexture(mat.textures[1]).textureHandle, m_uniformTexture1, 1, GL_TEXTURE_2D);
					m_textureManager->BindTexture(m_textureManager->GetTexture(mat.textures[2]).textureHandle, m_uniformTexture2, 2, GL_TEXTURE_2D);
					m_textureManager->BindTexture(m_textureManager->GetTexture(mat.textures[3]).textureHandle, m_uniformTexture3, 3, GL_TEXTURE_2D);

					//Set gamma
					m_shaderManager->SetUniform(gamma, m_gammaUniform);
				}

				if (meshID != currentMesh)
				{
					mesh = m_meshManager->GetMesh(meshID);
					currentMesh = meshID;

					glBindVertexArray(mesh.VAO);
				}

				if (layer != currentLayer)
				{
					currentLayer = layer;
				}
				instanceCount++;
			}


		}

		m_shaderManager->ResetProgram();

		glClearStencil(0);
		glClear(GL_STENCIL_BUFFER_BIT);
		glDisable(GL_STENCIL_TEST);

		ClearFBO();
		renderIndex = i;
		out_RenderInfo.numDrawCalls = numDrawCalls;
		out_RenderInfo.numTris = numTris;
	}

	void DeferredPainter::BindGBuffer(FBOTexture* depthBuffer, FBOTexture* normalDepth, FBOTexture* diffuse, FBOTexture* specular, FBOTexture* glowMatID)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);


		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthBuffer->GetTextureHandle(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, normalDepth->GetTextureHandle(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, diffuse->GetTextureHandle(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, specular->GetTextureHandle(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, glowMatID->GetTextureHandle(), 0);

		// define outputs
		GLenum drawBuffers[] = { GL_NONE, GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
		glDrawBuffers(5, drawBuffers);

	}
}

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
#include <glm/ext.hpp>

//Ugly fix for something that messed up OpenGL on GCC.
// causes no problems since there is only one instance of DeferredPainter anyway.
extern "C"
{
	static GLint m_modelMatrixUniform;
	static GLint m_invModelMatrixUniform;
	static GLint m_decalSizeUniform;
	static GLint m_diffuseTextureUniform;
	static GLint m_glowTextureUniform;
	static GLint m_normalDepthUniform;
	static GLint m_gammaUniform;
	static GLint m_invViewProjUniform;

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

		m_shaderManager->CreateProgram("DecalShader");
		m_shaderManager->LoadShader("shaders/decals/DecalsVS.glsl", "DecalsVS", GL_VERTEX_SHADER);
		m_shaderManager->LoadShader("shaders/decals/DecalsFS.glsl", "DecalsFS", GL_FRAGMENT_SHADER);
		m_shaderManager->AttachShader("DecalsVS", "DecalShader");
		m_shaderManager->AttachShader("DecalsFS", "DecalShader");
		m_shaderManager->LinkProgram("DecalShader");

		m_shaderManager->UseProgram("DecalShader");

		m_modelMatrixUniform = m_shaderManager->GetUniformLocation("DecalShader", "modelMatrix");
		m_decalSizeUniform = m_shaderManager->GetUniformLocation("DecalShader", "decalSize");

		m_invModelMatrixUniform = m_shaderManager->GetUniformLocation("DecalShader", "invModelMatrix");
		m_invViewProjUniform = m_shaderManager->GetUniformLocation("DecalShader", "invProjView");

		m_diffuseTextureUniform = m_shaderManager->GetUniformLocation("DecalShader", "gDiffuse");
		m_glowTextureUniform = m_shaderManager->GetUniformLocation("DecalShader", "gGlow");
		m_normalDepthUniform = m_shaderManager->GetUniformLocation("DecalShader", "gNormalDepth");
		m_gammaUniform = m_shaderManager->GetUniformLocation("DecalShader", "gGamma");

		m_shaderManager->ResetProgram();
		
	}

	void DecalPainter::Render(AnimationManagerGFX* animationManager, unsigned int& renderIndex,
		FBOTexture* depthBuffer, FBOTexture* normalDepth, FBOTexture* diffuse, FBOTexture* specular, FBOTexture* glowMatID, glm::mat4 viewMatrix, glm::mat4 projMatrix, const float& gamma)
	{
		BasePainter::Render();

		BindGBuffer(depthBuffer, diffuse, specular, glowMatID);

		glEnable(GL_DEPTH_TEST);
		glDepthMask(false);
		//glCullFace(GL_FRONT);
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


		m_shaderManager->UseProgram("DecalShader");

		m_textureManager->BindTexture(normalDepth->GetTextureHandle(), m_normalDepthUniform, 0, GL_TEXTURE_2D);
	
		glm::mat4 invProjView = glm::inverse(projMatrix * viewMatrix);
		m_shaderManager->SetUniform(1, invProjView, m_invViewProjUniform);

		m_shaderManager->SetUniform(gamma, m_gammaUniform);

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

		Material mat;
		Mesh mesh;
		GFXBitmask bitmask;

		//Loop over all decals
		//Set their texture, modelmatrix, inverse model matrix and decal size
		//Render da cube

		for (int i = 0; i < renderJobs.size(); i++)
		{
			bitmask = renderJobs[i].bitmask;

			objType = GetBitmaskValue(bitmask, BITMASK::TYPE);

			//IF NOT DECAL
			//CONTINUE
			if (objType != GFX::OBJECT_TYPES::DECAL_GEOMETRY)
			{
				continue;
			}

			
			//GET ALL SHIT FROM BITMASK
			viewport = GetBitmaskValue(bitmask, BITMASK::VIEWPORT_ID);
			layer = GetBitmaskValue(bitmask, BITMASK::LAYER);
			translucency = GetBitmaskValue(bitmask, BITMASK::TRANSLUCENCY_TYPE);
			meshID = GetBitmaskValue(bitmask, BITMASK::MESH_ID);
			material = GetBitmaskValue(bitmask, BITMASK::MATERIAL_ID);
			depth = GetBitmaskValue(bitmask, BITMASK::DEPTH);

			if (material != currentMaterial)
			{
				currentMaterial = material;
				mat = m_materialManager->GetMaterial(currentMaterial);
				//set textures
				m_textureManager->BindTexture(m_textureManager->GetTexture(mat.textures[0]).textureHandle, m_diffuseTextureUniform, 1, GL_TEXTURE_2D);
				//m_textureManager->BindTexture(m_textureManager->GetTexture(mat.textures[1]).textureHandle, m_uniformTexture1, 2, GL_TEXTURE_2D);
				//m_textureManager->BindTexture(m_textureManager->GetTexture(mat.textures[2]).textureHandle, m_uniformTexture2, 3, GL_TEXTURE_2D);
				m_textureManager->BindTexture(m_textureManager->GetTexture(mat.textures[3]).textureHandle, m_glowTextureUniform, 2, GL_TEXTURE_2D);
			}

			if (meshID != currentMesh)
			{
				mesh = m_meshManager->GetMesh(meshID);
				currentMesh = meshID;

				glBindVertexArray(mesh.VAO);
			}

			InstanceData smid = *(InstanceData*)renderJobs.at(i).value;
			
			m_shaderManager->SetUniform(1, smid.modelMatrix, m_modelMatrixUniform);

	
			m_shaderManager->SetUniform(1,  glm::inverse(smid.modelMatrix), m_invModelMatrixUniform);
			m_shaderManager->SetUniform(10, 10, 10, m_decalSizeUniform);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.IBO);
			glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, (GLvoid*)0);
		}

		
		glDepthMask(true);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glCullFace(GL_BACK);
		m_shaderManager->ResetProgram();
		ClearFBO();
	}

	void DecalPainter::BindGBuffer(FBOTexture* depthBuffer, FBOTexture* diffuse, FBOTexture* specular, FBOTexture* glowMatID)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthBuffer->GetTextureHandle(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, diffuse->GetTextureHandle(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, specular->GetTextureHandle(), 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, glowMatID->GetTextureHandle(), 0);

		// define outputs
		GLenum drawBuffers[] = { GL_NONE, GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
		glDrawBuffers(4, drawBuffers);

	}
}

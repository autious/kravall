#include "ParticlePainter.hpp"

#include <Shaders/ShaderManager.hpp>
#include <Buffers/UniformBufferManager.hpp>
#include <Renderer/RenderJobManager.hpp>
#include <Renderer/DeferredRenderer/FBOTexture.hpp>
#include <Material/MaterialManager.hpp>
#include <Textures/TextureManager.hpp>
#include <Particle/ParticleManager.hpp>

#include <logger/Logger.hpp>

#include <limits>

namespace GFX
{
    ParticlePainter::ParticlePainter(GFX::ShaderManager* shaderManager, GFX::UniformBufferManager* uniformBufferManager
            , GFX::RenderJobManager* renderJobManager, GFX::MaterialManager* materialManager
            , GFX::TextureManager* textureManager, GFX::ParticleManager* particleManager) : BasePainter(shaderManager, uniformBufferManager) 
    {
        m_renderJobManager = renderJobManager;
        m_materialManager = materialManager;
        m_textureManager = textureManager;
        m_particleManager = particleManager;

        m_outlineThickness = 2;
    }

    ParticlePainter::~ParticlePainter()
    {

    }

    void ParticlePainter::Initialize(GLuint FBO, GLuint VAO)
    {
        BasePainter::Initialize(FBO, VAO);

        m_shaderManager->CreateProgram("ParticleApply");
        m_shaderManager->LoadShader("shaders/particle/ParticleApplyVS.glsl", "ApplyVS", GL_VERTEX_SHADER );
        m_shaderManager->LoadShader("shaders/FSQuadGS.glsl", "ApplyGS", GL_GEOMETRY_SHADER );
        m_shaderManager->LoadShader("shaders/particle/ParticleApplyFS.glsl", "ApplyFS", GL_FRAGMENT_SHADER );
        m_shaderManager->AttachShader("ApplyVS", "ParticleApply");
        m_shaderManager->AttachShader("ApplyGS", "ParticleApply");
        m_shaderManager->AttachShader("ApplyFS", "ParticleApply");
        m_shaderManager->LinkProgram("ParticleApply");
         
        m_shaderManager->CreateProgram("SmokeGrenade");
        m_shaderManager->LoadShader("shaders/particle/SmokeGrenadeVS.glsl", "SmokeGrenadeVS", GL_VERTEX_SHADER);
        m_shaderManager->LoadShader("shaders/particle/SmokeGrenadeGS.glsl", "SmokeGrenadeGS", GL_GEOMETRY_SHADER);
        m_shaderManager->LoadShader("shaders/particle/SmokeGrenadeFS.glsl", "SmokeGrenadeFS", GL_FRAGMENT_SHADER);
        m_shaderManager->AttachShader("SmokeGrenadeVS", "SmokeGrenade");
        m_shaderManager->AttachShader("SmokeGrenadeGS", "SmokeGrenade");
        m_shaderManager->AttachShader("SmokeGrenadeFS", "SmokeGrenade");
        m_shaderManager->LinkProgram("SmokeGrenade");

        m_uniformBufferManager->SetUniformBlockBindingIndex(m_shaderManager->GetShaderProgramID("SmokeGrenade"), "PerFrameBlock", UniformBufferManager::CAMERA_BINDING_INDEX);

        GLint applyTextureLocation = m_shaderManager->GetUniformLocation("ParticleApply", "textureIN");
        if(applyTextureLocation >= 0)
        {
            m_shaderManager->UseProgram("ParticleApply");
            glUniform1i(applyTextureLocation, 0);
        }
    
    }

    void ParticlePainter::Render(unsigned int& renderIndex,GFX::FBOTexture* particleTarget, GFX::FBOTexture* depthBuffer, GFX::FBOTexture* normalDepth, GFX::FBOTexture* specular, GFX::FBOTexture* glowMatID, GLuint toneMappedTexture, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
    {
		BasePainter::Render();

        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, particleTarget->GetTextureHandle(), 0);

        glDrawBuffer(GL_COLOR_ATTACHMENT0);

        glClear(GL_COLOR_BUFFER_BIT);
        
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthBuffer->GetTextureHandle(), 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, glowMatID->GetTextureHandle(), 0);

        GLenum particleDrawBuffers[] = { GL_NONE, GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };

        glDrawBuffers(3, particleDrawBuffers);

        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);

	    BasicCamera bc;
		bc.viewMatrix = viewMatrix;
		bc.projMatrix = projectionMatrix;

		m_uniformBufferManager->SetBasicCameraUBO(bc);

        std::vector<GFX::RenderJobManager::RenderJob> renderJobs = m_renderJobManager->GetJobs();

        unsigned int currentShader = std::numeric_limits<decltype(currentShader)>::max();
        unsigned int currentMesh = std::numeric_limits<decltype(currentMesh)>::max();
        unsigned int currentMaterial = std::numeric_limits<decltype(currentMaterial)>::max();
        unsigned int currentLayer = std::numeric_limits<decltype(currentLayer)>::max();
        unsigned int currentTranslucency = std::numeric_limits<decltype(currentTranslucency)>::max();

        unsigned int mesh = 0;        
        unsigned int material = 0;        
        unsigned int layer = 0;
        unsigned int translucency = 0;
        unsigned int objectType = 0;

        const GFX::ParticleData* particleData;
        GFX::GFXBitmask renderJob;


        for(unsigned int i=0; renderIndex < renderJobs.size(); ++i, ++renderIndex)        
        {
            renderJob = renderJobs[renderIndex].bitmask;
            
            objectType = GFX::GetBitmaskValue(renderJob, GFX::BITMASK::TYPE);

            if(objectType != GFX::OBJECT_TYPES::PARTICLE_GEOMETRY)
            {
                renderIndex--;
                break;
            }            

            layer = GetBitmaskValue(renderJob, GFX::BITMASK::LAYER);
            translucency = GetBitmaskValue(renderJob, GFX::BITMASK::TRANSLUCENCY_TYPE);
            mesh = GetBitmaskValue(renderJob, GFX::BITMASK::MESH_ID);
            material = GetBitmaskValue(renderJob, GFX::BITMASK::MATERIAL_ID);


            if(material != currentMaterial)
            {
                GFX::Material mat = m_materialManager->GetMaterial(material);
                
                currentMaterial = material;

                if(mat.shaderProgramID != currentShader)
                {
                    currentShader = mat.shaderProgramID;

                    m_texture0Uniform = m_shaderManager->GetUniformLocation(currentShader, "gDiffuse");
                    m_texture1Uniform = m_shaderManager->GetUniformLocation(currentShader, "gNormal");
                    m_texture2Uniform = m_shaderManager->GetUniformLocation(currentShader, "gSpecular");
                    m_texture3Uniform = m_shaderManager->GetUniformLocation(currentShader, "gGlow");
                    m_depthBufferUniform = m_shaderManager->GetUniformLocation(currentShader, "gDepthBuffer");
                }
                
                glUseProgram(currentShader);

                m_textureManager->BindTexture(m_textureManager->GetTexture(mat.textures[0]).textureHandle, m_texture0Uniform, 0, GL_TEXTURE_2D);
                m_textureManager->BindTexture(m_textureManager->GetTexture(mat.textures[1]).textureHandle, m_texture1Uniform, 1, GL_TEXTURE_2D);
                m_textureManager->BindTexture(m_textureManager->GetTexture(mat.textures[2]).textureHandle, m_texture2Uniform, 2, GL_TEXTURE_2D);
                m_textureManager->BindTexture(m_textureManager->GetTexture(mat.textures[3]).textureHandle, m_texture3Uniform, 3, GL_TEXTURE_2D);

                if(m_depthBufferUniform >= 0)
                {

                    m_textureManager->BindTexture(m_textureManager->GetTexture(mat.textures[3]).textureHandle, m_texture3Uniform, 3, GL_TEXTURE_2D);
                }
            }

            if(mesh != currentMesh)
            {
                currentMesh = mesh;
                particleData = m_particleManager->GetParticleData(currentMesh);

                //TODO: Remove this check once the level heap is destroyed in a finalizer in the content manager
                //This check is needed since the level heap is destroyed before calling render. The particle data is destroyed when the level heap is cleared.
                if(particleData == nullptr)
                {
                    continue;
                }
                
                glBindBuffer(GL_ARRAY_BUFFER, particleData->VBO);
                glBindVertexArray(particleData->VAO);
            }
            
            if(translucency != currentTranslucency)
            {
                currentTranslucency = translucency;
                switch (currentTranslucency)
                {
				case GFX::TRANSLUCENCY_TYPES::OPAQUE_TRANSLUCENCY:
                    {
                        glDisable(GL_BLEND);
                        glBlendEquation(GL_FUNC_ADD);
                        glBlendFunc(GL_ONE, GL_ZERO);
                        break;
                    }

				case GFX::TRANSLUCENCY_TYPES::ADDITIVE_TRANSLUCENCY:
                    {
                        glEnable(GL_BLEND);
                        glBlendEquation(GL_FUNC_ADD);
                        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
                        break;
                    }

				case GFX::TRANSLUCENCY_TYPES::SUBTRACTIVE_TRANSLUCENCY:
                    {
                        glEnable(GL_BLEND);
                        glBlendEquation(GL_FUNC_SUBTRACT);
                        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                        break;
                    }

				case GFX::TRANSLUCENCY_TYPES::MULTIPLICATIVE_TRANSLUCENCY:
                    {
                        glEnable(GL_BLEND);
                        glBlendEquation(GL_FUNC_ADD);
                        glBlendFunc(GL_DST_COLOR, GL_ZERO);
                        break;
                    }
                }
            }

            if(layer != currentLayer)            
            {
                currentLayer = layer;
                switch (currentLayer)
                {
                    case GFX::LAYER_TYPES::OUTLINE_LAYER:
                    {
						glDisable(GL_DEPTH_TEST);
						glStencilFunc(GL_NOTEQUAL, 1, -1);
						glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
						glLineWidth(m_outlineThickness);
						glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                        break;
                    }
                    case GFX::LAYER_TYPES::MESH_LAYER:
                    {
						glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						glEnable(GL_DEPTH_TEST);
						glDisable(GL_STENCIL_TEST);
                        break;
                    }
                }
            }
            glDrawArrays(GL_POINTS, 0, particleData->particleCount);
        }
        
        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthBuffer->GetTextureHandle(), 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, normalDepth->GetTextureHandle(), 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, toneMappedTexture, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, specular->GetTextureHandle(), 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, glowMatID->GetTextureHandle(), 0);

        GLenum drawBuffers[] = { GL_NONE, GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };

        glDrawBuffers(5, drawBuffers);
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);    
       
        m_shaderManager->UseProgram("ParticleApply");
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, particleTarget->GetTextureHandle());

        glBindVertexArray(m_dummyVAO);
        glDrawArrays(GL_POINTS, 0, 1);
        
        glDisable(GL_BLEND);
        glDepthMask(GL_TRUE);
    }
}

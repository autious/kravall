#include "ParticlePainter.hpp"

#include <Shaders/ShaderManager.hpp>
#include <Buffers/UniformBufferManager.hpp>
#include <Renderer/RenderJobManager.hpp>
#include <Renderer/DeferredRenderer/FBOTexture.hpp>
#include <Material/MaterialManager.hpp>
#include <Textures/TextureManager.hpp>
#include <Particle/ParticleManager.hpp>

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
         
        m_shaderManager->CreateProgram("SmokeGrenade");
        m_shaderManager->LoadShader("shaders/particle/SmokeGrenadeVS.glsl", "SmokeGrenadeVS", GL_VERTEX_SHADER);
        m_shaderManager->LoadShader("shaders/particle/SmokeGrenadeGS.glsl", "SmokeGrenadeGS", GL_GEOMETRY_SHADER);
        m_shaderManager->LoadShader("shaders/particle/SmokeGrenadeFS.glsl", "SmokeGrenadeFS", GL_FRAGMENT_SHADER);
        m_shaderManager->AttachShader("SmokeGrenadeVS", "SmokeGrenade");
        m_shaderManager->AttachShader("SmokeGrenadeGS", "SmokeGrenade");
        m_shaderManager->AttachShader("SmokeGrenadeFS", "SmokeGrenade");
        m_shaderManager->LinkProgram("SmokeGrenade");
    }

    void ParticlePainter::Render(unsigned int& renderIndex, GFX::FBOTexture* depthBufer, GFX::FBOTexture* colorBuffer, const glm::mat4& viewMatrix, const glm::mat4& projMatrix)
    {
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

        std::vector<GFX::RenderJobManager::RenderJob> renderJobs = m_renderJobManager->GetJobs();
        const GFX::ParticleData* particleData;
        GFX::GFXBitmask renderJob;

        for(unsigned int i=0; renderIndex < renderJobs.size(); ++i, ++renderIndex)        
        {
            renderJob = renderJobs[renderIndex].bitmask;
            
            objectType = GFX::GetBitmaskValue(renderJob, GFX::BITMASK::TYPE);

            if(objectType != GFX::OBJECT_TYPES::PARTICLE_GEOMETRY)
            {
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
                }
                
                glUseProgram(currentShader);

                m_textureManager->BindTexture(m_textureManager->GetTexture(mat.textures[0]).textureHandle, m_texture0Uniform, 0, GL_TEXTURE_2D);
                m_textureManager->BindTexture(m_textureManager->GetTexture(mat.textures[1]).textureHandle, m_texture1Uniform, 1, GL_TEXTURE_2D);
                m_textureManager->BindTexture(m_textureManager->GetTexture(mat.textures[2]).textureHandle, m_texture2Uniform, 2, GL_TEXTURE_2D);
                m_textureManager->BindTexture(m_textureManager->GetTexture(mat.textures[3]).textureHandle, m_texture3Uniform, 3, GL_TEXTURE_2D);
            }

            if(mesh != currentMesh)
            {
                currentMesh = mesh;
                particleData = m_particleManager->GetParticleData(currentMesh);
                glBindVertexArray(particleData->VAO);
            }
            
            if(translucency != currentTranslucency)
            {
                currentTranslucency = translucency;
                switch (currentTranslucency)
                {
                    case GFX::TRANSLUCENCY_TYPES::OPAQUE:
                    {
                        glDisable(GL_BLEND);
                        glBlendEquation(GL_FUNC_ADD);
                        glBlendFunc(GL_ONE, GL_ZERO);
                        break;
                    }

                    case GFX::TRANSLUCENCY_TYPES::ADDITIVE:
                    {
                        glEnable(GL_BLEND);
                        glBlendEquation(GL_FUNC_ADD);
                        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                        break;
                    }

                    case GFX::TRANSLUCENCY_TYPES::SUBTRACTIVE:
                    {
                        glEnable(GL_BLEND);
                        glBlendEquation(GL_FUNC_SUBTRACT);
                        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                        break;
                    }

                    case GFX::TRANSLUCENCY_TYPES::MULTIPLICATIVE:
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
        glDisable(GL_BLEND);
    }
}

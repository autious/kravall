#ifndef GFX_PARTICLES_PARTICLE_PAINTER_HPP
#define GFX_PARTICLES_PARTICLE_PAINTER_HPP

#include <Renderer/BasePainter.hpp>
#include <GL/glew.h>

namespace GFX
{
    class FBOTexture;
    class ShaderManager;
    class UniformBufferManager;
    class RenderJobManager;
    class MaterialManager;
    class TextureManager;
    class ParticleManager;
}

namespace GFX
{
    class ParticlePainter : public GFX::BasePainter
    {
    public:
        ParticlePainter(GFX::ShaderManager* shaderManager, GFX::UniformBufferManager* uniformBufferManager
                ,GFX::RenderJobManager* renderJobManager, GFX::MaterialManager* materialManager
                ,GFX::TextureManager* textureManager, GFX::ParticleManager* particleManager);
        ~ParticlePainter();
        void Initialize(GLuint FBO, GLuint VAO) override;
        void Render(unsigned int& renderIndex, GFX::FBOTexture* depthBuffer, GFX::FBOTexture* normalDepth, GFX::FBOTexture* specular, GFX::FBOTexture* glowMatId, GLuint toneMappedTexture, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

    private:
        GFX::RenderJobManager* m_renderJobManager;
        GFX::MaterialManager* m_materialManager;
        GFX::TextureManager* m_textureManager;
        GFX::ParticleManager* m_particleManager;

        GLfloat m_outlineThickness;
        //Uniforms
        GLuint m_texture0Uniform;
        GLuint m_texture1Uniform;
        GLuint m_texture2Uniform;
        GLuint m_texture3Uniform;
    };
}


#endif

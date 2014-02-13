#ifndef GFX_PARTICLES_PARTICLE_DATA_HPP
#define GFX_PARTICLES_PARTICLE_DATA_HPP

#include <GL/glew.h>

namespace GFX
{
    struct ParticleData
    {
        GLuint VBO;
        GLuint VAO;
        unsigned int particleCount;
    };
}

#endif

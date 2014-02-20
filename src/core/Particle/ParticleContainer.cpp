#include "ParticleContainer.hpp"

#include <gfx/GFXInterface.hpp>

namespace Core
{
    ParticleContainer::~ParticleContainer()
    {
        GFX::Content::DeleteParticleBuffer(m_particleBufferId);       
    }
}

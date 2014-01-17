#include "NameSystem.hpp"

#include <SystemDef.hpp>

namespace Core
{
    NameSystem::NameSystem()
        : BaseSystem( EntityHandler::GenerateAspect<NameComponent>(), 0ULL )
    {
    }

    void NameSystem::Update( float delta )
    {
    }
}

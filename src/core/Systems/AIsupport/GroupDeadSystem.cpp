#include "GroupDeadSystem.hpp"
#include <SystemDef.hpp>
#include <World.hpp>

namespace Core
{
    GroupDeadSystem::GroupDeadSystem()
    : BaseSystem( EntityHandler::GenerateAspect<DeadUnitComponent>(), 0ULL )
    {

    }

    void GroupDeadSystem::Update( float delta ) 
    {
    }

    int GroupDeadSystem::GetDeadUnitCount( )
    {
        return m_entities.size();
    }
}

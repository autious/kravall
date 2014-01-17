#include "AreaLuaUpdateSystem.hpp"

#include <SystemDef.hpp>
#include <World.hpp>
#include <gfx/GFXInterface.hpp>
#include <utility/Colors.hpp>

namespace Core
{
    AreaLuaUpdateSystem::AreaLuaUpdateSystem()
        : BaseSystem( EntityHandler::GenerateAspect<AreaComponent,WorldPositionComponent>(), 0ULL )
    {
        
    }

    void AreaLuaUpdateSystem::DrawAreas()
    {
	    for (std::vector<Entity>::iterator it = m_entities.begin();
            it != m_entities.end();
            it++)
        {
            AreaComponent * ac = WGETC<AreaComponent>(*it);
            WorldPositionComponent *wpc = WGETC<WorldPositionComponent>(*it);

            GFX::Debug::DrawLine( AreaComponent::GetVec3( *ac, 0 )
                                  + WorldPositionComponent::GetVec3( *wpc ),
                                  AreaComponent::GetVec3( *ac, 1 )
                                  + WorldPositionComponent::GetVec3( *wpc ),
                                  Colors::Green, true);
            GFX::Debug::DrawLine( AreaComponent::GetVec3( *ac, 1 )
                                  + WorldPositionComponent::GetVec3( *wpc ),
                                  AreaComponent::GetVec3( *ac, 2 )
                                  + WorldPositionComponent::GetVec3( *wpc ),
                                  Colors::Green, true );
            GFX::Debug::DrawLine( AreaComponent::GetVec3( *ac, 2 )
                                  + WorldPositionComponent::GetVec3( *wpc ),
                                  AreaComponent::GetVec3( *ac, 3 )
                                  + WorldPositionComponent::GetVec3( *wpc ),
                                  Colors::Green, true);
            GFX::Debug::DrawLine( AreaComponent::GetVec3( *ac, 3 )
                                  + WorldPositionComponent::GetVec3( *wpc ),
                                  AreaComponent::GetVec3( *ac, 0 )
                                  + WorldPositionComponent::GetVec3( *wpc ),
                                  Colors::Green, true);
        }
    }

    void AreaLuaUpdateSystem::Update( float delta )
    {
        if( CONF.GetBool( "debugRenderAreas", true ) )
        {
            DrawAreas();
        }
        
    }
}

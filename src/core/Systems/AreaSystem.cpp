#include "AreaSystem.hpp"

#include <SystemDef.hpp>
#include <World.hpp>
#include <gfx/GFXInterface.hpp>
#include <utility/Colors.hpp>
#include <GameUtility/AreaUtility.hpp>

namespace Core
{
    AreaSystem::AreaSystem()
        : BaseSystem( {{EntityHandler::GenerateAspect<AreaComponent,WorldPositionComponent>(), 0ULL}, {EntityHandler::GenerateAspect<UnitTypeComponent,WorldPositionComponent>(), 0ULL}} )
    {
        
    }

    void AreaSystem::DrawAreas()
    {
	    for (std::vector<Entity>::iterator it = m_bags[0].m_entities.begin();
            it != m_bags[0].m_entities.end();
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

    void AreaSystem::Update( float delta )
    {
        if( CONF.GetBool( "debugRenderAreas", true ) )
        {
            DrawAreas();
        }

	    for (std::vector<Entity>::iterator areaIt = m_bags[0].m_entities.begin();
            areaIt != m_bags[0].m_entities.end();
            areaIt++)
        {
            for (std::vector<Entity>::iterator riotIt = m_bags[1].m_entities.begin();
                riotIt != m_bags[1].m_entities.end();
                riotIt++)
            {
                 
            }
        }
    }
}

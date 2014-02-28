#include "AreaSystem.hpp"

#include <SystemDef.hpp>
#include <World.hpp>
#include <gfx/GFXInterface.hpp>
#include <utility/Colors.hpp>
#include <GameUtility/AreaUtility.hpp>

namespace Core
{
    AreaSystem::AreaSystem()
        : BaseSystem( {{EntityHandler::GenerateAspect<AreaComponent,WorldPositionComponent>(), 0ULL}, {EntityHandler::GenerateAspect<AttributeComponent,WorldPositionComponent,UnitTypeComponent>(), 0ULL}} )
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

            
            glm::vec4 color =  Colors::Green;

            if( rioterContainList[*it].size() > 0 )
            {
                
                color = Colors::Red;
                GFX::Debug::DrawSphere( WorldPositionComponent::GetVec3( *wpc ),
                                        rioterContainList[*it].size()/10.0f,
                                        color, true );
            }

            GFX::Debug::DrawLine( AreaComponent::GetVec3( *ac, 0 )
                                  + WorldPositionComponent::GetVec3( *wpc ),
                                  AreaComponent::GetVec3( *ac, 1 )
                                  + WorldPositionComponent::GetVec3( *wpc ),
                                  color, true);
            GFX::Debug::DrawLine( AreaComponent::GetVec3( *ac, 1 )
                                  + WorldPositionComponent::GetVec3( *wpc ),
                                  AreaComponent::GetVec3( *ac, 2 )
                                  + WorldPositionComponent::GetVec3( *wpc ),
                                  color, true );
            GFX::Debug::DrawLine( AreaComponent::GetVec3( *ac, 2 )
                                  + WorldPositionComponent::GetVec3( *wpc ),
                                  AreaComponent::GetVec3( *ac, 3 )
                                  + WorldPositionComponent::GetVec3( *wpc ),
                                  color, true);
            GFX::Debug::DrawLine( AreaComponent::GetVec3( *ac, 3 )
                                  + WorldPositionComponent::GetVec3( *wpc ),
                                  AreaComponent::GetVec3( *ac, 0 )
                                  + WorldPositionComponent::GetVec3( *wpc ),
                                  color, true);
        
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
            AreaComponent * areaAC = WGETC<AreaComponent>(*areaIt);
            WorldPositionComponent * areaWPC = WGETC<WorldPositionComponent>(*areaIt);

            rioterContainList[*areaIt].clear();

            for (std::vector<Entity>::iterator riotIt = m_bags[1].m_entities.begin();
                riotIt != m_bags[1].m_entities.end();
                riotIt++)
            {
                WorldPositionComponent *rioterWPC = WGETC<WorldPositionComponent>(*riotIt);
                UnitTypeComponent *type = WGETC<UnitTypeComponent>(*riotIt);

                if( type->type == Core::UnitType::Rioter )
                {
                    if( Core::AreaUtility::CheckPointInsideNode(
                                WorldPositionComponent::GetVec3(*rioterWPC), 
                                areaAC->vertices, 
                                WorldPositionComponent::GetVec3(*areaWPC)))
                    {
                        rioterContainList[*areaIt].push_back( *riotIt );
                    }
                }
            }
        }
    }

    void AreaSystem::ChangedEntity( Entity id, Aspect old_asp, Aspect new_asp )
    {
        BaseSystem::ChangedEntity( id, old_asp, new_asp );
        const Aspect areaAspect = EntityHandler::GenerateAspect<AreaComponent>();

        //Check if we should clear the map list from this entity.
        if( (old_asp & areaAspect) == areaAspect && (new_asp & areaAspect) != areaAspect )            {
            LOG_DEBUG << "Removing areas entity list as it is no longer active" << std::endl;

            rioterContainList.erase( id );
        }
    }
        

    std::vector<Entity> AreaSystem::GetAreaRioters( Entity id )
    {
        try
        {
            std::vector<Entity> area = rioterContainList.at(id);
            return area;
        }
        catch( std::out_of_range ofr )
        {
            return std::vector<Entity>();
        }
    }

    int AreaSystem::GetAreaRioterCount( Entity id )
    {
        try
        {
            std::vector<Entity> area = rioterContainList.at(id);
            return area.size();
        }
        catch( std::out_of_range ofr )
        {
            return 0;
        }
    }

    int AreaSystem::GetAreaRioterCount( Entity id, int groupId )
    {
        AreaComponent * areaAC = WGETC<AreaComponent>(id);
        WorldPositionComponent * areaWPC = WGETC<WorldPositionComponent>(id);

        int count = 0;

        if( areaAC != nullptr && areaWPC != nullptr )
        {   
            for (std::vector<Entity>::iterator riotIt = m_bags[1].m_entities.begin();
                riotIt != m_bags[1].m_entities.end();
                riotIt++)
            {
                WorldPositionComponent *rioterWPC = WGETC<WorldPositionComponent>(*riotIt);
                UnitTypeComponent *type = WGETC<UnitTypeComponent>(*riotIt);
                AttributeComponent *attr = WGETC<AttributeComponent>(*riotIt);

                if( type->type == Core::UnitType::Rioter &&
                     attr->rioter.groupID == groupId )
                {
                    if( Core::AreaUtility::CheckPointInsideNode(
                                WorldPositionComponent::GetVec3(*rioterWPC), 
                                areaAC->vertices, 
                                WorldPositionComponent::GetVec3(*areaWPC)))
                    {
                        count++;
                    }
                }
            }
        }
        else
        {
            LOG_ERROR << "Trying to get the rioter count in an entity not and area!" << std::endl;
        }

        return count;
    }

    bool AreaSystem::IsAreaOnPoint( Entity id, const glm::vec3& position ) 
    {
        AreaComponent * areaAC = WGETC<AreaComponent>(id);
        WorldPositionComponent * areaWPC = WGETC<WorldPositionComponent>(id);

        if( areaAC && areaWPC ) 
        {
            return Core::AreaUtility::CheckPointInsideNode(
                    position, 
                    areaAC->vertices, 
                    WorldPositionComponent::GetVec3(*areaWPC));
        }
        else
        {
            LOG_ERROR << "Attempting to use non area entity as area in IsPointInArea check, returning false as answer." << std::endl;
            return false;
        }
    }
}

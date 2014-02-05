#include "SquadSystem.hpp"

#include <World.hpp>
#include <Systems/GroupDataSystem.hpp>
#include <glm/glm.hpp>
#include <GameUtility/GameData.hpp>

#include <gfx/GFXInterface.hpp>

#define FORMATION_COLUMN_SPACING_MINIMUM 1.5f
#define FORMATION_ROW_SPACING 1.5f

namespace Core
{
    SquadSystem::SquadSystem() : Core::BaseSystem(Core::EntityHandler::GenerateAspect<Core::SquadComponent>(), 0ULL)
    {
        
    }

    void SquadSystem::SetSquadGoal(int squadID, glm::vec3 target)
    {
        for(std::vector<Entity>::iterator squad_it = m_entities.begin(); squad_it != m_entities.end(); ++squad_it)        
        {
            Core::SquadComponent* sqdc = WGETC<Core::SquadComponent>(*squad_it);
            if(sqdc->squadID == squadID)
            {
                sqdc->squadGoal[0] = target.x;
                sqdc->squadGoal[2] = target.z;
            }
        }
    }

    void SquadSystem::SetSquadFormation(int squadID, Core::SquadFormation formation, const glm::vec3& startPos, const glm::vec3& endPos)
    {
        if(glm::dot(startPos-endPos, startPos-endPos) < 0.25f)
            return;

        for(std::vector<Entity>::iterator squad_it = m_entities.begin(); squad_it != m_entities.end(); ++squad_it)        
        {
            Core::SquadComponent* sqdc = WGETC<Core::SquadComponent>(*squad_it);
            if(sqdc->squadID == squadID)
            {
                std::vector<Core::Entity> squad = Core::world.m_systemHandler.GetSystem<Core::GroupDataSystem>()->GetMembersInGroup(sqdc->squadID);

                if(sqdc->squadLeader == INVALID_ENTITY)
                {
                    if(squad.size() > 0)
                    {
                        sqdc->squadLeader = squad[0];
                    }
                    else
                    {
                        //The group is empty
                        return;
                    }
                }

                glm::vec3 squadForward = glm::normalize(glm::cross(startPos-endPos, glm::vec3(0.0f, 1.0f, 0.0f)));
                sqdc->squadTargetForward[0] = squadForward.x;
                sqdc->squadTargetForward[1] = squadForward.z;

                int membersInGroup = Core::world.m_systemHandler.GetSystem<Core::GroupDataSystem>()->GetMemberCount(sqdc->squadID);
                 
                switch( formation)
                {
                    case Core::SquadFormation::LINE_FORMATION:
                    {
                        float xOffset = 0.0f;
                        float zOffset = 0.0f;
                        float distance =  glm::distance(startPos, endPos);
                        float xSpacing = distance / static_cast<float>(membersInGroup);
                        float zSpacing = static_cast<float>(world.m_config.GetDouble("squadFormationRowSpacing", FORMATION_ROW_SPACING));

                        xSpacing = (xSpacing > static_cast<float>(world.m_config.GetDouble("squadFormationColumnSpacingMinimum", FORMATION_COLUMN_SPACING_MINIMUM))) ? xSpacing : static_cast<float>(world.m_config.GetDouble("squadFormationSpacingMinimum", FORMATION_COLUMN_SPACING_MINIMUM));

                        for(std::vector<Core::Entity>::iterator it = squad.begin(); it != squad.end(); ++it)
                        {
                            Core::FormationComponent* frmc = WGETC<Core::FormationComponent>(*it);

                            frmc->relativePosition[0] = xOffset;
                            frmc->relativePosition[1] = zOffset;

                            xOffset += xSpacing;
                            if(xOffset >= distance)
                            {
                                xOffset = 0.0f;
                                zOffset += zSpacing;
                            }                   
                        }
                        break;
                    }
                    case Core::SquadFormation::CIRCLE_FORMATION:
                    {
                        LOG_ERROR << "Circle formation is not yet implemented" << std::endl; 
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
            }
        }
    }

    void SquadSystem::PreviewSquadFormation(int squadID, Core::SquadFormation formation, const glm::vec3& startPos, const glm::vec3& endPos)
    {
        if(glm::dot(startPos-endPos, startPos-endPos) < 0.25f)
            return;

        for(std::vector<Entity>::iterator squad_it = m_entities.begin(); squad_it != m_entities.end(); ++squad_it)        
        {
            Core::SquadComponent* sqdc = WGETC<Core::SquadComponent>(*squad_it);
            if(sqdc->squadID == squadID)
            {
                std::vector<Core::Entity> squad = Core::world.m_systemHandler.GetSystem<Core::GroupDataSystem>()->GetMembersInGroup(sqdc->squadID);

                if(sqdc->squadLeader == INVALID_ENTITY)
                {
                    if(squad.size() > 0)
                    {
                        sqdc->squadLeader = squad[0];
                    }
                    else
                    {
                        //The group is empty
                        return;
                    }
                }

                glm::vec3 squadForward = glm::normalize(glm::cross(startPos-endPos, glm::vec3(0.0f, 1.0f, 0.0f)));

                GFX::Debug::DrawLine((startPos+endPos)/2.0f, (startPos+endPos)/2.0f + squadForward, GFXColor(1.0f, 1.0f, 0.0f, 1.0f), false);
                float rotation = glm::atan(-squadForward.z, squadForward.x) - (3.14f / 2.0f);

                float cosVal = glm::cos(rotation);
                float sinVal = glm::sin(rotation);
                glm::mat2 rotMat = glm::mat2(cosVal, -sinVal, sinVal, cosVal);

                int membersInGroup = Core::world.m_systemHandler.GetSystem<Core::GroupDataSystem>()->GetMemberCount(sqdc->squadID);
                 
                Core::NavigationMesh* navMesh = Core::GetNavigationMesh();

                switch( formation)
                {
                    case Core::SquadFormation::LINE_FORMATION:
                    {
                        float xOffset = 0.0f;
                        float zOffset = 0.0f;
                        float distance = glm::distance(startPos, endPos);
                        float xSpacing = distance / static_cast<float>(membersInGroup);
                        float zSpacing = static_cast<float>(world.m_config.GetDouble("squadFormationRowSpacing", FORMATION_ROW_SPACING));

                        xSpacing = (xSpacing > static_cast<float>(world.m_config.GetDouble("squadFormationColumnSpacingMinimum", FORMATION_COLUMN_SPACING_MINIMUM))) ? xSpacing : static_cast<float>(world.m_config.GetDouble("squadFormationSpacingMinimum", FORMATION_COLUMN_SPACING_MINIMUM));

                        for(int i=0; i < membersInGroup; ++i)
                        {
                            glm::vec2 relPos2D = glm::vec2(xOffset, zOffset);
                            relPos2D = rotMat * relPos2D;
                            int goalNode;
                            glm::vec3 relPos = glm::vec3(relPos2D.x, 0.0f, relPos2D.y);
                            glm::vec3 finalPosition = startPos + relPos;
                            //TODO: Replace Debug draw with decals
				    		navMesh->GetClosestPointInsideMesh(finalPosition, startPos, goalNode, 0.2f);
                            GFX::Debug::DrawSphere(finalPosition, 0.5f, GFXColor(1.0f, 0.0f, 0.0f, 1.0f), false);

                            xOffset += xSpacing;
                            if(xOffset >= distance)
                            {
                                xOffset = 0.0f;
                                zOffset += zSpacing;
                            }                   
                        }
                        break;
                    }
                    case Core::SquadFormation::CIRCLE_FORMATION:
                    {
                        LOG_ERROR << "Circle formation is not yet implemented" << std::endl; 
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
            }
        }
    }
    void SquadSystem::Update(float delta)
    {
        for(std::vector<Entity>::iterator squad_it = m_entities.begin(); squad_it != m_entities.end(); ++squad_it)        
        {
            Core::SquadComponent* sqdc = WGETC<Core::SquadComponent>(*squad_it);
            std::vector<Core::Entity> squad = Core::world.m_systemHandler.GetSystem<Core::GroupDataSystem>()->GetMembersInGroup(sqdc->squadID);

            if(sqdc->squadLeader == INVALID_ENTITY)
            {
                if(squad.size() > 0)
                {
                    sqdc->squadLeader = squad[0];
                }
                else
                {
                    //The group is empty
                    continue;
                }
            }

            Core::WorldPositionComponent* leader_wpc = WGETC<Core::WorldPositionComponent>(sqdc->squadLeader);                

            glm::vec3 leaderPosition = Core::WorldPositionComponent::GetVec3(*leader_wpc);

            float rotation = 0.0f;            
            if(sqdc->squadMoveInFormation)
            {
                //TODO: If squad leader is not in combat, set squadForward to his facing
                //sqdc->squadForward[0] = leaderForward[0];
                //sqdc->squadForward[1] = leaderForward[1];
                rotation = glm::atan(-sqdc->squadForward[1], sqdc->squadForward[0]) - (3.14f / 2.0f);
            }
            else
            {
                rotation = glm::atan(-sqdc->squadTargetForward[1], sqdc->squadTargetForward[0])  - (3.14f / 2.0f);

            }

            float cosVal = glm::cos(rotation);
            float sinVal = glm::sin(rotation);
            glm::mat2 rotMat = glm::mat2(cosVal, -sinVal, sinVal, cosVal);

            //Calculate individual goals depending on formation settings.
            Core::NavigationMesh* navMesh = Core::GetNavigationMesh();
            for(std::vector<Entity>::iterator entity_it = squad.begin(); entity_it != squad.end(); ++entity_it)
            {
                Core::FormationComponent* frmc = WGETC<Core::FormationComponent>(*entity_it);
                Core::MovementComponent* mc = WGETC<Core::MovementComponent>(*entity_it);
                Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(*entity_it);


                glm::vec2 relPos2D = glm::vec2(frmc->relativePosition[0], frmc->relativePosition[1]);
                relPos2D = rotMat * relPos2D;

                glm::vec3 relativePosition = glm::vec3(relPos2D.x, 0.0f, relPos2D.y);
                if(sqdc->squadMoveInFormation)
                {
                    glm::vec3 formationPosition = leaderPosition + relativePosition;
                    int goalNode;

                    //If the formation position is outside the navigation mesh the formation position should be on
                    //    the point where the line from the leader to the formation position collides with the navmesh.
                    bool hasMovedFormationPos = navMesh->GetClosestPointInsideMesh(formationPosition, leaderPosition, goalNode, 0.2f);
                    if(!hasMovedFormationPos)
                    {
                        //If an entity is given a new position it can no longer be a straggler
                        frmc->isStraggler = glm::distance(formationPosition, WorldPositionComponent::GetVec3(*wpc)) > static_cast<float>(Core::world.m_config.GetDouble("formationStraggleDistance", 0.5));  
                        sqdc->waitForStraggler = !sqdc->waitForStraggler ? frmc->isStraggler : true;                                                    
                    }
                    mc->goal[0] = formationPosition.x;
                    mc->goal[1] = formationPosition.z;
                    mc->NavMeshGoalNodeIndex = goalNode;
                }
                else
                {
                    
                    glm::vec3 squadTargetPosition = glm::vec3(sqdc->squadGoal[0], sqdc->squadGoal[1], sqdc->squadGoal[2]);  
                    glm::vec3 formationPosition = squadTargetPosition + relativePosition;
                    int goalNode;

                    GFX::Debug::DrawLine(squadTargetPosition, squadTargetPosition + glm::vec3(sqdc->squadTargetForward[0], 0.0f, sqdc->squadTargetForward[1])*3.0f, GFXColor(0.0f, 1.0f, 0.0f, 1.0f),false);
                    //If the formation position is outside the navigation mesh the formation position should be on
                    //    the point where the line from the target position to the formation position collides with the navmesh.
                    
                    navMesh->GetClosestPointInsideMesh(formationPosition, squadTargetPosition, goalNode, 0.2f);

                    mc->goal[0] = formationPosition.x;
                    mc->goal[1] = formationPosition.z;
                    mc->NavMeshGoalNodeIndex = goalNode;
                }
            }

            sqdc->squadHealth = 0;
            sqdc->squadStamina = 0;
            sqdc->squadMorale = 0.0f;
            //Set attributes and calculate squad data.
            for(std::vector<Entity>::iterator entity_it = squad.begin(); entity_it != squad.end(); ++entity_it)
            {
                Core::FormationComponent* frmc = WGETC<Core::FormationComponent>(*entity_it);
                Core::MovementComponent* mc = WGETC<Core::MovementComponent>(*entity_it);
                Core::AttributeComponent* atrbc = WGETC<Core::AttributeComponent>(*entity_it);

                atrbc->police.stance = sqdc->squadStance;
                sqdc->squadHealth += atrbc->health;
                sqdc->squadStamina += atrbc->stamina;
                sqdc->squadMorale += atrbc->morale;

                if(!frmc->isStraggler)
                {
                    mc->desiredSpeed = 0.0f;                
                }
                else
                {
                    //TODO: If the unit had another desiredSpeed than walking it needs to be taken into account here                        
                    mc->desiredSpeed = Core::GameData::GetMovementDataWithState( mc->state ).speedToDesire; 
                }
            }
        }
    }
}

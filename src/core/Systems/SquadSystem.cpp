#include "SquadSystem.hpp"

#include <World.hpp>
#include <Systems/GroupDataSystem.hpp>
#include <glm/glm.hpp>
#include <GameUtility/GameData.hpp>

namespace Core
{
    SquadSystem::SquadSystem() : Core::BaseSystem(Core::EntityHandler::GenerateAspect<Core::SquadComponent>(), 0ULL)
    {
        
    }

    void SquadSystem::Update(float delta)
    {
        for(std::vector<Entity>::iterator squad_it = m_entities.begin(); squad_it != m_entities.end(); ++squad_it)        
        {
            Core::SquadComponent* sqdc = WGETC<Core::SquadComponent>(*squad_it);
            Core::WorldPositionComponent* leader_wpc = WGETC<Core::WorldPositionComponent>(sqdc->squadLeader);                
            Core::FlowfieldComponent* leader_ffc = WGETC<Core::FlowfieldComponent>(sqdc->squadLeader);

            sqdc->waitForStraggler = true;

            std::vector<Core::Entity> squad = Core::world.m_systemHandler.GetSystem<Core::GroupDataSystem>()->GetMembersInGroup(sqdc->squadID);

            glm::vec3 leaderPosition = Core::WorldPositionComponent::GetVec3(*leader_wpc);

            float rotation = 0.0f;            
            if(sqdc->squadMoveInFormation)
            {
                //TODO: If squad leader is not in combat, set squadForward to his facing
                //sqdc->squadForward[0] = leaderForward[0];
                //sqdc->squadForward[1] = leaderForward[1];
                rotation = glm::atan(sqdc->squadForward[1], sqdc->squadForward[0]);
            }
            else
            {
                rotation = glm::atan(sqdc->squadTargetForward[1], sqdc->squadTargetForward[0]);
            }

            float cosVal = glm::cos(rotation);
            float sinVal = glm::sin(rotation);
            glm::mat2 rotMat = glm::mat2(cosVal, -sinVal, sinVal, cosVal);

            //Calculate individual goals depending on formation settings.
            for(std::vector<Entity>::iterator entity_it = squad.begin(); entity_it != squad.end(); ++entity_it)
            {
                Core::FormationComponent* frmc = WGETC<Core::FormationComponent>(*entity_it);
                Core::MovementComponent* mc = WGETC<Core::MovementComponent>(*entity_it);
                Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(*entity_it);


                glm::vec2 relPos2D = glm::vec2(frmc->relativePosition[0], frmc->relativePosition[1]);
                relPos2D = rotMat * relPos2D;

                glm::vec3 relativePosition = glm::vec3(relPos2D.x, 0.0f, relPos2D.y);
                //If relative position is larger than zero
                if((relativePosition.x * relativePosition.x + relativePosition.y * relativePosition.y) > 0.0f)
                {
                    Core::NavigationMesh* navMesh = Core::GetNavigationMesh();
                    if(sqdc->squadMoveInFormation)
                    {
                        glm::vec3 formationPosition = leaderPosition + relativePosition;

                        //If the formation position is outside the navigation mesh the formation position should be on
                        //    the point where the line from the leader to the formation position collides with the navmesh.
                        bool hasMovedFormationPos = navMesh->GetClosestPointInsideMesh(formationPosition, leaderPosition, leader_ffc->node);
                        if(!hasMovedFormationPos)
                        {
                            //If a entity is given a new position he can no longer be a straggler
                            frmc->isStraggler = glm::distance(formationPosition, WorldPositionComponent::GetVec3(*wpc)) > static_cast<float>(Core::world.m_config.GetDouble("formationStraggleDistance", 0.5));  
                            sqdc->waitForStraggler = !sqdc->waitForStraggler ? frmc->isStraggler : true;                                                    
                        }
                        mc->goal[0] = formationPosition.x;
                        mc->goal[1] = formationPosition.z;
                    }
                    else
                    {
                        glm::vec3 squadTargetPosition = glm::vec3(sqdc->squadGoal[0], sqdc->squadGoal[1], sqdc->squadGoal[2]);  
                        glm::vec3 formationPosition = squadTargetPosition + relativePosition;

                        //If the formation position is outside the navigation mesh the formation position should be on
                        //    the point where the line from the target position to the formation position collides with the navmesh.
                        navMesh->GetClosestPointInsideMesh(formationPosition, squadTargetPosition, sqdc->squadGoalNode);
                        mc->goal[0] = formationPosition.x;
                        mc->goal[1] = formationPosition.z;
                    }
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
                    mc->desiredSpeed = Core::GameData::GetWalkingSpeed().speedToDesire;
                }
            }
        }
    }
}

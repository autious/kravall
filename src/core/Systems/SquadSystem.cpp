#include "SquadSystem.hpp"

#include <World.hpp>
#include <Systems/GroupDataSystem.hpp>
#include <glm/glm.hpp>
#include <GameUtility/GameData.hpp>

#include <gfx/GFXInterface.hpp>

#define FORMATION_COLUMN_SPACING_MINIMUM 2.0f
#define FORMATION_ROW_SPACING 2.0f

namespace Core
{
    SquadSystem::SquadSystem() : Core::BaseSystem(Core::EntityHandler::GenerateAspect<Core::SquadComponent>(), 0ULL)
    {
        
    }

    void SquadSystem::SetSquadGoal(int* squadIDs, int nSquads, glm::vec3 target)
    {
        for(int i=0; i<nSquads; ++i)
        {
            for(std::vector<Entity>::iterator squad_it = m_entities.begin(); squad_it != m_entities.end(); ++squad_it)        
            {
                Core::SquadComponent* sqdc = WGETC<Core::SquadComponent>(*squad_it);
                if(sqdc->squadID == squadIDs[i])
                {
                    sqdc->squadGoal[0] = target.x;
                    sqdc->squadGoal[2] = target.z;
                }
            }
        }
    }

    void SquadSystem::SetSquadFormation(int* squadIDs, int nSquads, Core::SquadFormation formation, const glm::vec3& startPos, const glm::vec3& endPos)
    {
        if(glm::dot(startPos-endPos, startPos-endPos) < 0.25f)
            return;

        //Retrieve units
        std::vector<Core::Entity> units;

        for(int i=0; i<nSquads; ++i)
        {
            std::vector<Core::Entity> squad = Core::world.m_systemHandler.GetSystem<Core::GroupDataSystem>()->GetMembersInGroup(squadIDs[i]);
            units.insert(units.end(), squad.begin(), squad.end());
        }

        //Set Squads forward direction and formation
        glm::vec3 squadForward = glm::normalize(glm::cross(startPos-endPos, glm::vec3(0.0f, 1.0f, 0.0f)));

        for(std::vector<Entity>::iterator squad_it = m_entities.begin(); squad_it != m_entities.end(); ++squad_it)        
        {
            Core::SquadComponent* sqdc = WGETC<Core::SquadComponent>(*squad_it);         
            for(int i=0; i<nSquads; ++i)
            {
                if(sqdc->squadID == squadIDs[i])
                {
                    sqdc->squadTargetForward[0] = squadForward.x;
                    sqdc->squadTargetForward[1] = squadForward.z;
                    sqdc->squadFormation = formation;
                }
            }
        }

        int membersInGroup = units.size();        
        bool isHalfCircle = false;

        Core::NavigationMesh* navMesh = Core::GetNavigationMesh();

        switch( formation)
        {
            //Line formation
            case Core::SquadFormation::LINE_FORMATION:
            {
                int goalNode;
                glm::vec3 center = (startPos + endPos) / 2.0f;
                float leftDistance = 0.0f;
                float rightDistance = 0.0f;
                
                glm::vec3 start = startPos;
                glm::vec3 end = endPos;

                if(!navMesh->CheckPointInsideNavigationMesh(center))
                    return;

                navMesh->GetClosestPointInsideMesh(start, center, goalNode, world.m_config.GetDouble("squadFormationRowSpacing", FORMATION_ROW_SPACING) / 2.0f);                
                navMesh->GetClosestPointInsideMesh(end, center, goalNode, world.m_config.GetDouble("squadFormationRowSpacing", FORMATION_ROW_SPACING) / 2.0f);

                leftDistance = glm::distance(start, center);
                rightDistance = glm::distance(end, center);

                float xSpacing = (leftDistance + rightDistance) / static_cast<float>(membersInGroup);
                float zSpacing = static_cast<float>(world.m_config.GetDouble("squadFormationRowSpacing", FORMATION_ROW_SPACING));

                xSpacing = (xSpacing > static_cast<float>(world.m_config.GetDouble("squadFormationColumnSpacingMinimum", FORMATION_COLUMN_SPACING_MINIMUM))) ? xSpacing : static_cast<float>(world.m_config.GetDouble("squadFormationSpacingMinimum", FORMATION_COLUMN_SPACING_MINIMUM));

                float xOffset = -leftDistance;
                float zOffset = 0.0f;                

                for(std::vector<Core::Entity>::iterator it = units.begin(); it != units.end(); ++it)
                {
                    Core::FormationComponent* frmc = WGETC<Core::FormationComponent>(*it);

                    frmc->relativePosition[0] = xOffset;
                    frmc->relativePosition[1] = zOffset;

                    xOffset += xSpacing;
                    if(xOffset > rightDistance)
                    {
                        xOffset = -leftDistance;
                        zOffset += zSpacing;
                    }                   
                }
                break;
            }
            //Circle and Half Circle formations
            case Core::SquadFormation::HALF_CIRCLE_FORMATION:            
            {
                isHalfCircle = true;
                case Core::SquadFormation::CIRCLE_FORMATION:
                {           
                    float circumferenceOffset = 0.0f;
                    float radius = glm::distance(startPos, endPos) / 2.0f;
                    float circumference = 3.14f * radius * 2.0f;
                    glm::vec3 centerPosition = isHalfCircle ? (startPos + endPos) / 2.0f: startPos;
                    float circleSpacing = circumference / static_cast<float>(membersInGroup);
                    float radiusSpacing = static_cast<float>(world.m_config.GetDouble("squadFormationRowSpacing", FORMATION_ROW_SPACING));

                    if(!navMesh->CheckPointInsideNavigationMesh(centerPosition))
                        return;

                    circleSpacing = (circleSpacing > static_cast<float>(world.m_config.GetDouble("squadFormationColumnSpacingMinimum", FORMATION_COLUMN_SPACING_MINIMUM))) ? circleSpacing : static_cast<float>(world.m_config.GetDouble("squadFormationSpacingMinimum", FORMATION_COLUMN_SPACING_MINIMUM));
                    for(std::vector<Core::Entity>::iterator it = units.begin(); it != units.end(); ++it)
                    {
                        Core::FormationComponent* frmc = WGETC<Core::FormationComponent>(*it);

                        float radianOffset = -(circumferenceOffset / circumference) * 3.14f * (isHalfCircle ? 1.0f : 2.0f);
                        float cosVal = glm::cos(radianOffset);
                        float sinVal = glm::sin(radianOffset);
                        glm::mat2 rotMat2D = glm::mat2(cosVal, -sinVal, sinVal, cosVal);

                        glm::vec2 relPos2D = glm::vec2(radius, 0.0f);
                        relPos2D = rotMat2D * relPos2D;

                        frmc->relativePosition[0] = relPos2D.x;// + (isHalfCircle ? radius : 0.0f);
                        frmc->relativePosition[1] = relPos2D.y;

                        circumferenceOffset += circleSpacing;
                        if(circumferenceOffset >= circumference)
                        {
                            circumferenceOffset = 0.0f;
                            radius += radiusSpacing;
                            circumference = 3.14f * radius * (isHalfCircle ? 1.0f : 2.0f);
                        }
                    }                    
                }
                break;
            }
            default:
            
                break;
            
        }
    }

    void SquadSystem::PreviewSquadFormation(int* squadIDs, int nSquads, Core::SquadFormation formation, const glm::vec3& startPos, const glm::vec3& endPos)
    {
        if(glm::dot(startPos-endPos, startPos-endPos) < 0.25f)
            return;

        //Retrieve units
        std::vector<Core::Entity> units;

        for(int i=0; i<nSquads; ++i)
        {
            std::vector<Core::Entity> squad= Core::world.m_systemHandler.GetSystem<Core::GroupDataSystem>()->GetMembersInGroup(squadIDs[i]);
            units.insert(units.end(), squad.begin(), squad.end());
        }

        //Calculate rotation matrix for position projection
        glm::vec3 squadForward = glm::normalize(glm::cross(startPos-endPos, glm::vec3(0.0f, 1.0f, 0.0f)));
        GFX::Debug::DrawLine((startPos+endPos)/2.0f, (startPos+endPos)/2.0f + squadForward, GFXColor(1.0f, 1.0f, 0.0f, 1.0f), false);
        float rotation = glm::atan(-squadForward.z, squadForward.x) - (3.14f / 2.0f);

        int membersInGroup = units.size();                 
        bool isHalfCircle = false;

        Core::NavigationMesh* navMesh = Core::GetNavigationMesh();

        switch( formation)
        {
            case Core::SquadFormation::LINE_FORMATION:
            {
                int goalNode;
                float cosVal = glm::cos(rotation);
                float sinVal = glm::sin(rotation);
                glm::mat2 rotMat2D = glm::mat2(cosVal, -sinVal, sinVal, cosVal);

                glm::vec3 center = (startPos + endPos) / 2.0f;
                glm::vec3 start = startPos;
                glm::vec3 end = endPos;

                float rightDistance = 0.0f;
                float leftDistance = 0.0f;

                if(!navMesh->CheckPointInsideNavigationMesh(center))
                    return;

                navMesh->GetClosestPointInsideMesh(start, center, goalNode, 0.0f);                
                navMesh->GetClosestPointInsideMesh(end, center, goalNode, 0.0f);

                leftDistance = glm::distance(start, center);
                rightDistance = glm::distance(end, center);

                float xSpacing = (leftDistance + rightDistance) / static_cast<float>(membersInGroup);
                float zSpacing = static_cast<float>(world.m_config.GetDouble("squadFormationRowSpacing", FORMATION_ROW_SPACING));

                xSpacing = (xSpacing > static_cast<float>(world.m_config.GetDouble("squadFormationColumnSpacingMinimum", FORMATION_COLUMN_SPACING_MINIMUM))) ? xSpacing : static_cast<float>(world.m_config.GetDouble("squadFormationSpacingMinimum", FORMATION_COLUMN_SPACING_MINIMUM));

                float xOffset = -leftDistance;
                float zOffset = 0.0f;

                for(int i=0; i < membersInGroup; ++i)
                {
                    glm::vec2 relPos2D = glm::vec2(xOffset, zOffset);
                    relPos2D = rotMat2D * relPos2D;
                    glm::vec3 relPos = glm::vec3(relPos2D.x, 0.0f, relPos2D.y);
                    glm::vec3 finalPosition = center + relPos;
                    //TODO: Replace Debug draw with decals
                    navMesh->GetClosestPointInsideMesh(finalPosition, center, goalNode, 0.2f);
                    GFX::Debug::DrawSphere(finalPosition, 0.5f, GFXColor(1.0f, 0.0f, 0.0f, 1.0f), false);

                    xOffset += xSpacing;
                    if(xOffset > rightDistance)
                    {
                        xOffset = -leftDistance;
                        zOffset += zSpacing;
                    }                   
                }
                break;
            }
            case Core::SquadFormation::HALF_CIRCLE_FORMATION:
            {
                isHalfCircle = true;
                case Core::SquadFormation::CIRCLE_FORMATION:            
                {
                    float circumferenceOffset = 0.0f;
                    float radius = glm::distance(startPos, endPos) / 2.0f;
                    float circumference = 3.14f * radius * 2.0f;
                    glm::vec3 centerPosition =  isHalfCircle ? (startPos + endPos) / 2.0f: startPos; 
                    float circleSpacing = circumference / static_cast<float>(membersInGroup);
                    float radiusSpacing = static_cast<float>(world.m_config.GetDouble("squadFormationRowSpacing", FORMATION_ROW_SPACING));

                    if(!navMesh->CheckPointInsideNavigationMesh(centerPosition))
                        return;
                    circleSpacing = (circleSpacing > static_cast<float>(world.m_config.GetDouble("squadFormationColumnSpacingMinimum", FORMATION_COLUMN_SPACING_MINIMUM))) ? circleSpacing : static_cast<float>(world.m_config.GetDouble("squadFormationSpacingMinimum", FORMATION_COLUMN_SPACING_MINIMUM));
                    for(int i=0; i < membersInGroup; ++i)
                    {
                        float radianOffset = -(circumferenceOffset / circumference) * 3.14f * (isHalfCircle ? 1.0f : 2.0f);
                        float cosVal = glm::cos(rotation + radianOffset);
                        float sinVal = glm::sin(rotation + radianOffset);
                        glm::mat2 rotMat2D = glm::mat2(cosVal, -sinVal, sinVal, cosVal);

                        glm::vec2 relPos2D = glm::vec2(radius, 0.0f);
                        relPos2D = rotMat2D * relPos2D;
                        int goalNode;
                        glm::vec3 relPos = glm::vec3(relPos2D.x, 0.0f, relPos2D.y);
                        glm::vec3 finalPosition = centerPosition + relPos;
                        //TODO: Replace Debug draw with decals
                        navMesh->GetClosestPointInsideMesh(finalPosition, centerPosition, goalNode, 0.2f);
                        GFX::Debug::DrawSphere(finalPosition, 0.5f, GFXColor(1.0f, 0.0f, 0.0f, 1.0f), false);

                        circumferenceOffset += circleSpacing;
                        if(circumferenceOffset >= circumference)
                        {
                            circumferenceOffset = 0.0f;
                            radius += radiusSpacing;
                            circumference = 3.14f * radius * (isHalfCircle ? 1.0f : 2.0f);
                        }
                    }
                }
                break;
            }
            default:
            {
                break;
            }
        }           
        
    }

    void SquadSystem::SetSquadStance(int* squadIDs, int nSquads, Core::PoliceStance stance)
    {
        for(int i=0; i<nSquads; ++i)
        {
            for(std::vector<Entity>::iterator squad_it = m_entities.begin(); squad_it != m_entities.end(); ++squad_it)        
            {
                Core::SquadComponent* sqdc = WGETC<Core::SquadComponent>(*squad_it);
                if(sqdc->squadID == squadIDs[i])
                {
                    sqdc->squadStance = stance;
                }
            }
        }
    }

    Core::Entity SquadSystem::GetSquadEntity(int squadID)
    {
        for(std::vector<Entity>::iterator squad_it = m_entities.begin(); squad_it != m_entities.end(); ++squad_it)        
        {
            Core::SquadComponent* sqdc = WGETC<Core::SquadComponent>(*squad_it);
            if(sqdc->squadID == squadID)
            {
                return *squad_it;
            }
        }
        return INVALID_ENTITY;
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
                    sqdc->squadLeader = squad[static_cast<int>(squad.size() / 2)];
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

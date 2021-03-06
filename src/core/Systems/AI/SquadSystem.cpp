#include "SquadSystem.hpp"

#include <World.hpp>
#include <Systems/AIsupport/GroupDataSystem.hpp>
#include <glm/glm.hpp>
#include <GameUtility/GameData.hpp>

#include <gfx/GFXInterface.hpp>
#include <gfx/InstanceData.hpp>

#define FORMATION_COLUMN_SPACING_MINIMUM 2.0f
#define FORMATION_ROW_SPACING 2.0f
#define ALLOW_MOVE_IN_FORMATION false



//#define DRAW_GOAL_FROM_SQUAD_SYSTEM
#ifdef DRAW_GOAL_FROM_SQUAD_SYSTEM
#define DEBUG_DRAW_GOAL( willDo ) willDo
#else
#define DEBUG_DRAW_GOAL( wontDo ) ;
#endif

namespace Core
{
    SquadSystem::SquadSystem() : Core::BaseSystem(Core::EntityHandler::GenerateAspect<Core::SquadComponent>(), 0ULL)
    {
		m_foundAssets = false;
    }

    void SquadSystem::SetSquadGoal(int* squadIDs, int nSquads, glm::vec3 target)
    {
		Core::NavigationMesh* instance = Core::GetNavigationMesh();
		if( !instance )
			return;
		
		if( !instance->CheckPointInsideNavigationMesh( target ) )
			return;

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

                navMesh->GetClosestPointInsideMesh(start, center, goalNode, (float)world.m_config.GetDouble("squadFormationRowSpacing", FORMATION_ROW_SPACING) / 2.0f);                
                navMesh->GetClosestPointInsideMesh(end, center, goalNode, (float)world.m_config.GetDouble("squadFormationRowSpacing", FORMATION_ROW_SPACING) / 2.0f);

                leftDistance = glm::distance(start, center);
                rightDistance = glm::distance(end, center);

                float xSpacing = (leftDistance + rightDistance) / static_cast<float>(membersInGroup);
                float zSpacing = static_cast<float>(world.m_config.GetDouble("squadFormationRowSpacing", FORMATION_ROW_SPACING));

                xSpacing = (xSpacing > static_cast<float>(world.m_config.GetDouble("squadFormationColumnSpacingMinimum", FORMATION_COLUMN_SPACING_MINIMUM))) ? xSpacing : static_cast<float>(world.m_config.GetDouble("squadFormationSpacingMinimum", FORMATION_COLUMN_SPACING_MINIMUM));

                float rowWidth = 0.0f;

                while(rowWidth < (leftDistance + rightDistance))
                {
                    rowWidth += xSpacing;
                }
                rowWidth -= xSpacing;

                leftDistance = rightDistance = (rowWidth / 2.0f);

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
                    float circumference = 3.14f * radius * (isHalfCircle ? 1.0f : 2.0f);
                    glm::vec3 centerPosition = isHalfCircle ? (startPos + endPos) / 2.0f: startPos;
                    float circleSpacing = circumference / static_cast<float>(membersInGroup);
                    float radiusSpacing = static_cast<float>(world.m_config.GetDouble("squadFormationRowSpacing", FORMATION_ROW_SPACING));

                    if(!navMesh->CheckPointInsideNavigationMesh(centerPosition))
                        return;

                    circleSpacing = (circleSpacing > static_cast<float>(world.m_config.GetDouble("squadFormationColumnSpacingMinimum", FORMATION_COLUMN_SPACING_MINIMUM))) ? circleSpacing : static_cast<float>(world.m_config.GetDouble("squadFormationSpacingMinimum", FORMATION_COLUMN_SPACING_MINIMUM));
                    for(std::vector<Core::Entity>::iterator it = units.begin(); it != units.end(); ++it)
                    {
                        Core::FormationComponent* frmc = WGETC<Core::FormationComponent>(*it);

                        float radianOffset = (circumferenceOffset / circumference) * 3.14f * (isHalfCircle ? 1.0f : 2.0f);
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

                float rowWidth = 0.0f;

                while(rowWidth < (leftDistance + rightDistance))
                {
                    rowWidth += xSpacing;
                }
                rowWidth -= xSpacing;

                leftDistance = rightDistance = (rowWidth / 2.0f);

                float xOffset = -leftDistance;
                float zOffset = 0.0f;

                for(int i=0; i < membersInGroup; ++i)
                {
                    glm::vec2 relPos2D = glm::vec2(xOffset, zOffset);
                    relPos2D = rotMat2D * relPos2D;
                    glm::vec3 relPos = glm::vec3(relPos2D.x, 0.0f, relPos2D.y);
                    glm::vec3 finalPosition = center + relPos;
                    navMesh->GetClosestPointInsideMesh(finalPosition, center, goalNode, 0.2f);
					PushDecal(finalPosition);

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
                    float circumference = 3.14f * radius * (isHalfCircle ? 1.0f : 2.0f);
                    glm::vec3 centerPosition =  isHalfCircle ? (startPos + endPos) / 2.0f: startPos; 
                    float circleSpacing = circumference / static_cast<float>(membersInGroup);
                    float radiusSpacing = static_cast<float>(world.m_config.GetDouble("squadFormationRowSpacing", FORMATION_ROW_SPACING));

                    if(!navMesh->CheckPointInsideNavigationMesh(centerPosition))
                        return;
                    circleSpacing = (circleSpacing > static_cast<float>(world.m_config.GetDouble("squadFormationColumnSpacingMinimum", FORMATION_COLUMN_SPACING_MINIMUM))) ? circleSpacing : static_cast<float>(world.m_config.GetDouble("squadFormationSpacingMinimum", FORMATION_COLUMN_SPACING_MINIMUM));
                    for(int i=0; i < membersInGroup; ++i)
                    {
                        float radianOffset = -(circumferenceOffset / circumference) * 3.14f * (isHalfCircle ? 1.0f : 2.0f) + 3.14f;
                        float cosVal = glm::cos(rotation + radianOffset);
                        float sinVal = glm::sin(rotation + radianOffset);
                        glm::mat2 rotMat2D = glm::mat2(cosVal, -sinVal, sinVal, cosVal);

                        glm::vec2 relPos2D = glm::vec2(radius, 0.0f);
                        relPos2D = rotMat2D * relPos2D;
                        int goalNode;
                        glm::vec3 relPos = glm::vec3(relPos2D.x, 0.0f, relPos2D.y);
                        glm::vec3 finalPosition = centerPosition + relPos;
                        navMesh->GetClosestPointInsideMesh(finalPosition, centerPosition, goalNode, 0.2f);
						PushDecal(finalPosition);

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
					if( sqdc->squadStance != Core::PoliceStance::Aggressive && stance == Core::PoliceStance::Aggressive )
						sqdc->prevSquadStance = sqdc->squadStance;
                    sqdc->squadStance = stance;
                }
            }
        }
    }

	PoliceStance SquadSystem::RevertSquadStanceFromAgressive( int* squadIDs, int nSquads )
    {
		PoliceStance stance = PoliceStance::Passive;
		PoliceStance backup = PoliceStance::Passive;
		bool set = false;
		bool backupSet = false;

        for(int i=0; i<nSquads; ++i)
        {
            for(std::vector<Entity>::iterator squad_it = m_entities.begin(); squad_it != m_entities.end(); ++squad_it)        
            {
                Core::SquadComponent* sqdc = WGETC<Core::SquadComponent>(*squad_it);
                if(sqdc->squadID == squadIDs[i])
                {
					if( sqdc->squadStance == Core::PoliceStance::Aggressive )
					{
						sqdc->squadStance = sqdc->prevSquadStance;
						if( !set )
						{
							set = true;
							stance = sqdc->squadStance;
						}
					}
					else
					{
						if( !backupSet )
						{
							backupSet = true;
							backup = sqdc->squadStance;
						}
					}
                }
            }
        }
		return set ? stance : backupSet ? backup : stance;
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

	void SquadSystem::SetSquadTargetGroup( int squadID, int targetSquadID )
	{
		Core::SquadComponent* sq = WGETC<Core::SquadComponent>( GetSquadEntity( squadID ) );
		sq->targetGroup = targetSquadID;
	}

    void SquadSystem::EnableOutline(int* squadIDs, int nSquads,const glm::vec4& Color)
    {        
        for(int i=0; i<nSquads; ++i)
        {
            std::vector<Core::Entity> squad = Core::world.m_systemHandler.GetSystem<Core::GroupDataSystem>()->GetMembersInGroup(squadIDs[i]);

            for(std::vector<Entity>::iterator it = squad.begin(); it != squad.end(); ++it)        
            {
				Core::TargetingComponent* tc = WGETC<Core::TargetingComponent>(*it);
				if( tc )
				{
					Core::GraphicsComponent* gfxc = WGETC<Core::GraphicsComponent>(*it);
					gfxc->outlineColor[0] = Color.x;
					gfxc->outlineColor[1] = Color.y;
					gfxc->outlineColor[2] = Color.z;
					gfxc->outlineColor[3] = Color.w;
					GFX::SetBitmaskValue(gfxc->bitmask, GFX::BITMASK::LAYER, GFX::LAYER_TYPES::OUTLINE_LAYER);
				}
            }
        }
    }

	void SquadSystem::EnableOutlineMoods(int* squadIDs, int nSquads)
	{
		for (int i = 0; i<nSquads; ++i)
		{
			std::vector<Core::Entity> squad = Core::world.m_systemHandler.GetSystem<Core::GroupDataSystem>()->GetMembersInGroup(squadIDs[i]);

			for (std::vector<Entity>::iterator it = squad.begin(); it != squad.end(); ++it)
			{
				Core::TargetingComponent* tc = WGETC<Core::TargetingComponent>(*it);
				if (tc)
				{
					Core::GraphicsComponent* gfxc = WGETC<Core::GraphicsComponent>(*it);
					Core::AttributeComponent* atc = WGETC<Core::AttributeComponent>(*it);

					switch (atc->rioter.stance)
					{
					case RioterStance::Normal:
						gfxc->outlineColor[0] = 0.0f / 255.0f;
						gfxc->outlineColor[1] = 225.0f / 255.0f;
						gfxc->outlineColor[2] = 255.0f / 255.0f;
						gfxc->outlineColor[3] = 2.0f;
						break;
					case RioterStance::Agitated:
						gfxc->outlineColor[0] = 1.0f;
						gfxc->outlineColor[1] = 0.5f;
						gfxc->outlineColor[2] = 0.0f;
						gfxc->outlineColor[3] = 2.0f;
						break;
					case RioterStance::Attacking:
						gfxc->outlineColor[0] = 1.0f;
						gfxc->outlineColor[1] = 0.0f;
						gfxc->outlineColor[2] = 0.0f;
						gfxc->outlineColor[3] = 2.0f;
						break;
					case RioterStance::Retreating:
						gfxc->outlineColor[0] = 1.0f;
						gfxc->outlineColor[1] = 1.0f;
						gfxc->outlineColor[2] = 1.0f;
						gfxc->outlineColor[3] = 2.0f;
						break;
					case RioterStance::Civilian:
						break;
					}

				
					GFX::SetBitmaskValue(gfxc->bitmask, GFX::BITMASK::LAYER, GFX::LAYER_TYPES::OUTLINE_LAYER);
				}
			}
		}
	}

    void SquadSystem::DisableOutline(int* squadIDs, int nSquads)
    {
        for(int i=0; i<nSquads; ++i)
        {
            std::vector<Core::Entity> squad = Core::world.m_systemHandler.GetSystem<Core::GroupDataSystem>()->GetMembersInGroup(squadIDs[i]);

            for(std::vector<Entity>::iterator it = squad.begin(); it != squad.end(); ++it)        
            {
				Core::GraphicsComponent* gfxc = WGETC<Core::GraphicsComponent>(*it);
				GFX::SetBitmaskValue(gfxc->bitmask, GFX::BITMASK::LAYER, GFX::LAYER_TYPES::MESH_LAYER);
            }
        }
    }

	void SquadSystem::StopGroup(int* squadIDs, int nSquads)
    {
		if( nSquads <= 0 )
			return;

		RevertSquadStanceFromAgressive( squadIDs, nSquads );
		for(int i=0; i<nSquads; ++i)
        {
            for(std::vector<Entity>::iterator squad_it = m_entities.begin(); squad_it != m_entities.end(); ++squad_it)        
            {
                Core::SquadComponent* sqdc = WGETC<Core::SquadComponent>(*squad_it);
                if(sqdc->squadID == squadIDs[i])
                {
					sqdc->squadGoal[0] = std::numeric_limits<float>::max();
                }
            }
        }
    }

    std::vector<Core::SquadAbility> SquadSystem::GetPossibleAbilities( int squadId )
    {
        std::vector<Core::SquadAbility> abilities;

        abilities.push_back( Core::SquadAbility::SPRINT );
        abilities.push_back( Core::SquadAbility::FLEE );
        abilities.push_back( Core::SquadAbility::ROUT );
        abilities.push_back( Core::SquadAbility::ATTACK );
        abilities.push_back( Core::SquadAbility::ARREST_INDIVIDUAL );
        abilities.push_back( Core::SquadAbility::ARREST_GROUP );
        abilities.push_back( Core::SquadAbility::TEAR_GAS );

        return abilities;
    }

    std::vector<int> SquadSystem::GetAllSquads()
    {
        std::vector<int> ids;
        for(std::vector<Entity>::iterator squad_it = m_entities.begin();
             squad_it != m_entities.end();
              ++squad_it)
        {
            Core::SquadComponent* sqdc = WGETC<Core::SquadComponent>(*squad_it);
            ids.push_back( sqdc->squadID );
        }
        return ids;
    }

    std::vector<Entity> SquadSystem::GetAllSquadEntities()
    {
        return m_entities;
    }

	void SquadSystem::PushDecal(glm::vec3 position)
	{
		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
		glm::mat4 scaleMatrix = glm::scale(1.0f, 1.0f, 1.0f);

		GFX::InstanceData* instanceData = Core::world.m_frameHeap.NewObject<GFX::InstanceData>();
		instanceData->modelMatrix = translationMatrix * scaleMatrix;
		GFX::Draw(m_bitmask, (void*)instanceData);

		GFX::LightData* lightData = Core::world.m_frameHeap.NewObject<GFX::LightData>();

		lightData->position = position;
		lightData->spot_angle = 0.0f;
		lightData->spot_penumbra = 0.0f;
		lightData->color[0] = 0.0f;
		lightData->color[1] = 1.0f;
		lightData->color[2] = 0.0f;
		lightData->spec_color[0] = 0.0f;
		lightData->spec_color[1] = 1.0f;
		lightData->spec_color[2] = 0.0f;
		lightData->radius_length = 5.0f;
		lightData->intensity = 0.1f;
		lightData->orientation = glm::vec3(0.0f);

		GFX::Draw(m_lightBitmask, (void*)lightData);

	}

	void SquadSystem::Update(float delta)
	{
		if (!m_foundAssets)
		{
			m_lightBitmask = 0;
			m_bitmask = 0;

			Core::world.m_contentManager.Load<Core::GnomeLoader>("assets/model/dev/cube.bgnome", [this](Core::BaseAssetLoader* baseLoader, Core::AssetHandle handle)
			{
				Core::ModelData data = *static_cast<Core::ModelData*>(handle);
				this->m_mesh = data.meshID;
			});
		
			Core::world.m_contentManager.Load<Core::MaterialLoader>("assets/texture/dev/positionPreview.material", [this](Core::BaseAssetLoader* baseLoader, Core::AssetHandle handle)
			{
				Core::MaterialData data = *static_cast<Core::MaterialData*>(handle);
				this->m_material = data.materialId;
			});
		
			GFX::SetBitmaskValue(m_bitmask, GFX::BITMASK::MESH_ID, m_mesh);
			GFX::SetBitmaskValue(m_bitmask, GFX::BITMASK::MATERIAL_ID, m_material);
			GFX::SetBitmaskValue(m_bitmask, GFX::BITMASK::LAYER, GFX::LAYER_TYPES::MESH_LAYER);
			GFX::SetBitmaskValue(m_bitmask, GFX::BITMASK::TYPE,	GFX::OBJECT_TYPES::DECAL_GEOMETRY);

		
			GFX::SetBitmaskValue(m_lightBitmask, GFX::BITMASK::LIGHT_TYPE, GFX::LIGHT_TYPES::POINT);
			GFX::SetBitmaskValue(m_lightBitmask, GFX::BITMASK::TYPE, GFX::OBJECT_TYPES::LIGHT);
		
			m_foundAssets = true;
		}
		else
		{
			for (std::vector<Entity>::iterator squad_it = m_entities.begin(); squad_it != m_entities.end(); ++squad_it)
			{
				Core::SquadComponent* sqdc = WGETC<Core::SquadComponent>(*squad_it);
				std::vector<Core::Entity> squad = Core::world.m_systemHandler.GetSystem<Core::GroupDataSystem>()->GetMembersInGroup(sqdc->squadID);

				Core::AttributeComponent* attribc = WGETC<Core::AttributeComponent>(sqdc->squadLeader);
				if (sqdc->squadLeader == INVALID_ENTITY || !attribc)
				{
					if (squad.size() > 0)
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
				sqdc->waitForStraggler = false;

				glm::vec3 leaderPosition = Core::WorldPositionComponent::GetVec3(*leader_wpc);

				float rotation = 0.0f;
				if (sqdc->squadMoveInFormation && ALLOW_MOVE_IN_FORMATION )
				{
					//TODO: If squad leader is not in combat, set squadForward to his facing
					//sqdc->squadForward[0] = leaderForward[0];
					//sqdc->squadForward[1] = leaderForward[1];
					rotation = glm::atan(-sqdc->squadForward[1], sqdc->squadForward[0]) - (3.14f / 2.0f);
				}
				else
				{
					rotation = glm::atan(-sqdc->squadTargetForward[1], sqdc->squadTargetForward[0]) - (3.14f / 2.0f);

				}

				float cosVal = glm::cos(rotation);
				float sinVal = glm::sin(rotation);
				glm::mat2 rotMat = glm::mat2(cosVal, -sinVal, sinVal, cosVal);

				//Calculate individual goals depending on formation settings.
				Core::NavigationMesh* navMesh = Core::GetNavigationMesh();
				for (std::vector<Entity>::iterator entity_it = squad.begin(); entity_it != squad.end(); ++entity_it)
				{
					Core::FormationComponent* frmc = WGETC<Core::FormationComponent>(*entity_it);
					Core::MovementComponent* mc = WGETC<Core::MovementComponent>(*entity_it);
					Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(*entity_it);

					// if the squad don't have a goal, set goal to current position...
					if( sqdc->squadGoal[0] == std::numeric_limits<float>::max() )
					{
						Core::FlowfieldComponent* ffc = WGETC<Core::FlowfieldComponent>( *entity_it );
						if( ffc )
							mc->SetGoal( wpc->position, ffc->node, Core::MovementGoalPriority::FormationGoalPriority );
						continue;
					}

					glm::vec2 relPos2D = glm::vec2(frmc->relativePosition[0], frmc->relativePosition[1]);
					relPos2D = rotMat * relPos2D;

					glm::vec3 relativePosition = glm::vec3(relPos2D.x, 0.0f, relPos2D.y);
					if (sqdc->squadMoveInFormation && ALLOW_MOVE_IN_FORMATION )
					{
						glm::vec3 formationPosition = leaderPosition + relativePosition;
						int goalNode;

						//If the formation position is outside the navigation mesh the formation position should be on
						//    the point where the line from the leader to the formation position collides with the navmesh.
						bool hasMovedFormationPos = navMesh->GetClosestPointInsideMesh(formationPosition, leaderPosition, goalNode, 0.2f);
						if (!hasMovedFormationPos)
						{
							//If an entity is given a new position it can no longer be a straggler
							frmc->isStraggler = glm::distance(formationPosition, WorldPositionComponent::GetVec3(*wpc)) > static_cast<float>(Core::world.m_config.GetDouble("formationStraggleDistance", 0.5));
							sqdc->waitForStraggler = !sqdc->waitForStraggler ? frmc->isStraggler : true;
						}

						formationPosition.y = 0.0f;
						mc->SetGoal(formationPosition, goalNode, Core::MovementGoalPriority::FormationGoalPriority);
					}
					else
					{

						glm::vec3 squadTargetPosition = glm::vec3(sqdc->squadGoal[0], sqdc->squadGoal[1], sqdc->squadGoal[2]);
						glm::vec3 formationPosition = squadTargetPosition + relativePosition;
						int goalNode;

						GFX::Debug::DrawLine(squadTargetPosition, squadTargetPosition + glm::vec3(sqdc->squadTargetForward[0], 0.0f, sqdc->squadTargetForward[1])*3.0f, GFXColor(0.0f, 1.0f, 0.0f, 1.0f), false);
						//If the formation position is outside the navigation mesh the formation position should be on
						//    the point where the line from the target position to the formation position collides with the navmesh.

						navMesh->GetClosestPointInsideMesh(formationPosition, squadTargetPosition, goalNode, 0.2f);

						formationPosition.y = 0.0f;
						mc->SetGoal(formationPosition, goalNode, Core::MovementGoalPriority::FormationGoalPriority);

						DEBUG_DRAW_GOAL( GFX::Debug::DrawLine( glm::vec3( wpc->position[0], wpc->position[1], wpc->position[2] ), formationPosition, GFXColor( 1, 1, 0, 1 ), false ) );
					}
				}

				sqdc->squadHealth = 0;
				sqdc->squadStamina = 0;
				sqdc->squadMorale = 0.0f;
				//Set attributes and calculate squad data.
				for (std::vector<Entity>::iterator entity_it = squad.begin(); entity_it != squad.end(); ++entity_it)
				{
					Core::FormationComponent* frmc = WGETC<Core::FormationComponent>(*entity_it);
					Core::MovementComponent* mc = WGETC<Core::MovementComponent>(*entity_it);
					Core::AttributeComponent* atrbc = WGETC<Core::AttributeComponent>(*entity_it);

					atrbc->police.stance = sqdc->squadStance;
					sqdc->squadHealth += atrbc->health;
					sqdc->squadStamina += atrbc->stamina;
					sqdc->squadMorale += atrbc->morale;

					if (sqdc->waitForStraggler)
					{
						if (!frmc->isStraggler)
						{
							mc->SetMovementState( Core::MovementState::Movement_idle, Core::MovementStatePriority::MovementState_SquadMoveInFormationPriority );
						}
					}
				}
			}
		}
	}
}

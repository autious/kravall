#include "RemovalSystem.hpp"
#include <World.hpp>

namespace Core
{

	RemovalSystem::RemovalSystem() :BaseSystem(EntityHandler::GenerateAspect<UnitTypeComponent>(), 0ULL)
	{}

	void RemovalSystem::Update(float delta)
	{
		Entity* rioterEntities = Core::world.m_frameHeap.NewPODArray<Entity>(m_entities.size());
		Entity* policeEntities = Core::world.m_frameHeap.NewPODArray<Entity>(m_entities.size());
		int rioterCount = 0;
		int policeCount = 0;

		for (std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++)
		{
			
			UnitTypeComponent* utc = WGETC<UnitTypeComponent>(*it);

			if (utc->type == UnitType::Rioter)
			{
				AttributeComponent* atc = WGETC<AttributeComponent>(*it);
				//Leave graphics, animation, WPC, scale and rotation so we can keep rendering the object
				if (atc->health <= 0)
				{
					//world.m_entityHandler.RemoveComponents<UnitTypeComponent, MovementComponent, AttributeComponent, TargetingComponent, BoundingVolumeComponent, FlowfieldComponent>(*it);
					rioterEntities[rioterCount++] = *it;
					
				}
				else if (atc->stamina <= 0)//Some kind of formula shiet here
				{
					//world.m_entityHandler.RemoveComponents<UnitTypeComponent, MovementComponent, AttributeComponent, TargetingComponent, BoundingVolumeComponent, FlowfieldComponent>(*it);
					rioterEntities[rioterCount++] = *it;
				}
			}
			else if (utc->type == UnitType::Police)
			{
				AttributeComponent* atc = WGETC<AttributeComponent>(*it);
				if (atc->health <= 0)
				{
					//world.m_entityHandler.RemoveComponents<UnitTypeComponent, MovementComponent, AttributeComponent, TargetingComponent, BoundingVolumeComponent, FlowfieldComponent>(*it);
					policeEntities[policeCount++] = *it;
				}
			}
		}

		for (int i = 0; i < rioterCount; i++)
			world.m_entityHandler.RemoveComponents<UnitTypeComponent, MovementComponent, AttributeComponent, TargetingComponent, BoundingVolumeComponent, FlowfieldComponent>(rioterEntities[i]);

		for (int i = 0; i < policeCount; i++)
			world.m_entityHandler.RemoveComponents<UnitTypeComponent, MovementComponent, AttributeComponent, TargetingComponent, BoundingVolumeComponent, FlowfieldComponent>(policeEntities[i]);
	}
}

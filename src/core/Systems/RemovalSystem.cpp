#include "RemovalSystem.hpp"
#include <World.hpp>

namespace Core
{

	RemovalSystem::RemovalSystem() :BaseSystem(EntityHandler::GenerateAspect<UnitTypeComponent>(), 0ULL)
	{}

	void RemovalSystem::Update(float delta)
	{
		for (std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++)
		{
			UnitTypeComponent* utc = WGETC<UnitTypeComponent>(*it);

			if (utc->type == UnitType::Rioter)
			{
				AttributeComponent* atc = WGETC<AttributeComponent>(*it);
				//Leave graphics, animation, WPC, scale and rotation so we can keep rendering the object
				if (atc->health <= 0)
				{
					world.m_entityHandler.RemoveComponents<UnitTypeComponent, MovementComponent, AttributeComponent, TargetingComponent, BoundingVolumeComponent, FlowfieldComponent>(*it);
				}
				else if (atc->stamina <= 0)//Some kind of formula shiet here
				{
					world.m_entityHandler.RemoveComponents<UnitTypeComponent, MovementComponent, AttributeComponent, TargetingComponent, BoundingVolumeComponent, FlowfieldComponent>(*it);
				}
			}
			else if (utc->type == UnitType::Police)
			{
				AttributeComponent* atc = WGETC<AttributeComponent>(*it);
				if (atc->health <= 0)
				{
					world.m_entityHandler.RemoveComponents<UnitTypeComponent, MovementComponent, AttributeComponent, TargetingComponent, BoundingVolumeComponent, FlowfieldComponent>(*it);
				}
			}
		}
	}
}

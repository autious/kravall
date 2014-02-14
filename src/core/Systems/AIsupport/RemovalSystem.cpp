#include "RemovalSystem.hpp"
#include <World.hpp>

namespace Core
{

	RemovalSystem::RemovalSystem() :BaseSystem(EntityHandler::GenerateAspect<UnitTypeComponent, AttributeComponent, TargetingComponent>(), 0ULL)
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
					rioterEntities[rioterCount++] = *it;
					
				}
				else if (atc->stamina <= 0)//Some kind of formula shiet here
				{
					rioterEntities[rioterCount++] = *it;
				}
			}
			else if (utc->type == UnitType::Police)
			{
				AttributeComponent* atc = WGETC<AttributeComponent>(*it);
				if (atc->health <= 0)
				{
					policeEntities[policeCount++] = *it;
				}
			}
		}

		for (int i = 0; i < rioterCount; i++)
		{
			// remove this entity form its attackers...
			Core::TargetingComponent* tc = WGETC<Core::TargetingComponent>(rioterEntities[i]);
			
			Core::TargetingComponent* attackerTc = WGETC<Core::TargetingComponent>(tc->target);
			if( attackerTc )
				Core::TargetingComponent::StopAttacking( rioterEntities[i], *attackerTc );

			for( int p = 0; p < MAX_ATTACKERS; p++ )
			{
				Core::TargetingComponent* attackerTcomp = WGETC<Core::TargetingComponent>(tc->attackers[p]);
				if( attackerTcomp )
				{
					if( attackerTcomp->target == rioterEntities[i] )
						attackerTcomp->target = INVALID_ENTITY;
				}
			}
			world.m_entityHandler.RemoveComponents<UnitTypeComponent, MovementComponent, AttributeComponent, TargetingComponent, BoundingVolumeComponent, FlowfieldComponent>(rioterEntities[i]);
		}


		for(int i = 0; i < policeCount; i++)
		{
			// remove this entity form its attackers...
			Core::TargetingComponent* tc = WGETC<Core::TargetingComponent>(policeEntities[i]);
			
			Core::TargetingComponent* attackerTc = WGETC<Core::TargetingComponent>(tc->target);
			if( attackerTc )
				Core::TargetingComponent::StopAttacking( policeEntities[i], *attackerTc );

			for( int p = 0; p < MAX_ATTACKERS; p++ )
			{
				Core::TargetingComponent* attackerTcomp = WGETC<Core::TargetingComponent>(tc->attackers[p]);
				if( attackerTcomp )
					attackerTcomp->target = INVALID_ENTITY;
			}

			world.m_entityHandler.RemoveComponents<UnitTypeComponent, MovementComponent, AttributeComponent, TargetingComponent, BoundingVolumeComponent, FlowfieldComponent>(policeEntities[i]);
		}
	}
}

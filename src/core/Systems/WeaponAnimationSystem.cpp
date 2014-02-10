#include "WeaponAnimationSystem.hpp"

#include <Input/InputManager.hpp>
#include <GameUtility/GameData.hpp>
#include "World.hpp"
#include <SystemDef.hpp>

#define MAX_TARGETING_TIME 2.0f
#define MAX_SQR_DISTANCE 441.0f

Core::WeaponAnimationSystem::WeaponAnimationSystem() : 
	BaseSystem(EntityHandler::GenerateAspect<WorldPositionComponent, TargetingComponent, UnitTypeComponent, AttributeComponent>(), 0ULL)
{
}

void Core::WeaponAnimationSystem::Update(float delta)
{
	for (std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++)
	{
		Core::TargetingComponent* tc = WGETC<Core::TargetingComponent>(*it);
		
		float distSqr = dx * dx + dy * dy + dz * dz;
		
		if (distSqr < weapon.range * weapon.range )
		{
			if (TargetingComponent::Attack(police, *tcTarget))
			{


			




		}


		Core::UnitTypeComponent* utc = WGETC<Core::UnitTypeComponent>(*it);
	






		
	}



}

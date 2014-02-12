#include "AttackAnimationSystem.hpp"

#include <Input/InputManager.hpp>
#include <GameUtility/GameData.hpp>
#include "World.hpp"
#include <SystemDef.hpp>
#include <Animation/AnimationManager.hpp>

Core::AttackAnimationSystem::AttackAnimationSystem() : 
	BaseSystem(EntityHandler::GenerateAspect<WorldPositionComponent, TargetingComponent, AnimationComponent, UnitTypeComponent, 
	AttributeComponent, MovementComponent, FlowfieldComponent>(), 0ULL)
{
}



void Core::AttackAnimationSystem::Update(float delta)
{
	for (std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++)
	{
		Core::TargetingComponent* tc = WGETC<Core::TargetingComponent>(*it);	

		if( tc->target != INVALID_ENTITY )
		{
			Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(*it);
			Core::WorldPositionComponent* targetWpc = WGETC<Core::WorldPositionComponent>( tc->target );
			Core::MovementComponent* mvmc = WGETC<Core::MovementComponent>(*it);
			const Core::WeaponData& weapon = Core::GameData::GetWeaponDataFromWeapon( tc->weapon );

			// if the unit can attack its target, start the attack animation.
			float distSqr = glm::dot( wpc->GetVec3( *wpc ) - targetWpc->GetVec3( *targetWpc ),
							 wpc->GetVec3( *wpc ) - targetWpc->GetVec3( *targetWpc ));
			if (distSqr < weapon.range * weapon.range )
				tc->isAttacking = true;
		

			// if attacking, handle animation and dmg
			if( tc->isAttacking )
			{
				mvmc->SetDesiredSpeed( 0.0f, Core::DesiredSpeedSetPriority::CombatAnimationDesiredSpeedPriority );

				Core::AnimationComponent* animac = WGETC<Core::AnimationComponent>(*it);
				if( animac->animationID != weapon.animationID )
					Core::AnimationManager::PlayAnimation( *it, weapon.animationID );

				if( animac->currentTime > weapon.animationDmgDealingtime && !tc->hasAttacked )
				{
					tc->hasAttacked = true;

					// deal dmg if target is still in range
					if (distSqr < weapon.range * weapon.range )
					{
						Core::AttributeComponent* attribc = WGETC<Core::AttributeComponent>( tc->target );
						attribc->health -= weapon.weaponDamage;
						attribc->morale -= weapon.moraleDamage;
					}
				}

				// attack is done, reset the chace
				if( !animac->playing )
				{
					Core::AnimationManager::LoopAnimation( *it, 0 );
					tc->isAttacking = false;
					tc->hasAttacked = false;
				}
			}

			// always set this, even when speed = 0, so that the unit is turned correctly
			Core::FlowfieldComponent* enemyFfc = WGETC<Core::FlowfieldComponent>(*it);
			mvmc->SetGoal( targetWpc->position, enemyFfc->node, Core::MovementGoalPriority::AttackGoalPriority );
		}
		else
		{
			// attack is done, reset the chace
			Core::AnimationComponent* animac = WGETC<Core::AnimationComponent>(*it);
			const Core::WeaponData& weapon = Core::GameData::GetWeaponDataFromWeapon( tc->weapon );
			if( animac->animationID == weapon.animationID )
			{
				Core::AnimationManager::LoopAnimation( *it, 0 );
				tc->isAttacking = false;
				tc->hasAttacked = false;

				Core::MovementComponent* mvmc = WGETC<Core::MovementComponent>(*it);
				mvmc->SetDesiredSpeed( Core::GameData::GetMovementDataWithState( mvmc->state ).speedToDesire, 
						Core::DesiredSpeedSetPriority::CombatAnimationDesiredSpeedPriority );
			}
		}		
	}

}

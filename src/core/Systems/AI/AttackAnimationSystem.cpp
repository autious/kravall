#include "AttackAnimationSystem.hpp"

#include <Input/InputManager.hpp>
#include <GameUtility/GameData.hpp>
#include "World.hpp"
#include <SystemDef.hpp>
#include <Animation/AnimationManager.hpp>

//#define DRAW_RANGE_AND_TARGET_LINE
#ifdef DRAW_RANGE_AND_TARGET_LINE
#define DEBUGDRAW( x ) x
#else
#define DEBUGDRAW( x ) ;
#endif

Core::AttackAnimationSystem::AttackAnimationSystem() : 
	BaseSystem(EntityHandler::GenerateAspect<WorldPositionComponent, TargetingComponent, AnimationComponent, UnitTypeComponent, 
	AttributeComponent, MovementComponent, FlowfieldComponent, GraphicsComponent>(), 0ULL)
{
}



void Core::AttackAnimationSystem::Update(float delta)
{
	for (std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++)
	{
		Core::TargetingComponent* tc = WGETC<Core::TargetingComponent>(*it);	
		if( tc->weapon < 0 )
			continue;

		Core::WorldPositionComponent* targetWpc = WGETC<Core::WorldPositionComponent>( tc->target );
		Core::TargetingComponent* targetTc = WGETC<Core::TargetingComponent>( tc->target );
		if( targetTc )
		{
			Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(*it);
			
			Core::MovementComponent* mvmc = WGETC<Core::MovementComponent>(*it);
			const Core::WeaponData& weapon = Core::GameData::GetWeaponDataFromWeapon( tc->weapon );
			

			// if the unit can attack its target, start the attack animation.
			float distSqr = glm::dot( wpc->GetVec3( *wpc ) - targetWpc->GetVec3( *targetWpc ),
							 wpc->GetVec3( *wpc ) - targetWpc->GetVec3( *targetWpc ));
			if( distSqr < weapon.range * weapon.range )
				tc->isAttacking = true;

			// draw range and target...
			DEBUGDRAW( GFX::Debug::DrawSphere( wpc->GetVec3( *wpc ), weapon.range, GFXColor( 0.5, 1, 0.5, 1 ), false ); )
			DEBUGDRAW( GFX::Debug::DrawLine( targetWpc->GetVec3( *targetWpc ), wpc->GetVec3( *wpc ), GFXColor( 0.5, 1, 0.5, 1 ), false ); )

			// if attacking, handle animation and dmg
			if( tc->isAttacking )
			{
				mvmc->SetMovementState( Core::MovementState::Movement_idle, Core::MovementStatePriority::MovementState_CombatAnimationPriority );

				Core::AnimationComponent* animac = WGETC<Core::AnimationComponent>(*it);
				Core::GraphicsComponent* grc = WGETC<Core::GraphicsComponent>(*it);

				int weaponAnimation = Core::AnimationManager::GetAnimationID( GFX::GetBitmaskValue( grc->bitmask, GFX::BITMASK::MESH_ID ), weapon.animationName );

				if( animac->animationID != weaponAnimation )
					Core::AnimationManager::PlayAnimation( *it, weaponAnimation );

				if( animac->currentTime > weapon.animationDmgDealingtime && !tc->hasAttacked )
				{
					tc->hasAttacked = true;
					
					Core::AttributeComponent* owmAttribc = WGETC<Core::AttributeComponent>( *it );
					Core::AttributeComponent* enemyAttribc = WGETC<Core::AttributeComponent>( tc->target );
					Core::UnitTypeComponent* enemyUtc = WGETC<Core::UnitTypeComponent>( tc->target );
					
					owmAttribc->stamina -= weapon.staminaCost;
					if( enemyUtc->type == Core::UnitType::Rioter )
					{
						enemyAttribc->rioter.rage += weapon.rageBuff;
						enemyAttribc->rioter.pressure += weapon.pressureBuff;
					}

					// deal dmg if target is still in range
					if (distSqr < weapon.range * weapon.range + weapon.graceDistance * weapon.graceDistance )
					{
						enemyAttribc->health -= (int)weapon.weaponDamage;
						enemyAttribc->morale -= weapon.moraleDamage;					
					}
					else
					{
						enemyAttribc->morale -= weapon.moralDamageOnMiss;
					}
				}

				// attack is done, reset the chace
				if( !animac->playing )
				{
					tc->isAttacking = false;
					tc->hasAttacked = false;
				}
			}

			// always set this, even when speed = 0, so that the unit is turned correctly
			Core::FlowfieldComponent* enemyFfc = WGETC<Core::FlowfieldComponent>( tc->target );
			mvmc->SetGoal( targetWpc->position, enemyFfc->node, Core::MovementGoalPriority::AttackGoalPriority );
		}
		else
		{
			// the unit no longer has a target...
			Core::AnimationComponent* animac = WGETC<Core::AnimationComponent>(*it);
			Core::GraphicsComponent* grc = WGETC<Core::GraphicsComponent>(*it);

			const Core::WeaponData& weapon = Core::GameData::GetWeaponDataFromWeapon( tc->weapon );
			int weaponAnimation = Core::AnimationManager::GetAnimationID( GFX::GetBitmaskValue( grc->bitmask, GFX::BITMASK::MESH_ID ), weapon.animationName );

			if( animac->animationID == weaponAnimation )
			{
				tc->isAttacking = false;
				tc->hasAttacked = false;
			}
		}		
	}
}

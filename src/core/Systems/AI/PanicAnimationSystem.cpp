#include "PanicAnimationSystem.hpp"

#include <Input/InputManager.hpp>
#include <GameUtility/GameData.hpp>
#include "World.hpp"
#include <SystemDef.hpp>
#include <Animation/AnimationManager.hpp>
#include <Systems/AI/MovementAnimationSystem.hpp>


Core::PanicAnimationSystem::PanicAnimationSystem() : 
	BaseSystem(EntityHandler::GenerateAspect<WorldPositionComponent, TargetingComponent, AnimationComponent, UnitTypeComponent, 
	AttributeComponent, MovementComponent, FlowfieldComponent, GraphicsComponent>(), 0ULL)
{
}



void Core::PanicAnimationSystem::Update(float delta)
{
	for (std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++)
	{
		Core::UnitTypeComponent* utc = WGETC<Core::UnitTypeComponent>(*it);
		if( utc->type != Core::Rioter )
			continue;
		
		Core::AnimationComponent* animc = WGETC<Core::AnimationComponent>(*it);
		Core::AttributeComponent* attric = WGETC<Core::AttributeComponent>(*it);
		Core::GraphicsComponent* grc = WGETC<Core::GraphicsComponent>(*it);
		Core::MovementDataComponent* mdc = WGETC<Core::MovementDataComponent>(*it);
		Core::TargetingComponent* trc = WGETC<TargetingComponent>(*it);
		Core::MovementComponent* mvmc = WGETC<MovementComponent>(*it);

		int fleeAnimationId = Core::AnimationManager::GetAnimationID( GFX::GetBitmaskValue( grc->bitmask, GFX::BITMASK::MESH_ID ), "flee" );

		bool playFleeAnimation = false;
		bool newAnimation = fleeAnimationId != animc->animationID;
		bool shouldStartAnimation = newAnimation || animc->playing == false;
		bool isMoving = mdc->frameSpeed > STILL_THRESHOLD;






		// unit is under attack...
		bool fleeFromAttack = playFleeAnimation = attric->rioter.stance != Core::RioterStance::Attacking && trc->numberOfAttackers > 0 && isMoving;

		// retreating and moving...
		playFleeAnimation = playFleeAnimation == true ? playFleeAnimation : 
			attric->rioter.stance == Core::RioterStance::Retreating && isMoving;
		
		// run away if under attack...
		if( fleeFromAttack )
		{
			Core::WorldPositionComponent* wpc = WGETC<WorldPositionComponent>(*it);
			glm::vec3 position = glm::vec3( wpc->position[0], wpc->position[1], wpc->position[2] );

			glm::vec3 attackerDirs = glm::vec3(0.0f);
			for( int i = 0; i < trc->numberOfAttackers; i++)
			{
				Core::TargetingComponent* trcOther = WGETC<TargetingComponent>(trc->attackers[i]);
				if( trcOther )
				{
					Core::WorldPositionComponent* wpcOther = WGETC<WorldPositionComponent>(trc->attackers[i]);
					if( wpcOther )
						attackerDirs += glm::normalize( glm::vec3( wpcOther->position[0], wpcOther->position[1], wpcOther->position[2] )  - position );
				}
			}
			
			if( glm::dot( attackerDirs, attackerDirs ) > 0.0f )
			{
				attackerDirs = glm::normalize( attackerDirs );

				mvmc->newDirection[0] = attackerDirs.x;
				mvmc->newDirection[1] = attackerDirs.y;
				mvmc->newDirection[2] = attackerDirs.z;
			}
		}
		









		if( playFleeAnimation )
			mvmc->SetMovementState( Core::MovementState::Movement_Jogging, Core::MovementStatePriority::MovementState_FleeAnimationPriority );

		if( playFleeAnimation && shouldStartAnimation )
		{	
			Core::AnimationManager::PlayAnimation( *it, "flee" ); 

			// randomize animations...
			if( newAnimation )
			{
				if( newAnimation )
					animc->currentTime = ((std::rand() % 1000) / 1000.0f) * 0.25f;
			}
		}
	}
}

#include "PanicAnimationSystem.hpp"

#include <Input/InputManager.hpp>
#include <GameUtility/GameData.hpp>
#include "World.hpp"
#include <SystemDef.hpp>
#include <Animation/AnimationManager.hpp>
#include <Systems/AI/MovementAnimationSystem.hpp>

//GRACE_THRESHOLD
//STILL_THRESHOLD


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

		Core::AttributeComponent* attric = WGETC<Core::AttributeComponent>(*it);
		if( attric->rioter.stance == Core::RioterStance::Retreating )
		{
			Core::MovementDataComponent* mdc = WGETC<Core::MovementDataComponent>(*it);
			if( mdc->frameSpeed > STILL_THRESHOLD )
			{
				Core::GraphicsComponent* grc = WGETC<Core::GraphicsComponent>(*it);
				Core::AnimationComponent* animc = WGETC<Core::AnimationComponent>(*it);
				
				int fleeAnimationId = Core::AnimationManager::GetAnimationID( GFX::GetBitmaskValue( grc->bitmask, GFX::BITMASK::MESH_ID ), "flee" );
				if( fleeAnimationId != animc->animationID || animc->playing == false )
				{
					bool newAnimation = fleeAnimationId != animc->animationID;
					Core::AnimationManager::PlayAnimation( *it, "flee" ); 

					// randomize animations...
					if( newAnimation )
						animc->currentTime = ((std::rand() % 1000) / 1000.0f) * 1.0f;

				}
			}
		}
	}
}

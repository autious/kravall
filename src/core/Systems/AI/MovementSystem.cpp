#include "MovementSystem.hpp"
#include "World.hpp"

#include <DebugMacros.hpp>
#include <GameUtility/GameData.hpp>

//#define drawEntityDirection
#ifdef drawEntityDirection
#define DEBUGDRAW( x ) x
#else
#define DEBUGDRAW( x ) ;
#endif

#define MOVEMENT_ACCELERATION 16.0f
#define MOVEMENT_DECELERATION -17.0f

const float Core::MovementSystem::TURN_FACTOR = 7.5f;

Core::MovementSystem::MovementSystem()  
: BaseSystem( {
	{ EntityHandler::GenerateAspect<WorldPositionComponent, MovementComponent>(), 0ULL}, 
	{ EntityHandler::GenerateAspect<AttributeComponent, MovementComponent, UnitTypeComponent, MovementDataComponent>(), 0ULL } })
{
}


void Core::MovementSystem::Update(float delta)
{
	for (std::vector<Entity>::iterator it = m_bags[0].m_entities.begin();
		it != m_bags[0].m_entities.end();
		it++)
	{
		WorldPositionComponent* wpc = WGETC<WorldPositionComponent>(*it);
		MovementComponent* mc = WGETC<MovementComponent>(*it);

		// process speed...
		float mod = mc->desiredSpeed[ mc->state ] > mc->speed ? MOVEMENT_ACCELERATION : MOVEMENT_DECELERATION;
		
		mc->speed += mod * delta;
		
		// cap speed...
		if (mc->speed > mc->desiredSpeed[ mc->state ])
			mc->speed = mc->desiredSpeed[ mc->state ];
		else if ( mc->speed < 0.0f )
			mc->speed = 0.0f;

		InterpolateDirections(mc, delta);

		wpc->position[0] += mc->direction[0] * mc->speed * delta;
		wpc->position[1] = 0.0f;
		wpc->position[2] += mc->direction[2] * mc->speed * delta;

		if (mc->direction[0] != 0 || mc->direction[1] != 0 || mc->direction[2] != 0)
		{
			// Rotate model after direction
			float angle = static_cast<float>(std::atan2(static_cast<double>(mc->direction[2]), 
														static_cast<double>(mc->direction[0])));
			RotationComponent* rc = WGETC<RotationComponent>(*it);
			*rc = RotationComponent::GetComponentRotateY(-angle - 3.141592f * 1.5f); 
		}

		// Draw the debug lines showing the rioter's direction.
		DEBUGDRAW( GFX::Debug::DrawSphere( glm::vec3( mc->goal[0], 0, mc->goal[2] ), 1.0f, GFXColor( 0.3, 1, 0.45, 1 ), false ); )
		DEBUGDRAW( GFX::Debug::DrawLine(Core::WorldPositionComponent::GetVec3(*wpc),
							 glm::vec3(wpc->position[0] + mc->direction[0],
									   wpc->position[1] + mc->direction[1],
									   wpc->position[2] + mc->direction[2]),
							 GFXColor(1.0f, 1.0f, 0.0f, 1.0f), false); )
	}


	// update police movement state...
	for (std::vector<Entity>::iterator it = m_bags[1].m_entities.begin(); it != m_bags[1].m_entities.end(); it++)
	{
		UnitTypeComponent* utc = WGETC<UnitTypeComponent>(*it);
		if( utc->type != Core::UnitType::Police )
			continue;

		AttributeComponent* attribc = WGETC<AttributeComponent>(*it);
		MovementComponent* mvmc = WGETC<MovementComponent>(*it);

		if( attribc->stamina > 30.0f )
			mvmc->SetMovementState( Core::MovementState::Movement_Jogging, Core::MovementStatePriority::MovementState_MovementSystemPriority );
		else
			mvmc->SetMovementState( Core::MovementState::Movement_Walking, Core::MovementStatePriority::MovementState_MovementSystemPriority );
	}

}

void Core::MovementSystem::InterpolateDirections(MovementComponent* mc, float delta)
{
	if (mc->direction[0] == 0.0f && mc->direction[1] == 0.0f && mc->direction[2] == 0.0f)
	{
		mc->direction[0] = mc->newDirection[0];
		mc->direction[1] = mc->newDirection[1];
		mc->direction[2] = mc->newDirection[2];
	}
	if (mc->newDirection[0] == 0.0f && mc->newDirection[1] == 0.0f && mc->newDirection[2] == 0.0f)
	{
		mc->direction[0] = mc->newDirection[0];
		mc->direction[1] = mc->newDirection[1];
		mc->direction[2] = mc->newDirection[2];
	}
	else
	{
		glm::vec3 oldDir = glm::vec3(mc->direction[0], mc->direction[1], mc->direction[2]);
		glm::vec3 newDir = glm::vec3(mc->newDirection[0], mc->newDirection[1], mc->newDirection[2]);
	
		float differenceFactor = (((glm::dot( oldDir, newDir ) * -1.0f ) + 1.0f) / 2.0f) + 1.0f;

		newDir = glm::lerp(oldDir, newDir, TURN_FACTOR * differenceFactor * delta);

        //Must check if the new dir is 0 before normalizing
        if( glm::length( newDir ) > 0 )
        {
		    newDir = glm::normalize(newDir);
        }

        mc->direction[0] = newDir.x;
        mc->direction[1] = newDir.y;
        mc->direction[2] = newDir.z;
	}
}

#undef drawEntityDirection
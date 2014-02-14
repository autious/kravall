#include "MovementSystem.hpp"
#include "World.hpp"

#include <DebugMacros.hpp>
#include <GameUtility/GameData.hpp>

//const float Core::MovementSystem::TURN_FACTOR = 10.0f;
const float Core::MovementSystem::TURN_FACTOR = 7.5f;

Core::MovementSystem::MovementSystem()  
: BaseSystem( {
	{ EntityHandler::GenerateAspect<WorldPositionComponent, MovementComponent>(), 0ULL}, 
	{ EntityHandler::GenerateAspect<AttributeComponent, MovementComponent, UnitTypeComponent>(), 0ULL } })
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
		const Core::MovementData& movementData = Core::GameData::GetMovementDataWithState( mc->state );
		float mod = mc->desiredSpeed > mc->speed ? movementData.acceleration : movementData.deceleration;
		
		mc->speed += mod * delta;
		
		// cap speed...
		if (mc->speed > mc->desiredSpeed)
			mc->speed = mc->desiredSpeed;
		else if ( mc->speed < 0.0f )
			mc->speed = 0.0f;

		InterpolateDirections(mc, delta);

		wpc->position[0] += mc->direction[0] * mc->speed * delta;
		wpc->position[1] = 0.0f; //+= mc->direction[1] * mc->speed * delta;
		wpc->position[2] += mc->direction[2] * mc->speed * delta;

		if (mc->direction[0] != 0 || mc->direction[1] != 0 || mc->direction[2] != 0)
		{
			// Rotate model after direction
			float angle = static_cast<float>(std::atan2(static_cast<double>(mc->direction[2]), 
														static_cast<double>(mc->direction[0])));

			RotationComponent* rc = WGETC<RotationComponent>(*it);

			// We need to solve this, model might be wrong or something :) // johan sais wait and see ;) -> blame maya? yes. fin.
			*rc = RotationComponent::GetComponentRotateY(-angle - 3.141592f * 1.5f); 
		}

		GFX::Debug::DrawSphere( glm::vec3( mc->goal[0], 0, mc->goal[2] ), 2.0f, GFXColor(), false );

		// Draw the debug lines showing the rioter's direction.
		/*GFX::Debug::DrawLine(Core::WorldPositionComponent::GetVec3(*wpc),
							 glm::vec3(wpc->position[0] + mc->direction[0],
									   wpc->position[1] + mc->direction[1],
									   wpc->position[2] + mc->direction[2]),
							 GFXColor(1.0f, 0.0f, 0.0f, 1.0f), false);*/
	}


	// update police movement state...
	for (std::vector<Entity>::iterator it = m_bags[1].m_entities.begin(); it != m_bags[1].m_entities.end(); it++)
	{
		UnitTypeComponent* utc = WGETC<UnitTypeComponent>(*it);
		if( utc->type != Core::Police )
			continue;

		AttributeComponent* attribc = WGETC<AttributeComponent>(*it);
		MovementComponent* mvmc = WGETC<MovementComponent>(*it);

		if( attribc->stamina > 30.0f )
		{
			mvmc->state = Core::MovementState::Movement_Sprinting;
		}
		else
		{
			mvmc->state = Core::MovementState::Movement_Walking;
		}

		if( mvmc->movedThisFrame )
			attribc->stamina -= Core::GameData::GetMovementDataWithState( mvmc->state ).staminaCostPerSecond;
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
	
		newDir = glm::lerp(oldDir, newDir, TURN_FACTOR * delta);

        //Must check if the new dir is 0 before normalizing
        if( glm::length( newDir ) > 0 )
        {
		    newDir = glm::normalize(newDir);
        }

        mc->direction[0] = newDir.x;
        mc->direction[1] = newDir.y;
        mc->direction[2] = newDir.z;
	}

	//mc->direction[0] = mc->newDirection[0];
	//mc->direction[1] = mc->newDirection[1];
	//mc->direction[2] = mc->newDirection[2];
}

#include "MovementSystem.hpp"
#include "World.hpp"

#include <DebugMacros.hpp>
#include <GameUtility/GameData.hpp>

//const float Core::MovementSystem::TURN_FACTOR = 10.0f;
const float Core::MovementSystem::TURN_FACTOR = 7.5f;

void Core::MovementSystem::Update(float delta)
{


	for (std::vector<Entity>::iterator it = m_entities.begin();
		it != m_entities.end();
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
		
		// process position...
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

			*rc = RotationComponent::GetComponentRotateY(-angle - 3.141592f * 0.5f);
		}

		// Draw the debug lines showing the rioter's direction.
		/*GFX::Debug::DrawLine(Core::WorldPositionComponent::GetVec3(*wpc),
							 glm::vec3(wpc->position[0] + mc->direction[0],
									   wpc->position[1] + mc->direction[1],
									   wpc->position[2] + mc->direction[2]),
							 GFXColor(1.0f, 0.0f, 0.0f, 1.0f), false);*/

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

#include "TargetingSystem.hpp"

#include <Input/InputManager.hpp>
#include <GameUtility/GameData.hpp>
#include "World.hpp"
#include <SystemDef.hpp>

#define MAX_TARGETING_TIME 2.0f
#define MAX_SQR_DISTANCE 441.0f

Core::TargetingSystem::TargetingSystem() : BaseSystem(EntityHandler::GenerateAspect<WorldPositionComponent, UnitTypeComponent, 
		AttributeComponent, MovementComponent, FlowfieldComponent>(), 0ULL), m_lastClickedEntity(INVALID_ENTITY), 
		m_wasFPressed(false), m_currentTarget(INVALID_ENTITY)
{}

void Core::TargetingSystem::Update(float delta)
{
	for (std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++)
	{
		Core::UnitTypeComponent* utc = WGETC<Core::UnitTypeComponent>(*it);
		
		if (utc->type == UnitType::Police)
			HandlePoliceTargeting(*it, delta);
		else if (utc->type == UnitType::Rioter)
			HandleRioterTargeting(*it, delta);
	}

	//TemporaryFunction();
}

void Core::TargetingSystem::HandlePoliceTargeting(Core::Entity police, float delta)
{
	Core::TargetingComponent* tc = WGETC<Core::TargetingComponent>(police);

	// return if the unit does not have a weapon
	if( tc->weapon < 0 )
		return; 

	Core::AttributeComponent* ac = WGETC<Core::AttributeComponent>(police);
	Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(police);
	Core::MovementComponent* mc = WGETC<Core::MovementComponent>(police);
	const Core::WeaponData& weapon = Core::GameData::GetWeaponDataFromWeapon( tc->weapon );

	GFXColor colour = GFXColor(1.0f, 1.0f, 1.0f, 1.0f);	

	Core::TargetingComponent* tcTarget = WGETC<Core::TargetingComponent>(tc->target);
	if( tcTarget )
	{		
		Core::WorldPositionComponent* twpc = WGETC<Core::WorldPositionComponent>(tc->target);
		
		float dx = twpc->position[0] - wpc->position[0];
		float dy = twpc->position[1] - wpc->position[1];
		float dz = twpc->position[2] - wpc->position[2];

		float distSqr = dx * dx + dy * dy + dz * dz;

		if (distSqr < weapon.range * weapon.range )
		{
			TargetingComponent::Attack(police, *tcTarget);

			//if (TargetingComponent::Attack(police, *tcTarget))
			//	std::cout << "Police: " << police << " is attacking rioter " << tc->target << std::endl;
		}

		tc->attackTime += delta;

		if (tc->attackTime > MAX_TARGETING_TIME) // Reset target, attack time is reset upon aquiring new target.
		{
			tc->target = INVALID_ENTITY;
		}
		else if( ac->police.stance != PoliceStance::Passive )
			return;
	}
	else
		tc->target = INVALID_ENTITY;

	switch (ac->police.stance)
	{
		case PoliceStance::Aggressive:
			if (tc->target == INVALID_ENTITY)
			{
				tc->target = FindClosestTarget(wpc, UnitType::Rioter);
				tc->attackTime = 0.0f;
			}
			else
			{
				colour = GFXColor(1.0f, 0.0f, 0.0f, 1.0f);
			}
			break;
		case PoliceStance::Defensive:
			if (tc->target != INVALID_ENTITY)
			{
				TargetingComponent* tcTarget = WGETC<Core::TargetingComponent>(tc->target);
				if( tcTarget )
					TargetingComponent::StopAttacking(police, *tcTarget);
			}

			tc->target = FindClosestAttacker(tc, wpc);

			break;
		case PoliceStance::Passive:
			if (tc->target != INVALID_ENTITY)
			{
				TargetingComponent* tcTarget = WGETC<Core::TargetingComponent>(tc->target);
				TargetingComponent::StopAttacking(police, *tcTarget);
			}			

			tc->target = INVALID_ENTITY;
			break;
	}

	

	if (tc->target != INVALID_ENTITY)
	{
		//Core::WorldPositionComponent* twpc = WGETC<Core::WorldPositionComponent>(tc->target);
		//Core::BoundingVolumeComponent* tbvc = WGETC<Core::BoundingVolumeComponent>(tc->target);
		//Core::BoundingSphere* tSphere = reinterpret_cast<Core::BoundingSphere*>(tbvc->data);
		//GFX::Debug::DrawSphere(twpc->GetVec3(*twpc) + *reinterpret_cast<glm::vec3*>(tSphere->offset), tSphere->radius, GFXColor(1.0f, 0.0f, 0.0f, 1.0f), false);
	}
	
	//Core::BoundingVolumeComponent* bvc = WGETC<Core::BoundingVolumeComponent>(police);
	//Core::BoundingSphere* sphere = reinterpret_cast<Core::BoundingSphere*>(bvc->data);
	//GFX::Debug::DrawSphere(wpc->GetVec3(*wpc) + *reinterpret_cast<glm::vec3*>(sphere->offset), sphere->radius, colour, false);
}

void Core::TargetingSystem::HandleRioterTargeting(Core::Entity rioter, float delta)
{
	Core::TargetingComponent* tc = WGETC<Core::TargetingComponent>(rioter);

	// return if the unit does not have a weapon
	if( tc->weapon < 0 )
		return; 

	Core::AttributeComponent* ac = WGETC<Core::AttributeComponent>(rioter);
	Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(rioter);
	Core::MovementComponent* mc = WGETC<Core::MovementComponent>(rioter);
	const Core::WeaponData& weapon = Core::GameData::GetWeaponDataFromWeapon( tc->weapon );

	if (ac->rioter.stance != RioterStance::Attacking)
		return;

	Core::TargetingComponent* tcTarget = WGETC<Core::TargetingComponent>(tc->target);
	if( tcTarget )
	{
		Core::WorldPositionComponent* twpc = WGETC<Core::WorldPositionComponent>(tc->target);

		float dx = twpc->position[0] - wpc->position[0];
		float dy = twpc->position[1] - wpc->position[1];
		float dz = twpc->position[2] - wpc->position[2];

		float distSqr = dx * dx + dy * dy + dz * dz;

		if (distSqr < weapon.range * weapon.range ) // Attack
		{
			if (TargetingComponent::Attack(rioter, *tcTarget))
				;//std::cout << "Rioter: " << rioter << " is attacking police " << tc->target << std::endl;
		}

		tc->attackTime += delta;

		if (tc->attackTime > MAX_TARGETING_TIME) // Reset target, attack time is reset upon aquiring new target.
		{
			tc->target = INVALID_ENTITY;
		}
		else
			return;
	}
	else
		tc->target = INVALID_ENTITY;

	tc->target = FindClosestAttacker(tc, wpc);
	if (tc->target != INVALID_ENTITY)
		return;

	tc->target = FindClosestTarget(wpc, UnitType::Police);
	if (tc->target != INVALID_ENTITY)
		return;


	// 1. Attacker
	// 2. Closest police
	// 3. Other rioter of differing alignment
}

Core::Entity Core::TargetingSystem::FindClosestTarget(Core::WorldPositionComponent* origin, Core::UnitType targetType)
{
	float minDist = std::numeric_limits<float>::max();
	Entity minDistEntity = INVALID_ENTITY;
	for (std::vector<Entity>::iterator it = m_entities.begin();	it != m_entities.end();	it++)
	{
		Core::UnitTypeComponent* utc = WGETC<Core::UnitTypeComponent>(*it);
		if (utc->type != targetType)
			continue;

		Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(*it);
		Core::TargetingComponent* tc = WGETC<Core::TargetingComponent>(*it);

		float dx = wpc->position[0] - origin->position[0];
		float dy = wpc->position[1] - origin->position[1];
		float dz = wpc->position[2] - origin->position[2];

		float distSqr = dx * dx + dy * dy + dz * dz;

		if (distSqr > 0.1f && distSqr < minDist && tc->numberOfAttackers < MAX_ATTACKERS)
		{
			minDist = distSqr;
			minDistEntity = *it;
		}
	}

	if (minDist < MAX_SQR_DISTANCE)
		return minDistEntity;
	else
		return INVALID_ENTITY;
}

Core::Entity Core::TargetingSystem::FindClosestAttacker(Core::TargetingComponent* originTC, Core::WorldPositionComponent* originWPC)
{
	Entity target = INVALID_ENTITY;
	float minDist = std::numeric_limits<float>::max();

	if (originTC->numberOfAttackers > 0)
	{
		for (int i = 0; i < originTC->numberOfAttackers; ++i)
		{
			Core::WorldPositionComponent* twpc = WGETC<Core::WorldPositionComponent>(originTC->attackers[i]);

			float dx = twpc->position[0] - originWPC->position[0];
			float dy = twpc->position[1] - originWPC->position[1];
			float dz = twpc->position[2] - originWPC->position[2];

			float distSqr = dx * dx + dy * dy + dz * dz;

			if (distSqr < minDist)
			{
				target = originTC->attackers[i];
				minDist = distSqr;
			}
		}
	}

	if (minDist < MAX_SQR_DISTANCE)
	{
		// check if target is still alive...
		Core::TargetingComponent* tcTarget12 = WGETC<Core::TargetingComponent>(target);
		if( !tcTarget12 )
		{
			Core::TargetingComponent::StopAttacking( target, *originTC );
			return INVALID_ENTITY;
		}
		else
			return target;
	}
	else
		return INVALID_ENTITY;
}

void Core::TargetingSystem::TemporaryFunction()
{
	/*for (std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++)
	{
		Core::UnitTypeComponent* utc = WGETC<Core::UnitTypeComponent>(*it);
		if (utc->type != UnitType::Rioter)
			continue;

		Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(*it);
		Core::AttributeComponent* ac = WGETC<Core::AttributeComponent>(*it);

		GFXColor colour = GFXColor(1.0f, 1.0f, 1.0f, 1.0f);
		if (ac->rioter.stance == RioterStance::Attacking)
			colour = GFXColor(1.0f, 0.0f, 0.0f, 1.0f);

		/*if (ac->police.stance == PoliceStance::Aggressive)
			colour = GFXColor(1.0f, 0.0f, 0.0f, 1.0f);
		else if (ac->police.stance == PoliceStance::Defensive)
			colour = GFXColor(0.0f, 0.0f, 1.0f, 1.0f);
		else if (ac->police.stance == PoliceStance::Passive)
			colour = GFXColor(0.0f, 1.0f, 0.0f, 1.0f);*/

		/*Core::BoundingVolumeComponent* bvc = WGETC<Core::BoundingVolumeComponent>(*it);
		Core::BoundingSphere* sphere = reinterpret_cast<Core::BoundingSphere*>(bvc->data);
		GFX::Debug::DrawSphere(wpc->GetVec3(*wpc) + *reinterpret_cast<glm::vec3*>(sphere->offset), sphere->radius, colour, false);
	}*/

	Entity selectedEntity = Core::world.m_systemHandler.GetSystem<Core::PickingSystem>()->GetLastHitEntity();

	if (Core::GetInputManager().GetMouseState().IsButtonDown(GLFW_MOUSE_BUTTON_1))
	{
		if (selectedEntity != INVALID_ENTITY)
		{
			m_lastClickedEntity = selectedEntity;
			std::cout << "Selected entitiy: " << m_lastClickedEntity << std::endl;
		}
		else
			m_lastClickedEntity = INVALID_ENTITY;
	}

	if (m_lastClickedEntity != INVALID_ENTITY)
	{
		Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(m_lastClickedEntity);

		bool isFPressed = Core::GetInputManager().GetKeyboardState().IsKeyDown(GLFW_KEY_F);
		if (isFPressed /*&& !m_wasFPressed*/)
		{
			m_currentTarget = FindClosestTarget(wpc, UnitType::Police);
			if (m_currentTarget != INVALID_ENTITY)
			{
				Core::TargetingComponent* tc = WGETC<Core::TargetingComponent>(m_lastClickedEntity);
				Core::MovementComponent* mc = WGETC<Core::MovementComponent>(m_lastClickedEntity);
				Core::WorldPositionComponent* twpc = WGETC<Core::WorldPositionComponent>(m_currentTarget);

				tc->target = m_currentTarget;

				mc->goal[0] = twpc->position[0];
				mc->goal[1] = twpc->position[1];
				mc->goal[2] = twpc->position[2];
			}
		}
		else
		{
			Core::TargetingComponent* tc = WGETC<Core::TargetingComponent>(m_lastClickedEntity);
			Core::MovementComponent* mc = WGETC<Core::MovementComponent>(m_lastClickedEntity);

			tc->target = INVALID_ENTITY;

			mc->goal[0] = FLT_MAX;
			mc->goal[1] = 0.0f;
			mc->goal[2] = 0.0f;
		}
		/*else if (Core::GetInputManager().GetKeyboardState().IsKeyDown(GLFW_KEY_P)) // Passive
		{
			int selectedSquad = WGETC<Core::AttributeComponent>(m_lastClickedEntity)->police.squadID;

			for (std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++)
			{
				Core::UnitTypeComponent* utc = WGETC<Core::UnitTypeComponent>(*it);
				if (utc->type != UnitType::Police)
					continue;

				Core::AttributeComponent* ac = WGETC<Core::AttributeComponent>(*it);
				Core::MovementComponent* mc = WGETC<Core::MovementComponent>(*it);

				if (ac->police.squadID != selectedSquad)
					continue;

				ac->police.stance = PoliceStance::Passive;
				
				mc->goal[0] = FLT_MAX;
				mc->goal[1] = 0.0f;
				mc->goal[2] = 0.0f;

				mc->speed = 0.0f;
				mc->desiredSpeed = 0.0f;
			}
		}
		else if (Core::GetInputManager().GetKeyboardState().IsKeyDown(GLFW_KEY_O)) // Defensive
		{
			int selectedSquad = WGETC<Core::AttributeComponent>(m_lastClickedEntity)->police.squadID;

			for (std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++)
			{
				Core::UnitTypeComponent* utc = WGETC<Core::UnitTypeComponent>(*it);
				if (utc->type != UnitType::Police)
					continue;

				Core::AttributeComponent* ac = WGETC<Core::AttributeComponent>(*it);
				Core::MovementComponent* mc = WGETC<Core::MovementComponent>(*it);

				if (ac->police.squadID != selectedSquad)
					continue;

				ac->police.stance = PoliceStance::Defensive; 
				mc->goal[0] = FLT_MAX;
				mc->goal[1] = 0.0f;
				mc->goal[2] = 0.0f;

				mc->speed = 0.0f;
				mc->desiredSpeed = 0.0f;
			}
		}
		else if (Core::GetInputManager().GetKeyboardState().IsKeyDown(GLFW_KEY_I)) // Aggressive
		{
			int selectedSquad = WGETC<Core::AttributeComponent>(m_lastClickedEntity)->police.squadID;

			for (std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++)
			{
				Core::UnitTypeComponent* utc = WGETC<Core::UnitTypeComponent>(*it);
				if (utc->type != UnitType::Police)
					continue;

				Core::AttributeComponent* ac = WGETC<Core::AttributeComponent>(*it);

				if (ac->police.squadID != selectedSquad)
					continue;

				ac->police.stance = PoliceStance::Aggressive;
			}
		}*/

		Core::BoundingVolumeComponent* bvc = WGETC<Core::BoundingVolumeComponent>(m_lastClickedEntity);
		Core::BoundingSphere* sphere = reinterpret_cast<Core::BoundingSphere*>(bvc->data);
		GFX::Debug::DrawSphere(wpc->GetVec3(*wpc) + *reinterpret_cast<glm::vec3*>(sphere->offset), sphere->radius, GFXColor(1.0f, 0.7f, 0.0f, 1.0f), false);
		m_wasFPressed = isFPressed;
	}

	if (m_currentTarget != INVALID_ENTITY)
	{
		Core::WorldPositionComponent* twpc = WGETC<Core::WorldPositionComponent>(m_currentTarget);
		Core::BoundingVolumeComponent* tbvc = WGETC<Core::BoundingVolumeComponent>(m_currentTarget);
		Core::BoundingSphere* tSphere = reinterpret_cast<Core::BoundingSphere*>(tbvc->data);
		GFX::Debug::DrawSphere(twpc->GetVec3(*twpc) + *reinterpret_cast<glm::vec3*>(tSphere->offset), tSphere->radius, GFXColor(1.0f, 0.0f, 0.0f, 1.0f), false);
	}
}

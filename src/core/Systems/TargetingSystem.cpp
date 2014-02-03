#include "TargetingSystem.hpp"
#include <Input/InputManager.hpp>
#include "World.hpp"

#include <SystemDef.hpp>

//#include <DebugMacros.hpp>
//#include <GameUtility/GameData.hpp>

Core::TargetingSystem::TargetingSystem() : BaseSystem(EntityHandler::GenerateAspect<WorldPositionComponent, UnitTypeComponent, AttributeComponent>(), 0ULL),
	m_lastClickedEntity(INVALID_ENTITY), m_wasFPressed(false), m_currentTarget(INVALID_ENTITY)
{}

void Core::TargetingSystem::Update(float delta)
{
	return; // NOCOMMIT
	TemporaryFunction();

	for (std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++)
	{
		Core::UnitTypeComponent* utc = WGETC<Core::UnitTypeComponent>(*it);
		
		if (utc->type == UnitType::Police)
			HandlePoliceTargeting(*it);
		else if (utc->type == UnitType::Rioter)
			HandleRioterTargeting(*it);
	}
}

void Core::TargetingSystem::HandlePoliceTargeting(Core::Entity police)
{
	Core::AttributeComponent* ac = WGETC<Core::AttributeComponent>(police);
	Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(police);
	
	GFXColor colour = GFXColor(1.0f, 1.0f, 1.0f, 1.0f);

	switch (ac->police.stance)
	{
		case PoliceStance::Aggressive:
			FindClosestTarget(wpc, UnitType::Rioter);
			colour = GFXColor(1.0f, 0.0f, 0.0f, 1.0f);
			break;
		case PoliceStance::Defensive:
			colour = GFXColor(0.0f, 1.0f, 0.0f, 1.0f);
			break;
	}

	
	Core::BoundingVolumeComponent* bvc = WGETC<Core::BoundingVolumeComponent>(police);
	Core::BoundingSphere* sphere = reinterpret_cast<Core::BoundingSphere*>(bvc->data);
	GFX::Debug::DrawSphere(wpc->GetVec3(*wpc) + *reinterpret_cast<glm::vec3*>(sphere->offset), sphere->radius, colour, false);
}

void Core::TargetingSystem::HandleRioterTargeting(Core::Entity rioter)
{
	Core::AttributeComponent* ac = WGETC<Core::AttributeComponent>(rioter);
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

		float dx = wpc->position[0] - origin->position[0];
		float dy = wpc->position[1] - origin->position[1];
		float dz = wpc->position[2] - origin->position[2];

		float distSqr = dx * dx + dy * dy + dz * dz;

		if (distSqr > 0.1f && distSqr < minDist)
		{
			minDist = distSqr;
			minDistEntity = *it;
		}
	}

	return minDistEntity;
}

void Core::TargetingSystem::TemporaryFunction()
{
	for (std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++)
	{
		Core::UnitTypeComponent* utc = WGETC<Core::UnitTypeComponent>(*it);
		if (utc->type != UnitType::Police)
			continue;

		Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(*it);
		Core::AttributeComponent* ac = WGETC<Core::AttributeComponent>(*it);

		GFXColor colour = GFXColor(1.0f, 1.0f, 1.0f, 1.0f);
		/*if (ac->police.stance == PoliceStance::Aggressive)
			colour = GFXColor(1.0f, 0.0f, 0.0f, 1.0f);
		else if (ac->police.stance == PoliceStance::Defensive)
			colour = GFXColor(0.0f, 0.0f, 1.0f, 1.0f);
		else if (ac->police.stance == PoliceStance::Passive)
			colour = GFXColor(0.0f, 1.0f, 0.0f, 1.0f);*/

		Core::BoundingVolumeComponent* bvc = WGETC<Core::BoundingVolumeComponent>(*it);
		Core::BoundingSphere* sphere = reinterpret_cast<Core::BoundingSphere*>(bvc->data);
		GFX::Debug::DrawSphere(wpc->GetVec3(*wpc) + *reinterpret_cast<glm::vec3*>(sphere->offset), sphere->radius, colour, false);
	}

	int selectedEntity = Core::world.m_systemHandler.GetSystem<Core::PickingSystem>()->GetLastHitEntity();

	if (Core::GetInputManager().GetMouseState().IsButtonDown(GLFW_MOUSE_BUTTON_1))
	{
		if (selectedEntity != INVALID_ENTITY)
			m_lastClickedEntity = selectedEntity;
		else
			m_lastClickedEntity = INVALID_ENTITY;
	}

	if (m_lastClickedEntity != INVALID_ENTITY)
	{
		Core::WorldPositionComponent* wpc = WGETC<Core::WorldPositionComponent>(m_lastClickedEntity);

		bool isFPressed = Core::GetInputManager().GetKeyboardState().IsKeyDown(GLFW_KEY_F);
		if (isFPressed /*&& !m_wasFPressed*/)
			m_currentTarget = FindClosestTarget(wpc, UnitType::Rioter);
		else if (Core::GetInputManager().GetKeyboardState().IsKeyDown(GLFW_KEY_P)) // Passive
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

				ac->police.stance = PoliceStance::Passive;
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

				if (ac->police.squadID != selectedSquad)
					continue;

				ac->police.stance = PoliceStance::Defensive;
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
		}

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
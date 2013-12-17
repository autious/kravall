#include "FieldReactionSystem.hpp"
#include "World.hpp"
#include <logger/Logger.hpp>


Core::FieldReactionSystem::FieldReactionSystem() : BaseSystem(EntityHandler::GenerateAspect<WorldPositionComponent, MovementComponent,
	UnitTypeComponent, AttributeComponent>(), 0ULL), m_showPF(true)
{
	for (int i = 0; i < FIELD_SIDE_CELL_COUNT; ++i)
	{
		for (int j = 0; j < FIELD_SIDE_CELL_COUNT; ++j)
		{
			m_field[i][j] = 1.0f;
		}
	}
}

void Core::FieldReactionSystem::Update(float delta)
{
	/*
	1.	foreach agent a
	2.		if a is rioter
	3.			for -1:1
	4.				for -1:1
	5.					foreach agent b
	6.						if a != b
	7.							sum += b:getChargeAt(a.position)
	8.			highestSumPos: Position with highest sum [-1:1] [-1:1]
	9.			a:direction = highestSumPos
	*/

	for (std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++) // 1.
	{
		WorldPositionComponent* wpc = WGETC<WorldPositionComponent>(*it);
		MovementComponent* mc = WGETC<MovementComponent>(*it);
		UnitTypeComponent* utc = WGETC<UnitTypeComponent>(*it);
		AttributeComponent* ac = WGETC<AttributeComponent>(*it);

		if (utc->type == UnitType::Rioter) // 2.
		{
			glm::vec2 bestIndex;
			float highestSum = GetEffectOnAgent(wpc);

			for (int i = -1; i < 2; ++i) // 3.
			{
				for (int j = -1; j < 2; ++j) // 4.
				{
					if (i == 0 && j == 0)
						continue;

					float chargeSum = GetEffectOnAgent(&WorldPositionComponent(wpc->position[0] + i, wpc->position[1],
						wpc->position[2] + j));

					if (chargeSum > highestSum)
					{
						highestSum = chargeSum;
						bestIndex = glm::vec2(i, j);
					}
				}
			}

			mc->direction[0] = bestIndex.x;
			mc->direction[2] = bestIndex.y;
		}
	}
}

float Core::FieldReactionSystem::GetEffectOnAgent(WorldPositionComponent* agentPos)
{
	float sum = 0.0f;

	for (std::vector<Entity>::iterator it2 = m_entities.begin(); it2 != m_entities.end(); it2++) // 5.
	{
		sum += GetChargeAt(*it2, WorldPositionComponent::GetVec3(agentPos->position)); // 7.
	}

	return sum;
}

float Core::FieldReactionSystem::GetChargeAt(Entity chargedAgent, glm::vec3 queryPosition)
{
	float cutoff = 5.0f;
	float repelRadius = 0.8f;
	float peakCharge = 1.0f;
	float decline = peakCharge / (cutoff - repelRadius);
	WorldPositionComponent* wpc = WGETC<WorldPositionComponent>(chargedAgent);
	glm::vec3 distVec = (WorldPositionComponent::GetVec3(wpc->position) - queryPosition);
	float distance = (distVec.x * distVec.x) + (distVec.z * distVec.z);
	
	if (distance <= 0.001f || distance > cutoff)
		return 0.0f;
	else if (distance < repelRadius)
		return -100 + distance * (repelRadius / 100);
	else
		return peakCharge - distance * decline;
}
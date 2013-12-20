#include "FieldReactionSystem.hpp"
#include "World.hpp"
#include <logger/Logger.hpp>

const float Core::FieldReactionSystem::FIELD_CELL_SIDE_SIZE = FIELD_SIDE_LENGTH / static_cast<float>(FIELD_SIDE_CELL_COUNT);

Core::FieldReactionSystem::FieldReactionSystem() : BaseSystem(EntityHandler::GenerateAspect<WorldPositionComponent, MovementComponent,
	UnitTypeComponent, AttributeComponent>(), 0ULL), m_showPF(false)
{
	for (int i = 0; i < FIELD_SIDE_CELL_COUNT; ++i)
	{
		for (int j = 0; j < FIELD_SIDE_CELL_COUNT; ++j)
		{
			m_field[i][j] = -1.0f + i * 0.025f + j * 0.025f;
		}
	}
}

void Core::FieldReactionSystem::Update(float delta)
{
	UpdateAgents();

	if (m_showPF)
	{
		UpdateDebugField(m_entities[1]);
		//DrawDebugField();
	}

	DrawDebugField();
}

void Core::FieldReactionSystem::UpdateAgents()
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

                    WorldPositionComponent wpct(wpc->position[0] + i, wpc->position[1],
						wpc->position[2] + j);

					float chargeSum = GetEffectOnAgent( &wpct );

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
		sum += GetChargeAt(*it2, WorldPositionComponent::GetVec3(*agentPos)); // 7.
	}

	return sum;
}

float Core::FieldReactionSystem::GetChargeAt(Entity chargedAgent, glm::vec3 queryPosition)
{
	float cutoff = 15.0f;
	float repelRadius = 0.8f;
	float peakCharge = 1.0f;
	float decline = peakCharge / (cutoff - repelRadius);
	WorldPositionComponent* wpc = WGETC<WorldPositionComponent>(chargedAgent);
	glm::vec3 distVec = (WorldPositionComponent::GetVec3(*wpc) - queryPosition);
	float distance = (distVec.x * distVec.x) + (distVec.z * distVec.z);
	
	if (distance <= 0.001f || distance > cutoff)
		return 0.0f;
	else if (distance < repelRadius)
		return -100 + distance * (repelRadius / 100);
	else
		return peakCharge - distance * decline;
}

void Core::FieldReactionSystem::UpdateDebugField(Entity selectedAgent)
{
	for (int i = 0; i < FIELD_SIDE_CELL_COUNT; ++i)
	{
		for (int j = 0; j < FIELD_SIDE_CELL_COUNT; ++j)
		{
			WorldPositionComponent pos = WorldPositionComponent(GetPositionFromFieldIndex(i, j));
			m_field[i][j] = GetEffectOnAgent(&pos) * 0.1f;
		}
	}
}

void Core::FieldReactionSystem::DrawDebugField()
{
	float cellHalfSize = FIELD_CELL_SIDE_SIZE * 0.5f;
	float fieldStart = -FIELD_SIDE_LENGTH * 0.5f - cellHalfSize;
	float yPos = 0.0f;

	for (int i = 0; i < FIELD_SIDE_CELL_COUNT; ++i)
	{
		for (int j = 0; j < FIELD_SIDE_CELL_COUNT; ++j)
		{
			GFXColor colour = GFXColor(1.0f, 1.0f, 1.0f, 1.0f);

			if (m_field[i][j] > 0.0f)
				colour = GFXColor(0.0f, m_field[i][j], 0.0f, 1.0f);
			else
				colour = GFXColor(-m_field[i][j], 0.0f, 0.0f, 1.0f);

			GFX::Debug::DrawBox(GetPositionFromFieldIndex(i, j, yPos),
				GFXVec3(FIELD_CELL_SIDE_SIZE + 0.1f, 0.1f, FIELD_CELL_SIDE_SIZE + 0.1f), true, colour, true);
		}
	}
}

glm::vec3 Core::FieldReactionSystem::GetPositionFromFieldIndex(int xIndex, int zIndex, int yPos)
{
	float cellHalfSize = FIELD_CELL_SIDE_SIZE * 0.5f;
	float fieldStart = -FIELD_SIDE_LENGTH * 0.5f - cellHalfSize;

	return GFXVec3(fieldStart + FIELD_CELL_SIDE_SIZE * xIndex, 
				   yPos, 
				   fieldStart + FIELD_CELL_SIDE_SIZE * zIndex);
}

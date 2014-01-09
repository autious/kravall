#include "FieldReactionSystem.hpp"
#include "World.hpp"
#include <logger/Logger.hpp>

#define frsChargeCurve Core::FieldReactionSystem::ChargeCurve

const float Core::FieldReactionSystem::STAY_LIMIT = 0.1f;

const float Core::FieldReactionSystem::FIELD_CELL_SIDE_SIZE = FIELD_SIDE_LENGTH / static_cast<float>(FIELD_SIDE_CELL_COUNT);
const frsChargeCurve Core::FieldReactionSystem::CURVE[1][2] =
{
	{ frsChargeCurve::ChargeCurve(1.0f, 15.0f, 1.0f), frsChargeCurve::ChargeCurve(-100.0f, 15.0f, 1.0f) }
};

/*const frsChargeCurve Core::FieldReactionSystem::CURVE[2] =
	{ frsChargeCurve::ChargeCurve(1.0f, 15.0f, 0.8f), frsChargeCurve::ChargeCurve(-1.0f, 15.0f, 0.8f) };*/

Core::FieldReactionSystem::FieldReactionSystem() : BaseSystem(EntityHandler::GenerateAspect<WorldPositionComponent, MovementComponent,
	UnitTypeComponent, AttributeComponent>(), 0ULL), m_showPF(false), m_updateCounter(0)
{
	for (int i = 0; i < FIELD_SIDE_CELL_COUNT; ++i)
	{
		for (int j = 0; j < FIELD_SIDE_CELL_COUNT; ++j)
		{
			m_field[i][j] = -1.0f + i * 0.025f + j * 0.025f;
			m_calculatingField[i][j] = 0.0f;
		}
	}
}

void Core::FieldReactionSystem::Update(float delta)
{
	//return; // NOCOMMIT
	UpdateAgents();

	if (m_showPF)
	{
		UpdateDebugField();
		DrawDebugField();
	}
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
			glm::vec2 bestIndex = glm::vec2(0.0f, 0.0f);

			float highestSum = GetEffectOnAgentAt(*it, wpc);
			float staySum = highestSum;

			for (int i = -1; i < 2; ++i) // 3.
			{
				for (int j = -1; j < 2; ++j) // 4.
				{
					if (i == 0 && j == 0)
						continue;

                    WorldPositionComponent wpct(wpc->position[0] + i, wpc->position[1],
						wpc->position[2] + j);

					float chargeSum = GetEffectOnAgentAt(*it, &wpct);

					if (chargeSum > highestSum)
					{
						highestSum = chargeSum;
						bestIndex = glm::vec2(i, j);
					}
				}
			}

			if (highestSum - staySum > STAY_LIMIT)
			{
				float invLength = 1.0f / std::sqrt(bestIndex.x * bestIndex.x + bestIndex.y * bestIndex.y);
				mc->direction[0] = bestIndex.x * invLength;
				mc->direction[2] = bestIndex.y * invLength;
			}
			else
			{
				mc->direction[0] = 0.0f;
				mc->direction[2] = 0.0f;
			}
		}
	}
}

float Core::FieldReactionSystem::GetEffectOnAgentAt(const Entity queryAgent, WorldPositionComponent* queryPosition)
{
	float sum = 0.0f;

	for (std::vector<Entity>::iterator it2 = m_entities.begin(); it2 != m_entities.end(); it2++) // 5.
	{
		if (queryAgent != (*it2)) // 6.
			sum += GetAgentsChargeAt(*it2, WorldPositionComponent::GetVec3(*queryPosition)); // 7.
	}

	return sum;
}

float Core::FieldReactionSystem::GetAgentsChargeAt(Entity chargedAgent, glm::vec3 queryPosition)
{
	Core::UnitTypeComponent* utc = WGETC<UnitTypeComponent>(chargedAgent);
	int indexFromType = static_cast<int>(utc->type);

	WorldPositionComponent* wpc = WGETC<WorldPositionComponent>(chargedAgent);
	glm::vec3 distVec = (WorldPositionComponent::GetVec3(*wpc) - queryPosition);
	float distanceSqr = (distVec.x * distVec.x) + (distVec.z * distVec.z);
	
	if (distanceSqr <= 0.001f || distanceSqr > CURVE[0][indexFromType].cutoff)
		return 0.0f;
	else if (distanceSqr < CURVE[0][indexFromType].repelRadius)
		return -100 + distanceSqr * (CURVE[0][indexFromType].repelRadius / 100);
	else
		return CURVE[0][indexFromType].charge - distanceSqr * CURVE[0][indexFromType].decline;
}

void Core::FieldReactionSystem::UpdateDebugField()
{
	int startRow = m_updateCounter * FIELD_UPDATE_ROW_COUNT;
	int endRow = m_updateCounter == FIELD_UPDATE_FRAME_COUNT - 1 ?
				 FIELD_SIDE_CELL_COUNT : ((m_updateCounter + 1 ) * FIELD_UPDATE_ROW_COUNT);

	for (int i = startRow; i < endRow; ++i)
	{
		for (int j = 0; j < FIELD_SIDE_CELL_COUNT; ++j)
		{
			WorldPositionComponent pos = WorldPositionComponent(GetPositionFromFieldIndex(i, j));
            //std::numeric_limits is never given to any real entity.
			m_calculatingField[i][j] = GetEffectOnAgentAt( std::numeric_limits<Entity>::max(), &pos) * 0.5f;
		}
	}

	if (++m_updateCounter >= FIELD_UPDATE_FRAME_COUNT)
	{
		m_updateCounter = 0;
		CommitDebugField();
	}

	/*for (int i = 0; i < FIELD_SIDE_CELL_COUNT; ++i)
	{
		for (int j = 0; j < FIELD_SIDE_CELL_COUNT; ++j)
		{
			WorldPositionComponent pos = WorldPositionComponent(GetPositionFromFieldIndex(i, j));
			m_field[i][j] = GetEffectOnAgent(&pos) * 0.1f;
		}
	}*/
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
				colour = GFXColor(0.0f, 0.3f - m_field[i][j], m_field[i][j], 1.0f);
			else
				colour = GFXColor(-m_field[i][j], 0.3f + m_field[i][j], 0.0f, 1.0f);

			GFX::Debug::DrawBox(GetPositionFromFieldIndex(i, j, yPos),
				GFXVec3(FIELD_CELL_SIDE_SIZE + 0.1f, 0.1f, FIELD_CELL_SIDE_SIZE + 0.1f), true, colour, true);
		}
	}
}

void Core::FieldReactionSystem::CommitDebugField()
{
	for (int i = 0; i < FIELD_SIDE_CELL_COUNT; ++i)
	{
		for (int j = 0; j < FIELD_SIDE_CELL_COUNT; ++j)
		{
			m_field[i][j] = m_calculatingField[i][j];
		}
	}
}

glm::vec3 Core::FieldReactionSystem::GetPositionFromFieldIndex(int xIndex, int zIndex, float yPos)
{
	float cellHalfSize = FIELD_CELL_SIDE_SIZE * 0.5f;
	float fieldStart = -FIELD_SIDE_LENGTH * 0.5f - cellHalfSize;

	return GFXVec3(fieldStart + FIELD_CELL_SIDE_SIZE * xIndex, 
				   yPos, 
				   fieldStart + FIELD_CELL_SIDE_SIZE * zIndex);
}

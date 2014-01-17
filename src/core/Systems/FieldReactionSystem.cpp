#include "FieldReactionSystem.hpp"
#include "World.hpp"
#include <logger/Logger.hpp>

#define frsChargeCurve Core::FieldReactionSystem::ChargeCurve

const float Core::FieldReactionSystem::STAY_LIMIT = 0.1f;

const float Core::FieldReactionSystem::FIELD_CELL_SIDE_SIZE = FIELD_SIDE_LENGTH / static_cast<float>(FIELD_SIDE_CELL_COUNT);
const frsChargeCurve Core::FieldReactionSystem::CURVE[1][2] =
{
	{{1.0f, 15.0f, 2.0f}, {-100.0f, 15.0f, 1.0f} }
};

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

			float highestSum = GetEffectOnAgentAt(*it, wpc, ac->rioter.groupID);
			float staySum = highestSum;
			
			for (int i = -1; i < 2; ++i) // 3.
			{
				for (int j = -1; j < 2; ++j) // 4.
				{
					if (i == 0 && j == 0)
						continue;

                    WorldPositionComponent wpct(wpc->position[0] + i, wpc->position[1],
						wpc->position[2] + j);

					float chargeSum = GetEffectOnAgentAt(*it, &wpct, ac->rioter.groupID);

					if (chargeSum > highestSum)
					{
						highestSum = chargeSum;
						//bestIndex = glm::vec2(i, j);
						bestIndex.x = i;
						bestIndex.y = j;
					}
				}
			}
			
			#define FF_VS_PF_FACTOR 1.0f
			glm::vec3 pfVector;

			if (highestSum - staySum > STAY_LIMIT)
			{
				if (bestIndex.x == 0.0f || bestIndex.y == 0.0f)
					pfVector = glm::vec3( bestIndex.x, 0.0f, bestIndex.y);
				else
				{
					//float invLength = 1.0f / std::sqrt(bestIndex.x * bestIndex.x + bestIndex.y * bestIndex.y);
					//MovementComponent::InterpolateToDirection(mc->direction, bestIndex.x * invLength, 0.0f, bestIndex.y * invLength);
					pfVector = glm::normalize( glm::vec3( bestIndex.x, 0, bestIndex.y ) );
				}

				//*reinterpret_cast<glm::vec3*>(mc->direction) = glm::normalize( 
				//	*reinterpret_cast<glm::vec3*>(mc->direction) +
				//	pfVector * FF_VS_PF_FACTOR );

				glm::vec3 newDir = glm::vec3(mc->newDirection[0] + pfVector.x * FF_VS_PF_FACTOR,
											 mc->newDirection[1] + pfVector.y * FF_VS_PF_FACTOR,
											 mc->newDirection[2] + pfVector.z * FF_VS_PF_FACTOR);

				if ((std::abs(newDir.x) + std::abs(newDir.y) + std::abs(newDir.z)) > 0.1f)
					glm::normalize(newDir);
					
				MovementComponent::SetDirection(mc, mc->newDirection[0], mc->newDirection[1], mc->newDirection[2]);
			}
			else
			{
				pfVector = glm::vec3(0.0f);
				//MovementComponent::InterpolateToDirection(mc->direction, 0.0f, 0.0f, 0.0f);
			}

			glm::vec3 position = *reinterpret_cast<glm::vec3*>(wpc->position);
			GFX::Debug::DrawLine(position, position + *reinterpret_cast<glm::vec3*>(mc->newDirection), GFXColor(1.0f, 0.0f, 0.0f, 1.0f), false);
		}
	}
}

float Core::FieldReactionSystem::GetEffectOnAgentAt(const Entity queryAgent, WorldPositionComponent* queryPosition, int groupID)
{
	float positiveSum = 0.0f;
	float negativeSum = 0.0f;
	float currentSum = 0.0f;

	for (std::vector<Entity>::iterator it2 = m_entities.begin(); it2 != m_entities.end(); it2++) // 5.
	{
		if (queryAgent != (*it2)) // 6.
		{
			/*int matchID = -1;
			if (groupID >= 0)
			{
				Core::AttributeComponent* ac = WGETC<AttributeComponent>(*it2);
				matchID = ac->rioter.groupID;
			}

			if (matchID != groupID)
				continue;*/

			WorldPositionComponent* wpc = WGETC<WorldPositionComponent>(*it2);
			WorldPositionComponent distVec = WorldPositionComponent(wpc->position[0] - queryPosition->position[0],
																	wpc->position[1] - queryPosition->position[1],
																	wpc->position[2] - queryPosition->position[2]);

			float distanceSqr = (distVec.position[0] * distVec.position[0]) +
				(distVec.position[2] * distVec.position[2]);

			/*float dx = wpc->position[0] - queryPosition->position[0];
			float dz = wpc->position[2] - queryPosition->position[2];
			float distanceSqr = dx * dx + dz * dz;*/

			int matchID = -1;
			if (groupID >= 0)
			{
				Core::AttributeComponent* ac = WGETC<AttributeComponent>(*it2);
				matchID = ac->rioter.groupID;
			}

			if (distanceSqr >= CURVE[0][0].repelRadius && matchID != groupID)
				continue;

			currentSum += GetAgentsChargeAt(*it2, distanceSqr); // 7.

			if (currentSum >= 0)
				positiveSum += currentSum;
			else
				negativeSum += currentSum;
		}
	}

	return positiveSum + negativeSum;
}

float Core::FieldReactionSystem::GetAgentsChargeAt(Entity chargedAgent, float distSqrd)
{
	Core::UnitTypeComponent* utc = WGETC<UnitTypeComponent>(chargedAgent);
	int indexFromType = static_cast<int>(utc->type);

	if (distSqrd > 0.001f && distSqrd < CURVE[0][indexFromType].repelRadius)
		return -100 + distSqrd * (CURVE[0][indexFromType].repelRadius / 100);
	else if (distSqrd <= CURVE[0][indexFromType].cutoff /*&& groupID == matchGroupID*/)
		return CURVE[0][indexFromType].charge - distSqrd * CURVE[0][indexFromType].decline;
	else
		return 0.0f;
}

// Original functions
/*float Core::FieldReactionSystem::GetEffectOnAgentAt(const Entity queryAgent, WorldPositionComponent* queryPosition, int groupID)
{
	float positiveSum = 0.0f;
	float negativeSum = 0.0f;
	float currentSum = 0.0f;

	for (std::vector<Entity>::iterator it2 = m_entities.begin(); it2 != m_entities.end(); it2++) // 5.
	{
		if (queryAgent != (*it2)) // 6.
		{
			currentSum += GetAgentsChargeAt(*it2, WorldPositionComponent::GetVec3(*queryPosition), groupID); // 7.

			if (currentSum >= 0)
				positiveSum += currentSum;
			else
				negativeSum += currentSum;
		}
	}

	return positiveSum + negativeSum;
}*/

/*float Core::FieldReactionSystem::GetAgentsChargeAt(Entity chargedAgent, glm::vec3 queryPosition, int matchGroupID)
{
	Core::UnitTypeComponent* utc = WGETC<UnitTypeComponent>(chargedAgent);
	int indexFromType = static_cast<int>(utc->type);

	WorldPositionComponent* wpc = WGETC<WorldPositionComponent>(chargedAgent);
	int groupID = -1;
	if (matchGroupID >= 0)
	{
		Core::AttributeComponent* ac = WGETC<AttributeComponent>(chargedAgent);
		groupID = ac->rioter.groupID;
	}

	glm::vec3 distVec = (WorldPositionComponent::GetVec3(*wpc) - queryPosition);
	float distanceSqr = (distVec.x * distVec.x) + (distVec.z * distVec.z);
	
	if (distanceSqr > 0.001f && distanceSqr < CURVE[0][indexFromType].repelRadius)
		return -100 + distanceSqr * (CURVE[0][indexFromType].repelRadius / 100);
	else if (distanceSqr <= CURVE[0][indexFromType].cutoff && groupID == matchGroupID)
		return CURVE[0][indexFromType].charge - distanceSqr * CURVE[0][indexFromType].decline;
	else
		return 0.0f;
}*/

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

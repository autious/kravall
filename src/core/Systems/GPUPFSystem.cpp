#include "GPUPFSystem.hpp"
#include "World.hpp"
#include "logger/Logger.hpp"
#include <gfx/GFXInterface.hpp>
#include <math.h>

namespace Core
{
	GPUPFSystem::GPUPFSystem() : BaseSystem(EntityHandler::GenerateAspect<WorldPositionComponent, MovementComponent,
		UnitTypeComponent, AttributeComponent>(), 0ULL)
	{
		m_foundShader = false;
		m_readBack = false;

		m_inData = new DataIN[MAXIMUM_ENTITIES];
		m_outData = new DataOUT[MAXIMUM_ENTITIES];
	}


	void GPUPFSystem::Update(float delta)
	{

		if (!m_foundShader)
		{
			GFX::Content::GetShaderId(m_shaderID, "GPUPFCompute");
			
			glUseProgram(m_shaderID);
			
			glGenBuffers(1, &m_inDataBuffer);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_inDataBuffer);
			glBufferData(GL_SHADER_STORAGE_BUFFER, MAXIMUM_ENTITIES * sizeof(DataIN), NULL, GL_STREAM_COPY);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_inDataBuffer);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
			
			glGenBuffers(1, &m_outDataBuffer);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_outDataBuffer);
			glBufferData(GL_SHADER_STORAGE_BUFFER, MAXIMUM_ENTITIES * sizeof(DataOUT), NULL, GL_STREAM_READ);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_outDataBuffer);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

			m_entityCount = glGetUniformLocation(m_shaderID, "gEntityCount");

			glUseProgram(0);
			
			m_foundShader = true;
		}
		else
		{
			if (m_entities.size() > 0)
			{
				glUseProgram(m_shaderID);

				//if (!m_readBack)
				{
					//Set data in shader
					glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_inDataBuffer);

					DataIN* in = (DataIN*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, m_entities.size() * sizeof(DataIN),
						GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
					int i = 0;
					for (std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++)
					{
						WorldPositionComponent* wpc = WGETC<WorldPositionComponent>(*it);
						MovementComponent* mc = WGETC<MovementComponent>(*it);
						UnitTypeComponent* utc = WGETC<UnitTypeComponent>(*it);
						AttributeComponent* ac = WGETC<AttributeComponent>(*it);

						in[i].position_unitType = glm::vec4(wpc->position[0], wpc->position[1], wpc->position[2], utc->type);
						in[i].newDirection_empty = glm::vec4(mc->newDirection[0], mc->newDirection[1], mc->newDirection[2], mc->speed);

						if (utc->type == UnitType::Rioter)
						{
							in[i].health_stamina_morale_stancealignment = glm::vec4(ac->health, ac->stamina, ac->morale, ac->rioter.stance);
							in[i].groupSquadID_defenseRage_mobilityPressure_empty = glm::vec4(ac->rioter.groupID, ac->rioter.rage, ac->rioter.pressure, 0);
						}
						else if (utc->type == UnitType::Police)
						{
							in[i].health_stamina_morale_stancealignment = glm::vec4(ac->health, ac->stamina, ac->morale, ac->rioter.stance); //Attacking state should override stance here
							in[i].groupSquadID_defenseRage_mobilityPressure_empty = glm::vec4(ac->rioter.groupID, 1, 1, 0);
						}

						i++;
					}
					glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
					glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
					glInvalidateBufferData(m_inDataBuffer);

					//Set entity count
					glUniform1ui(m_entityCount, i);

					//Run the shader
					glDispatchCompute(GLuint(1), GLuint(1), GLuint(1));
					//}
					//else
					//{
					glMemoryBarrier(GL_ALL_BARRIER_BITS);

					//Get data from shader
					
					glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_outDataBuffer);
					DataOUT* out = (DataOUT*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, m_entities.size() * sizeof(DataOUT), GL_MAP_READ_BIT);

					i = 0;
					for (std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++)
					{
						MovementComponent* mc = WGETC<MovementComponent>(*it);
						AttributeComponent* ac = WGETC<AttributeComponent>(*it);
						UnitTypeComponent* utc = WGETC<UnitTypeComponent>(*it);

						mc->newDirection[0] = out[i].newDirection_speed.x;
						mc->newDirection[1] = out[i].newDirection_speed.y;
						mc->newDirection[2] = out[i].newDirection_speed.z;
						mc->speed = out[i].newDirection_speed.w;

						ac->morale = out[i].morale_rage_pressure_empty.x;

						if (utc->type == UnitType::Rioter)
						{
							ac->rioter.rage = out[i].morale_rage_pressure_empty.y;
							ac->rioter.pressure = out[i].morale_rage_pressure_empty.z;

							ac->rioter.rage = glm::clamp(ac->rioter.rage, 0.0f, 100.0f);
							ac->rioter.pressure = glm::clamp(ac->rioter.pressure, 0.0f, 100.0f);
						}

						ac->morale = glm::clamp(ac->morale, 0.0f, 2.0f);

						i++;
					}
					glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
					glInvalidateBufferData(m_outDataBuffer);
					glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

				}

				m_readBack = !m_readBack;

				//Reset program
				glUseProgram(0);
			}
		}

	}
}
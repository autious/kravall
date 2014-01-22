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
			glUseProgram(m_shaderID);

			//if (!m_readBack)
			{
				//Set data in shader
				glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_inDataBuffer);
			
				DataIN* in = (DataIN*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, MAXIMUM_ENTITIES * sizeof(DataIN),
					GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
				int i = 0;
				for (std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++)
				{
					WorldPositionComponent* wpc = WGETC<WorldPositionComponent>(*it);
					MovementComponent* mc = WGETC<MovementComponent>(*it);
					UnitTypeComponent* utc = WGETC<UnitTypeComponent>(*it);
					AttributeComponent* ac = WGETC<AttributeComponent>(*it);
			
					in[i].position_unitType = glm::vec4(wpc->position[0], wpc->position[1], wpc->position[2], utc->type);
					in[i].direction_speed = glm::vec4(mc->newDirection[0], mc->newDirection[1], mc->newDirection[2], mc->speed);
					in[i].goal_maxSpeed = glm::vec4(mc->goal[0], mc->goal[1], mc->goal[2], mc->maxSpeed);
					in[i].health_stamina_morale_stancealignment = glm::vec4(ac->health, ac->stamina, ac->morale, ac->rioter.alignment);
					in[i].groupSquadID_defenseRage_mobilityPressure_empty = glm::vec4(ac->rioter.groupID, ac->rioter.rage, ac->rioter.pressure, 0);
					i++;
				}
				glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
				glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
				glInvalidateBufferData(m_inDataBuffer);
			
				//Set entity count
				glUniform1ui(m_entityCount, i);
		
				//Run the shader
				glDispatchCompute(GLuint(1), GLuint(1), GLuint(1));
			}
			//else
			{
				glMemoryBarrier(GL_ALL_BARRIER_BITS);
			
				//Get data from shader
				glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_outDataBuffer);
				DataOUT* out = (DataOUT*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, MAXIMUM_ENTITIES * sizeof(DataOUT), GL_MAP_READ_BIT);

				int i = 0;
				for (std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++)
				{
					MovementComponent* mc = WGETC<MovementComponent>(*it);

					mc->newDirection[0] = out[i].direction_speed.x;
					mc->newDirection[1] = out[i].direction_speed.y;
					mc->newDirection[2] = out[i].direction_speed.z;
					mc->speed = out[i].direction_speed.w;
				
					mc->goal[0] = out[i].goal_maxSpeed.x;
					mc->goal[1] = out[i].goal_maxSpeed.y;
					mc->goal[2] = out[i].goal_maxSpeed.z;
					mc->maxSpeed = out[i].goal_maxSpeed.w;
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
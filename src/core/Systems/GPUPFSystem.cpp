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


		//Init all curves to no effect
		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < 13; j++)
			{
				m_moodCurves[i][j].range_mo_pre_rage = glm::vec4(0);
			}
		}

		//barrier();
		/************************************************************************************/
		//normal rioter vs passive police
		m_moodCurves[0][0].range_mo_pre_rage = glm::vec4(5, -0.001f, 0.01f, 0.01f);

		//normal rioter vs defensive police
		m_moodCurves[0][1].range_mo_pre_rage = glm::vec4(5, 0.0f, 0.0f, 0.01f);

		//normal rioter vs agggresive police
		m_moodCurves[0][2].range_mo_pre_rage = glm::vec4(15, 0.0f, 0.5f, 0.1f);

		//normal rioter vs Attacking police
		m_moodCurves[0][3].range_mo_pre_rage = glm::vec4(15, -0.5f, 5000.0f, 5000.0f);//Attacking state doesn't actually exist yet, fix this

		//normal rioter vs normal rioter
		//No effect

		//normal rioter vs rioter agitated
		m_moodCurves[0][7].range_mo_pre_rage = glm::vec4(3, 0.0f, 0.0001f, 0.00001f);

		//normal rioter vs rioter attacking
		m_moodCurves[0][8].range_mo_pre_rage = glm::vec4(5, 0.0f, 0.0002f, 0.0002f);

		//normal rioter vs rioter retreat
		m_moodCurves[0][9].range_mo_pre_rage = glm::vec4(3, -0.0001f, 0.0001f, 0.0001f);

		//normal rioter vs rioter civilian
		//No effect

		/************************************************************************************/
		//agitated rioter vs passive police
		m_moodCurves[1][0].range_mo_pre_rage = glm::vec4(5, 0.0f, 0.01f, 0.1f);

		//agitated rioter vs defensive police
		m_moodCurves[1][1].range_mo_pre_rage = glm::vec4(5, 0.0f, 0.01f, 0.1f);

		//agitated rioter vs agggresive police
		m_moodCurves[1][2].range_mo_pre_rage = glm::vec4(15, 0.0f, 0.5f, 0.1f);

		//agitated rioter vs normal rioter
		m_moodCurves[1][6].range_mo_pre_rage = glm::vec4(15, 0.0f, 0.5f, 0.1f);

		//agitated rioter vs agitated rioter
		m_moodCurves[1][7].range_mo_pre_rage = glm::vec4(15, 0.0f, 0.005f, 0.1f);

		//agitated rioter vs rioter attacking
		m_moodCurves[1][8].range_mo_pre_rage = glm::vec4(5, 0.0f, 0.0001f, 0.0002f);

		//agitated rioter vs rioter retreat
		m_moodCurves[1][9].range_mo_pre_rage = glm::vec4(3, -0.0001f, 0.0001f, 0.0002f);

		//agitated rioter vs rioter civilian
		//No effect

		//barrier();
		/************************************************************************************/

		//attacking rioter vs passive police
		m_moodCurves[2][0].range_mo_pre_rage = glm::vec4(5, 0.0f, 0.01f, 0.1f);

		//attacking rioter vs defensive police
		m_moodCurves[2][1].range_mo_pre_rage = glm::vec4(5, 0.0, 0.0, 0.1f);

		//attacking rioter vs agggresive police
		m_moodCurves[2][2].range_mo_pre_rage = glm::vec4(15, 0.0f, 0.5f, 0.1f);

		//attacking rioter vs normal rioter
		//No effect

		//attacking rioter vs agitated rioter
		//No effect

		//attacking rioter vs rioter attacking
		m_moodCurves[2][8].range_mo_pre_rage = glm::vec4(5, 0.0001f, 0.0001f, 0.0002f);

		//attacking rioter vs rioter retreat
		m_moodCurves[2][9].range_mo_pre_rage = glm::vec4(3, -0.0001f, 0.0001f, 0.0f);

		//agitated rioter vs rioter civilian
		//No effect

		//barrier();
		/************************************************************************************/

		//retreating rioter
		//No effects

		/************************************************************************************/

		//civilian rioter vs passive police
		//No effect

		//civilian rioter vs defensive police
		//No effect

		// civilian rioter vs agggresive police
		m_moodCurves[4][2].range_mo_pre_rage = glm::vec4(15, 0.0f, 0.5f, 0.1f);

		//civilian rioter vs normal rioter
		//No effect

		//civilian rioter vs agitated rioter
		m_moodCurves[4][7].range_mo_pre_rage = glm::vec4(5, 0.0f, 0.005f, 0.001f);

		//civilian rioter vs rioter attacking
		m_moodCurves[4][8].range_mo_pre_rage = glm::vec4(5, 0.0f, 0.001f, 0.002f);

		//civilian rioter vs rioter retreat
		m_moodCurves[4][9].range_mo_pre_rage = glm::vec4(3, -0.0001f, 0.0f, 0.0f);

		//civilian rioter vs rioter civilian
		//No effect

		/************************************************************************************/

		//police vs rioter neut 
		//No effect

		//police vs rioter agi
		m_moodCurves[5][7].range_mo_pre_rage = glm::vec4(1, -0.0001f, 0.0f, 0.0f);

		//police vs rioter att
		m_moodCurves[5][8].range_mo_pre_rage = glm::vec4(15, -0.001f, 0.0f, 0.0f);

		//police vs rioter ret
		m_moodCurves[5][9].range_mo_pre_rage = glm::vec4(15, -0.001f, 0.0f, 0.0f);

		//pol vs rioter civ
		//No effect
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
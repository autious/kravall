#include "GPUPFSystem.hpp"
#include "World.hpp"
#include "logger/Logger.hpp"
#include <gfx/GFXInterface.hpp>
#include <math.h>

#include <SystemDef.hpp>

//#define DRAW_PF_DIRECTION
#ifdef DRAW_PF_DIRECTION
#define DEBUG_DRAW_PF_DIRECTION( x ) x
#else
#define DEBUG_DRAW_PF_DIRECTION( x ) ;
#endif

static int frameCount = 0;
namespace Core
{
	GPUPFSystem::GPUPFSystem() : BaseSystem(EntityHandler::GenerateAspect<WorldPositionComponent, MovementComponent,
		UnitTypeComponent, AttributeComponent, FlowfieldComponent, MovementDataComponent>(), 0ULL)
	{
		m_foundShader = false;
		m_readBack = false;
		m_dispatch = true;

		m_inData = new DataIN[MAXIMUM_ENTITIES];
		m_outData = new DataOUT[MAXIMUM_ENTITIES];
		m_prevSize = 0;
	}


	void GPUPFSystem::Update(float delta)
	{
		GLint result = GL_UNSIGNALED;
		frameCount++;
		if (!m_foundShader)
		{
			GFX::Content::GetShaderId(m_shaderID, "GPUPFCompute");

			glUseProgram(m_shaderID);

			glGenBuffers(1, &m_inDataBuffer);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_inDataBuffer);
			glBufferData(GL_SHADER_STORAGE_BUFFER, MAXIMUM_ENTITIES * sizeof(DataIN), NULL, GL_STATIC_COPY);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_inDataBuffer);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

			glGenBuffers(1, &m_outDataBuffer);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_outDataBuffer);
			glBufferData(GL_SHADER_STORAGE_BUFFER, MAXIMUM_ENTITIES * sizeof(DataOUT), NULL, GL_DYNAMIC_READ);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_outDataBuffer);
			glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

			m_entityCount = glGetUniformLocation(m_shaderID, "gEntityCount");

			glUseProgram(0);

			m_foundShader = true;
		}
		else
		{

			if (m_entities.size() == 0 || m_entities.size() != m_prevSize)
			{
				m_dispatch = true;
				m_readBack = false;
				m_prevSize = m_entities.size();
				return;
			}

			m_prevSize = m_entities.size();

			//We've read back data, dispatch new data to GPU
			if (m_dispatch)
			{
				glUseProgram(m_shaderID);

				//Delete the old fence
				glDeleteSync(m_sync);

				//Set data in shader
				glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_inDataBuffer);
				glInvalidateBufferData(m_inDataBuffer);
				DataIN* in = (DataIN*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, MAXIMUM_ENTITIES * sizeof(DataIN),
					GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_UNSYNCHRONIZED_BIT);

				int i = 0;
				for (std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++)
				{
					if (i > MAXIMUM_ENTITIES)
						break;


					WorldPositionComponent* wpc = WGETC<WorldPositionComponent>(*it);
					MovementComponent* mc = WGETC<MovementComponent>(*it);
					UnitTypeComponent* utc = WGETC<UnitTypeComponent>(*it);
					AttributeComponent* ac = WGETC<AttributeComponent>(*it);

					FlowfieldComponent* ffc = WGETC<FlowfieldComponent>(*it);

					// did not know glsl had bitwise operations, derp
					// pack rough vector to closest edge of the navmesh and distance to it into one float
					//float navMeshValue = (ffc->wallDirX * 1000);
					//navMeshValue += navMeshValue < 0 ? ffc->wallDirZ == 0 ? -200.0f : ffc->wallDirZ < 0 ? -ffc->wallDirZ * 100.0f : 
					//	-ffc->wallDirZ * 300.0f : ffc->wallDirZ == 0 ? 200.0f : ffc->wallDirZ * 300.0f;
					//navMeshValue += navMeshValue < 0 ? -ffc->distance * 1.000f : ffc->distance * 1.000f;

					unsigned int navMeshValue = ((ffc->wallDirX + 1) << 30) | ((ffc->wallDirZ + 1) << 28) | (unsigned int)(ffc->distance * 10000);

					// how to unpack the navMesh data...
					//int x = (navMeshVal2 >> 30) - 1;
					//int y = ((navMeshVal2 >> 28) & 0x3) - 1;
					//float distance = (navMeshVal2 & 0xFFFFFFF) * 0.0001;


					int team = 0; // UnitType::Object will default to zero here.
					in[i].position_unitType = glm::vec4(wpc->position[0], wpc->position[1], wpc->position[2], utc->type);
					
					if (utc->type == UnitType::Rioter)
					{
						in[i].health_stamina_morale_stancealignment = glm::vec4(ac->health, ac->stamina, ac->morale, ac->rioter.stance);
						in[i].groupSquadID_defenseRage_mobilityPressure_navMeshIndexAndDistance = glm::vec4(ac->rioter.groupID, ac->rioter.rage, ac->rioter.pressure, navMeshValue);
						team = Core::GetNavigationMesh()->flowfields[ ac->rioter.groupID ].team;
					}
					else if (utc->type == UnitType::Police)
					{
						in[i].health_stamina_morale_stancealignment = glm::vec4(ac->health, ac->stamina, ac->morale, ac->rioter.stance); //Attacking state should override stance here
						in[i].groupSquadID_defenseRage_mobilityPressure_navMeshIndexAndDistance = glm::vec4(ac->rioter.groupID, 1, 1, navMeshValue);

						// police will always be seen as an enemy group in the pf calculation
						team = 0; //Core::GetNavigationMesh()->flowfields[ ac->police.squadID ].team;
					}
					else if (utc->type == UnitType::Object)
					{
						in[i].health_stamina_morale_stancealignment = glm::vec4(100.0f, 100.0f, 2.0f, ac->pfObject.pfObjectType); //Attacking state should override stance here
						in[i].groupSquadID_defenseRage_mobilityPressure_navMeshIndexAndDistance = glm::vec4(0, 1, 1, 0);
					}

					in[i].newDirection_team = glm::vec4(mc->newDirection[0], mc->newDirection[1], mc->newDirection[2], team);
					i++;					
				}
				glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
				//glInvalidateBufferData(m_inDataBuffer);

				//unbind the buffer
				glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

				//Set entity count
				glUniform1ui(m_entityCount, i);

				//Run the shader
				glDispatchCompute(GLuint(1), GLuint(1), GLuint(1));

				//Create a new fence that will finish after the dispatch
				m_sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

				//No dispatch or readback until dispatch is complete
				m_dispatch = false;
				m_readBack = false;

				//Reset program
				glUseProgram(0);

				//glFinish();

			}

			//Get the status of our sync fence
			glGetSynciv(m_sync, GL_SYNC_STATUS, sizeof(GLint), NULL, &result);

			//Sync is done, read back data from GPU
			if (result == GL_SIGNALED)
				m_readBack = true;


			//Dispatch is not complete, use the old PF vector to calculate our position
			if (m_readBack == false)
			{
				int i = 0;
				for (std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++)
				{
					if (i > MAXIMUM_ENTITIES)
						break;


					MovementComponent* mc = WGETC<MovementComponent>(*it);
					MovementDataComponent *mdc = WGETC<MovementDataComponent>( *it );
					AttributeComponent* ac = WGETC<AttributeComponent>(*it);
					UnitTypeComponent* utc = WGETC<UnitTypeComponent>(*it);

					#define PF_FACTOR 0.5f
					#define FF_FACTOR 0.5f

					float dirDot = glm::dot(glm::vec2(mc->newDirection[0], mc->newDirection[2]), glm::vec2(mdc->oldPFDir[0], mdc->oldPFDir[1]));

					glm::vec3 FFDirection = glm::vec3(mc->newDirection[0], mc->newDirection[1], mc->newDirection[2]);
					FFDirection = FFDirection * float(!(dirDot < 0.0f));

					glm::vec3 newDir = glm::vec3(
						FFDirection.x * FF_FACTOR + mdc->oldPFDir[0] * PF_FACTOR,
						0 * FF_FACTOR + 0 * PF_FACTOR,
						FFDirection.z * FF_FACTOR + mdc->oldPFDir[1] * PF_FACTOR);

					if (glm::length(newDir) > 0.1f)
						newDir = glm::normalize(newDir);

					if( utc->type == Core::UnitType::Rioter )
					{
						mc->newDirection[0] = newDir.x;
						mc->newDirection[1] = newDir.y;
						mc->newDirection[2] = newDir.z;
					}

					i++;
				}
			}


			//Dispatch is done, read data from GPU and use it for direction
			if (m_readBack)
			{
				glUseProgram(m_shaderID);


				//Get data from shader
				glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_outDataBuffer);
				DataOUT* out = (DataOUT*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, MAXIMUM_ENTITIES * sizeof(DataOUT), GL_MAP_READ_BIT);

				int i = 0;
				for (std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++)
				{
					if (i > MAXIMUM_ENTITIES)
						break;

					MovementComponent* mc = WGETC<MovementComponent>(*it);
					MovementDataComponent *mdc = WGETC<MovementDataComponent>( *it );
					AttributeComponent* ac = WGETC<AttributeComponent>(*it);
					UnitTypeComponent* utc = WGETC<UnitTypeComponent>(*it);

					#define PF_FACTOR 0.5f
					#define FF_FACTOR 0.5f

					mdc->oldPFDir[0] = out[i].newDirection_speed.x;
					mdc->oldPFDir[1] = out[i].newDirection_speed.z;

					float dirDot = glm::dot(glm::vec2(mc->newDirection[0], mc->newDirection[2]), glm::vec2(out[i].newDirection_speed.x, out[i].newDirection_speed.z));

					glm::vec3 FFDirection = glm::vec3(mc->newDirection[0], mc->newDirection[1], mc->newDirection[2]);
					FFDirection = FFDirection * float(!(dirDot < 0.0f));

					glm::vec3 newDir = glm::vec3(
						FFDirection.x * FF_FACTOR + out[i].newDirection_speed.x * PF_FACTOR,
						0 * FF_FACTOR + 0 * PF_FACTOR,
						FFDirection.z * FF_FACTOR + out[i].newDirection_speed.z * PF_FACTOR);

					if (glm::length(newDir) > 0.1f)
						newDir = glm::normalize(newDir);

					ac->morale = out[i].morale_rage_pressure_empty.x;

					if (utc->type == UnitType::Rioter)
					{
						mc->newDirection[0] = newDir.x;
						mc->newDirection[1] = newDir.y;
						mc->newDirection[2] = newDir.z;

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

				m_dispatch = true;
				m_readBack = false;

				//Reset program
				glUseProgram(0);
			}

			DEBUG_DRAW_PF_DIRECTION( 
				for (std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++)
				{
					MovementDataComponent *mdc = WGETC<MovementDataComponent>( *it );
					WorldPositionComponent* wpc = WGETC<WorldPositionComponent>(*it);					

					glm::vec3 pos = glm::vec3( wpc->position[0], wpc->position[1], wpc->position[2] );
					glm::vec3 dir = glm::vec3( mdc->oldPFDir[0], 0.0f, mdc->oldPFDir[1] );

					GFX::Debug::DrawLine( pos, pos + dir, GFXColor( 1, 1, 0, 1 ), false );
				} );


		}

	}
}
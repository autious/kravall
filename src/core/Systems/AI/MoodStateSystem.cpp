#include "MoodStateSystem.hpp"
#include <gfx/GFXInterface.hpp>
#include <utility/Colors.hpp>
#include <World.hpp>
#include <GameUtility/GameData.hpp>

#define SHOW_MOOD_STATE_DEBUG 
#ifdef SHOW_MOOD_STATE_DEBUG
	#define DRAW_DEBUG( x ) x
#else
	#define DRAW_DEBUG( x ) ;
#endif

namespace Core
{

	MoodStateSystem::MoodStateSystem() :BaseSystem(EntityHandler::GenerateAspect<WorldPositionComponent, AttributeComponent, UnitTypeComponent, FlowfieldComponent>(), 0ULL)
	{}

	void MoodStateSystem::Update(float delta)
	{
		for (std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++)
		{
			UnitTypeComponent* utc = WGETC<UnitTypeComponent>(*it);

			if (utc->type == UnitType::Rioter)
			{
				UpdateRioter(delta, &*it);
			}
			else if (utc->type == UnitType::Police)
			{
				UpdatePolice(delta, &*it);
			}
		}

	}

	void MoodStateSystem::UpdatePolice(float dt, Entity* it)
	{
		AttributeComponent* atc = WGETC<AttributeComponent>(*it);
		float morale = atc->morale;
		float health = atc->health;
		float stamina = atc->stamina;

		PoliceState state = atc->police.state;
		PoliceStance stance = atc->police.stance;

		float routFactor = morale * (health + stamina) / 4.0f;
		
		switch (state)
		{
			case PoliceState::PS_Normal:
			{
				 std::cout << state << std::endl;
				switch (stance)
				{
					//case PoliceStance::Passive:
					//	break;
					//
					//case PoliceStance::Defensive:
					//	break;
					//
					//case PoliceStance::Aggressive:
					//	break;
					default:
						if (routFactor < 20)
							atc->police.state = PoliceState::PS_Routing;
						break;
				}
			}
			break;

			case PoliceState::PS_Attacking:
			{
				switch (stance)
				{
					//case PoliceStance::Passive:
					//	break;
					//
					//case PoliceStance::Defensive:
					//	break;
					//
					//case PoliceStance::Aggressive:
					//	break;
					default:
						if (routFactor < 20)
							atc->police.state = PoliceState::PS_Routing;
						break;
				}
			}
			break;

			case PoliceState::PS_Fleeing:
			{
				switch (stance)
				{
					//case PoliceStance::Passive:
					//	break;
					//
					//case PoliceStance::Defensive:
					//	break;
					//
					//case PoliceStance::Aggressive:
					//	break;

				}
			}
			break;

			case PoliceState::PS_Routing:
			{
				switch (stance)
				{
					//case PoliceStance::Passive:
					//	break;
					//
					//case PoliceStance::Defensive:
					//	break;
					//
					//case PoliceStance::Aggressive:
					//	break;
				}
			}
			break;
		}

		

	}

	void MoodStateSystem::UpdateRioter(float dt, Entity* it)
	{
		AttributeComponent* atc = WGETC<AttributeComponent>(*it);
		float morale = atc->morale;
		float rage = atc->rioter.rage;
		float pressure = atc->rioter.pressure;
		float health = atc->health;
		float stamina = atc->stamina;
		RioterStance state = atc->rioter.stance;
		RioterAlignment alignment = atc->rioter.alignment;

		//State change factors, based upon what is found in GDD
		//Upshift and downshift
		//ranges from 0 to 400
		float stateChangeFactor = morale * (rage + pressure) / 4.0f;
		//ranges from 0 to 400
		float routFactor = morale * (health + stamina) / 4.0f;

		//Normal, Agitated, Attacking, Retreating, Civilian

		//State chain
		//Downshift/upshift only or freeform state changes?
		//Civilian -> Normal -> Agitated -> Attacking
		//All states can go into a retreating state

		switch (state)
		{
			case RioterStance::Normal:
			{
				switch (alignment)
				{
					case RioterAlignment::Anarchist:
					{
						if (routFactor < 20) 	//should be set to routing
						{
							atc->rioter.stance = RioterStance::Retreating;
						}
						else if (stateChangeFactor > 30) //Should be set to agitated
						{
							atc->rioter.stance = RioterStance::Agitated;
						}
					}
					break;

					case RioterAlignment::Pacifist:
					{
						if (routFactor < 20) 	//should be set to routing
						{
							atc->rioter.stance = RioterStance::Retreating;
						}
						else if (stateChangeFactor > 50) //Should be set to agitated
						{
							atc->rioter.stance = RioterStance::Agitated;
						}
					}
					break;
				}

				DRAW_DEBUG( GFX::Debug::DrawSphere(WorldPositionComponent::GetVec3(*WGETC<WorldPositionComponent>(*it)), 1, Colors::CornflowerBlue, false); )
			}
			break;

			case RioterStance::Agitated:
			{
				switch (alignment)
				{
					case RioterAlignment::Anarchist:
					{
						if (routFactor < 30) 	//should be set to routing
						{
							atc->rioter.stance = RioterStance::Retreating;
						}
						else if (stateChangeFactor > 50) //Should be set to attacking
						{
							atc->rioter.stance = RioterStance::Attacking;
						}
						else if (stateChangeFactor < 20) //Should be set to normal
						{
							atc->rioter.stance = RioterStance::Normal;
						}
					}
					break;

					case RioterAlignment::Pacifist:
					{
						if (routFactor < 20) 	//should be set to routing
						{
							atc->rioter.stance = RioterStance::Retreating;
						}
						else if (stateChangeFactor > 50) //Should be set to attacking
						{
							//atc->rioter.stance = RioterStance::Attacking;
						}
						else if (stateChangeFactor < 20) //Should be set to normal
						{
							atc->rioter.stance = RioterStance::Normal;
						}
					}
					break;
				}
				DRAW_DEBUG( GFX::Debug::DrawSphere(WorldPositionComponent::GetVec3(*WGETC<WorldPositionComponent>(*it)), 1, Colors::Yellow, false); )
			}
			break;

			case RioterStance::Attacking:
			{
				switch (alignment)
				{
				case RioterAlignment::Anarchist:
				{
					if (routFactor < 10) 	//should be set to routing
					{
						atc->rioter.stance = RioterStance::Retreating;
					}
					else if (stateChangeFactor < 35) //Should be set to agitated
					{
						atc->rioter.stance = RioterStance::Agitated;
					}
				}
				break;

				case RioterAlignment::Pacifist:
				{
					if (routFactor < 10) 	//should be set to routing
					{
						atc->rioter.stance = RioterStance::Retreating;
					}
					else if (stateChangeFactor < 50) //Should be set to agitated
					{
						atc->rioter.stance = RioterStance::Agitated;
					}
				}
					break;
				}

				DRAW_DEBUG( GFX::Debug::DrawSphere(WorldPositionComponent::GetVec3(*WGETC<WorldPositionComponent>(*it)), 1, Colors::Red, false); )
			}
			break;

			case RioterStance::Civilian:
			{
				switch (alignment)
				{
					case RioterAlignment::Anarchist:
					{
						if (stateChangeFactor > 10) //Should be set to normal
						{
							atc->rioter.stance = RioterStance::Normal;
						}
					}
					break;

					case RioterAlignment::Pacifist:
					{
						if (stateChangeFactor > 10) //Should be set to normal
						{
							atc->rioter.stance = RioterStance::Normal;
						}
					}
					break;
				}

				DRAW_DEBUG( GFX::Debug::DrawSphere(WorldPositionComponent::GetVec3(*WGETC<WorldPositionComponent>(*it)), 1, Colors::White, false); )
			}
			break;

				//Retreating units should not change state right now
			case RioterStance::Retreating:
			{
				switch (alignment)
				{
					case RioterAlignment::Anarchist:
					{
						int exitGroup = Core::GameData::GetEscapePointGroup( WGETC<FlowfieldComponent>(*it)->node );
						if( exitGroup >= 0 )
							atc->rioter.groupID = exitGroup;
					}
					break;

					case RioterAlignment::Pacifist:
					{
						int exitGroup = Core::GameData::GetEscapePointGroup( WGETC<FlowfieldComponent>(*it)->node );
						if( exitGroup >= 0 )
							atc->rioter.groupID = exitGroup;
					}
					break;
				}
				DRAW_DEBUG( GFX::Debug::DrawSphere(WorldPositionComponent::GetVec3(*WGETC<WorldPositionComponent>(*it)), 1, Colors::DeepPink, false); )
			}
			break;
		}
	}
}

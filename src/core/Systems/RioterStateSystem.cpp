#include "RioterStateSystem.hpp"
#include <gfx/GFXInterface.hpp>
#include <utility/Colors.hpp>
#include <World.hpp>

namespace Core
{

	RioterStateSystem::RioterStateSystem() :BaseSystem(EntityHandler::GenerateAspect<AttributeComponent, UnitTypeComponent>(), 0ULL)
	{}

	void RioterStateSystem::Update(float delta)
	{
		for (std::vector<Entity>::iterator it = m_entities.begin(); it != m_entities.end(); it++)
		{
			UnitTypeComponent* utc = WGETC<UnitTypeComponent>(*it);

			if (utc->type == UnitType::Rioter)
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
				float stateChangeFactor = morale * (rage + pressure);
				//ranges from 0 to 400
				float routFactor = morale * (health + stamina);

				//Normal, Agitated, Attacking, Retreating, Civilian

				//State chain
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
								else if (stateChangeFactor > 50) //Should be set to attacking
								{
									atc->rioter.stance = RioterStance::Attacking;
								}
								else if (stateChangeFactor > 30) //Should be set to agitated
								{
									atc->rioter.stance = RioterStance::Agitated;
								}
							}
							break;

						case RioterAlignment::Pacifist:
							break;
						}

						GFX::Debug::DrawSphere(WorldPositionComponent::GetVec3(*WGETC<WorldPositionComponent>(*it)), 1, Colors::CornflowerBlue, false);
					}
					break;

					case RioterStance::Agitated:

						switch (alignment)
						{
							case RioterAlignment::Anarchist:
								break;

							case RioterAlignment::Pacifist:
								break;
						}
						GFX::Debug::DrawSphere(WorldPositionComponent::GetVec3(*WGETC<WorldPositionComponent>(*it)), 1, Colors::Yellow, false);
					break;

					case RioterStance::Attacking:

						switch (alignment)
						{
						case RioterAlignment::Anarchist:
							break;

						case RioterAlignment::Pacifist:
							break;
						}

						GFX::Debug::DrawSphere(WorldPositionComponent::GetVec3(*WGETC<WorldPositionComponent>(*it)), 1, Colors::Red, false);
					break;

					case RioterStance::Civilian:

						switch (alignment)
						{
						case RioterAlignment::Anarchist:
							break;

						case RioterAlignment::Pacifist:
							break;
						}

						GFX::Debug::DrawSphere(WorldPositionComponent::GetVec3(*WGETC<WorldPositionComponent>(*it)), 1, Colors::White, false);

					break;

					case RioterStance::Retreating:

						switch (alignment)
						{
						case RioterAlignment::Anarchist:
							break;

						case RioterAlignment::Pacifist:
							break;
						}
						GFX::Debug::DrawSphere(WorldPositionComponent::GetVec3(*WGETC<WorldPositionComponent>(*it)), 1, Colors::DeepPink, false);
					break;
				}

			}


			
		}

	}
}

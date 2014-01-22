#include "GameData.hpp"
#include <World.hpp>

#include <logger/Logger.hpp>


struct TempInitGameData
{
	TempInitGameData()
	{
		Core::GameData::m_walking.speedToDesire = 5.8f;
		Core::GameData::m_walking.acceleration = 17.0f;
		Core::GameData::m_walking.deceleration = 17.0f;
	}

} dummyInit;


namespace Core
{
	MovementData GameData::m_walking;
	





}
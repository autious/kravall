#ifndef GAME_UTILITY_GAMEDATA_HPP
#define GAME_UTILITY_GAMEDATA_HPP

#include <Systems/AIDebugSystem.hpp>

struct TempInitGameData; // NOCOMMIT

namespace Core
{

	struct MovementData
	{
		float speedToDesire;
		float acceleration;
		float deceleration;
	};


	class GameData
	{
	private:
		GameData();


	public:
		static const MovementData& GetWalkingSpeed() { return m_walking; }

	private:
		static MovementData m_walking;


		friend TempInitGameData;
	};
}

#endif













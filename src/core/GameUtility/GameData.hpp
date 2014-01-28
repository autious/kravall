#ifndef GAME_UTILITY_GAMEDATA_HPP
#define GAME_UTILITY_GAMEDATA_HPP

#include <Systems/AIDebugSystem.hpp>

struct TempInitGameData; // This should be moved to config || scenario? || gamemode? fix when implementing running etc.

namespace Core
{

	struct MovementData
	{
		float speedToDesire;
		float acceleration;
		float deceleration;
	};


	/*!
		class responsible for holding speed data for different states.
	*/
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













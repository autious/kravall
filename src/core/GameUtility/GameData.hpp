#ifndef GAME_UTILITY_GAMEDATA_HPP
#define GAME_UTILITY_GAMEDATA_HPP

#include <Systems/AIDebugSystem.hpp>

struct TempInitGameData;


namespace Core
{
	enum MovementState
	{
		Walking,
		Sprinting,
		COUNT,
	};

	/*!
		Struct used for storing metadata for moveing entities.
	*/
	struct MovementData
	{
		MovementData()
		{
			speedToDesire = 5.8f;
			acceleration = 17.0f;
			deceleration = 17.0f;
		}

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

		/*!
			Will return the MovementData for the respective state.
		*/
		static const MovementData& GetMovementDataWithState( MovementState state );

	private:

		static MovementData m_movementData[ MovementState::COUNT ];

		friend TempInitGameData;
	};
}

#endif













#ifndef GAME_UTILITY_GAMEDATA_HPP
#define GAME_UTILITY_GAMEDATA_HPP

#include <Components/MovementComponent.hpp>

struct TempInitGameData;


namespace Core
{
	/*!
		Struct used for storing metadata for moveing entities.
	*/
	struct MovementData
	{
		/*!
			These values is the default ones. Use lua to change them.
		*/
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

		/*!
			Will set the MovementData for the respective state. Should not be used carelessly form C.
		*/
		static void SetMovementDataForState( MovementState state, float speedToDesire, float acceleration, float deceleration );

	private:

		static MovementData m_movementData[ MovementState::COUNT ];

		friend TempInitGameData;
	};
}

#endif













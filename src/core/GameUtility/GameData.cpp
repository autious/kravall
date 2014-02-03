#include "GameData.hpp"
#include <World.hpp>

#include <logger/Logger.hpp>


struct TempInitGameData
{
	TempInitGameData()
	{
		for( int i = 0; i < Core::MovementState::COUNT; i++ )
			Core::GameData::m_movementData[ i ] = Core::MovementData();
	}

} dummyInit;


namespace Core
{
	MovementData GameData::m_movementData[ Core::MovementState::COUNT ];
	

	const MovementData& Core::GameData::GetMovementDataWithState( MovementState state )
	{
		return m_movementData[ state ];
	}


	void Core::GameData::SetMovementDataForState( MovementState state, float speedToDesire, float acceleration, float deceleration )
	{
		m_movementData[ state ].speedToDesire = speedToDesire;
		m_movementData[ state ].acceleration = acceleration;
		m_movementData[ state ].deceleration = deceleration;
	}


}
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
	MovementData GameData::m_movementData[ MOVEMENTTYPE_ENUM_SIZE_GOES_HERE ];
	

	const MovementData& Core::GameData::GetMovementDataWithState( MovementState state )
	{
		return m_movementData[ state ];
	}
}
#include "GameData.hpp"
#include <World.hpp>

#include <logger/Logger.hpp>


struct TempInitGameData
{
	TempInitGameData()
	{
		for( int i = 0; i < Core::MovementState::MOVEMENTSTATE_COUNT; i++ )
			Core::GameData::m_movementData[ i ] = Core::MovementData();

		for( int i = 0; i < Core::WeaponType::WEAPONTYPE_COUNT; i++ )
			Core::GameData::m_weaponData[ i ] = Core::WeaponData();
	}

} dummyInit;


namespace Core
{
	MovementData GameData::m_movementData[ Core::MovementState::MOVEMENTSTATE_COUNT ];
	WeaponData GameData::m_weaponData[ Core::WeaponType::WEAPONTYPE_COUNT ];


	const MovementData& Core::GameData::GetMovementDataWithState( MovementState state )
	{
		return m_movementData[ state ];
	}

	const WeaponData& Core::GameData::GetWeaponDataFromWeapon( Core::WeaponType weapon )
	{
		return m_weaponData[ weapon ];
	}


	void Core::GameData::SetMovementDataForState( MovementState state, float speedToDesire, float acceleration, float deceleration )
	{
		m_movementData[ state ].speedToDesire	= speedToDesire;
		m_movementData[ state ].acceleration	= acceleration;
		m_movementData[ state ].deceleration	= deceleration;
	}

	void Core::GameData::SetWeaponDataForWeapon( WeaponType weapon, float range, float damage, float animationDmgDealingtime, int animationID )
	{
		m_weaponData[ weapon ].range					= range;
		m_weaponData[ weapon ].damage					= damage;
		m_weaponData[ weapon ].animationDmgDealingtime	= animationDmgDealingtime;
		m_weaponData[ weapon ].animationID				= animationID;
	}


}
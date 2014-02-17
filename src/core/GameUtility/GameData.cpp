#include "GameData.hpp"
#include <World.hpp>

#include <logger/Logger.hpp>


struct TempInitGameData
{
	TempInitGameData()
	{
		for( int i = 0; i < Core::MovementState::MOVEMENTSTATE_COUNT; i++ )
			Core::GameData::m_movementData[ i ] = Core::MovementData();
	}

} dummyInit;


namespace Core
{
	MovementData GameData::m_movementData[ Core::MovementState::MOVEMENTSTATE_COUNT ];
	std::vector< WeaponData > GameData::m_weaponData;


	const MovementData& Core::GameData::GetMovementDataWithState( MovementState state )
	{
		return m_movementData[ state ];
	}

	const WeaponData& Core::GameData::GetWeaponDataFromWeapon( int weapon )
	{
		assert( weapon >= 0 && weapon < m_weaponData.size() );
		return m_weaponData[ weapon ];
	}


	void Core::GameData::SetMovementDataForState( MovementState state, float speedToDesire, float acceleration, float deceleration, float staminaCostPerSecond )
	{
		m_movementData[ state ].speedToDesire			= speedToDesire;
		m_movementData[ state ].acceleration			= acceleration;
		m_movementData[ state ].deceleration			= deceleration;
		m_movementData[ state ].staminaCostPerSecond	= staminaCostPerSecond;
	}

	int Core::GameData::PushWeaponData( float range, float weaponDamage, float moraleDamage, float moralDamageOnMiss, float staminaCost, float animationDmgDealingtime, std::string animationName )
	{
		m_weaponData.push_back( WeaponData() );

		m_weaponData.back().range					= range;
		m_weaponData.back().weaponDamage			= weaponDamage;
		m_weaponData.back().moraleDamage			= moraleDamage;
		m_weaponData.back().moralDamageOnMiss		= moralDamageOnMiss;
		m_weaponData.back().staminaCost				= staminaCost;
		m_weaponData.back().animationDmgDealingtime	= animationDmgDealingtime;
		m_weaponData.back().animationName			= animationName;

		return m_weaponData.size() - 1;
	}


}
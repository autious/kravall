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
	std::vector< int > GameData::m_escapePointGroups;	

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


	int Core::GameData::RegisterEscapePoint( glm::vec3 point )
	{
		Core::NavigationMesh* instance = Core::GetNavigationMesh();
		if( !instance )
			return -1;

		int escapeGroup = instance->CreateGroup();
		
		if( !instance->CalculateFlowfieldForGroup( point, escapeGroup ) )
			return -1;

		m_escapePointGroups.push_back( escapeGroup );
		return escapeGroup;
	}
	
	int Core::GameData::GetEscapePointGroup( int node )
	{
		if( m_escapePointGroups.size() == 0 )
			return -1;

		Core::NavigationMesh* instance = Core::GetNavigationMesh();
		if( !instance )
			return -1;

		float closest = std::numeric_limits<float>::max();
		int index = -1;
		for( int i = 0; i < m_escapePointGroups.size(); i++ )
		{
			float dist = instance->flowfields[ m_escapePointGroups[i] ].distanceToGoal[node];
			dist < closest ? closest = dist, index = i : index = index ;
		}

		return index;
	}


	void Core::GameData::ClearData()
	{
		// reset movementData
		dummyInit = TempInitGameData();

		// remove weapons
		m_weaponData.clear();

		// clear all stored escape routes
		m_escapePointGroups.clear();
	}




}
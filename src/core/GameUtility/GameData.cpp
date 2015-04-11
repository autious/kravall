#include "GameData.hpp"
#include <World.hpp>

#include <logger/Logger.hpp>



namespace Core
{
	std::vector< WeaponData > GameData::m_weaponData;
	std::vector< int > GameData::m_validEscapePointGroups;	
	std::vector< int > GameData::m_reqisteredEscapePointGroups;

	const WeaponData& Core::GameData::GetWeaponDataFromWeapon( int weapon )
	{
		assert( weapon >= 0 && weapon < m_weaponData.size() );
		return m_weaponData[ weapon ];
	}


	int Core::GameData::PushWeaponData( float range, float graceDistance, float weaponDamage, float moraleDamage, float moralDamageOnMiss, float rageBuff, float pressureBuff, float staminaCost, float animationDmgDealingtime, std::string animationName )
	{
		m_weaponData.push_back( WeaponData() );

		m_weaponData.back().range					= range;
		m_weaponData.back().graceDistance			= graceDistance;
		m_weaponData.back().weaponDamage			= weaponDamage;
		m_weaponData.back().moraleDamage			= moraleDamage;
		m_weaponData.back().moralDamageOnMiss		= moralDamageOnMiss;
		m_weaponData.back().rageBuff				= rageBuff;
		m_weaponData.back().pressureBuff			= pressureBuff;
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
		
		m_reqisteredEscapePointGroups.push_back( escapeGroup );

		if( instance->CalculateFlowfieldForGroup( point, escapeGroup ) )
			m_validEscapePointGroups.push_back( escapeGroup );

		return escapeGroup;
	}
	
	int Core::GameData::GetEscapePointGroup( int node )
	{
		if( m_validEscapePointGroups.size() == 0 )
			return -1;

		Core::NavigationMesh* instance = Core::GetNavigationMesh();
		if( !instance )
			return -1;

		float closest = std::numeric_limits<float>::max();
		int index = -1;
		for( unsigned int i = 0; i < m_validEscapePointGroups.size(); i++ )
		{
			float dist = instance->flowfields[ m_validEscapePointGroups[i] ].distanceToGoal[node];
			dist < closest ? closest = dist, index = i : index = index ;
		}

		return index;
	}

	bool Core::GameData::CheckIfEscapeSquad( int groupID )
	{
		int size = m_reqisteredEscapePointGroups.size();
		for( int i = 0; i < size; i++ )
			if( m_reqisteredEscapePointGroups[i] == groupID )
				return true;
		return false;
	}


	void Core::GameData::ClearData()
	{
		// remove weapons
		m_weaponData.clear();

		// clear all stored escape routes
		m_validEscapePointGroups.clear();
		m_reqisteredEscapePointGroups.clear();
	}




}
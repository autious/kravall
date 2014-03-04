#ifndef GAME_UTILITY_GAMEDATA_HPP
#define GAME_UTILITY_GAMEDATA_HPP

#include <Components/MovementComponent.hpp>
#include <Components/TargetingComponent.hpp>
#include <vector>
#include <string>

struct TempInitGameData;


namespace Core
{
	/*!
		Struct used for storing metadata for weaponTypes.
	*/
	struct WeaponData
	{
		/*!
			These values is the default ones. Use lua to change them.
		*/
		WeaponData()
		{
			range = 1.5f;
			graceDistance = 0.75f;
			weaponDamage = 30;
			moraleDamage = 0.2f;
			staminaCost = 0.0f;
			moralDamageOnMiss = 0.0f;
			animationDmgDealingtime = 0.5f;
			rageBuff = 0.1f;
			pressureBuff = 0.2f;
			animationName = "";
		}
		
		float range;
		float graceDistance;
		float weaponDamage;
		float moraleDamage;
		float moralDamageOnMiss;
		float rageBuff;
		float pressureBuff;
		float staminaCost;
		float animationDmgDealingtime;
		std::string animationName;
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
			Will return the respective data for the weapon.
		*/
		static const WeaponData& GetWeaponDataFromWeapon( int weapon );

		/*!
			Will push weaponData to the list and return the id for the weapon. This function is meant to be used form lua.
		*/
		static int PushWeaponData( float range, float graceDistance, float weaponDamage, float moraleDamage, float moralDamageOnMiss, float rageBuff, float pressureBuff, float staminaCost, float animationDmgDealingtime, std::string animationName );

		/*!
			Will allocate a flowfield leading to this point and save it for use in the GetEscapePointGroup() function.
		*/
		static int RegisterEscapePoint( glm::vec3 point );

		/*!	
			Will return the flowfield group index whos escape point is closest to this point.
		*/
		static int GetEscapePointGroup( int node );

		/*!
			Will deallocate all dynamic game data and reset all other data.
		*/
		static void ClearData();

	private:

		static std::vector< WeaponData > m_weaponData;
		static std::vector< int > m_escapePointGroups;

	};
}

#endif













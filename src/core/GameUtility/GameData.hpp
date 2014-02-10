#ifndef GAME_UTILITY_GAMEDATA_HPP
#define GAME_UTILITY_GAMEDATA_HPP

#include <Components/MovementComponent.hpp>
#include <Components/TargetingComponent.hpp>

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
			damage = 30;
			animationDmgDealingtime = 0.5f;
			animationID = 0;
		}
		
		float range;
		float damage;
		float animationDmgDealingtime;
		int animationID;
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
			Will return the respective data for the weapon.
		*/
		static const WeaponData& Core::GameData::GetWeaponDataFromWeapon( WeaponType weapon );

		/*!
			Will set the MovementData for the respective state. Should not be used carelessly form C.
		*/
		static void SetMovementDataForState( MovementState state, float speedToDesire, float acceleration, float deceleration );
		
		/*!
			Will set the WeaponData for the respective weapon. Should not be used carelessly form C.
		*/
		static void SetWeaponDataForWeapon( WeaponType weapon, float range, float damage, float animationDmgDealingtime, int animationID );


	private:

		static MovementData m_movementData[ MovementState::MOVEMENTSTATE_COUNT ];
		static WeaponData m_weaponData[ WeaponType::WEAPONTYPE_COUNT ];

		friend TempInitGameData;
	};
}

#endif













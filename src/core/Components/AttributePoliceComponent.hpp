#ifndef SRC_CORE_COMPONENTS_ATTRIBUTE_POLICE_COMPONENT_HPP
#define SRC_CORE_COMPONENTS_ATTRIBUTE_POLICE_COMPONENT_HPP

namespace Core
{
	/*!
		Enum specifying the stance of a police units. 
		There are three different stances: Passive, Defensive and Aggressive
	*/
	/*enum PoliceStance
	{
		Passive, Defensive, Aggressive
	};*/

	/*!
		Component holding all the police's attributes: Health, Stamina, Defense, Morale, Mobiliity and Stance.
	*/
	struct AttributePoliceComponent
	{
		/*! The police's health represented by an integer with range [0, 100]. */
		int health;

		/*! The police's stamina represented by an integer with range [0, 100]. */
		int stamina;

		/*! The police's defense represented by an integer with range [0, 100]. */
		int defense;

		/*! The police's morale represented by a float with range [0.0f, 2.0f]. */
		float morale;

		/*! The police's mobility represented by a float with range [0.0f, 100.0f]. */
		float mobility;

		/*! The police's stance represented by the PoliceStance enum. */
		PoliceStance stance;

		/*!
			Default constructor. Initialises all members to 0 except stance which is defaulted to Passive.
		*/
		AttributePoliceComponent() : health(0), stamina(0), defense(0), morale(0.0f), mobility(0.0f), stance(PoliceStance::Passive)
		{
		}

		/*!
			Constructor setting all members to starting values.
			\param startHealth The initial value for the health.
			\param startStamina The initial value for the stamina.
			\param startDefense The initial value for the defense.
			\param startMorale The initial value for the morale.
			\param startMobility The initial value for the mobility.
			\param startStance The initial value for the stance.
		*/
		AttributePoliceComponent(const int& startHealth, const int&  startStamina, const int&  startDefense, 
			float startMorale, const float& startMobility, const PoliceStance& startStance)
			: health(startHealth), stamina(startStamina), defense(startDefense), morale(startMorale), 
			mobility(startMobility), stance(PoliceStance::Passive)
		{
		}

        inline static const char* GetName()
        {
            return "AttributePoliceComponent";
        }
	};
}
#endif

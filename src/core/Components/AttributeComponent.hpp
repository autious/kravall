#ifndef SRC_CORE_COMPONENTS_ATTRIBUTE_COMPONENT_HPP
#define SRC_CORE_COMPONENTS_ATTRIBUTE_COMPONENT_HPP

namespace Core
{
	/*!
	Enum specifying the stance of a police units.
	There are three different stances: Passive, Defensive and Aggressive
	*/
	enum PoliceStance
	{
		Passive, Defensive, Aggressive
	};

	/*!
	Enum specifying the alignment of the rioters.
	A rioter's alignment defines its behaviour and reactions in different situations. There are two alignments:
	Anarchist and Pacifist.
	*/
	enum RioterAlignment
	{
		Anarchist, Pacifist
	};

	enum RioterStance
	{
		Normal, Agitated, Attacking, Retreating, Civilian
	};

	struct AttributeComponent
	{
		/*! The police's health represented by an integer with range [0, 100]. */
		int health;

		/*! The police's stamina represented by an integer with range [0, 100]. */
		int stamina;

		/*! The police's morale represented by a float with range [0.0f, 2.0f]. */
		float morale;

		union
		{
			struct
			{
				/*! The police's stance represented by the PoliceStance enum. */
				PoliceStance stance;

				/*! The police's squad id for orders. */
				int squadID;

				/*! The police's defense represented by an integer with range [0, 100]. */
				int defense;

				/*! The police's mobility represented by a float with range [0.0f, 100.0f]. */
				float mobility;
			} police;
			struct
			{
				/*! The rioter's stance represented by the RioterStance enum. */
				RioterStance stance;

				/*! The rioter's conceptual group ID. Used for determining which flow field to use.*/
				int groupID;

				/*! The rioter's rage represented by an integer with range [0, 100]. */
				int rage;

				/*! The rioter's pressure represented by an integer with range [0, 100]. */
				int pressure;

				/*! The rioter's alignment represented by the RioterAlignment enum. */
				RioterAlignment alignment;
			} rioter;
		};

		/*!
		Default constructor. Initialises all members to 0 except stance which is defaulted to Passive.
		*/
		AttributeComponent() : health(0), stamina(0), morale(0.0f)
		{
			police.stance = PoliceStance::Passive;
			police.defense = 0;
			police.mobility = 0.0f;
		}

		/*!
		Constructor for police initialising all members to starting values.
		\param startHealth The initial health for the police unit.
		\param startStamina The initial stamina for the police unit.
		\param startMorale The initial morale for the police unit.
		\param policeStance The initial stance of the police unit represented by the PoliceStance enum.
		\param policeDefense The initial defence for the police unit.
		\param policeMobility The initial mobility for the police unit.
		*/
		AttributeComponent(const int& startHealth, const int& startStamina, const float& startMorale,
			const PoliceStance& policeStance, const int& policeSquad, const int& policeDefense,
			const float& policeMobility) : health(startHealth), stamina(startStamina), morale(startMorale)
		{
			police.stance = policeStance;
			police.squadID = policeSquad;
			police.defense = policeDefense;
			police.mobility = policeMobility;
		}

		/*!
		Constructor for rioter initialising all members to starting values.
		\param startHealth The initial health for the rioter.
		\param startStamina The initial stamina for the rioter.
		\param startMorale The initial morale for the rioter.
		\param rioterAlignment The initial alignment of the rioter represented by the RioterAlignment enum.
		\param rioterRage The rioter's initial rage.
		\param rioterPressure The initial pressure for the rioter.
		*/
		AttributeComponent(const int& startHealth, const int& startStamina, const float& startMorale,
			const RioterAlignment& rioterAlignment, const int& rioterGroupID, const int& rioterRage,
			const int& rioterPressure, const RioterStance& rioterStance) : health(startHealth), stamina(startStamina),
			morale(startMorale)
		{
			rioter.alignment = rioterAlignment;
			rioter.groupID = rioterGroupID;
			rioter.rage = rioterRage;
			rioter.pressure = rioterPressure;
			rioter.stance = rioterStance;
		}

		inline static const char * GetName()
		{
			return "AttributeComponent";
		}
	};
}
#endif

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
			struct PolicePart
			{
				/*! The police's defense represented by an integer with range [0, 100]. */
				int defense;

				/*! The police's mobility represented by a float with range [0.0f, 100.0f]. */
				float mobility;

				/*! The police's stance represented by the PoliceStance enum. */
				PoliceStance stance;

			} police;
			struct RioterPart
			{
				/*! The rioter's rage represented by an integer with range [0, 100]. */
				int rage;

				/*! The rioter's pressure represented by an integer with range [0, 100]. */
				int pressure;

				/*! The rioter's alignment represented by the RioterAlignment enum. */
				RioterAlignment alignment;
			}rioter;
		};
	};
}
#endif
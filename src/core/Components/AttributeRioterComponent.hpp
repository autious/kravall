#ifndef SRC_CORE_COMPONENTS_ATTRIBUTE_RIOTER_COMPONENT_HPP
#define SRC_CORE_COMPONENTS_ATTRIBUTE_RIOTER_COMPONENT_HPP

namespace Core
{
	/*!
		Enum specifying the alignment of the rioters.
		A rioter's alignment defines its behaviour and reactions in different situations. There are two alignments: 
		Anarchist and Pacifist.
	*/
	enum RioterAlignment
	{
		Anarchist, Pacifist
	};

	/*!
		Component holding all the rioters's attributes: Alignment, Health, Stamina, Rage, Pressure and Morale.
	*/
	struct AttributeRioterComponent
	{
		/*! The rioter's alignment represented by the RioterAlignment enum. */
		RioterAlignment alignment;

		/*! The rioter's health represented by an integer with range [0, 100]. */
		int health;

		/*! The rioter's stamina represented by an integer with range [0, 100]. */
		int stamina;

		/*! The rioter's rage represented by an integer with range [0, 100]. */
		int rage;

		/*! The rioter's pressure represented by an integer with range [0, 100]. */
		int pressure;
		
		/*! The rioter's morale represented by a float with range [0.0f, 2.0f]. */
		float morale;

		/*!
			Default constructor. Initialises all members to 0 except alignment which is defaulted to Pacifist.
		*/
		AttributeRioterComponent() : alignment(RioterAlignment::Pacifist), health(0), stamina(0), rage(0), pressure(0),
			morale(0.0f)
		{
		}

		/*!
			Constructor setting all members to starting values.
			\param align The initial value for the alignment.
			\param startHealth The initial value for the health.
			\param startStamina The initial value for the stamina.
			\param startRage The initial value for the rage.
			\param startPressure The initial value for the pressure.
			\param startMorale The initial value for the morale.
		*/
		AttributeRioterComponent(RioterAlignment align, int startHealth, int startStamina, int startRage, 
			int startPressure, float startMorale) 
			: alignment(align), health(startHealth), stamina(startStamina), rage(startRage), pressure(startPressure), 
			morale(startMorale)
		{
		}

        inline static const char* GetName()
        {
            return "AttributeRioterComponent";
        }
	};
}
#endif

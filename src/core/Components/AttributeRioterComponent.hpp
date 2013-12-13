#ifndef SRC_CORE_COMPONENTS_ATTRIBUTE_RIOTER_COMPONENT_HPP
#define SRC_CORE_COMPONENTS_ATTRIBUTE_RIOTER_COMPONENT_HPP

namespace Core
{
	enum RioterAlignment
	{
		Anarchist, Pacifist
	};

	struct AttributeRioterComponent
	{
		// Health, Stamina, Rage, Pressure, Morale, Alignment
		RioterAlignment alignment;
		int health; // 0 - 100
		int stamina; // 0 - 100
		int rage; // 0 - 100
		int pressure; // 0 - 100
		float morale; // 0 - 2

		AttributeRioterComponent() : alignment(RioterAlignment::Pacifist), health(0), stamina(0), rage(0), pressure(0),
			morale(0.0f)
		{
		}

		AttributeRioterComponent(RioterAlignment align, int startHealth, int startStamina, int startRage, 
			int startPressure, float startMorale) 
			: alignment(align), health(startHealth), stamina(startStamina), rage(startRage), pressure(startPressure), 
			morale(startMorale)
		{
		}
	};
}
#endif
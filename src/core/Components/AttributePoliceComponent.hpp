#ifndef ATTRIBUTE_POLICE_COMPONENT_HPP
#define ATTRIBUTE_POLICE_COMPONENT_HPP

namespace Core
{
	enum PoliceStance
	{
		Passive, Defensive, Aggressive
	};

	struct AttributePoliceComponent
	{
		// Health, Stamina, Defense, Morale, Mobiliity, Stance
		int health; // 0 - 100
		int stamina; // 0 - 100
		int defense; // 0 - 100
		float morale; // 0 - 2
		float mobility; // 0 - 100
		PoliceStance stance;

		AttributePoliceComponent() : health(0), stamina(0), defense(0), morale(0.0f), mobility(0.0f), stance(PoliceStance::Passive)
		{
		}

		AttributePoliceComponent(int startHealth, int startStamina, int startDefense, float startMorale, 
			float startMobility, float startStance) 
			: health(startHealth), stamina(startStamina), defense(startDefense), morale(startMorale), 
			mobility(startMobility), stance(PoliceStance::Passive)
		{
		}
	};
}
#endif
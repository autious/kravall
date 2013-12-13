#ifndef SRC_CORE_COMPONENTS_UNIT_TYPE_COMPONENT_HPP
#define SRC_CORE_COMPONENTS_UNIT_TYPE_COMPONENT_HPP

namespace Core
{
	enum UnitType
	{
		Rioter, Police, Object
	};

	struct UnitTypeComponent
	{
		UnitType Type;

		UnitTypeComponent() : Type(UnitType::Object)
		{
		}

		UnitTypeComponent(UnitType typeOfUnit) : Type(typeOfUnit)
		{
		}
	};
}
#endif
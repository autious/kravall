#ifndef SRC_CORE_COMPONENTS_UNIT_TYPE_COMPONENT_HPP
#define SRC_CORE_COMPONENTS_UNIT_TYPE_COMPONENT_HPP

namespace Core
{
	/*! Enum specifying what type of unit the object is. There are three types of objects: Rioter, Police and Object. */
	enum UnitType
	{
		Rioter, Police, Object, Count
	};

	/*! 
		Component specifying the type of an object. With a type, the object may have a charge to influence the 
		potential field.
	*/
	struct UnitTypeComponent
	{
		/*! The type of the unit represented by the UnitType enum. */
		UnitType type;

		/*! Default constructor. The type is defaulted to Object. */
		UnitTypeComponent() : type(UnitType::Object)
		{
		}

		/*!
			Constructor setting all members to starting values.
			\param typeOfUnit The type of the unit. 
		*/
		UnitTypeComponent(const UnitType& typeOfUnit) : type(typeOfUnit)
		{
		}

        inline static const char* GetName()
        {
            return "UnitTypeComponent";
        }
	};
}
#endif

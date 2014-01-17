#ifndef SRC_CORE_COMPONENTS_AREACOMPONENT_H
#define SRC_CORE_COMPONENTS_AREACOMPONENT_H

namespace Core
{
	/*!
		Component specifying a 2D area with WorldPositionComponent as origo.
        Spans over x and z axis.
        Consists of 4 vertices (a quad)
	*/
	struct AreaComponent
	{
        float vertices[2*4];

        inline static const char* GetName()
        {
            return "AreaComponent";
        }
	};
}
#endif

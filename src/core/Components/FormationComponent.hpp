#ifndef CORE_COMPONENTS_FORMATION_COMPONENT_HPP
#define CORE_COMPONENTS_FORMATION_COMPONENT_HPP

#include <array>

namespace Core
{
    struct FormationComponent    
    {
		std::array<float, 2> relativePosition = { { 0.0f, 0.0f } };
        bool isStraggler = false;

        FormationComponent()
        {

        }

        FormationComponent(float relativeX, float relativeY)
        {
            relativePosition[0] = relativeX;
            relativePosition[1] = relativeY;
        }

        inline static const char* GetName()
        {
            return "FormationComponent";
        }
    };
}

#endif

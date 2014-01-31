#ifndef CORE_COMPONENTS_FORMATION_COMPONENT_HPP
#define CORE_COMPONENTS_FORMATION_COMPONENT_HPP

namespace Core
{
    struct FormationComponent    
    {
        float relativePosition[2] = {0.0f, 0.0f};
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

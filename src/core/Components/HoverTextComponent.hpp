#ifndef SRC_CORE_COMPONENTS_HOVERTEXTCOMPONENT_H
#define SRC_CORE_COMPONENTS_HOVERTEXTCOMPONENT_H

#include <utility/Colors.hpp>

namespace Core
{
    /*!
        Text hovering on screen, either directly to screen coordinates
        or projected from world space coordinates
    */
    struct HoverTextComponent
    {
        int hoverTextId;
        float size;
        float color[4];
        bool render;
        
        HoverTextComponent()
        {
            hoverTextId = -1;
            size = 1;
            render = true;
            SetColor( *this, Colors::White );
        }

        inline static void SetColor( HoverTextComponent& m, Color c )
        {
           m.color[0] = c[0];
           m.color[1] = c[1];
           m.color[2] = c[2];
           m.color[3] = c[3];
        }

        inline static Color GetColor( HoverTextComponent& m )
        {
           return Color( m.color[0], m.color[1], m.color[2], m.color[3] );
        }

        inline static const char* GetName()
        {
            return "HoverTextComponent";
        }
    };
}

#endif

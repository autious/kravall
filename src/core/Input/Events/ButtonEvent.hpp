#ifndef CORE_INPUT_EVENTS_BUTTON_EVENT_HPP
#define CORE_INPUT_EVENTS_BUTTON_EVENT_HPP

namespace Core
{
    struct ButtonEvent
    {
        int button;
        int action;
        int mods;
    };
}


#endif

#ifndef CORE_INPUT_EVENTS_KEY_EVENT_HPP
#define CORE_INPUT_EVENTS_KEY_EVENT_HPP

namespace Core
{
    struct KeyEvent
    {
        int key;
        int scancode;
        int action;
        int mods;
    };
}

#endif

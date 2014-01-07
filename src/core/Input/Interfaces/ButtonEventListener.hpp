#ifndef CORE_INPUT_INTERFACES_BUTTON_EVENT_LISTENER_HPP
#define CORE_INPUT_INTERFACES_BUTTON_EVENT_LISTENER_HPP

#include <Input/Events/ButtonEvent.hpp>

namespace Core
{
    class ButtonEventListener
    {
    public:
        virtual void OnButtonEvent(const Core::ButtonEvent &e) = 0;
    };
}

#endif

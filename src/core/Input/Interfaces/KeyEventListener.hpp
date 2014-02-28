#ifndef CORE_INPUT_INTERFACES_KEY_EVENT_LISTENER_HPP
#define CORE_INPUT_INTERFACES_KEY_EVENT_LISTENER_HPP

#include <Input/Events/KeyEvent.hpp>

namespace Core
{
    class KeyEventListener
    {
    public:
        virtual void OnKeyEvent(const Core::KeyEvent &e) = 0;
    };
}

#endif

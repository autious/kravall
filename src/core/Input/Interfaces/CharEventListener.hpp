#ifndef CORE_INPUT_INTERFACES_CHAR_EVENT_LISTENER_HPP
#define CORE_INPUT_INTERFACES_CHAR_EVENT_LISTENER_HPP

#include <Input/Events/CharEvent.hpp>

namespace Core
{
    class CharEventListener
    {
    public:
        virtual void OnCharEvent(const Core::CharEvent &e) = 0;
    };
}

#endif

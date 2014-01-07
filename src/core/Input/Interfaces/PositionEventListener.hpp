#ifndef CORE_INPUT_INTERFACES_POSITION_EVENT_LISTENER_HPP
#define CORE_INPUT_INTERFACES_POSITION_EVENT_LISTENER_HPP

#include <Input/Events/PositionEvent.hpp>

namespace Core
{
    class PositionEventListener
    {
    public:
        virtual void OnPositionEvent(const Core::PositionEvent &e) = 0;
    };
}

#endif

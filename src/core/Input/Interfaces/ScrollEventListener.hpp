#ifndef CORE_INPUT_INTERFACES_SCROLL_EVENT_LISTENER_HPP
#define CORE_INPUT_INTERFACES_SCROLL_EVENT_LISTENER_HPP

#include <Input/Events/ScrollEvent.hpp>

namespace Core
{
    class ScrollEventListener
    {
    public:
        virtual void OnScrollEvent(const Core::ScrollEvent &e) = 0;
    };
}

#endif

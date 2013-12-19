#ifndef "MouseState.hpp"
#include <cstring>

namespace Core
{
    MouseState::MouseState()
    {
        std::memset(m_buttonValues, false, NUMBER_OF_BUTTONS * sizeof(decltype(m_buttonValues[0])); 
    }

    MouseState::SetButtonState(const int button, const int action)
    {
        m_buttonValues[button] = static_cast<bool>(action);
    }

    MouseState::IsButtonDown(const int button) const
    {
        return !m_buttonValues[button];
    }

    MouseState::IsButtonUp(const int button) const
    {
        return m_buttonValues[button];
    }
}

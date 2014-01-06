#include "MouseState.hpp"
#include <cstring>

#include <WindowHandling/GLFWInclude.hpp>

namespace Core
{
    MouseState::MouseState()
    {
        std::memset(m_buttonValues, false, NUMBER_OF_BUTTONS * sizeof(decltype(m_buttonValues[0]))); 
    }

    void MouseState::SetButtonState(const int button, const int action)
    {
        if( action != GLFW_REPEAT )
            m_buttonValues[button] = (action == GLFW_PRESS);
    }

    const bool MouseState::IsButtonDown(const int button) const
    {
        return m_buttonValues[button];
    }

    const bool MouseState::IsButtonUp(const int button) const
    {
        return !m_buttonValues[button];
    }

    void MouseState::SetCursorPosition(const int x, const int y)
    {
        m_cursorPositionX = x;
        m_cursorPositionY = y;
    }

    void MouseState::GetCursorPosition(int& x, int& y)
    {
        x = m_cursorPositionX;
        y = m_cursorPositionY;
    }
}

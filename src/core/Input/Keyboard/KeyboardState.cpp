#include "KeyboardState.hpp"
#include <cstring>

#include <WindowHandling/GLFWInclude.hpp>

namespace Core
{
    KeyboardState::KeyboardState()
    {
        std::memset(m_keyValues, false, NUMBER_OF_KEYS * sizeof(decltype(m_keyValues[0])));
    }

    void KeyboardState::SetKeyState(const int key, const int action)
    {
        if( action != GLFW_REPEAT )
            m_keyValues[key] = (action == GLFW_PRESS);
    }

    const bool KeyboardState::IsKeyDown(const int key) const
    {
        return m_keyValues[key];
    }

    const bool KeyboardState::IsKeyUp(const int key) const
    {
        return !m_keyValues[key];
    }
}

#include "KeyboardState.hpp"
#include <cstring>

namespace Core
{
    KeyboardState::KeyboardState()
    {
        std::memset(m_keyValues, false, NUMBER_OF_KEYS * sizeof(decltype(m_keyValues[0])));
    }

    void KeyboardState::SetKeyState(const int key, const int action)
    {
        m_keyValues[key] = static_cast<bool>(action);
    }

    const bool KeyboardState::IsKeyDown(const int key) const
    {
        return !m_keyValues[key];
    }

    const bool KeybordState::IsKeyUp(const int key) const
    {
        return m_keyValues[key];
    }
}

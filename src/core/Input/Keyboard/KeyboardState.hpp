#ifndef CORE_INPUT_KEYBOARD_KEYBOARD_STATE_HPP
#define CORE_INPUT_KEYBOARD_KEYBOARD_STATE_HPP

#include <WindowHandling/GLFWInclude.hpp>

namespace Core
{
    class KeyboardState
    {
    public:
        explicit KeyboardState();

        void SetKeyState(const int key, const int action);
        const bool IsKeyDown(const int key) const;
        const bool IsKeyUp(const int key) const;

    private:
        static const int NUMBER_OF_KEYS = GLFW_KEY_LAST+1;
        bool m_keyValues[NUMBER_OF_KEYS];
    };
}

#endif

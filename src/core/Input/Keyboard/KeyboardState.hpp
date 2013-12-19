#ifndef CORE_INPUT_KEYBOARD_KEYBOARD_STATE_HPP
#define CORE_INPUT_KEYBOARD_KEYBOARD_STATE_HPP


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
        static const int NUMBER_OF_KEYS = 256;
        bool m_keyValues[NUMBER_OF_KEYS];
    };
}

#endif

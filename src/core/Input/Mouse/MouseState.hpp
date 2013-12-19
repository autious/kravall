#ifndef CORE_INPUT_MOUSE_MOUSE_HPP
#define CORE_INPUT_MOUSE_MOUSE_HPP


namespace Core
{
    class MouseState
    {
    public:
        explicit MouseState();

        void SetButtonState(const int button, const int action);
        const bool IsButtonDown(const int button) const;
        const bool IsButtonUp(const int button) const;

        void SetCursorPosition(const int x, const int y);
        void GetCursorPosition(int& x, int& y);

    private:        
        static const int NUMBER_OF_BUTTONS = 3;
        bool m_buttonValues[NUMBER_OF_BUTTONS];

        int m_cursorPositionX;
        int m_cursorPositionY;
    };
}

#endif 

#ifndef CORE_INPUT_MOUSE_MOUSE_HPP
#ifndef CORE_INPUT_MOUSE_MOUSE_HPP


namespace Core
{
    class MouseState
    {
    public:
        explicit MouseState();

        void SetButtonState(const int button, const int action);
        const bool IsButtonDown(const int button) const;
        const bool IsButtonUp(const int button) const;

    priviate:
        static const int NUMBER_OF_BUTTONS = 3;
        bool m_buttonValues[NUMBER_OF_BUTTONS];

    };
}

#endif 

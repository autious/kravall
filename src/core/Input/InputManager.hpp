#ifndef CORE_INPUT_INPUT_MANAGER_HPP
#define CORE_INPUT_INPUT_MANAGER_HPP

#include <WindowHandling/GLFWInclude.hpp>

#include <Input/Keyboard/KeyboardState.hpp>
#include <Input/Mouse/MouseState.hpp>

#include <Input/Events/KeyEvent.hpp>
#include <Input/Events/CharEvent.hpp>
#include <Input/Events/MouseEvent.hpp>
#include <Input/Events/PositionEvent.hpp>
#include <Input/Events/ScrollEvent.hpp>

namespace Core
{

    Core::KeyEvent* KeyEvents;
    Core::CharEvent* CharEvents;
    Core::MouseEvent* MouseEvents;
    Core::PositionEvent* PositionEvents;
    Core::ScrollEvent* ScrollEvents;

    class InputManager
    {
    public:
        explicit InputManager(GLFWwindow* window);
        ~InputManager();

        void PollEvents();
        void CallListeners();
        const Core::KeyboardState GetKeyboardState();
        const Core::MouseState GetMouseState();

    private:
        static const unsigned int NUMBER_OF_KEY_EVENTS = 100;
        static const unsigned int NUMBER_OF_CHAR_EVENTS = 100;
        static const unsigned int NUMBER_OF_MOUSE_EVENTS = 50;
        static const unsigned int NUMBER_OF_POSITION_EVENTS = 300;
        static const unsigned int NUMBER_OF_SCROLL_EVENTS = 50;

        GLFWwindow* m_window;
        Core::KeyboardState m_keyboardState;
        Core::MouseState m_mouseState;
        
        Core::KeyEvent* m_startKeyEventPtr;
        Core::CharEvent* m_startCharEventPtr;
        Core::MouseEvent* m_startMouseEventPtr;
        Core::PositionEvent* m_startPositionEventPtr;
        Core::ScrollEvent* m_startScrollEventPtr;

        Core::KeyEvent* m_currentKeyEventPtr;
        Core::CharEvent* m_currentCharEventPtr;
        Core::MouseEvent* m_currentMouseEventPtr;
        Core::PositionEvent* m_currentPositionEventPtr;
        Core::ScrollEvent* m_currentScrollEventPtr;
    };
}


#endif

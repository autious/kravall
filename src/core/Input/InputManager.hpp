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
    namespace 
    {   
        Core::KeyboardState keyboardState();
        Core::MouseState mouseState();
    }

    Core::KeyEvent* KeyEvents;
    Core::CharEvent* CharEvents;
    Core::MouseEvent* MouseEvents;
    Core::PositionEvent* PositionEvents;
    Core::ScrollEvent* ScrollEvents;

    int nKeyEvents = 0;
    int nCharEvents = 0;
    int nMouseEvents = 0;
    int nPositionEvents = 0;
    int nScrollEvents = 0;

    class InputManager
    {
    public:
        explicit InputManager(GLFWwindow* window);
        ~InputManager();

        void PollEvents();
        void CallListeners();
        inline static Core::KeyboardState& GetKeyboardState(){ return keyboardState; }
        inline static Core::MouseState& GetMouseState(){ return mouseState; }

    private:
        static const int NUMBER_OF_KEY_EVENTS = 300;
        static const int NUMBER_OF_CHAR_EVENTS = 300;
        static const int NUMBER_OF_MOUSE_EVENTS = 50;
        static const int NUMBER_OF_POSITION_EVENTS = 300;
        static const int NUMBER_OF_SCROLL_EVENTS = 50;

        GLFWwindow* m_window;
        
        Core::KeyEvent* m_startKeyEventPtr;
        Core::CharEvent* m_startCharEventPtr;
        Core::MouseEvent* m_startMouseEventPtr;
        Core::PositionEvent* m_startPositionEventPtr;
        Core::ScrollEvent* m_startScrollEventPtr;
    };
}


#endif

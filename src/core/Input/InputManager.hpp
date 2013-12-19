#ifndef CORE_INPUT_INPUT_MANAGER_HPP
#define CORE_INPUT_INPUT_MANAGER_HPP

#include <vector>

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
    class KeyEventListener;
    class CharEventListener;
    class ButtonEventListener;
    class PositionEventListener;
    class ScrollEventListener;
}

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
        /*!
          Explicit constructor of InputManager.
          \param window The GLFWwindow that the input should be captured from.
          */
        explicit InputManager(GLFWwindow* window);

        /*!
          Polls events and updates states. More than one call per frame will result in erroneous behaviour.
          */
        void PollEvents();

        /*!
          Calls the subscribed listeners with their respective events. Can be called more than once per frame.        
          */
        void CallListeners();

        /*!
          Retreives the current Core::KeyboardState. Changes made to the returned state will change the current state.
          \returns A reference to the keyboardstate.
          */
        inline static Core::KeyboardState& GetKeyboardState(){ return keyboardState; }

        /*!
          Retreives the current Core::MouseState. Changes made to the returned state will change the current state.
          \returns A reference to the mousestate.
          */
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

        std::vector<Core::KeyEventListener*> m_keyEventListeners;
        std::vector<Core::CharEventListener*> m_charEventListeners;
        std::vector<Core::ButtonEventListener*> m_buttonEventListeners;
        std::vector<Core::PositionEventListener*> m_positionEventListeners;
        std::vector<Core::ScrollEventListener*> m_scrollEventListeners;
    };
}


#endif

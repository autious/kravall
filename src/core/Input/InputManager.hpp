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
          Initialization function. Call only once per application.
          \param window The GLFWwindow that the input should be captured from.
          */
        void Init(GLFWwindow* window);

        /*!
          Polls events and updates states. More than one call per frame will result in erroneous behaviour.
          */
        void PollEvents();

        /*!
          Calls the subscribed listeners with their respective events. Can be called more than once per frame.        
          */
        void CallListeners();

        /*!
          Adds a KeyEventListener that is called when KeyEvents occur
          */
        void AddKeyEventListener(Core::KeyEventListener* eventListener);

        /*!
          Adds a CharEventListener that is called when CharEvents occur
          */
        void AddCharEventListener(Core::CharEventListener* eventListener);

        /*!
          Adds a ButtonEventListener that is called when ButtonEvents occur
          */
        void AddButtonEventListener(Core::ButtonEventListener* eventListener);

        /*!
          Adds a PositionEventListener that is called when PositionEvents occur
          */
        void AddPositionEventListener(Core::PositionEventListener* eventListener);

        /*!
          Adds a ScrollEventListener that is called when ScrollEvents occur
          */
        void AddScrollEventListener(Core::ScrollEventListener* eventListener);

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

    /*!
      Retrieval function for the static input manager.
      \returns A reference to the InputManager.
      */
    InputManager& GetInputManager();


}





#endif

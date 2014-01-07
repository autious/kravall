#include "InputManager.hpp"

#include <algorithm>

#include <World.hpp>

#include <Input/Interfaces/KeyEventListener.hpp>
#include <Input/Interfaces/CharEventListener.hpp>
#include <Input/Interfaces/ButtonEventListener.hpp>
#include <Input/Interfaces/PositionEventListener.hpp>
#include <Input/Interfaces/ScrollEventListener.hpp>

#include <logger/Logger.hpp>


namespace Core
{

    namespace States
    {   
        KeyboardState keyboardState;
        MouseState mouseState;
        KeyboardState prevKeyboardState;
        MouseState prevMouseState;
    }

    static Core::KeyEvent* KeyEvents;
    static Core::CharEvent* CharEvents;
    static Core::ButtonEvent* ButtonEvents;
    static Core::PositionEvent* PositionEvents;
    static Core::ScrollEvent* ScrollEvents;

    static int nKeyEvents = 0;
    static int nCharEvents = 0;
    static int nButtonEvents = 0;
    static int nPositionEvents = 0;
    static int nScrollEvents = 0;

    static const int NUMBER_OF_KEY_EVENTS = 300;
    static const int NUMBER_OF_CHAR_EVENTS = 300;
    static const int NUMBER_OF_BUTTON_EVENTS = 50;
    static const int NUMBER_OF_POSITION_EVENTS = 300;
    static const int NUMBER_OF_SCROLL_EVENTS = 50;

    InputManager& GetInputManager()
    {
        static Core::InputManager inputManager;
        return inputManager;
    }

    void InputManagerKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if( nKeyEvents < NUMBER_OF_KEY_EVENTS && key >= 0 )
        {
            InputManager::GetKeyboardState().SetKeyState(key, action);

            KeyEvents[nKeyEvents].key = key;
            KeyEvents[nKeyEvents].action= action;
            KeyEvents[nKeyEvents].mods = mods;
            nKeyEvents++;
        }
        else
        {
            LOG_ERROR << "Unable to queue more key events" << std::endl;
        }
    }

    void InputManagerCharCallback(GLFWwindow* window, unsigned int codepoint)
    {
        if( nCharEvents < NUMBER_OF_CHAR_EVENTS )
        {
            CharEvents[nCharEvents].codepoint = codepoint;
            nCharEvents++;
        }
        else
        {
            LOG_ERROR << "Unable to queue more characters" << std::endl;
        }
    }

    void InputManagerMouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        if( nButtonEvents < NUMBER_OF_BUTTON_EVENTS )
        {
            InputManager::GetMouseState().SetButtonState(button, action);

            ButtonEvents[nButtonEvents].button = button;
            ButtonEvents[nButtonEvents].action = action;
            ButtonEvents[nButtonEvents].mods = mods;
            nButtonEvents++;
        }
        else
        {
            LOG_ERROR << "Unable to queue more button events" << std::endl;
        }
    }

    void InputManagerCursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
    {
        if( nPositionEvents < NUMBER_OF_POSITION_EVENTS )
        {
            PositionEvents[nPositionEvents].xpos = xpos;
            PositionEvents[nPositionEvents].ypos = ypos;
            nPositionEvents++;
        }
        else
        {
            LOG_ERROR << "Unable to queue more cursor position events" << std::endl;
        }
    }

    void InputManagerMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        if( nScrollEvents < NUMBER_OF_SCROLL_EVENTS )
        { 
            ScrollEvents[nScrollEvents].xoffset = xoffset;
            ScrollEvents[nScrollEvents].yoffset = yoffset;
            nScrollEvents++;
        }
        else
        {
            LOG_ERROR << "Unable to queue more scroll events" << std::endl; 
        }
    }

    void InputManager::Init(GLFWwindow* window)
    {
        m_window = window;

        glfwSetKeyCallback(m_window, InputManagerKeyCallback);
        glfwSetCharCallback(m_window, InputManagerCharCallback);
        glfwSetMouseButtonCallback(m_window, InputManagerMouseButtonCallback);
        glfwSetCursorPosCallback(m_window, InputManagerCursorPositionCallback);
        glfwSetScrollCallback(m_window, InputManagerMouseScrollCallback);

        KeyEvents = Core::world.m_constantHeap.NewPODArray<Core::KeyEvent>(NUMBER_OF_KEY_EVENTS); 
        CharEvents = Core::world.m_constantHeap.NewPODArray<Core::CharEvent>(NUMBER_OF_CHAR_EVENTS); 
        ButtonEvents = Core::world.m_constantHeap.NewPODArray<Core::ButtonEvent>(NUMBER_OF_BUTTON_EVENTS); 
        PositionEvents = Core::world.m_constantHeap.NewPODArray<Core::PositionEvent>(NUMBER_OF_POSITION_EVENTS); 
        ScrollEvents = Core::world.m_constantHeap.NewPODArray<Core::ScrollEvent>(NUMBER_OF_SCROLL_EVENTS); 
    }

    void InputManager::PollEvents()
    {
        nKeyEvents = 0;
        nCharEvents = 0;
        nButtonEvents = 0;
        nPositionEvents = 0;
        nScrollEvents = 0;
        
        InputManager::GetPrevKeyboardState() = InputManager::GetKeyboardState();
        InputManager::GetPrevMouseState() = InputManager::GetMouseState();

        glfwPollEvents();

        double x = 0;
        double y = 0; 
        glfwGetCursorPos(m_window, &x, &y);
        GetMouseState().SetCursorPosition(static_cast<int>(x), static_cast<int>(y));
    }

    void InputManager::CallListeners()
    {
        for(std::vector<Core::KeyEventListener*>::iterator it = m_keyEventListeners.begin(); it != m_keyEventListeners.end(); ++it)
        {
            for(int i = 0; i < nKeyEvents; ++i)
            {
                (*it)->OnKeyEvent(KeyEvents[i]);
            }   
        }

        for(std::vector<Core::CharEventListener*>::iterator it = m_charEventListeners.begin(); it != m_charEventListeners.end(); ++it)
        {
            for(int i = 0; i < nCharEvents; ++i)
            {
                (*it)->OnCharEvent(CharEvents[i]);
            }   
        }

        for(std::vector<Core::ButtonEventListener*>::iterator it = m_buttonEventListeners.begin(); it != m_buttonEventListeners.end(); ++it)
        {
            for(int i = 0; i < nButtonEvents; ++i)
            {
                (*it)->OnButtonEvent(ButtonEvents[i]);
            }   
        }

        for(std::vector<Core::PositionEventListener*>::iterator it = m_positionEventListeners.begin(); it != m_positionEventListeners.end(); ++it)
        {
            for(int i = 0; i < nPositionEvents; ++i)
            {
                (*it)->OnPositionEvent(PositionEvents[i]);
            }   
        }

        for(std::vector<Core::ScrollEventListener*>::iterator it = m_scrollEventListeners.begin(); it != m_scrollEventListeners.end(); ++it)
        {
            for(int i = 0; i < nScrollEvents; ++i)
            {
                (*it)->OnScrollEvent(ScrollEvents[i]);
            }   
        }
    }

    void InputManager::AddKeyEventListener(Core::KeyEventListener* eventListener)
    {
        m_keyEventListeners.push_back(eventListener);
    }

    void InputManager::RemoveKeyEventListener(Core::KeyEventListener* eventListener)
    {
       auto pos = std::find( m_keyEventListeners.begin(), m_keyEventListeners.end(), eventListener );

        if( pos != m_keyEventListeners.end() )
            m_keyEventListeners.erase( pos );
    }

    void InputManager::AddCharEventListener(Core::CharEventListener* eventListener)
    {
        m_charEventListeners.push_back(eventListener);
    }

    void InputManager::RemoveCharEventListener(Core::CharEventListener* eventListener)
    {
        auto pos = std::find( m_charEventListeners.begin(), m_charEventListeners.end(), eventListener );
    
        if( pos != m_charEventListeners.end() )
            m_charEventListeners.erase( pos ); 
    }

    void InputManager::AddButtonEventListener(Core::ButtonEventListener* eventListener)
    {
        m_buttonEventListeners.push_back(eventListener);
    }

    void InputManager::RemoveButtonEventListener(Core::ButtonEventListener* eventListener)
    {
        auto pos = std::find( m_buttonEventListeners.begin(), m_buttonEventListeners.end(), eventListener );

        if( pos != m_buttonEventListeners.end() )
            m_buttonEventListeners.erase( pos );
    }

    void InputManager::AddPositionEventListener(Core::PositionEventListener* eventListener)
    {
        m_positionEventListeners.push_back(eventListener);
    }

    void InputManager::RemovePositionEventListener(Core::PositionEventListener* eventListener)
    {
        auto pos = std::find( m_positionEventListeners.begin(), m_positionEventListeners.end(), eventListener );

        if( pos != m_positionEventListeners.end() )
            m_positionEventListeners.erase( pos );
    }

    void InputManager::AddScrollEventListener(Core::ScrollEventListener* eventListener)    
    {
        m_scrollEventListeners.push_back(eventListener);
    }

    void InputManager::RemoveScrollEventListener(Core::ScrollEventListener* eventListener)    
    {
        auto pos = std::find( m_scrollEventListeners.begin(), m_scrollEventListeners.end(), eventListener );

        if( pos != m_scrollEventListeners.end() )
            m_scrollEventListeners.erase( pos ); 
    }

    bool InputManager::IsKeyPressedOnce( const int id )
    {
        return GetKeyboardState().IsKeyDown( id ) && GetPrevKeyboardState().IsKeyUp( id );
    }

    void InputManager::GetPosDiff( int& x, int &y )
    {
        int cx,cy;
        int px,py;

        GetPrevMouseState().GetCursorPosition( px,py );
        GetMouseState().GetCursorPosition( cx,cy );
    
        x = px - cx;
        y = py - cy;
    }
}

#include "InputManager.hpp"

#include <World.hpp>

namespace Core
{
    void InputManagerKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        Core::KeyboardState kb_State = InputManager::GetKeyboardState():
        kb_State.SetKeyState(key, action);

        KeyEvents[nKeyEvents].key = key;
        KeyEvents[nKeyEvents].action= action;
        KeyEvents[nKeyEvents].mods = mods;
        nKeyEvents++;
    }

    void InputManagerCharCallback(GLFWwindow* window, unsigned int codepoint)
    {
        CharEvents[nCharEvents].codepoint = codepoint;
        nCharEvents++;
    }

    void InputManagerMousebuttonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        Core::MouseState mouseState = InputManager::GetMouseState();
        mouseState.SetButtonState(button, action);

        MouseEvents[nMouseEvents].button = button;
        MouseEvents[nMouseEvents].action = action;
        MouseEvents[nMouseEvents].mods = mods;
        nMouseEvents++;
    }

    void InputManagerCursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
    {
        PositionEvents[nPositionEvents].xpos = xpos;
        PositionEvents[nPositionEvents].ypos = ypos;
        nPositionEvents++;
    }

    void InputManagerMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        ScrollEvents[nScrollEvents].xoffset = xoffset;
        ScrollEvents[nScrollEvents].yoffset = yoffset;
        nScrollEvents++;
    }

    InputManager::InputManager(GLFWwindow* window)
    {
        m_window = window;

        glfwSetKeyCallback(m_window, InputManagerKeyCallback);
        glfwSetCharCallback(m_window, InputManagerCharCallback);
        glfwSetMouseButtonCallback(m_window, InputManagerMouseButtonCallback);
        glfwSetCursorPosCallback(m_window, InputManagerCursorPositionCallback);
        glfwSetScrollCallback(m_window, InputManagerMouseScrollCallback);

        KeyEvents = Core::world.m_constantHeap.NewPODArray<Core::KeyEvent>(NUMBER_OF_KEY_EVENTS); 
        CharEvents = Core::world.m_constantHeap.NewPODArray<Core::CharEvent>(NUMBER_OF_CHAR_EVENTS); 
        MouseEvents = Core::world.m_constantHeap.NewPODArray<Core::MouseEvent>(NUMBER_OF_MOUSE_EVENTS); 
        PositionEvents = Core::world.m_constantHeap.NewPODArray<Core::PositionEvent>(NUMBER_OF_POSITION_EVENTS); 
        ScrollEvents = Core::world.m_constantHeap.NewPODArray<Core::ScrollEvent>(NUMBER_OF_SCROLL_EVENTS); 
    }

    InputManager::~InputManager()
    {

    }

    void InputManager::PollEvents()
    {
        nKeyEvents = 0;
        nCharEvents = 0;
        nMouseEvents = 0;
        nPositionEvents = 0;
        nScrollEvents = 0;

        glfwPollEvents();

        int x = 0;
        int y = 0; 
        glfwGetCursorPos(window, x, y);
        m_mouseState.SetCursorPosition(x, y);
    }

    void InputManager::CallListeners()
    {

    }
}

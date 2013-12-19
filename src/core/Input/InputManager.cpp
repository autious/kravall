#include "InputManager.hpp"


namespace Core
{
    void InputManagerKeyCallback(GLFWwindow* window, intkey, int scancode, int action, int mods)
    {

    }

    void InputManagerCharCallback(GLFWwindow* window, unsigned int codepoint)
    {

    }

    void InputManagerMousebuttonCallback(GLFWwindow* window, int button, int action, int mods)
    {

    }

    void InputManagerCursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
    {

    }

    void InputManagerMouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {

    }

    InputManager::InputManager(GLFWwindow* window)
    {
        m_window = window;
        m_keyboardState = Core::KeyboardState();
        m_mouseState = Core::MouseState();

        glfwSetKeyCallback(m_window, InputManagerKeyCallback);
        glfwSetCharCallback(m_window, InputManagerCharCallback);
        glfwSetMouseButtonCallback(m_window, InputManagerMouseButtonCallback);
        glfwSetCursorPosCallback(m_window, InputManagerCursorPositionCallback);
        glfwSetScrollCallback(m_window, InputManagerMouseScrollCallback);
    }

    InputManager::~InputManager()
    {

    }

    void InputManager::PollEvents()
    {
        glfwPollEvents();
        int x = 0;
        int y = 0; 
        glfwGetCursorPos(window, x, y);
        m_mouseState.SetCursorPosition(x, y);
    }

    void InputManager::CallListeners()
    {

    }

    const Core::KeyboardState& InputManager::GetKeyboardState() const
    {
        return m_keyboardState;
    }

    const Core::MouseState& InputManager::GetMouseState()const
    {
        return m_mouseState;
    }
}

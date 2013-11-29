#include "GLFWInput.hpp"
#include <stdlib.h>

namespace Core
{

	GLFWInput::GLFWInput(GLFWwindow* window)
	{
		m_mouseButtonPressed[0] = m_mouseButtonPressed[1] = m_mouseButtonPressed[2] = false;
		m_window = window;
	}

	GLFWInput::~GLFWInput()
	{

	}

	void GLFWInput::UpdateInput()
	{
		glfwPollEvents();
		glfwGetCursorPos(m_window, &m_posX, &m_posY);

	}

	bool GLFWInput::IsKeyPressedOnce(int GLFW_Key)
	{
		if (IsKeyPressed(GLFW_Key)){

			if (m_keyPressed.find(GLFW_Key) == m_keyPressed.end())
			{
				m_keyPressed.insert(std::pair<int, int>(GLFW_Key, GLFW_Key));
				return true;
			}
			else
			{
				return false;
			}

		}
		else
		{
			m_keyPressed.erase(GLFW_Key);
			return false;
		}
	}

	bool GLFWInput::IsKeyPressed(int GLFW_Key)
	{
		return (glfwGetKey(m_window, GLFW_Key) == GLFW_PRESS);
	}

	bool GLFWInput::IsMouseButtonPressed(int GLFW_MouseButton)
	{
		return (glfwGetMouseButton(m_window, GLFW_MouseButton) == GLFW_PRESS);
	}

	bool GLFWInput::IsMouseButtonPressedOnce(int GLFW_MouseButton)
	{
		if (IsMouseButtonPressed(GLFW_MouseButton))
		{

			if (!m_mouseButtonPressed[GLFW_MouseButton])
			{
				m_mouseButtonPressed[GLFW_MouseButton] = true;
				return true;
			}
			else
			{
				return false;
			}

		}
		else
		{
			m_mouseButtonPressed[GLFW_MouseButton] = false;
			return false;
		}
	}

	double	GLFWInput::GetXPos()
	{
		return m_posX;
	}

	double GLFWInput::GetYPos()
	{
		return m_posY;
	}

	int GLFWInput::GetXPosDiff()
	{
		double xpos, ypos;

		glfwGetCursorPos(m_window, &xpos, &ypos);

		int diff = xpos - m_posX;

		return diff;
	}

	int GLFWInput::GetYPosDiff()
	{
		double xpos, ypos;

		glfwGetCursorPos(m_window, &xpos, &ypos);

		int diff = ypos - m_posY;

		return diff;
	}

	int GLFWInput::GetMouseWheel()
	{
		return -1;
	}

	void GLFWInput::SetMousePos(int xpos, int ypos){
		glfwSetCursorPos(m_window, xpos, ypos);
	}

	void GLFWInput::SetMouseWheel(int pos)
	{
		//glfwSetMouseWheel(pos);
	}
}
#ifndef _GLFWINPUT_HPP_
#define _GLFWINPUT_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <map>
#include <algorithm>
namespace Core
{

	class GLFWInput
	{
	private:
		double m_posX;
		double m_posY;

		std::map<int, int> m_keyPressed;

		bool m_mouseButtonPressed[3];
		GLFWwindow* m_window;

	public:
		GLFWInput(GLFWwindow* window);
		~GLFWInput();

		void UpdateInput();

		bool IsKeyPressed(int GLFW_Key);
		bool IsMouseButtonPressed(int GLFW_MouseButton);
		double	GetXPos();
		double GetYPos();

		//Doesn't work
		int GetMouseWheel();

		int GetXPosDiff();
		int GetYPosDiff();

		bool IsKeyPressedOnce(int GLFW_Key);
		bool IsMouseButtonPressedOnce(int GLFW_MouseButton);

		void SetMousePos(int xpos, int ypos);
		void SetMouseWheel(int pos);
	};
}
#endif
#ifndef _GLFWINPUT_HPP_
#define _GLFWINPUT_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <map>
#include <algorithm>
#include <iostream>

namespace Core
{

	class GLFWInput
	{
		friend GLFWInput& GetInput();
	private:
		double m_posX;
		double m_posY;
		double m_scrollX;
		double m_scrollY;
		char m_inputChar;

		std::map<int, int> m_keyPressed;

		bool m_mouseButtonPressed[3];
		GLFWwindow* m_window;

		GLFWInput();
		~GLFWInput();
	public:
		void Initialize(GLFWwindow* window);
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

		void SetScroll(double x, double y);

		void SetChar(char c);

		char GetChar();

		void ResetInput();

		void SetCharCallback(bool on);
	};

	GLFWInput& GetInput();
}
#endif
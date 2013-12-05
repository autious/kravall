#ifndef SRC_CORE_CONSOLE_CONSOLE_HPP
#define SRC_CORE_CONSOLE_CONSOLE_HPP

#include <vector>
#include <string>

#include <utility/Colors.hpp>

namespace Core
{

	class DebugConsole
	{
	public:
		
		/*!
		Struct used for sending text to the console
		*/
		struct Line
		{
			std::string text;
			Color color;
		};
	public:
		/*!
		Sets the string at the console input line
		\param inputLine Value to set the input line
		*/
		void SetInputLine(const std::string& inputLine);

		/*!
		Executes the current line and adds the command to console and history.
		*/
		void Add();

		/*!
		Sets the current input to last command in the history.
		*/
		void LastHistory();

		/*!
		Sets the current input to next command in the history.
		*/
		void NextHistory();

		/*!
		Scrolls the console.
		\param offset The number of rows to scroll the console text
		*/
		void Scroll(int offset);

		/*!
		Updates the console by sending draw calls to the graphics
		*/
		void Update();

		void Show();

		void Hide();

		void Toggle();

		void Clear();

		void ClearInput();

		void PrintLine(std::string str, Color color);

		inline bool IsVisible() { return m_visible; }

	private:

		friend DebugConsole& Console();

		void DebugConsole::HandleInput();

		DebugConsole();
		~DebugConsole();

	private:


		bool m_visible;

		// Current position in the history list
		int m_historyIndex;

		int m_offset;

		const int m_numRows = 25;

		// "Width" of a character in the console to use as reference for wrapping text
		const float m_wrapCharWidth = 8.9f;

		std::string m_inputLine;
		std::vector<Line> m_console;
		std::vector<std::string> m_history;

	};
	DebugConsole& Console();

	// List of commands
	void ClopLuaCommand();

}

#endif
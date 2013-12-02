#ifndef SRC_CORE_CONSOLE_CONSOLE_HPP
#define SRC_CORE_CONSOLE_CONSOLE_HPP

#include <vector>
#include <string>

namespace Core
{

	class DebugConsole
	{
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

	private:
		friend DebugConsole& Console();

		DebugConsole();
		~DebugConsole();

	private:
		bool m_visible;

		// Current position in the history list
		unsigned int m_historyIndex;

		int m_offset;

		std::string m_inputLine;
		std::vector<std::string> m_console;
		std::vector<std::string> m_history;

	};
	DebugConsole& Console();

}

#endif
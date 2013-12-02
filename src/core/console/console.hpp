#ifndef SRC_CORE_CONSOLE_CONSOLE_HPP
#define SRC_CORE_CONSOLE_CONSOLE_HPP

#include <vector>
#include <string>

class DebugConsole
{
public:
	/*!
	Sets the string at the console input line
	\param inputLine Value to set the input line
	*/
	void SetInputLine(std::string& inputLine);

	/*!
	Adds a line to the console window.
	\param line The string to add to the console
	*/
	void AddLine(std::string& line);

private:
	friend DebugConsole& Console();

	DebugConsole();
	~DebugConsole();

private:
	bool m_visible;

	std::string m_inputLine;
	std::vector<std::string> m_console;
	std::vector<std::string> m_history;

};
DebugConsole& Console();

#endif
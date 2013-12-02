#include "console.hpp"
#include "clop.hpp"

namespace Core
{
	DebugConsole::DebugConsole()
	{
		m_visible = false;
		m_historyIndex = 0;
		m_offset = 0;
	}
	DebugConsole::~DebugConsole()
	{

	}

	void DebugConsole::SetInputLine(const std::string& inputLine)
	{
		m_inputLine = inputLine;
	}

	void DebugConsole::Add()
	{
		if (!m_inputLine.empty())
		{
			// Add to console
			m_console.push_back(m_inputLine);

			// Add command to history
			if (m_history.size() > m_historyIndex)
			{
				if (m_history[m_historyIndex].compare(m_inputLine) != 0)
				{
					m_history.push_back(m_inputLine);
					m_historyIndex = m_history.size();
				}
			}

			// Execute command
			clop::Command(m_inputLine);

			// Reset input line
			m_inputLine == "";
		}
	}

	void DebugConsole::LastHistory()
	{
		if (!m_history.empty())
		{
			m_historyIndex = m_historyIndex - 1;
			if (m_historyIndex < 0)
				m_historyIndex = 0;
			m_inputLine = m_history[m_historyIndex];
		}
	}

	void DebugConsole::NextHistory()
	{
		if (!m_history.empty())
		{
			m_historyIndex = m_historyIndex + 1;
			if (m_historyIndex >= m_history.size())
				m_historyIndex = m_history.size()-1;
			m_inputLine = m_history[m_historyIndex];
		}
	}

	void DebugConsole::Scroll(int offset)
	{
		m_offset += offset;
	}

	DebugConsole& Console()
	{
		static DebugConsole console;
		return console;
	}

}
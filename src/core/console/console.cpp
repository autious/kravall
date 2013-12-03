#include "console.hpp"
#include "clop.hpp"
#include <gfx/GFXInterface.hpp>
#include <utility/Colors.hpp>

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
			bool add = true;

			if (m_history.size() > m_historyIndex && m_history[m_historyIndex].compare(m_inputLine) == 0)
			{
				add = false;
				m_historyIndex++;
			}

			if (add)
			{
				m_history.push_back(m_inputLine);
				m_historyIndex = m_history.size();
			}

			// Execute command
			clop::Command(m_inputLine);

			// Reset input line
			m_inputLine.clear();
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
			if (m_historyIndex >= (int)m_history.size())
				m_historyIndex = m_history.size()-1;
			m_inputLine = m_history[m_historyIndex];
		}
	}

	void DebugConsole::Scroll(int offset)
	{
		m_offset += offset;
		if (m_offset < 0)
			m_offset = 0;
		if (m_offset > m_console.size()-5)
			m_offset = m_console.size()-5;
		std::cout << m_offset << std::endl;
	}

	void DebugConsole::Update()
	{
		if (m_visible)
		{
			GFX::ShowConsole();
			// Draw lines
			for (int i = 0; i < m_numRows; i++)
			{
				int index = m_console.size() - 1 - i - m_offset;
				if (index >= 0 && index < m_console.size())
					GFX::RenderText(glm::vec2(10, 376 - i * 20), glm::vec2(6, 12), Colors::Silver, m_console[index].c_str());
			}

			// Draw input line
			GFX::RenderText(glm::vec2(10, 397), glm::vec2(6, 12), Colors::Silver, m_inputLine.c_str());
		}
		else
		{
			GFX::HideConsole();
		}
	}

	void DebugConsole::Show()
	{
		m_visible = true;
	}

	void DebugConsole::Hide()
	{
		m_visible = false;
	}
	void DebugConsole::Toggle()
	{
		if (m_visible)
			Hide();
		else
			Show();
	}

	DebugConsole& Console()
	{
		static DebugConsole console;
		return console;
	}

}
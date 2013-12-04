#include "console.hpp"
#include "clop.hpp"
#include <gfx/GFXInterface.hpp>
#include <utility/Colors.hpp>
#include <sstream>

namespace Core
{
	void ClopClearConsole(clop::ArgList args)
	{
		Console().Clear();
	}

	DebugConsole::DebugConsole()
	{
		m_visible = false;
		m_historyIndex = 0;
		m_offset = 0;
		clop::Register("clear", ClopClearConsole);
		clop::Register("clr", ClopClearConsole);
		
		Line line = {"Welcome to the console, have a nice day.", Colors::Gold};
		m_console.push_back(line);
	}
	DebugConsole::~DebugConsole()
	{

	}

	void DebugConsole::SetInputLine(const std::string& inputLine)
	{
		m_inputLine = inputLine;
	}

	void DebugConsole::Print(std::string str, Color color)
	{
		// Add lines
		std::string newLine;
		std::string tab = "\t";
		std::vector<std::string> lines;
		std::istringstream ss(str);
		while (!ss.eof())
		{
			std::getline(ss, newLine);
			
			//Find and replace \t with spaces
			std::size_t sIndex = newLine.find(tab);
			while (sIndex != newLine.npos)
			{
				newLine.replace(sIndex, tab.length(), "     ");
				sIndex = newLine.find(tab);
			}
			


			lines.push_back(newLine);
		}

		// Add all lines to the console
		Line l;
		l.color = color;
		for (unsigned int i = 0; i < lines.size(); i++)
		{
			l.text = lines[i];
			m_console.push_back(l);
		}

		// tabs
		// Add to console
	}

	void DebugConsole::Add()
	{
		if (!m_inputLine.empty())
		{
			// Add to console
			Line line = {m_inputLine, Colors::Silver};
			m_console.push_back(line);

			// Add command to history
			bool add = true;

			if ((int)m_history.size() > m_historyIndex && m_history[m_historyIndex].compare(m_inputLine) == 0)
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
			if (!clop::Command(m_inputLine))
			{
				Line errLine = {"ERROR: Unknown command", Colors::Red};
				m_console.push_back(errLine);
			}

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
		if (m_console.size() > 5)
		{
			m_offset += offset;
			if (m_offset < 0)
				m_offset = 0;
			if (m_offset >(int)m_console.size() - 5)
				m_offset = (int)m_console.size() - 5;
		}
		std::cout << m_offset << std::endl;
	}

#define DEBUG_LINE_NUMBER GFX::RenderText(glm::vec2(0, 376 - (i)* 15), 1.0f, color, ("[" + std::to_string(i) + "]").c_str());

	void DebugConsole::Update()
	{
		const int x = 40;
		if (m_visible)
		{
			GFX::ShowConsole();

			// Draw lines
			int lineIndex = m_console.size() - 1 - m_offset;
			int totalWraps = 0;
			for (int i = 0; i < m_numRows; i++)
			{

				if (lineIndex >= 0 && lineIndex < (int)m_console.size())
				{
					std::string line = m_console[lineIndex].text;
					Color color = m_console[lineIndex].color;
				

					int wrapLength = (GFX::GetScreenWidth()-x-20) / m_wrapCharWidth;
					int nrWraps = line.length() / wrapLength;

					if (nrWraps == 0) // Single line
					{
						DEBUG_LINE_NUMBER
						GFX::RenderText(glm::vec2(x, 376 - (i) * 15), 1.0f,color, line.c_str());
					}
					else // Wrapped lines
					{
						// Draw remainder
						int remainder = line.length() % wrapLength;
						if (remainder != 0)
						{
							DEBUG_LINE_NUMBER
							line = std::string(
								m_console[lineIndex].text.end() - remainder, 
								m_console[lineIndex].text.end());
							GFX::RenderText(glm::vec2(x, 376 - (i)* 15), 1.0f, color, line.c_str());
							i++;
						}

						// Draw wrapped lines
						for (int w = 0; w < nrWraps && i < m_numRows; w++)
						{
							DEBUG_LINE_NUMBER
							line = std::string(
								m_console[lineIndex].text.end() - remainder - wrapLength * (w+1), 
								m_console[lineIndex].text.end() - remainder - wrapLength * (w));
							GFX::RenderText(glm::vec2(x, 376 - (i) * 15), 1.0f, color, line.c_str());
							i++;
						}
						i--;
					}

				}
				lineIndex--;
			}

			// Draw input line
			GFX::RenderText(glm::vec2(10, 397), 1.0f, Colors::Silver, ("> " + m_inputLine).c_str());

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
	void DebugConsole::Clear()
	{
		m_console.clear();
	}
	void DebugConsole::ClearInput()
	{
		m_inputLine.clear();
	}

	DebugConsole& Console()
	{
		static DebugConsole console;
		return console;
	}

}
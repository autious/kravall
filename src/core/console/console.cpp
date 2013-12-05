#include "console.hpp"
#include "clop.hpp"
#include <gfx/GFXInterface.hpp>
#include <utility/Colors.hpp>
#include <sstream>

#include <Lua/LuaState.hpp>
#include <World.hpp>
#include <GLFWInput.hpp>

namespace Core
{
	void ClopClearConsole(clop::ArgList args)
	{
		Console().Clear();
	}
	
	void ClopLuaCommand( clop::ArgList args )
	{
		std::stringstream ss;
		for (int i = 1; i < args.size(); i++)
			ss << (std::string)args[i];
		std::string src = ss.str();
		Core::world.m_luaState.DoBlock(src.c_str());
	}

	DebugConsole::DebugConsole()
	{
		m_visible = false;
		m_historyIndex = 0;
		m_offset = 0;
		clop::Register("clear", ClopClearConsole);
		clop::Register("clr", ClopClearConsole);

		// Register lua
		clop::Register("lua", ClopLuaCommand);
		
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

	void DebugConsole::PrintLine(std::string str, Color color)
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
				std::istringstream ss(m_inputLine);
				std::string uCmd;
				ss >> uCmd;
				Line errLine = {"ERROR: Unknown command \'"+ uCmd +"\'", Colors::Red};
				m_console.push_back(errLine);
			}

			// Reset input line
			m_inputLine.clear();

			if (m_offset != 0)
				m_offset++;
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
			if (m_offset >(int)m_console.size() - 1)
				m_offset = (int)m_console.size() - 1;
		}
	}

	void DebugConsole::HandleInput()
	{
		if (Core::GetInput().IsKeyPressedOnce(GLFW_KEY_ESCAPE))
			ClearInput();
		
		if (Core::GetInput().IsKeyPressedOnce(GLFW_KEY_UP))
		{
			LastHistory();
		}
		if (Core::GetInput().IsKeyPressedOnce(GLFW_KEY_DOWN))
		{
			NextHistory();
		}
		if (Core::GetInput().IsKeyPressedOnce(GLFW_KEY_PAGE_UP) || Core::GetInput().GetScrollY() > 0)
			Scroll(1);
		if (Core::GetInput().IsKeyPressedOnce(GLFW_KEY_PAGE_DOWN) || Core::GetInput().GetScrollY() < 0)
			Scroll(-1);

		if (Core::GetInput().IsKeyPressedOnce(GLFW_KEY_ENTER))
		{
			Add();
		}
		if (Core::GetInput().IsKeyPressedOnce(GLFW_KEY_BACKSPACE))
		{
			if (m_inputLine.size() > 1)
				m_inputLine.erase(m_inputLine.end() - 1);
			else
			{
				m_inputLine.clear();
			}
		}

		char c = Core::GetInput().GetChar();

		if (c != 0)
		{
			m_inputLine.insert(m_inputLine.end(), 1, c);
		}
	}

	void DebugConsole::Update()
	{
		
		if (Core::GetInput().IsKeyPressedOnce(GLFW_KEY_TAB))
		{
			Toggle();
			Core::GetInput().SetCharCallback(Core::Console().IsVisible());
		}
		const int x = 10;
		if (m_visible)
		{
			HandleInput();

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
					totalWraps += nrWraps;

					if (nrWraps == 0) // Single line
					{
						GFX::RenderText(glm::vec2(x+1, 376+1 - (i) * 15), 1.0f,Colors::Black, line.c_str());
						GFX::RenderText(glm::vec2(x, 376 - (i) * 15), 1.0f,color, line.c_str());
					}
					else // Wrapped lines
					{
						// Draw remainder
						int remainder = line.length() % wrapLength;
						if (remainder != 0)
						{
							line = std::string(
								m_console[lineIndex].text.end() - remainder, 
								m_console[lineIndex].text.end());
							GFX::RenderText(glm::vec2(x+1, 376+1 - (i) * 15), 1.0f,Colors::Black, line.c_str());
							GFX::RenderText(glm::vec2(x, 376 - (i)* 15), 1.0f, color, line.c_str());
							i++;
						}

						// Draw wrapped lines
						for (int w = 0; w < nrWraps && i < m_numRows; w++)
						{
							line = std::string(
								m_console[lineIndex].text.end() - remainder - wrapLength * (w+1), 
								m_console[lineIndex].text.end() - remainder - wrapLength * (w));
							GFX::RenderText(glm::vec2(x+1, 376+1 - (i) * 15), 1.0f,Colors::Black, line.c_str());
							GFX::RenderText(glm::vec2(x, 376 - (i) * 15), 1.0f, color, line.c_str());
							i++;
						}
						i--;
					}

				}
				lineIndex--;
			}

			// Draw scroll indicator
			float sy = 80.0f + 300.0f / static_cast<float>(m_console.size() + 1);
			float dy = (380.0f - sy) - 380.0f * (m_offset/static_cast<float>(m_console.size() + 1));
			GFX::Debug::DrawRectangle(glm::vec2(0.0f, dy), glm::vec2(5.0f, sy), true, Colors::DarkGreen);

			// Draw input line
			GFX::RenderText(glm::vec2(11, 398), 1.0f, Colors::Black, ("> " + m_inputLine).c_str());
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
		m_offset = 0;
	}

	DebugConsole& Console()
	{
		static DebugConsole console;
		return console;
	}


}
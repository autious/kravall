#include "Console.hpp"
#include "CLOP.hpp"
#include <gfx/GFXInterface.hpp>
#include <utility/Colors.hpp>
#include <sstream>

#include <Lua/LuaState.hpp>
#include <World.hpp>
#include <Input/InputManager.hpp>

#include <Timer.hpp>
#include <logger/Logger.hpp>

#include <WindowHandling/GLFWInclude.hpp>

namespace Core
{
	void ClopClearConsole(clop::ArgList args)
	{
		Console().Clear();
	}
	
	void ClopShowSys(clop::ArgList args)
	{
		clop::Command("lua showSys()");
	}

	void ClopShowDebugLightVolumes(clop::ArgList args)
	{
		clop::Command("lua toggleLightVolumes()");
	}

	void ClopSetLUT(clop::ArgList args)
	{
		if (args.size() == 2)
		{
			std::string t = (std::string)args[1];
			
			size_t startpos = t.find_first_not_of(" \t");
			if (std::string::npos != startpos)
			{
				t = t.substr(startpos);
			}

			if (t == "reload")
				GFX::Content::ReloadLUT();
			else
				GFX::ColorSettings::SetLUT(t.c_str());
		}
	}
	
	void ClopShowFBO(clop::ArgList args)
	{
		if (args.size() == 2)
		{
			std::string cmd = "lua showFBO(";
			cmd += std::string(args[1]);
			cmd += std::string(")");
			clop::Command(cmd);
		}
		else
		{
			std::string errStr = "Usage: \'";
			errStr += std::string(args[0]);
			errStr += " n\' \n\tn:\n\t\t0: Display normal.\n\t\t1: Display miniature render targets over the rendered frame.\n\t\t2-5: Display a fullscreen render target.";
			Console().PrintLine(errStr, Colors::Chocolate);
		}
	}

	void ClopLipsum(clop::ArgList args)
	{
		int nr = 1;
		if (args.size() == 2)
			nr = (int)args[1];
		for (int i = 0; i < nr; i++)
			Console().PrintLine("Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.", Colors::Silver);
	}

	void ClopLuaCommand( clop::ArgList args )
	{
		std::stringstream ss;
		if (args.size() > 1)
		{
			for (unsigned int i = 1; i < args.size(); i++)
				ss << (std::string)args[i];
			std::string src = ss.str();
			Core::world.m_luaState.DoBlock(src.c_str());
		}
		else
		{
			Console().EnableInteractiveLua();
		}
	}

    const char* DebugConsole::HISTORY_FILE_NAME = "console_history_file.dbt";

    void DebugConsole::Init(GFX::FontData* font)
    {
        m_font = font;
    }

	DebugConsole::DebugConsole()
	{
		m_interactiveLua = false;
		m_visible = false;
		m_historyIndex = 0;
		m_offset = 0;
		m_cursorOffset = 0;
		m_lastCursorOffset = 0;
        m_font = nullptr;

		clop::Register("clear", ClopClearConsole);
		clop::Register("clr", ClopClearConsole);
		
		clop::Register("lipsum", ClopLipsum);

		clop::Register("lut", ClopSetLUT);

		// Register lua
		clop::Register("lua", ClopLuaCommand);
		
		clop::Register("sysinfo", ClopShowSys);
		clop::Register("lights", ClopShowDebugLightVolumes);
		clop::Register("fbo", ClopShowFBO);
		
		Line line = {"Welcome to the console, have a nice day.", Colors::Gold};
		m_console.push_back(line);

        //Load console history
        std::fstream hf( HISTORY_FILE_NAME, std::fstream::in );

        if( hf.is_open() )
        {
            std::string line;
            while( getline( hf, line ) )
            {
                m_history.push_back( line ); 
            }
            hf.close();
        }

		m_historyIndex = m_history.size();

        Core::GetInputManager().AddScrollEventListener( this );
        Core::GetInputManager().AddCharEventListener( this );
        Core::GetInputManager().AddKeyEventListener( this );
	}

	DebugConsole::~DebugConsole()
	{
        Core::GetInputManager().RemoveScrollEventListener( this );
        Core::GetInputManager().RemoveCharEventListener( this );
        Core::GetInputManager().RemoveKeyEventListener( this );
	}

	void DebugConsole::EnableInteractiveLua()
	{
		if (!m_interactiveLua)
			Console().PrintLine("Interactive lua enabled", Colors::Gold);
		m_interactiveLua = true;
	}

	void DebugConsole::DisableInteractiveLua()
	{
		if (m_interactiveLua)
			Console().PrintLine("Interactive lua disabled", Colors::Gold);
		m_interactiveLua = false;
	}

    void DebugConsole::OnScrollEvent( const Core::ScrollEvent &e )
    {
        if( IsVisible() )
        {
            Scroll(e.yoffset*10);
        }
    }

    void DebugConsole::OnCharEvent( const Core::CharEvent &e )
    {
        if( IsVisible() )
        {
            m_inputLine.insert(m_inputLine.begin()+m_cursorOffset, 1, (unsigned char)e.codepoint);
            m_cursorOffset++;
        }
    }

	void DebugConsole::PasteClipboard()
	{
		std::string clipBoard = glfwGetClipboardString(mainWindow);
		std::string newInputLine = m_inputLine;
		newInputLine.insert(m_cursorOffset, clipBoard);
		m_inputLine = newInputLine;
		m_cursorOffset = m_cursorOffset + clipBoard.length();
	}

    void DebugConsole::OnKeyEvent( const Core::KeyEvent &e )
    {
        if( IsVisible() )
        {
            if( e.action != GLFW_RELEASE )
            {
                if (e.key == GLFW_KEY_ESCAPE)
                    ClearInput();
                
                if (e.key == GLFW_KEY_UP)
                    LastHistory();

                if (e.key == GLFW_KEY_DOWN)
                    NextHistory();


                // Word jump functions
                if ( (e.mods & GLFW_MOD_CONTROL) > 0 )
                {
                    if (e.key == GLFW_KEY_LEFT)
                        JumpCursorLeft();

                    if (e.key == GLFW_KEY_RIGHT)
                        JumpCursorRight();

                    if (e.key == GLFW_KEY_DELETE)
                        DeleteWord();
					
                    if ( e.key == GLFW_KEY_BACKSPACE ) 
                        BackspaceWord();

					if ( e.key == GLFW_KEY_C )
						DisableInteractiveLua();

					if ( e.key == GLFW_KEY_V )
						PasteClipboard();
                }
                else
                {
                    if (e.key == GLFW_KEY_LEFT)
                        MoveCursorLeft();

                    if (e.key == GLFW_KEY_RIGHT)
                        MoveCursorRight();
                    
                    if (e.key == GLFW_KEY_DELETE)
                        DeleteLetter();

                    if ( e.key == GLFW_KEY_BACKSPACE  ) 
                        BackspaceLetter();
                }
                
                if (e.key == GLFW_KEY_HOME)
                    m_cursorOffset = 0;

                if (e.key == GLFW_KEY_END)
                    m_cursorOffset = m_inputLine.length();

                if (e.key == GLFW_KEY_PAGE_UP)
                    Scroll(10);

                if (e.key == GLFW_KEY_PAGE_DOWN)
                    Scroll(-10);

                if (e.key == GLFW_KEY_ENTER || e.key == GLFW_KEY_KP_ENTER )
                    Add();
            }
        }
    }

	void DebugConsole::SetInputLine(const std::string& inputLine)
	{
		m_inputLine = inputLine;
		m_cursorOffset = m_inputLine.length();
	}

    std::string DebugConsole::GetInputLine( )
    {
        return m_inputLine;
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
			PrintLine(m_inputLine, Colors::Silver);

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
                if( m_history.size() > HISTORY_LIMIT ) 
                {
                    m_history.erase( m_history.begin() );
                }
            
                std::fstream hf( HISTORY_FILE_NAME, std::fstream::out | std::fstream::trunc );
        
                for( std::vector<std::string>::iterator it = m_history.begin();
                        it != m_history.end();
                        it++ )
                {
                    hf << *it << std::endl;
                }

                hf.close();
                
				m_historyIndex = m_history.size();
			}

			// Execute command
			if (m_interactiveLua)
			{
				clop::Command("lua " + m_inputLine);
			}
			else if (!clop::Command(m_inputLine))
			{
				std::istringstream ss(m_inputLine);
				std::string uCmd;
				ss >> uCmd;
				LOG_ERROR << "Unknown command \'" + uCmd + "\'";
				//Line errLine = {"ERROR: Unknown command \'"+ uCmd +"\'", Colors::Red};
				//m_console.push_back(errLine);
			}

			// Reset input line
			m_inputLine.clear();
			m_cursorOffset = 0;

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
			m_cursorOffset = m_inputLine.length();
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
			m_cursorOffset = m_inputLine.length();
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
	
	void DebugConsole::MoveCursorLeft()
	{
		m_cursorOffset--;
		if (m_cursorOffset < 0)
			m_cursorOffset = 0;
	}

	void DebugConsole::MoveCursorRight()
	{
		m_cursorOffset++;
		if (m_cursorOffset > (int)m_inputLine.length())
			m_cursorOffset = m_inputLine.length();
	}

	void DebugConsole::JumpCursorLeft()
	{
		int pos = m_cursorOffset-1;
		if (pos < 0)
		{
			m_cursorOffset = 0;
			return;
		}

		bool isDelimiter = false;
		char del = '\0';

		//Check if delimiter
		for (unsigned int i = 0; i < m_delChars.length(); i++)
		{
			if (m_inputLine[pos] == m_delChars[i])
			{
				del = m_delChars[i];
				isDelimiter = true;
				break;
			}
		}

		if (isDelimiter)
		{
			bool found = false;
			while (pos > 0)
			{
				for (unsigned int i = 0; i < m_delChars.length(); i++)
				{

					if (m_inputLine[pos] != del)
					{
						m_cursorOffset = pos + 1;
						found = true;
						break;
					}
				}
				if (found)
					break;
				pos--;
			}
			if (!found)
				m_cursorOffset = 0;
		}
		else
		{
			bool found = false;
			while (pos > 0)
			{
				for (unsigned int i = 0; i < m_delChars.length(); i++)
				{
					if (m_inputLine[pos] == m_delChars[i])
					{
						m_cursorOffset = pos + 1;
						found = true;
						break;
					}
				}
				if (found)
					break;
				pos--;
			}
			if (!found)
				m_cursorOffset = 0;
		}

		if (m_cursorOffset < 0)
			m_cursorOffset = 0;
		if (m_cursorOffset > (int)m_inputLine.length())
			m_cursorOffset = m_inputLine.length();
	}

	void DebugConsole::JumpCursorRight()
	{
		int pos = m_cursorOffset;
		int l = m_inputLine.length();
		if (pos > l)
		{
			m_cursorOffset = l;
			return;
		}

		bool isDelimiter = false;
		char del = '\0';

		//Check if delimiter
		for (unsigned int i = 0; i < m_delChars.length(); i++)
		{
			if (m_inputLine[pos] == m_delChars[i])
			{
				del = m_delChars[i];
				isDelimiter = true;
				break;
			}
		}

		if (isDelimiter)
		{
			bool found = false;
			while (pos < l)
			{
				for (unsigned int i = 0; i < m_delChars.length(); i++)
				{

					if (m_inputLine[pos] != del)
					{
						m_cursorOffset = pos;
						found = true;
						break;
					}
				}
				if (found)
					break;
				pos++;
			}
			if (!found)
				m_cursorOffset = l;
		}
		else
		{
			bool found = false;
			while (pos < l)
			{
				for (unsigned int i = 0; i < m_delChars.length(); i++)
				{
					if (m_inputLine[pos] == m_delChars[i])
					{
						m_cursorOffset = pos;
						found = true;
						break;
					}
				}
				if (found)
					break;
				pos++;
			}
			if (!found)
				m_cursorOffset = l;
		}

		if (m_cursorOffset < 0)
			m_cursorOffset = 0;
		if (m_cursorOffset > (int)m_inputLine.length())
			m_cursorOffset = m_inputLine.length();
	}

	void DebugConsole::DeleteWord()
	{
		int pos = m_cursorOffset;
		int l = m_inputLine.length();
		int startPos = pos;
		if (pos > l)
		{
			m_cursorOffset = l;
			return;
		}

		bool isDelimiter = false;
		char del = '\0';

		//Check if delimiter
		for (unsigned int i = 0; i < m_delChars.length(); i++)
		{
			if (m_inputLine[pos] == m_delChars[i])
			{
				del = m_delChars[i];
				isDelimiter = true;
				break;
			}
		}

		if (isDelimiter)
		{
			bool found = false;
			while (pos < l)
			{
				for (unsigned int i = 0; i < m_delChars.length(); i++)
				{

					if (m_inputLine[pos] != del)
					{
						m_cursorOffset = pos;
						found = true;
						break;
					}
				}
				if (found)
					break;
				pos++;
			}
			if (!found)
				m_cursorOffset = l;
		}
		else
		{
			bool found = false;
			while (pos < l)
			{
				for (unsigned int i = 0; i < m_delChars.length(); i++)
				{
					if (m_inputLine[pos] == m_delChars[i])
					{
						m_cursorOffset = pos;
						found = true;
						break;
					}
				}
				if (found)
					break;
				pos++;
			}
			if (!found)
				m_cursorOffset = l;
		}

		// Remove
		if (m_cursorOffset < 0)
			m_cursorOffset = 0;
		if (m_cursorOffset > (int)m_inputLine.length())
			m_cursorOffset = m_inputLine.length();

		for (int i = 0; i <= m_cursorOffset - startPos - 1; i++)
			if (startPos < (int)m_inputLine.length())
			{
				m_inputLine.erase(m_inputLine.begin() + startPos);
			}
			else
			{
				break;
			}
			m_cursorOffset = startPos;
	}

	void DebugConsole::DeleteLetter()
	{
		if (m_cursorOffset < (int)m_inputLine.length())
		{
			m_inputLine.erase(m_inputLine.begin() + m_cursorOffset);
		}
	}
	
	void DebugConsole::BackspaceWord()
	{
		int pos = m_cursorOffset-1;
		int endPos = pos;
		if (pos < 0)
		{
			m_cursorOffset = 0;
			return;
		}

		bool isDelimiter = false;
		char del = '\0';

		//Check if delimiter
		for (unsigned int i = 0; i < m_delChars.length(); i++)
		{
			if (m_inputLine[pos] == m_delChars[i])
			{
				del = m_delChars[i];
				isDelimiter = true;
				break;
			}
		}

		if (isDelimiter)
		{
			bool found = false;
			while (pos > 0)
			{
				for (unsigned int i = 0; i < m_delChars.length(); i++)
				{

					if (m_inputLine[pos] != del)
					{
						m_cursorOffset = pos + 1;
						found = true;
						break;
					}
				}
				if (found)
					break;
				pos--;
			}
			if (!found)
				m_cursorOffset = 0;
		}
		else
		{
			bool found = false;
			while (pos > 0)
			{
				for (unsigned int i = 0; i < m_delChars.length(); i++)
				{
					if (m_inputLine[pos] == m_delChars[i])
					{
						m_cursorOffset = pos + 1;
						found = true;
						break;
					}
				}
				if (found)
					break;
				pos--;
			}
			if (!found)
				m_cursorOffset = 0;
		}

		if (m_cursorOffset < 0)
			m_cursorOffset = 0;
		if (m_cursorOffset > (int)m_inputLine.length())
			m_cursorOffset = m_inputLine.length();

		// Remove
		for (int i = 0; i <= endPos - m_cursorOffset; i++)
			if (m_inputLine.length() > 0)
			{
				m_inputLine.erase(m_inputLine.begin() + m_cursorOffset);
			}
			else
			{
				break;
			}

	}

	void DebugConsole::BackspaceLetter()
	{
		if (m_inputLine.length() > 0)
		{
			m_inputLine.erase(m_inputLine.begin() + m_cursorOffset - 1);
			m_cursorOffset--;
			if (m_cursorOffset < 0)
				m_cursorOffset = 0;
		}
	}

	void DebugConsole::Update()
	{
		if (m_lastCursorOffset != m_cursorOffset)
			m_timer.Reset();
		m_lastCursorOffset = m_cursorOffset;
		long long t = (m_timer.GetTotal()) % 1000;
		bool showCursor = (t < 500) ? true : false;

		if (Core::GetInputManager().IsKeyPressedOnce(GLFW_KEY_TAB))
		{
			Toggle();
		}
		const int x = 10;
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
				

					int wrapLength = int((GFX::GetScreenWidth()-x-20) / m_wrapCharWidth);
					int nrWraps = line.length() / wrapLength;
					totalWraps += nrWraps;

					if (nrWraps == 0) // Single line
					{
						GFX::RenderText(m_font, glm::vec2(x+1, 376+1 - (i) * 15), 1.0f,Colors::Black, line.c_str());
						GFX::RenderText(m_font, glm::vec2(x, 376 - (i) * 15), 1.0f,color, line.c_str());
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
							GFX::RenderText(m_font, glm::vec2(x+1, 376+1 - (i) * 15), 1.0f,Colors::Black, line.c_str());
							GFX::RenderText(m_font, glm::vec2(x, 376 - (i)* 15), 1.0f, color, line.c_str());
							i++;
						}

						// Draw wrapped lines
						for (int w = 0; w < nrWraps && i < m_numRows; w++)
						{
							line = std::string(
								m_console[lineIndex].text.end() - remainder - wrapLength * (w+1), 
								m_console[lineIndex].text.end() - remainder - wrapLength * (w));
							GFX::RenderText(m_font, glm::vec2(x+1, 376+1 - (i) * 15), 1.0f,Colors::Black, line.c_str());
							GFX::RenderText(m_font, glm::vec2(x, 376 - (i) * 15), 1.0f, color, line.c_str());
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

			// Append cursor to input line
			std::string outInputLine = m_interactiveLua ? "> " + m_inputLine : m_inputLine;
			if (showCursor)
				outInputLine.replace(m_cursorOffset + (int)m_interactiveLua * 2, 1, 1, '_');


			GFX::RenderText(m_font, glm::vec2(11, 398), 1.0f, Colors::Black, (outInputLine).c_str());
			GFX::RenderText(m_font, glm::vec2(10, 397), 1.0f, Colors::Silver, (outInputLine).c_str());

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
		m_offset = 0;
	}
	void DebugConsole::ClearInput()
	{
		m_inputLine.clear();
		m_offset = 0;
		m_cursorOffset = 0;
	}

	DebugConsole& Console()
	{
		static DebugConsole console;
		return console;
	}


}

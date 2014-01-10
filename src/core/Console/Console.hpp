#ifndef SRC_CORE_CONSOLE_CONSOLE_HPP
#define SRC_CORE_CONSOLE_CONSOLE_HPP

#include <vector>
#include <string>
#include <fstream>

#include <utility/Colors.hpp>
#include <gfx/FontData.hpp>
#include <Input/Interfaces/ScrollEventListener.hpp>
#include <Input/Interfaces/CharEventListener.hpp>
#include <Input/Interfaces/KeyEventListener.hpp>
#include <Timer.hpp>

namespace Core
{

	class DebugConsole : public ScrollEventListener, public CharEventListener, public KeyEventListener
	{
	public:
        /*!
            From CharEventListener
        */
        virtual void OnCharEvent( const Core::CharEvent &e );

        /*!
            From ScrollEventListener
        */
        virtual void OnScrollEvent( const Core::ScrollEvent &e );

        /*!
            From KeyEventListener
        */
        virtual void OnKeyEvent( const Core::KeyEvent &e );

        static const unsigned int HISTORY_LIMIT = 100;
        static const char * HISTORY_FILE_NAME;		
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
        Init function used to setting the console's font
        \param font A pointer to a GFX::FontData struct loaded by the ContentManager.
        */
        void Init(GFX::FontData* font);

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
		
		void EnableInteractiveLua();

		void DisableInteractiveLua();

	private:

		friend DebugConsole& Console();

		void MoveCursorLeft();
		void MoveCursorRight();
		void JumpCursorLeft();
		void JumpCursorRight();
		
		void DeleteWord();
		void DeleteLetter();

		void BackspaceWord();
		void BackspaceLetter();

		DebugConsole();
		~DebugConsole();

	private:

		bool m_visible;
		bool m_interactiveLua;

        GFX::FontData* m_font;

		// Current position in the history list
		int m_historyIndex;

		int m_offset;
		int m_cursorOffset;
		int m_lastCursorOffset;

		const std::string m_delChars = " +-*/\"\'.,!?(){}[]";

		const int m_numRows = 25;

		// "Width" of a character in the console to use as reference for wrapping text
		const float m_wrapCharWidth = 8.9f;

		std::string m_inputLine;
		std::vector<Line> m_console;
		std::vector<std::string> m_history;

		Core::HighresTimer m_timer;
	};
	DebugConsole& Console();

	// List of commands
	void ClopLuaCommand();

}

#endif

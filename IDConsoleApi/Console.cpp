#include <Console.hpp>
#undef min
#include <algorithm>

// -------------------------------------------------------------------------
// Used to wrap class method in a global function pointer for underlying API
// -------------------------------------------------------------------------
namespace
{
	Console* g_console = nullptr;
}

bool SetCtrlHandler_Wrapper(unsigned long ctrlEvent)
{
	if (g_console != nullptr)
		return g_console->_ctrlEventHandler->Execute(ctrlEvent);
	else
		return false;
}
// -------------------------------------------------------------------------

Console::Console()
{
	_console.SetInputMode(ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_WINDOW_INPUT);

	CONSOLE_SCREEN_BUFFER_INFO sbi = _console.GetScreenBufferInfo();
	_buffer.resize(sbi.dwSize.X * sbi.dwSize.Y);

	// Initialize global pointer for message wrapping
	g_console = this;
}

Console::~Console()
{
	g_console = nullptr;
}

Console& Console::Instance()
{
	static std::unique_ptr<Console> _instance(new Console);
	return *_instance;
}

std::shared_ptr<ConsoleState> Console::SaveState() const
{
	std::shared_ptr<ConsoleState> state(new ConsoleState);
	state->screenBufferInfo = _console.GetScreenBufferInfo();
	state->buffer.resize(state->screenBufferInfo.dwSize.X * state->screenBufferInfo.dwSize.Y);
	state->bufferCoord = COORD{ 0 };

	SMALL_RECT bufferRect{ 0 };
	bufferRect.Bottom = state->screenBufferInfo.dwSize.Y - 1;
	bufferRect.Bottom = state->screenBufferInfo.dwSize.X - 1;

	bufferRect = _console.ReadOutput(state->screenBufferInfo, state->buffer, state->bufferCoord);
	state->cursorInfo = _console.GetCursorInfo();

	state->title = _console.GetTitle();

	state->consoleMode = _console.GetMode();

	return state;
}

void Console::RestoreState(ConsoleState const& state)
{
	_console.SetInputMode(state.consoleMode);

	SMALL_RECT sr{ 0 };
	_console.SetWindowInfo(sr, true);
	_console.SetScreenBufferSize(state.screenBufferInfo.dwSize);
	_console.SetWindowInfo(state.screenBufferInfo.srWindow, true);

	_console.SetCursorInfo(state.cursorInfo);
	_console.SetCursorPosition(state.screenBufferInfo.dwCursorPosition);

	_console.WriteOutput(state.screenBufferInfo, state.buffer, state.bufferCoord);

	_console.SetTitle(state.title);

	_buffer.resize(state.screenBufferInfo.dwSize.X * state.screenBufferInfo.dwSize.Y);
}

void Console::Clear(unsigned short colour) const
{
	_console.FillOutputCharacter(' ', (unsigned short)_buffer.size(), COORD{ 0, 0 });
	_console.FillOutputAttribute(colour, (unsigned short)_buffer.size(), COORD{ 0, 0 });
}

void Console::SetTitle(std::string const& title) const
{
	_console.SetTitle(title);
}

void Console::SetSize(unsigned short width, unsigned short height)
{
	SMALL_RECT sr{ 0 };
	_console.SetWindowInfo(sr, true);

	COORD bufferSize;
	bufferSize.X = width;
	bufferSize.Y = height;
	_console.SetScreenBufferSize(bufferSize);

	CONSOLE_SCREEN_BUFFER_INFO sbi = _console.GetScreenBufferInfo();
	sr.Top = sr.Left = 0;
	width = std::min((short)width, sbi.dwMaximumWindowSize.X);
	height = std::min((short)height, sbi.dwMaximumWindowSize.Y);
	sr.Right = width - 1;
	sr.Bottom = height - 1;

	_console.SetWindowInfo(sr, true);

	// Resize buffer
	_buffer.resize(width * height);
}

void Console::SetCursorActive(bool active) const
{
	CONSOLE_CURSOR_INFO cci = _console.GetCursorInfo();
	cci.bVisible = (BOOL)active;
	_console.SetCursorInfo(cci);
}

void Console::SetCharacterAtCell(unsigned short x, unsigned short y, char ch) const
{
	CONSOLE_SCREEN_BUFFER_INFO sbi = _console.GetScreenBufferInfo();
	COORD loc{ 0 };
	loc.X = std::min((short)x, sbi.dwMaximumWindowSize.X);
	loc.Y = std::min((short)y, sbi.dwMaximumWindowSize.Y);
	_console.WriteOutputCharacter(ch, loc);
}

void Console::SetStringAtCell(unsigned short x, unsigned short y, std::string const& ch) const
{
	CONSOLE_SCREEN_BUFFER_INFO sbi = _console.GetScreenBufferInfo();
	COORD loc{ 0 };
	loc.X = std::min((short)x, sbi.dwMaximumWindowSize.X);
	loc.Y = std::min((short)y, sbi.dwMaximumWindowSize.Y);
	_console.WriteOutputCharacter(ch, loc);
}

void Console::SetColourAtCell(unsigned short x, unsigned short y, unsigned short colour) const
{
	CONSOLE_SCREEN_BUFFER_INFO sbi = _console.GetScreenBufferInfo();
	COORD loc{ 0 };
	loc.X = std::min((short)x, sbi.dwMaximumWindowSize.X);
	loc.Y = std::min((short)y, sbi.dwMaximumWindowSize.Y);
	_console.WriteOutputAttribute(colour, loc);
}

void Console::SetCtrlEventHandler(std::shared_ptr<CtrlCommand> command, bool add)
{
	_ctrlEventHandler = command;
	_console.SetCtrlEventHandler(SetCtrlHandler_Wrapper, add);
}

void Console::SetKeyEventHandler(std::shared_ptr<KeyCommand> command)
{
	_keyEventHandler = command;
}
void Console::SetMouseEventHandler(std::shared_ptr<MouseCommand> command)
{
	_mouseEventHandler = command;
}

void Console::Update()
{
	unsigned long numEvents = _console.ReadInput(_buffer);
	for (unsigned long iEv = 0; iEv < numEvents; ++iEv)
	{
		switch (_buffer[iEv].EventType)
		{
		case KEY_EVENT:
			if (_keyEventHandler != nullptr)
				_keyEventHandler->Execute(_buffer[iEv].Event.KeyEvent);
			break;

		case MOUSE_EVENT:
			if (_mouseEventHandler != nullptr)
				_mouseEventHandler->Execute(_buffer[iEv].Event.MouseEvent);
			break;
		}
	}
}
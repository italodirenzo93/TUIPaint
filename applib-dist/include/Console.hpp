#ifndef __CONSOLE__
#define __CONSOLE__

#include <Win32ConsoleAPI.hpp>
#include <Command.hpp>
#include <vector>
#include <memory>

class ConsoleState
{
private:
	friend class Console;

	CONSOLE_SCREEN_BUFFER_INFO	screenBufferInfo;
	CONSOLE_CURSOR_INFO			cursorInfo;
	std::vector<CHAR_INFO>		buffer;
	COORD						bufferCoord;
	unsigned long				consoleMode;
	unsigned short				consoleWidth;
	std::string					title;

	ConsoleState() { }
	ConsoleState(ConsoleState const& other) = delete;
	ConsoleState& operator= (ConsoleState const& other) = delete;

public:
	CONSOLE_SCREEN_BUFFER_INFO GetScreenBufferInfo() const { return screenBufferInfo; }
	CONSOLE_CURSOR_INFO GetCursorInfo() const { return cursorInfo; }
	unsigned long GetConsoleMode() const { return consoleMode; }
	unsigned short GetConsoleWidth() const { return consoleWidth; }
	std::string GetTitle() const { return title; }
};

class Console
{
private:
	Win32ConsoleAPI		_console;
	std::vector<INPUT_RECORD>	_buffer;

	std::shared_ptr<CtrlCommand>	_ctrlEventHandler;
	std::shared_ptr<KeyCommand>		_keyEventHandler;
	std::shared_ptr<MouseCommand>	_mouseEventHandler;

	Console();

	friend bool SetCtrlHandler_Wrapper(unsigned long ctrlEvent);

public:
	~Console();

	static Console& Instance();

	std::shared_ptr<ConsoleState> SaveState() const;
	void RestoreState(ConsoleState const& state);

	void Clear(unsigned short colour = 0) const;

	void SetTitle(std::string const& title) const;
	void SetSize(unsigned short width, unsigned short height);
	void SetCursorActive(bool active) const;
		
	void SetCharacterAtCell(unsigned short x, unsigned short y, char ch) const;
	void SetStringAtCell(unsigned short x, unsigned short y, std::string const& msg) const;
	void SetColourAtCell(unsigned short x, unsigned short y, unsigned short colour) const;

	void SetCtrlEventHandler(std::shared_ptr<CtrlCommand> command, bool add);
	void SetKeyEventHandler(std::shared_ptr<KeyCommand> command);
	void SetMouseEventHandler(std::shared_ptr<MouseCommand> command);
	void Update();
};

#endif
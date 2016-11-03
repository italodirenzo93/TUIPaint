#ifndef __WIN32CONSOLEAPI__
#define __WIN32CONSOLEAPI__

#include <string>
#include <vector>

#include <Windows.h>

class Win32ConsoleAPI
{
private:
	HANDLE		hInput_;
	HANDLE		hOutput_;

public:
	Win32ConsoleAPI();
	~Win32ConsoleAPI();

	CONSOLE_SCREEN_BUFFER_INFO GetScreenBufferInfo() const;
	CONSOLE_CURSOR_INFO GetCursorInfo() const;
	unsigned long GetMode() const;
	std::string GetTitle() const;
	unsigned long ReadInput(std::vector<INPUT_RECORD>& buffer) const;
	SMALL_RECT ReadOutput(CONSOLE_SCREEN_BUFFER_INFO const& bufferInfo, std::vector<CHAR_INFO>& buffer, COORD const& coord) const;

	void SetTitle(std::string const& title) const;
	void SetCtrlEventHandler(bool(*handler)(unsigned long), bool add) const;
	void SetScreenBufferSize(COORD const& size) const;
	void SetWindowInfo(SMALL_RECT const& windowRect, bool absolute) const;
	void SetCursorInfo(CONSOLE_CURSOR_INFO const& cursorInfo) const;
	void SetInputMode(unsigned long mode) const;
	void SetOutputMode(unsigned long mode) const;
	void SetCursorPosition(COORD const& pos) const;

	void WriteOutput(CONSOLE_SCREEN_BUFFER_INFO const& bufferInfo, std::vector<CHAR_INFO> const& buffer, COORD const& coord) const;
	unsigned long FillOutputCharacter(char ch, unsigned short bufferSize, COORD const& cursorPos) const;
	unsigned long FillOutputAttribute(unsigned short attr, unsigned short bufferSize, COORD const& cursorPos) const;
	unsigned long WriteOutputCharacter(char ch, COORD const& loc) const;
	unsigned long WriteOutputCharacter(std::string const& chars, COORD const& loc) const;
	unsigned long WriteOutputAttribute(unsigned short attr, COORD const& loc) const;
	unsigned long WriteOutputAttribute(std::vector<unsigned short> const& attr, COORD const& loc) const;
};

#endif
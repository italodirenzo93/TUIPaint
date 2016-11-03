#include <Win32ConsoleAPI.hpp>
#include <Win32ConsoleException.hpp>
#include <cassert>

Win32ConsoleAPI::Win32ConsoleAPI()
{
	hInput_ = GetStdHandle(STD_INPUT_HANDLE);
	hOutput_ = GetStdHandle(STD_OUTPUT_HANDLE);
}

Win32ConsoleAPI::~Win32ConsoleAPI()
{
	hInput_ = nullptr;
	hOutput_ = nullptr;
}

CONSOLE_SCREEN_BUFFER_INFO Win32ConsoleAPI::GetScreenBufferInfo() const
{
	CONSOLE_SCREEN_BUFFER_INFO csbi{ 0 };
	THROW_IF_CONSOLE_ERROR(GetConsoleScreenBufferInfo(hOutput_, &csbi));
	return csbi;
}

CONSOLE_CURSOR_INFO Win32ConsoleAPI::GetCursorInfo() const
{
	CONSOLE_CURSOR_INFO cci{ 0 };
	THROW_IF_CONSOLE_ERROR(GetConsoleCursorInfo(hOutput_, &cci));
	return cci;
}

unsigned long Win32ConsoleAPI::GetMode() const
{
	unsigned long mode = 0;
	THROW_IF_CONSOLE_ERROR(GetConsoleMode(hInput_, &mode));
	return mode;
}

std::string Win32ConsoleAPI::GetTitle() const
{
	char str[128];
	THROW_IF_CONSOLE_ERROR(GetConsoleTitleA(str, 128));
	return std::string(str);
}

unsigned long Win32ConsoleAPI::ReadInput(std::vector<INPUT_RECORD>& buffer) const
{
	assert(buffer.size() > 0);
	DWORD numEvent;
	THROW_IF_CONSOLE_ERROR(ReadConsoleInput(hInput_, buffer.data(), (DWORD)buffer.size(), &numEvent));
	return numEvent;
}

SMALL_RECT Win32ConsoleAPI::ReadOutput(CONSOLE_SCREEN_BUFFER_INFO const& bufferInfo, std::vector<CHAR_INFO>& buffer, COORD const & coord) const
{
	assert(buffer.size() > 0);

	SMALL_RECT sr{ 0 };
	sr.Bottom = bufferInfo.dwSize.Y - 1;
	sr.Right = bufferInfo.dwSize.X - 1;

	THROW_IF_CONSOLE_ERROR(ReadConsoleOutput(hOutput_, buffer.data(), bufferInfo.dwSize, coord, &sr));

	return sr;
}

void Win32ConsoleAPI::SetTitle(std::string const& title) const
{
	assert(title.size() <= 128);
	THROW_IF_CONSOLE_ERROR(SetConsoleTitleA((LPCSTR)title.c_str()));
}

void Win32ConsoleAPI::SetCtrlEventHandler(bool(*handler)(unsigned long), bool add) const
{
	THROW_IF_CONSOLE_ERROR(SetConsoleCtrlHandler((PHANDLER_ROUTINE)handler, add));
}

void Win32ConsoleAPI::SetScreenBufferSize(COORD const& size) const
{
	THROW_IF_CONSOLE_ERROR(SetConsoleScreenBufferSize(hOutput_, size));
}

void Win32ConsoleAPI::SetWindowInfo(SMALL_RECT const& windowRect, bool absolute) const
{
	THROW_IF_CONSOLE_ERROR(SetConsoleWindowInfo(hOutput_, (BOOL)absolute, &windowRect));
}

void Win32ConsoleAPI::SetCursorInfo(CONSOLE_CURSOR_INFO const & cursorInfo) const
{
	THROW_IF_CONSOLE_ERROR(SetConsoleCursorInfo(hOutput_, &cursorInfo));
}

void Win32ConsoleAPI::SetInputMode(unsigned long mode) const
{
	THROW_IF_CONSOLE_ERROR(SetConsoleMode(hInput_, mode));
}

void Win32ConsoleAPI::SetOutputMode(unsigned long mode) const
{
	THROW_IF_CONSOLE_ERROR(SetConsoleMode(hOutput_, mode));
}

void Win32ConsoleAPI::SetCursorPosition(COORD const & pos) const
{
	THROW_IF_CONSOLE_ERROR(SetConsoleCursorPosition(hOutput_, pos));
}

void Win32ConsoleAPI::WriteOutput(CONSOLE_SCREEN_BUFFER_INFO const & bufferInfo, std::vector<CHAR_INFO> const& buffer, COORD const & coord) const
{
	SMALL_RECT sr{ 0 };
	sr.Bottom = bufferInfo.dwSize.Y - 1;
	sr.Right = bufferInfo.dwSize.X - 1;
	THROW_IF_CONSOLE_ERROR(WriteConsoleOutput(hOutput_, buffer.data(), bufferInfo.dwSize, coord, &sr));
}

unsigned long Win32ConsoleAPI::FillOutputCharacter(char ch, unsigned short bufferSize, COORD const & cursorPos) const
{
	DWORD nCharsWritten;
	THROW_IF_CONSOLE_ERROR(FillConsoleOutputCharacterA(hOutput_, ch, bufferSize, cursorPos, &nCharsWritten));
	return nCharsWritten;
}

unsigned long Win32ConsoleAPI::FillOutputAttribute(unsigned short attr, unsigned short bufferSize, COORD const & cursorPos) const
{
	DWORD nCharsWritten;
	THROW_IF_CONSOLE_ERROR(FillConsoleOutputAttribute(hOutput_, attr, bufferSize, cursorPos, &nCharsWritten));
	return nCharsWritten;
}

unsigned long Win32ConsoleAPI::WriteOutputCharacter(char ch, COORD const& loc) const
{
	DWORD nCharsWritten;
	THROW_IF_CONSOLE_ERROR(WriteConsoleOutputCharacterA(hOutput_, &ch, 1, loc, &nCharsWritten));
	return nCharsWritten;
}

unsigned long Win32ConsoleAPI::WriteOutputCharacter(std::string const & chars, COORD const & loc) const
{
	DWORD nCharsWritten;
	THROW_IF_CONSOLE_ERROR(WriteConsoleOutputCharacterA(hOutput_, chars.c_str(), (DWORD)chars.size(), loc, &nCharsWritten));
	return nCharsWritten;
}

unsigned long Win32ConsoleAPI::WriteOutputAttribute(unsigned short attr, COORD const & loc) const
{
	DWORD nCharsWritten;
	THROW_IF_CONSOLE_ERROR(WriteConsoleOutputAttribute(hOutput_, &attr, 1, loc, &nCharsWritten));
	return nCharsWritten;
}

unsigned long Win32ConsoleAPI::WriteOutputAttribute(std::vector<unsigned short> const& attr, COORD const & loc) const
{
	DWORD nCharsWritten;
	THROW_IF_CONSOLE_ERROR(WriteConsoleOutputAttribute(hOutput_, attr.data(), (DWORD)attr.size(), loc, &nCharsWritten));
	return nCharsWritten;
}
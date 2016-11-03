#ifndef __WIN32CONSOLEEXCEPTION__
#define __WIN32CONSOLEEXCEPTION__

#include <Windows.h>
#include <string>

std::string ErrorDescription(DWORD dwMessageID);

class Win32ConsoleException
{
public:
	using id_type = decltype(GetLastError());
	using file_type = char const *;
	using string_type = std::string;
private:
	id_type code_;
	int line_;
	file_type file_;
public:
	Win32ConsoleException(int line, file_type file) : code_(GetLastError()), line_(line), file_(file) {}

	auto code() const -> id_type { return code_; }
	auto file() const -> file_type { return file_; }
	int line() const { return line_; }

	string_type msg() const;
};

#define THROW_CONSOLE_ERROR() throw Win32ConsoleException(__LINE__, __FILE__)
#define THROW_IF_CONSOLE_ERROR(res) if (!res) throw Win32ConsoleException(__LINE__, __FILE__)

#endif
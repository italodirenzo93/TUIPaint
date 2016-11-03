#include <Win32ConsoleException.hpp>
#include <sstream>

std::string ErrorDescription(DWORD dwMessageID) 
{
	char* msg;
	auto c = FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS |
		FORMAT_MESSAGE_MAX_WIDTH_MASK,

		NULL,
		dwMessageID,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&msg,
		0,
		NULL);

	std::string strMsg = (c == 0) ? "unknown" : msg;
	LocalFree(msg);
	return strMsg;
}

Win32ConsoleException::string_type Win32ConsoleException::msg() const 
{
	std::ostringstream oss;
	oss << "Error: " << code() << "\n";
	oss << ErrorDescription(code()) << "\n";
	oss << "In: " << file() << "\n";
	oss << "Line: " << line() << "\n";
	return oss.str();
}
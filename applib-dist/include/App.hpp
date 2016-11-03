#pragma once

// Everything needed for creating an app from this library
#include <string>
#include <vector>
#include <Windows.h>
#include <Console.hpp>

class App
{
private:
	using Args = std::vector<std::string>;
	Args _args;
	static App* thisApp;
	static int _main(int argc, char* argv[]);
	friend int main(int argc, char* argv[]);
protected:
	App();
	virtual ~App();
	Args const& GetArgs() const { return _args; }
	virtual int Execute() = 0;
};


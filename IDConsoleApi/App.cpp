#include <App.hpp>
#include <Win32ConsoleException.hpp>
#include <iostream>

int main(int argc, char* argv[]) try
{
	return App::_main(argc, argv);
}
catch (Win32ConsoleException const& e)
{
	std::cerr << e.msg() << std::endl;
}
catch (std::exception const& e)
{
	std::cerr << e.what() << std::endl;
}
catch (...)
{
	std::cerr << "An unknown error occurred." << std::endl;
}

App* App::thisApp = nullptr;

App::App()
{
	if (thisApp)
		throw std::exception("Error: App already initialized");
	thisApp = this;
}

App::~App()
{
}

int App::_main(int argc, char* argv[])
{
	for (int i = 1; i < argc; ++i)
		thisApp->_args.push_back(argv[i]);
	return thisApp->Execute();
}

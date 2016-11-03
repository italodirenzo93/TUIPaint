#include <App.hpp>
#include "Handlers.hpp"

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

class PaintApp : public App
{
private:
	std::shared_ptr<ConsoleState> prevConsoleState;

	bool done;
	bool mouseDown;
	unsigned short currentColour;

public:
	PaintApp() : App(),
		done(false),
		mouseDown(false),
		currentColour(BACKGROUND_RED)
	{
	}

	int Execute() override
	{
#if defined(_DEBUG)
		int dbgFlags = ::_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
		// bitwise or checks the block integrity on every memory call
		dbgFlags |= _CRTDBG_CHECK_ALWAYS_DF;
		//don't always remove blocks on delete
		dbgFlags |= _CRTDBG_DELAY_FREE_MEM_DF;
		//check for memory leaks at process terminatio
		dbgFlags |= _CRTDBG_LEAK_CHECK_DF;
		_CrtSetDbgFlag(dbgFlags);
#endif
		prevConsoleState = Console::Instance().SaveState();

		Console::Instance().SetSize(70, 50);
		Console::Instance().Clear(BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY);

		Console::Instance().SetCursorActive(false);
		Console::Instance().SetTitle("TUI Paint");

		Console::Instance().SetCtrlEventHandler(std::make_shared<CtrlHandler>(prevConsoleState, done), true);
		Console::Instance().SetKeyEventHandler(std::make_shared<KeyHandler>());
		Console::Instance().SetMouseEventHandler(std::make_shared<MouseHandler>(currentColour, mouseDown));

		while (!done) {
			Console::Instance().Update();
		}

		return EXIT_SUCCESS;
	}
} paintApp;

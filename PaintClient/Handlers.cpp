#include "Handlers.hpp"

#pragma region Control Key Handler
CtrlHandler::CtrlHandler(std::shared_ptr<ConsoleState> prevState, bool& done) 
	: _prevState(prevState), _done(done) { }

bool CtrlHandler::Execute(unsigned long ctrlEvent)
{
	switch (ctrlEvent)
	{
	case CTRL_C_EVENT:
		Console::Instance().RestoreState(*_prevState);
		_done = true;
		return true;

	default:
		return false;
	}
}
#pragma endregion

#pragma region Keyboard Handler
KeyHandler::KeyHandler() { }

void KeyHandler::Execute(KEY_EVENT_RECORD const& ker)
{
	if (ker.bKeyDown)
	{
		if (ker.uChar.AsciiChar == 'c' || ker.uChar.AsciiChar == 'C')
		{
			Console::Instance().Clear(BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
		}
	}
}
#pragma endregion

#pragma region Mouse Handler
MouseHandler::MouseHandler(unsigned short& colour, bool& mouseDown)
	: _colour(colour), _mouseDown(mouseDown) { }

void MouseHandler::Execute(MOUSE_EVENT_RECORD const& mer)
{
	switch (mer.dwEventFlags)
	{
	case 0:
	{
		if (mer.dwButtonState & RIGHTMOST_BUTTON_PRESSED)
		{
			switch (_colour)
			{
			case BACKGROUND_RED:
				_colour = BACKGROUND_GREEN;
				break;
			case BACKGROUND_GREEN:
				_colour = BACKGROUND_BLUE;
				break;
			case BACKGROUND_BLUE:
				_colour = BACKGROUND_RED;
				break;
			}
		}
		if (mer.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			Console::Instance().SetColourAtCell(mer.dwMousePosition.X, mer.dwMousePosition.Y, _colour | BACKGROUND_INTENSITY);
			_mouseDown = true;
		}
		if (mer.dwButtonState == 0)
			_mouseDown = false;
	}
	break;

	case MOUSE_MOVED:
	{
		if (_mouseDown)
		{
			Console::Instance().SetColourAtCell(mer.dwMousePosition.X, mer.dwMousePosition.Y, _colour | BACKGROUND_INTENSITY);
		}
	}
	break;
	}
}
#pragma endregion

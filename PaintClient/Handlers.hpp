#pragma once

#include <Command.hpp>
#include <Console.hpp>

class CtrlHandler : public CtrlCommand
{
private:
	std::shared_ptr<ConsoleState> _prevState;
	bool& _done;
public:
	CtrlHandler(std::shared_ptr<ConsoleState> prevState, bool& done);
	bool Execute(unsigned long ctrlEvent) override;
};

class KeyHandler : public KeyCommand
{
public:
	KeyHandler();
	void Execute(KEY_EVENT_RECORD const& ker) override;
};

class MouseHandler : public MouseCommand
{
	unsigned short& _colour;
	bool& _mouseDown;
public:
	MouseHandler(unsigned short& colour, bool& mouseDown);
	void Execute(MOUSE_EVENT_RECORD const& mer) override;
};
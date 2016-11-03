#pragma once

#include <Windows.h>
#include <iostream>

class CtrlCommand
{
public:
	virtual ~CtrlCommand() { }
	virtual bool Execute(unsigned long ctrlEvent) = 0;
};

class KeyCommand
{
public:
	virtual ~KeyCommand() { }
	virtual void Execute(KEY_EVENT_RECORD const& ker) = 0;
};

class MouseCommand
{
public:
	virtual ~MouseCommand() { }
	virtual void Execute(MOUSE_EVENT_RECORD const& ker) = 0;
};
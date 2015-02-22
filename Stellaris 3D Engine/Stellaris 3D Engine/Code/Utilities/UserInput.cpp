//====================================================================================
// FILE:    InputSystem.cpp ----------------------------------------------------------
// PURPOSE: Contains the events and methods handling of the InputSystem --------------
//          Class --------------------------------------------------------------------
//------------------------------------------------------------------------------------


//====================================================================================
// INTERNAL INCLUDES -----------------------------------------------------------------
//------------------------------------------------------------------------------------
#include "UserInput.h"


//====================================================================================
// GLOBALS ---------------------------------------------------------------------------
//------------------------------------------------------------------------------------
EKeyState garrKeyStates[MAX_KEY_CODES];


//====================================================================================
// INITIALISATION --------------------------------------------------------------------
//------------------------------------------------------------------------------------
void InitialiseInput()
{
	// Initialise the input data
	for (int i = 0; i < MAX_KEY_CODES; ++i)
	{
		garrKeyStates[i] = KEY_NOT_PRESSED;
	}
}


//====================================================================================
// KEY PRESS EVENTS ------------------------------------------------------------------
//------------------------------------------------------------------------------------
void KeyPressedEvent(EKeyState keyCode)
{
	if (garrKeyStates[keyCode] == KEY_NOT_PRESSED /*|| garrKeyStates[keyCode] == KEY_JUST_RELEASED*/)
	{
		garrKeyStates[keyCode] = KEY_PRESSED;
	}
	else
	{
		garrKeyStates[keyCode] = KEY_HELD;
	}
}

//void KeyReleaseEvent(EKeyState keyCode)
//{
//	garrKeyStates[keyCode] = KEY_JUST_RELEASED;
//}


//====================================================================================
// INPUT FUNCTIONS -------------------------------------------------------------------
//------------------------------------------------------------------------------------
bool KeyPressed(EKeyCode keyCode)
{
	if (garrKeyStates[keyCode] == KEY_PRESSED)
	{
		garrKeyStates[keyCode] = KEY_HELD;
		return true;
	}
	return false;
}

bool KeyDown(EKeyCode keyCode)
{
	if (garrKeyStates[keyCode] == KEY_NOT_PRESSED /*|| garrKeyStates[keyCode] == KEY_JUST_RELEASED*/)
	{
		return false;
	}
	garrKeyStates[keyCode] = KEY_HELD;
	return true;
}

//bool KeyReleased(EKeyCode keyCode)
//{
//	if (garrKeyStates[keyCode] == KEY_JUST_RELEASED)
//	{
//		garrKeyStates[keyCode] = KEY_NOT_PRESSED;
//		return true;
//	}
//	return false;
//}

bool KeyNotDown(EKeyCode keyCode)
{
	if (/*garrKeyStates[keyCode] == KEY_JUST_RELEASED ||*/ garrKeyStates[keyCode] == KEY_NOT_PRESSED)
	{
		return true;
	}
	return false;
}

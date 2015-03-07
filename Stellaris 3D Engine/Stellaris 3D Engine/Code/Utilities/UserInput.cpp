//====================================================================================
// FILE:    InputSystem.cpp ----------------------------------------------------------
//------------------------------------------------------------------------------------

#include "UserInput.h"


namespace sge
{
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
	void KeyDownEvent(EKeyCode Key)
	{
		if (garrKeyStates[Key] == KEY_NOT_PRESSED)
		{
			garrKeyStates[Key] = KEY_PRESSED;
		}
		else
		{
			garrKeyStates[Key] = KEY_HELD;
		}
	}

	void KeyUpEvent(EKeyCode Key)
	{
		garrKeyStates[Key] = KEY_NOT_PRESSED;
	}


	//====================================================================================
	// INPUT FUNCTIONS -------------------------------------------------------------------
	//------------------------------------------------------------------------------------
	bool KeyHit(EKeyCode eKeyCode)
	{
		if (garrKeyStates[eKeyCode] == KEY_PRESSED)
		{
			garrKeyStates[eKeyCode] = KEY_HELD;
			return true;
		}
		return false;
	}

	bool KeyHeld(EKeyCode eKeyCode)
	{
		if (garrKeyStates[eKeyCode] == KEY_NOT_PRESSED)
		{
			return false;
		}
		garrKeyStates[eKeyCode] = KEY_HELD;
		return true;
	}
}

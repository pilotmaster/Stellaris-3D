//====================================================================================
// FILE:    InputSystem.cpp ----------------------------------------------------------
//------------------------------------------------------------------------------------

#include "UserInput.h"


namespace sge
{
	//====================================================================================
	// USER INPUT CLASS CONSTRUCTOR & DESTRUCTOR
	//------------------------------------------------------------------------------------
	CUserInput::CUserInput()
	{
		// Initialise the input data
		for (int i = 0; i < MAX_KEY_CODES; ++i)
		{
			mKeyStates[i] = KEY_NOT_PRESSED;
		}
	}

	CUserInput::~CUserInput()
	{

	}


	//====================================================================================
	// USER INPUT CLASS KEY PRESS EVENTS
	//------------------------------------------------------------------------------------
	void CUserInput::KeyDownEvent(EKeyCode Key)
	{
		if (mKeyStates[Key] == KEY_NOT_PRESSED)
		{
			mKeyStates[Key] = KEY_PRESSED;
		}
		else
		{
			mKeyStates[Key] = KEY_HELD;
		}
	}

	void CUserInput::KeyUpEvent(EKeyCode Key)
	{
		mKeyStates[Key] = KEY_NOT_PRESSED;
	}


	//====================================================================================
	// USER INPUT CLASS METHODS
	//------------------------------------------------------------------------------------
	bool CUserInput::KeyHit(EKeyCode eKeyCode)
	{
		if (mKeyStates[eKeyCode] == KEY_PRESSED)
		{
			mKeyStates[eKeyCode] = KEY_HELD;
			return true;
		}
		return false;
	}

	bool CUserInput::KeyHeld(EKeyCode eKeyCode)
	{
		if (mKeyStates[eKeyCode] == KEY_NOT_PRESSED)
		{
			return false;
		}
		mKeyStates[eKeyCode] = KEY_HELD;
		return true;
	}
}

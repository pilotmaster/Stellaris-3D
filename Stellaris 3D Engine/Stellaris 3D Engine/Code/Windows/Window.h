//====================================================================================
// FILE:    Window.h -----------------------------------------------------------------
//------------------------------------------------------------------------------------

#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <string>

#include "..\Utilities\Timer.h"
#include "..\Utilities\UserInput.h"

#define WIN32_LEAN_AND_MEAN


namespace sge
{
	//====================================================================================
	// WINDOW CLASS -----------------------------------------------------------------------
	//------------------------------------------------------------------------------------
	class CWindow
	{
	public:
		// CONSTRUCTORS & DESTRUCTORS
		//---------------------------------
		CWindow();
		~CWindow();


		// WINDOWS MESSAGE PROCEDURE
		//---------------------------------
		bool InitialiseWindow(HINSTANCE hInstance, UINT wndWidth, UINT wndHeight, std::wstring wndTitle);


		// WINDOWS MESSAGE PROCEDURE
		//---------------------------------
		LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


		// ACCESSORS
		//---------------------------------
		inline bool WindowPaused()
		{
			return mWindowState.mIsPaused;
		}

		inline bool WindowMinimised()
		{
			return mWindowState.mIsMinimised;
		}

		inline bool WindowMaxmised()
		{
			return mWindowState.mIsMaximised;
		}

		inline bool WindowResizing()
		{
			return mWindowState.mIsResizing;
		}

		inline bool WindowRunning()
		{
			return mWindowState.mIsRunning;
		}


		// MUTATORS
		//---------------------------------



	private:
		// ATTRIBUTES
		//---------------------------------
		union UWindowStates
		{
			bool mIsPaused;
			bool mIsMinimised;
			bool mIsMaximised;
			bool mIsResizing;
			bool mIsRunning;
		} mWindowState;

		HWND mHanWindow;
		HINSTANCE mHanInstance;
		UINT mClientWidth;
		UINT mClientHeight;
		std::wstring mWndTitle;
		DWORD mWndStyle;
	};
}


#endif /* _WINDOW_H_ */

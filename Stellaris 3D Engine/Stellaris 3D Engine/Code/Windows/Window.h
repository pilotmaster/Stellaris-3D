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
		// DESTRUCTOR
		//---------------------------------
		~CWindow();


		// WINDOWS MESSAGE PROCEDURE
		//---------------------------------
		bool InitialiseWindow(HINSTANCE hInstance, UINT wndWidth, UINT wndHeight, std::wstring wndTitle);


		// WINDOWS MESSAGE PROCEDURE
		//---------------------------------
		LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


		// ACCESSORS
		//---------------------------------
		static CWindow* GetInstance()
		{
			// If an instance does not exist, create one
			if (!mpWndInstance)
			{
				mpWndInstance = new CWindow();
			}

			return mpWndInstance;
		}

		inline bool WindowPaused()
		{
			return mIsPaused;
		}

		inline bool WindowMinimised()
		{
			return mIsMinimised;
		}

		inline bool WindowMaximised()
		{
			return mIsMaximised;
		}

		inline bool WindowResizing()
		{
			return mIsResizing;
		}

		inline bool WindowRunning()
		{
			return mIsRunning;
		}


		// MUTATORS
		//---------------------------------



	private:
		// PRIVATE CONSTRUCTOR
		//---------------------------------
		CWindow();

		static CWindow* mpWndInstance;


		// ATTRIBUTES
		//---------------------------------
		bool mIsPaused;
		bool mIsMinimised;
		bool mIsMaximised;
		bool mIsResizing;
		bool mIsRunning;

		HWND mHanWindow;
		HINSTANCE mHanInstance;
		UINT mClientWidth;
		UINT mClientHeight;
		std::wstring mWndTitle;
		DWORD mWndStyle;
	};
}


#endif /* _WINDOW_H_ */

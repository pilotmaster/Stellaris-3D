//====================================================================================
// FILE:    Window.cpp ---------------------------------------------------------------
//------------------------------------------------------------------------------------

#include "Window.h"

namespace sge
{
	//====================================================================================
	// WINDOWS CALLBACK FUNCTION ---------------------------------------------------------
	//------------------------------------------------------------------------------------
	LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{

	}

	
	//====================================================================================
	// CONSTRUCTOR & DESTRUCTOR ----------------------------------------------------------
	//------------------------------------------------------------------------------------
	CWindow::CWindow()
	{
		// Default values
		mWindowState.mIsMaximised = false;
		mWindowState.mIsMinimised = false;
		mWindowState.mIsPaused = false;
		mWindowState.mIsResizing = false;
		mWindowState.mIsRunning = false;

		mHanWindow = 0;
		mHanInstance = 0;
		mClientWidth = 600;
		mClientHeight = 480;
		mWndTitle = L"";
		mWndStyle = WS_OVERLAPPED;
	}

	CWindow::~CWindow()
	{

	}


	//====================================================================================
	// INITIALISATION --------------------------------------------------------------------
	//------------------------------------------------------------------------------------
	bool CWindow::InitialiseWindow(HINSTANCE hInstance, UINT wndWidth, UINT wndHeight, std::wstring wndTitle)
	{

	}


	//====================================================================================
	// WINDOWS MESSAGE PROCEDURE ---------------------------------------------------------
	//------------------------------------------------------------------------------------
	LRESULT CWindow::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		PAINTSTRUCT ps;
		HDC hdc;

		// Check the message which has been sent to the application
		switch (msg)
		{
		case WM_ACTIVATE:
			if (LOWORD(wParam) == WA_INACTIVE)
			{
				// Pause the app if it becomes inactive
				mWindowState.mIsPaused = true;
				mpAppTimer->Stop();
			}
			else
			{
				// Ensure unpaused when active
				mAppIsPaused = false;
				mpAppTimer->Start();
			}
			break;

		case WM_SIZE:
			mClientWidth = LOWORD(lParam);
			mClientHeight = HIWORD(lParam);

			// Check device
			if (gpDevice)
			{
				if (wParam == SIZE_MINIMIZED)
				{
					// App is minimised
					mAppIsPaused = true;
					mAppIsMinimised = true;
					mAppIsMaximised = false;
				}
				else if (wParam == SIZE_MAXIMIZED)
				{
					// App is maximised
					mAppIsPaused = false;
					mAppIsMinimised = false;
					mAppIsMaximised = true;
				}
				else if (wParam == SIZE_RESTORED)
				{
					if (mAppIsMinimised)
					{
						// Restoring from minimised
						mAppIsPaused = false;
						mAppIsMinimised = false;
					}
					else if (mAppIsMaximised)
					{
						// Restoring from maximised
						mAppIsPaused = false;
						mAppIsMaximised = false;
					}
				}
			}
			break;

		case WM_ENTERSIZEMOVE:
			// User is resizing the window
			mAppIsPaused = true;
			mAppIsResizing = true;
			mpAppTimer->Stop();
			break;

		case WM_EXITSIZEMOVE:
			// User has released resize handles
			// User is resizing the window
			mAppIsPaused = false;
			mAppIsResizing = false;
			mpAppTimer->Start();
			break;

		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;

		case WM_DESTROY:
			// Message to quit the application
			mAppIsRunning = false;
			PostQuitMessage(0);
			break;

		case WM_MENUCHAR:
			// Full screen application instead of beeping
			return MAKELRESULT(0, MNC_CLOSE);
			break;


			// KEY PRESSES
			//------------------------------
		case WM_KEYDOWN:
			KeyPressedEvent(static_cast<EKeyState>(wParam));
			break;

		case WM_KEYUP:
			break;

		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
			break;

		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
			break;

		case WM_MOUSEMOVE:
			break;

		default:
			// hand over message to window handle
			return DefWindowProc(hWnd, inMsg, wParam, lParam);
			break;
		}

		return 0;
	}
}

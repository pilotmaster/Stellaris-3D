//====================================================================================
// FILE:    Window.cpp ---------------------------------------------------------------
//------------------------------------------------------------------------------------

#include "Window.h"


//====================================================================================
// WINDOWS CALLBACK FUNCTION ---------------------------------------------------------
//------------------------------------------------------------------------------------
LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

}


namespace sge
{
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
		// Create a window class
		WNDCLASS wc;


		// SET WINDOW OBJECT PROPERTIES
		//------------------------------
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.hInstance = mHanInstance;
		wc.lpfnWndProc = MainWndProc;
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = L"StellarisApp";

		// Register window class with OS
		if (!RegisterClass(&wc))
		{
			// Failed to register class - return false
			MessageBox(mHanWindow, L"Failed to register class with the OS.", L"CLASS NOT REGISTERED", MB_OK);
			return false;
		}


		// CREATE VIEWPORT RECTANGLE
		//------------------------------
		mClientWidth = wndWidth;
		mClientHeight = wndHeight;

		// Rectangle for size of window
		RECT r = { 0, 0, mClientWidth, mClientHeight };
		// Create window rectangle based on size, style, and whether there is a menu
		AdjustWindowRect(&r, mWndStyle, false);
		// Determine correct height and width
		float width = r.right - r.left;
		float height = r.bottom - r.top;


		// CREATE WINDOW
		//------------------------------
		mWndTitle = wndTitle;

		mHanWindow = CreateWindow(L"DXAPP", mWndTitle.c_str, mWndStyle, CW_USEDEFAULT, CW_USEDEFAULT,
			width, height, NULL, NULL, mHanInstance, NULL);
		// Check if the window itself was created
		if (!mHanWindow)
		{
			// Failed to create the window
			MessageBox(mHanWindow, L"Failed to create window.", L"WINDOW CREATION ERROR", MB_OK);
			return false;
		}

		// Show the window
		ShowWindow(mHanWindow, SW_SHOW);
		UpdateWindow(mHanWindow);

		// Everything succeeded
		mWindowState.mIsRunning = true;
		return true;
	}


	//====================================================================================
	// WINDOWS MESSAGE PROCEDURE ---------------------------------------------------------
	//------------------------------------------------------------------------------------
	LRESULT CWindow::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		PAINTSTRUCT ps;
		HDC hdc;
		CTimer* pTimer = CTimer::GetTimerInstace();


		// Check the message which has been sent to the application
		switch (msg)
		{
		case WM_ACTIVATE:
			if (LOWORD(wParam) == WA_INACTIVE)
			{
				// Pause the app if it becomes inactive
				mWindowState.mIsPaused = true;
				pTimer->Stop();
			}
			else
			{
				// Ensure unpaused when active
				mWindowState.mIsPaused = false;
				pTimer->Start();
			}
			break;

		case WM_SIZE:
			mClientWidth = LOWORD(lParam);
			mClientHeight = HIWORD(lParam);

			if (wParam == SIZE_MINIMIZED)
			{
				// App is minimised
				mWindowState.mIsPaused = true;
				mWindowState.mIsMinimised = true;
				mWindowState.mIsMaximised = false;
			}
			else if (wParam == SIZE_MAXIMIZED)
			{
				// App is maximised
				mWindowState.mIsPaused = false;
				mWindowState.mIsMinimised = false;
				mWindowState.mIsMaximised = true;
			}
			else if (wParam == SIZE_RESTORED)
			{
				if (WindowMinimised())
				{
					// Restoring from minimised
					mWindowState.mIsPaused = false;
					mWindowState.mIsMinimised = false;
				}
				else if (WindowMaximised())
				{
					// Restoring from maximised
					mWindowState.mIsPaused = false;
					mWindowState.mIsMaximised = false;
				}
			}
			break;

		case WM_ENTERSIZEMOVE:
			// User is resizing the window
			mWindowState.mIsPaused = true;
			mWindowState.mIsResizing = true;
			pTimer->Stop();
			break;

		case WM_EXITSIZEMOVE:
			// User has released resize handles
			// User is resizing the window
			mWindowState.mIsPaused = false;
			mWindowState.mIsResizing = false;
			pTimer->Start();
			break;

		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;

		case WM_DESTROY:
			// Message to quit the application
			mWindowState.mIsRunning = false;
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
			return DefWindowProc(hWnd, msg, wParam, lParam);
			break;
		}

		return 0;
	}
}

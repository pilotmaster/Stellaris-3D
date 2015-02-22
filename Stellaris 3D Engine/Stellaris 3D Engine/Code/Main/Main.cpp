//====================================================================================
// FILE:    Main.cpp -----------------------------------------------------------------
//------------------------------------------------------------------------------------
#include "..\DirectX\Engine.h"


//====================================================================================
// WIN32 ENTRY POINT -----------------------------------------------------------------
//------------------------------------------------------------------------------------
int WINAPI WinMain(__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd)
{
	sge::CWindow* pWindow = sge::CWindow::GetInstance();

	pWindow->InitialiseWindow(hInstance, 1600U, 900U, L"Test");

	while (pWindow->WindowRunning())
	{

	}

	delete pWindow;
}

//====================================================================================
// FILE:    Main.cpp -----------------------------------------------------------------
//------------------------------------------------------------------------------------
#include "..\DirectX\Engine.h"


//====================================================================================
// WIN32 ENTRY POINT -----------------------------------------------------------------
//------------------------------------------------------------------------------------
int WINAPI WinMain(__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd)
{
	// CRETAE INSTANCE OF ENGINE	
	sge::CStellaris3D* pEngine = new sge::CStellaris3D();

	if (!pEngine->InitialiseEngine(hInstance))
	{
		delete pEngine;
		return 0;
	}

	
	// SETUP
	sge::CMesh* mshTest = pEngine->LoadMesh("Cube.x");
	sge::CModel* mdlTest = pEngine->CreateModel(mshTest);


	// ENGINE LOOP
	while (pEngine->EngineRunning())
	{
		// Run the application
		pEngine->Run();
	}

	delete pEngine;

	return 0;
}

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
	sge::CCamera* camMain = pEngine->CreateCamera();

	DirectX::XMFLOAT3 pos{ 0.0f, 0.0f, 50.0f };
	sge::CMesh* mshTest = pEngine->LoadMesh("Cube.x");
	sge::CModel* mdlTest = pEngine->CreateModel(mshTest, pos);


	// ENGINE LOOP
	while (pEngine->Run())
	{
		// Render scene
		pEngine->Render(camMain);
	}

	delete pEngine;

	return 0;
}

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
	DirectX::XMFLOAT3 camPos{ -15.0f, 20.0f, -40.0f };
	sge::CCamera* camMain = pEngine->CreateCamera(camPos);

	DirectX::XMFLOAT3 modelPos{ 0.0f, 10.0f, 0.0f };
	sge::CMesh* mshTest = pEngine->LoadMesh("Cube.x");
	sge::CModel* mdlTest = pEngine->CreateModel(mshTest, modelPos);


	// ENGINE LOOP
	while (pEngine->Run())
	{
		// Render scene
		pEngine->Render(camMain);
	}

	delete pEngine;

	return 0;
}

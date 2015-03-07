//====================================================================================
// FILE:    Main.cpp -----------------------------------------------------------------
//------------------------------------------------------------------------------------
#include "..\DirectX\Engine.h"


//====================================================================================
// WIN32 ENTRY POINT -----------------------------------------------------------------
//------------------------------------------------------------------------------------
int WINAPI WinMain(__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd)
{
	// ENGINE CREATION & INITIALISATION
	//---------------------------------
	sge::CStellaris3D* pEngine = new sge::CStellaris3D();

	if (!pEngine->InitialiseEngine(hInstance))
	{
		delete pEngine;
		return 0;
	}

	
	// SCENE SET UP
	//---------------------------------
	sge::CCamera* camMain = pEngine->CreateCamera(DirectX::XMFLOAT3(-15.0f, 20.0f, -40.0f));

	sge::CMesh* mshCube = pEngine->LoadMesh("Media\\Cube.x");
	sge::CModel* mdlCube = pEngine->CreateModel(mshCube, DirectX::XMFLOAT3(0.0f, 10.0f, 0.0f));

	sge::CMesh* mshFloor = pEngine->LoadMesh("Media\\Floor.x");
	sge::CModel* mdlFloor = pEngine->CreateModel(mshFloor);

	// movement variables
	const float MOVE_SPEED = 50.0f;
	const float ROTATE_SPEED = 150.0f;

	// frame time variable
	float delta = 0.0f; sge::CTimer::GetTimerInstace()->GetDeltaTime();


	// MAIN GAME LOOP
	//---------------------------------
	while (pEngine->Run())
	{
		// RENDER CURRENT SCENE
		//---------------------------------
		pEngine->Render(camMain);

		// Update delta time 
		delta = sge::CTimer::GetTimerInstace()->GetDeltaTime();


		// CHECK FOR KEY PRESSES
		//---------------------------------
		// Local camera movement
		if (sge::KeyHeld(sge::KEY_W))
		{
			camMain->MoveLocalZ(MOVE_SPEED * delta);
		}

		if (sge::KeyHeld(sge::KEY_S))
		{
			camMain->MoveLocalZ(-MOVE_SPEED * delta);
		}

		if (sge::KeyHeld(sge::KEY_A))
		{
			camMain->MoveLocalX(-MOVE_SPEED * delta);
		}

		if (sge::KeyHeld(sge::KEY_D))
		{
			camMain->MoveLocalX(MOVE_SPEED * delta);
		}

		// Camera rotation
		if (sge::KeyHeld(sge::KEY_UP))
		{
			camMain->RotateX(-ROTATE_SPEED * delta);
		}

		if (sge::KeyHeld(sge::KEY_DOWN))
		{
			camMain->RotateX(ROTATE_SPEED * delta);
		}

		if (sge::KeyHeld(sge::KEY_LEFT))
		{
			camMain->RotateY(-ROTATE_SPEED * delta);
		}

		if (sge::KeyHeld(sge::KEY_RIGHT))
		{
			camMain->RotateY(ROTATE_SPEED * delta);
		}
	}

	delete pEngine;

	return 0;
}

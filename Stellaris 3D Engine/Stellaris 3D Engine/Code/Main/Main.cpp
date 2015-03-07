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
	// movement variables
	const float MOVE_SPEED = 50.0f;
	const float ROTATE_SPEED = 150.0f;
	const float ORBIT_RADIUS = 20.0f;
	const float ORBIT_SPEED = 0.5f;

	// Model loading
	sge::CCamera* camMain = pEngine->CreateCamera(DirectX::XMFLOAT3(-15.0f, 20.0f, -40.0f));

	sge::CMesh* mshCube = pEngine->LoadMesh("Media\\Cube.x");
	sge::CModel* mdlCube = pEngine->CreateModel(mshCube, DirectX::XMFLOAT3(0.0f, 10.0f, 0.0f));

	sge::CMesh* mshFloor = pEngine->LoadMesh("Media\\Floor.x");
	sge::CModel* mdlFloor = pEngine->CreateModel(mshFloor);

	sge::CMesh* mshLight = pEngine->LoadMesh("Media\\Light.x");
	sge::CModel* mdlLight1 = pEngine->CreateModel(mshLight);
	sge::CModel* mdlLight2 = pEngine->CreateModel(mshLight, DirectX::XMFLOAT3(-20.0f, 30.0f, 50.0f));

	// frame time variable
	float delta = 0.0f; sge::CTimer::GetTimerInstace()->GetDeltaTime();
	float rotation = 0.0f;


	// MAIN GAME LOOP
	//---------------------------------
	while (pEngine->Run())
	{
		// RENDER CURRENT SCENE
		//---------------------------------
		pEngine->Render(camMain);

		// Update delta time & light orbit rotation
		delta = sge::CTimer::GetTimerInstace()->GetDeltaTime();
		rotation -= ORBIT_SPEED * delta;


		// UPDATE MODEL POSITIONS
		//---------------------------------
		mdlLight1->SetPosition(DirectX::XMFLOAT3(cosf(rotation) * ORBIT_RADIUS, 10.0f, sinf(rotation) * ORBIT_RADIUS));


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

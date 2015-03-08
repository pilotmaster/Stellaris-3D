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

	// Set ambient colour
	pEngine->SetAmbientColour(DirectX::XMFLOAT3(0.05f, 0.1f, 0.2f));

	// Model loading
	sge::CCamera* camMain = pEngine->CreateCamera(DirectX::XMFLOAT3(-15.0f, 20.0f, -40.0f));

	DirectX::XMFLOAT3 cubePos{ 0.0f, 10.0f, 0.0f };
	sge::CMesh* mshCube = pEngine->LoadMesh("Media\\Cube.x", sge::R_LIT_TEXTURED);
	sge::CModel* mdlCube = pEngine->CreateModel(mshCube, cubePos);

	sge::CMesh* mshFloor = pEngine->LoadMesh("Media\\Floor.x", sge::R_LIT_TEXTURED);
	sge::CModel* mdlFloor = pEngine->CreateModel(mshFloor);

	sge::CMesh* mshTeapot = pEngine->LoadMesh("Media\\Teapot.x", sge::R_NORMAL_MAPPED);
	sge::CModel* mdlTeapot = pEngine->CreateModel(mshTeapot, DirectX::XMFLOAT3(-20.0f, 5.0f, 25.0f));

	sge::CMesh* mshLight = pEngine->LoadMesh("Media\\Light.x", sge::R_LIGHT);
	sge::CLight* mdlLight1 = pEngine->CreateLight(mshLight);
	mdlLight1->SetLightBrightness(2.0f);

	sge::CLight* mdlLight2 = pEngine->CreateLight(mshLight, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(-20.0f, 30.0f, 50.0f));
	mdlLight2->SetLightBrightness(4.0f);
	mdlLight2->SetLightColour(DirectX::XMFLOAT3(1.0f, 0.8f, 0.2f));


	// Variables & set up for altering the lights behaviour
	DirectX::XMFLOAT3 light1RGB = { 1.0f, 1.0f, 1.0f };
	mdlLight1->SetLightColour(light1RGB);
	DirectX::XMFLOAT3 light1HSL;

	// frame time variable
	float delta = 0.0f;
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
		mdlCube->GetPosition(cubePos);
		mdlLight1->SetPosition(DirectX::XMFLOAT3((cosf(rotation) * ORBIT_RADIUS) + cubePos.x, cubePos.y, (sinf(rotation) * ORBIT_RADIUS) + cubePos.z));


		// LIGHT 1 MOVING THROUGH COLOURS
		//---------------------------------
		// Get the DirectX vector of the light1's RGB colour & convert to HSL
		DirectX::XMVECTOR vecLight1Colour = DirectX::XMColorRGBToHSL(DirectX::XMLoadFloat3(&light1RGB));
		DirectX::XMStoreFloat3(&light1HSL, vecLight1Colour);

		// Increment HSL 'hue' value by an amount & loop round if it is greater than 1
		light1HSL.x += delta / 10.0f;
		light1HSL.y = 0.5f;
		light1HSL.z = 0.3f;

		// Convert back to RGB & set the light to the new colour
		vecLight1Colour = DirectX::XMColorHSLToRGB(DirectX::XMLoadFloat3(&light1HSL));
		DirectX::XMStoreFloat3(&light1RGB, vecLight1Colour);
		mdlLight1->SetLightColour(light1RGB);


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

		// Cube local movement
		if (sge::KeyHeld(sge::KEY_PERIOD))
		{
			mdlCube->MoveLocalZ(MOVE_SPEED * delta);
		}

		if (sge::KeyHeld(sge::KEY_COMMA))
		{
			mdlCube->MoveLocalZ(-MOVE_SPEED * delta);
		}

		// Cube rotation
		if (sge::KeyHeld(sge::KEY_I))
		{
			mdlCube->RotateX(-ROTATE_SPEED * delta);
		}

		if (sge::KeyHeld(sge::KEY_K))
		{
			mdlCube->RotateX(ROTATE_SPEED * delta);
		}

		if (sge::KeyHeld(sge::KEY_J))
		{
			mdlCube->RotateY(-ROTATE_SPEED * delta);
		}

		if (sge::KeyHeld(sge::KEY_L))
		{
			mdlCube->RotateY(ROTATE_SPEED * delta);
		}

		if (sge::KeyHeld(sge::KEY_U))
		{
			mdlCube->RotateZ(ROTATE_SPEED * delta);
		}

		if (sge::KeyHeld(sge::KEY_O))
		{
			mdlCube->RotateZ(-ROTATE_SPEED * delta);
		}
	}

	delete pEngine;

	return 0;
}

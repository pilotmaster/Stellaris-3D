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
	auto pEngine = std::make_unique<sge::CStellaris3D>();

	if (!pEngine->InitialiseEngine(hInstance))
	{
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
	pEngine->SetAmbientColour(DirectX::XMFLOAT3(0.1f, 0.2f, 0.25f));

	// Model loading
	sge::CCamera* camMain = pEngine->CreateCamera(DirectX::XMFLOAT3(-15.0f, 35.0f, -70.0f), { DirectX::XMConvertToRadians(10.0f), DirectX::XMConvertToRadians(18.0f), 0.0f });

	sge::CMesh* mshFloor = pEngine->LoadMesh("Media\\Hills.x", sge::FX_PARALLAX);
	sge::CModel* mdlFloor = pEngine->CreateModel(mshFloor, DirectX::XMFLOAT3(-30.0f, 0.0f, 0.0f));

	sge::CMesh* mshMirror = pEngine->LoadMesh("Media\\Mirror.x", sge::FX_MIRROR);
	sge::CModel* mdlMirror = pEngine->CreateMirror(mshMirror, { 0.0f, 20.0f, 30.0f }, { 0.0f, DirectX::XMConvertToRadians(-180.0f), 0.0f });

	sge::CMesh* mshWiggleSphere = pEngine->LoadMesh("Media\\Sphere.x", sge::FX_WIGGLE);
	sge::CModel* mdlWiggleSphere = pEngine->CreateModel(mshWiggleSphere, DirectX::XMFLOAT3(20.0f, 5.0f, -25.0f));
	mdlWiggleSphere->SetScale(DirectX::XMFLOAT3(0.5f, 0.5f, 0.5f));

	DirectX::XMFLOAT3 cubePos{ 0.0f, 5.0f, 0.0f };
	sge::CMesh* mshCube = pEngine->LoadMesh("Media\\Cube.x", sge::FX_LIT_TEXTURED);
	sge::CModel* mdlCube = pEngine->CreateModel(mshCube, cubePos);

	sge::CMesh* mshTeapot = pEngine->LoadMesh("Media\\Teapot.x", sge::FX_NORMAL_MAPPED);
	sge::CModel* mdlTeapot = pEngine->CreateModel(mshTeapot, DirectX::XMFLOAT3(-20.0f, 0.0f, 25.0f));

	sge::CMesh* mshTroll = pEngine->LoadMesh("Media\\Troll.x", sge::FX_CELL_SHADED);
	sge::CModel* mdlTroll = pEngine->CreateModel(mshTroll, DirectX::XMFLOAT3(20.0f, 0.0f, 25.0f));
	mdlTroll->Scale(DirectX::XMFLOAT3(5.0f, 5.0f, 5.0f));
	mdlTroll->SetModelColour(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));

	sge::CMesh* mshLight = pEngine->LoadMesh("Media\\Light.x", sge::FX_LIGHT);
	sge::CLight* mdlLight1 = pEngine->CreateLight(mshLight, sge::POINT_LIGHT);
	mdlLight1->SetLightBrightness(6.0f);

	sge::CLight* mdlLight2 = pEngine->CreateLight(mshLight, sge::POINT_LIGHT, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f), DirectX::XMFLOAT3(-20.0f, 30.0f, 50.0f));
	mdlLight2->SetLightBrightness(4.0f);
	mdlLight2->SetLightColour(DirectX::XMFLOAT3(1.0f, 0.8f, 0.2f));

	sge::CLight* mdlSpotlight = pEngine->CreateLight(mshLight, sge::SPOT_LIGHT, { 5.0f, 5.0f, 5.0f }, { 20.0f, 25.0f, -50.0f });
	mdlSpotlight->SetLightBrightness(7.0f);




	// Variables & set up for altering the lights behaviour
	DirectX::XMFLOAT3 light1RGB = { 1.0f, 1.0f, 1.0f };
	mdlLight1->SetLightColour(light1RGB);
	DirectX::XMFLOAT3 light1HSL;

	const float brightnessMulti = 4.0f;		// Multiplier for determining brightness
	float counter = 0.0f;					// Counter increments by a certain amount each frame & used to determine light brightness
	float sinBrightness = 0.0f;				// Sin amount used for sine wave based on counter's value. Used as actual brightness

	// frame time variable
	float delta = 0.0f;
	float rotation = 0.0f;


	// MAIN GAME LOOP
	//---------------------------------
	while (pEngine->Run())
	{
		// UPDATE & RENDER CURRENT SCENE
		//---------------------------------
		pEngine->Update();
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
		light1HSL.z = 0.5f;

		// Convert back to RGB & set the light to the new colour
		vecLight1Colour = DirectX::XMColorHSLToRGB(DirectX::XMLoadFloat3(&light1HSL));
		DirectX::XMStoreFloat3(&light1RGB, vecLight1Colour);
		mdlLight1->SetLightColour(light1RGB);


		// LIGHT 2 PULSATING ON & OFF
		//---------------------------------
		counter += delta;
		sinBrightness = (sinf(counter) * brightnessMulti) + brightnessMulti;

		// Use new brightness on light 2
		mdlLight2->SetLightBrightness(sinBrightness);


		// WIGGLING MODELS UPDATES
		//---------------------------------
		mdlWiggleSphere->IncrementWiggle(delta * 2.0f);


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
			mdlSpotlight->MoveLocalZ(MOVE_SPEED * delta);
		}

		if (sge::KeyHeld(sge::KEY_COMMA))
		{
			mdlSpotlight->MoveLocalZ(-MOVE_SPEED * delta);
		}

		// Cube rotation
		if (sge::KeyHeld(sge::KEY_I))
		{
			mdlSpotlight->RotateX(-ROTATE_SPEED * delta);
		}

		if (sge::KeyHeld(sge::KEY_K))
		{
			mdlSpotlight->RotateX(ROTATE_SPEED * delta);
		}

		if (sge::KeyHeld(sge::KEY_J))
		{
			mdlSpotlight->RotateY(-ROTATE_SPEED * delta);
		}

		if (sge::KeyHeld(sge::KEY_L))
		{
			mdlSpotlight->RotateY(ROTATE_SPEED * delta);
		}

		if (sge::KeyHeld(sge::KEY_U))
		{
			mdlSpotlight->RotateZ(ROTATE_SPEED * delta);
		}

		if (sge::KeyHeld(sge::KEY_O))
		{
			mdlSpotlight->RotateZ(-ROTATE_SPEED * delta);
		}
	}

	return 0;
}

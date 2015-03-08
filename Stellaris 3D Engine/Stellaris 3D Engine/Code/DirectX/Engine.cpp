//====================================================================================
// FILE:    Engine.cpp ---------------------------------------------------------------
//------------------------------------------------------------------------------------

#include "Engine.h"

namespace sge
{
	//====================================================================================
	// ENGINE CLASS CONSTRUCTOR & DESTRUCTOR
	//------------------------------------------------------------------------------------
	CStellaris3D::CStellaris3D()
	{
		// Initialise entity manager
		mpEntityManager = new CEntityManager();

		// Give a base ambient colour
		mAmbientColour = DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f);
	}

	CStellaris3D::~CStellaris3D()
	{
		if (mpEntityManager) delete mpEntityManager;
	}


	//====================================================================================
	// ENGINE CLASS OVERRIDE METHODS
	//------------------------------------------------------------------------------------
	bool CStellaris3D::InitialiseEngine(HINSTANCE hInstance)
	{
		// Call initialisation of application
		if (!CD3DApp::InitialiseApplication(hInstance))
		{
			return false;
		}

		// Initialise scene
		mpBasicShader = new CShader(mpDevice);
		
		// Begin timer
		CTimer::GetTimerInstace()->Reset();

		return true;
	}

	void CStellaris3D::Update()
	{
		// Update entities in the entity manager
		mpEntityManager->Update();
	}

	void CStellaris3D::Render(CCamera* pCamera)
	{
		// CLEAR CURRENT SCENE
		//---------------------------------
		float ambientColour[4] = { mAmbientColour.x, mAmbientColour.y, mAmbientColour.z, 1.0f };
		mpDevice->ClearRenderTargetView(mpRenderTarget, ambientColour);
		mpDevice->ClearDepthStencilView(mpDepthStencilView, D3D10_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, 1.0f, 0U);


		// RENDER ENTITIES IN THE SCENE
		//---------------------------------
		// Use the basic shader to set the required variables
		mpBasicShader->GetFXAmbientColour()->SetRawValue(&mAmbientColour, 0U, 12U);

		mpEntityManager->Render(mpDevice, pCamera, mpBasicShader);


		// PRESENT SWAP CHAIN
		//---------------------------------
		HR(mpSwapChain->Present(0U, 0U));
	}


	CCamera* CStellaris3D::CreateCamera(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, float fov, float nearClip, float farClip)
	{
		return mpEntityManager->CreateCameraEntity(pos, rot, fov, nearClip, farClip);
	}

	CMesh* CStellaris3D::LoadMesh(std::string fileName, ERenderTypes renderType)
	{
		// Determine type of rendering in order to decide which technique it will require & whether or not it
		// needs tangents in order to fulfil the effect
		bool needsTangents = false;
		ID3D10EffectTechnique* pTechnique = nullptr;
		
		switch (renderType)
		{
		case FX_LIT_TEXTURED:
			pTechnique = mpBasicShader->GetLitTexTechnique();
			break;

		case FX_WIGGLE:
			pTechnique = mpBasicShader->GetWiggleTechnique();
			break;

		case FX_LIGHT:
			pTechnique = mpBasicShader->GetLightDrawTechnique();
			break;

		case FX_NORMAL_MAPPED:
			pTechnique = mpBasicShader->GetNormalMappingTechnique();
			needsTangents = true;
			break;
		}

		
		// Create new mesh & load it
		CMesh* pMesh = new CMesh();
		if (pMesh->LoadMesh(mpDevice, fileName, pTechnique, renderType, needsTangents))
		{
			// Succeeded in creating the mesh - return the new mesh
			return pMesh;
		}
		
		// Failed to create the mesh - return null
		delete pMesh;
		return nullptr;
	}

	CModel* CStellaris3D::CreateModel(CMesh* pMesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, DirectX::XMFLOAT3 scale)
	{
		// Request a new model from the entity manager and return its reuslt
		return mpEntityManager->CreateModelEntity(pMesh, pos, rot, scale);
	}

	CLight* CStellaris3D::CreateLight(CMesh* pMesh, DirectX::XMFLOAT3 colour, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot,
		DirectX::XMFLOAT3 scale)
	{
		return mpEntityManager->CreateLightEntity(pMesh, pos, rot, scale, colour);
	}
}

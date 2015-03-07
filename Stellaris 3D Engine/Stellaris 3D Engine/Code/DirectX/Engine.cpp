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
		mpDevice->ClearRenderTargetView(mpRenderTarget, DirectX::Colors::LightBlue);
		mpDevice->ClearDepthStencilView(mpDepthStencilView, D3D10_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, 1.0f, 0U);


		// RENDER ENTITIES IN THE SCENE
		//---------------------------------
		mpEntityManager->Render(mpDevice, pCamera, mpBasicShader);


		// PRESENT SWAP CHAIN
		//---------------------------------
		HR(mpSwapChain->Present(0U, 0U));
	}


	CCamera* CStellaris3D::CreateCamera(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, float fov, float nearClip, float farClip)
	{
		return mpEntityManager->CreateCameraEntity(pos, rot, fov, nearClip, farClip);
	}

	CMesh* CStellaris3D::LoadMesh(std::string fileName)
	{
		// Create new mesh & load it
		CMesh* pMesh = new CMesh();
		if (pMesh->LoadMesh(mpDevice, fileName, mpBasicShader->GetTechnique()))
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
}

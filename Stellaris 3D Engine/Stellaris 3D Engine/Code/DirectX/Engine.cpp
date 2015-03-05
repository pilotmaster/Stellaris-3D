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

		
		// Begin timer
		CTimer::GetTimerInstace()->Reset();

		return true;
	}

	void CStellaris3D::Update()
	{
		// Update entities in the entity manager
		mpEntityManager->UpdateAndRenderEntities();
	}

	void CStellaris3D::Render()
	{
		// CLEAR CURRENT SCENE
		//---------------------------------
		mpDevice->ClearRenderTargetView(mpRenderTarget, DirectX::Colors::LightBlue);
		mpDevice->ClearDepthStencilView(mpDepthStencilView, D3D10_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, 1.0f, 0U);


		// PRESENT SWAP CHAIN
		//---------------------------------
		HR(mpSwapChain->Present(0U, 0U));
	}
}

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

		mpShadowMapTexture = nullptr;
		mpShadowMapDepthView = nullptr;
		mpShadowMap = nullptr;
	}

	CStellaris3D::~CStellaris3D()
	{
		if (mpEntityManager) delete mpEntityManager;
		if (mpBasicShader) delete mpBasicShader;

		if (mpShadowMap) mpShadowMap->Release();
		if (mpShadowMapDepthView) mpShadowMapDepthView->Release();
		if (mpShadowMapTexture) mpShadowMapTexture->Release();
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

		// Shadow map data
		D3D10_TEXTURE2D_DESC texDesc;
		texDesc.Width = 2056; // Size of the shadow map determines quality / resolution of shadows
		texDesc.Height = 2056;
		texDesc.MipLevels = 1; // 1 level, means just the main texture, no additional mip-maps. Usually don't use mip-maps when rendering to textures (or we would have to render every level)
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_R32_TYPELESS; // The shadow map contains a single 32-bit value [tech gotcha: have to say typeless because depth buffer and texture see things slightly differently]
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D10_USAGE_DEFAULT;
		texDesc.BindFlags = D3D10_BIND_DEPTH_STENCIL | D3D10_BIND_SHADER_RESOURCE; // Indicate we will use texture as render target, and will also pass it to shaders
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = 0;
		if (FAILED(mpDevice->CreateTexture2D(&texDesc, NULL, &mpShadowMapTexture))) return false;

		// Create the depth stencil view, i.e. indicate that the texture just created is to be used as a depth buffer
		D3D10_DEPTH_STENCIL_VIEW_DESC descDSV;
		descDSV.Format = DXGI_FORMAT_D32_FLOAT; // See "tech gotcha" above
		descDSV.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		if (FAILED(mpDevice->CreateDepthStencilView(mpShadowMapTexture, &descDSV, &mpShadowMapDepthView))) return false;

		// We also need to send this texture (a GPU memory resource) to the shaders. To do that we must create a shader-resource "view"	
		D3D10_SHADER_RESOURCE_VIEW_DESC srDesc;
		srDesc.Format = DXGI_FORMAT_R32_FLOAT; // See "tech gotcha" above
		srDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
		srDesc.Texture2D.MostDetailedMip = 0;
		srDesc.Texture2D.MipLevels = 1;
		if (FAILED(mpDevice->CreateShaderResourceView(mpShadowMapTexture, &srDesc, &mpShadowMap))) return false;


		// Portal map data
		D3D10_TEXTURE2D_DESC portalDesc;
		portalDesc.Width = 2056;  // Size of the portal texture determines its quality
		portalDesc.Height = 2056;
		portalDesc.MipLevels = 1; // No mip-maps when rendering to textures (or we would have to render every level)
		portalDesc.ArraySize = 1;
		portalDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // RGBA texture (8-bits each)
		portalDesc.SampleDesc.Count = 1;
		portalDesc.SampleDesc.Quality = 0;
		portalDesc.Usage = D3D10_USAGE_DEFAULT;
		portalDesc.BindFlags = D3D10_BIND_RENDER_TARGET | D3D10_BIND_SHADER_RESOURCE; // Indicate we will use texture as render target, and pass it to shaders
		portalDesc.CPUAccessFlags = 0;
		portalDesc.MiscFlags = 0;
		if (FAILED(mpDevice->CreateTexture2D(&portalDesc, NULL, &mpPortalTexture))) return false;

		if (FAILED(mpDevice->CreateRenderTargetView(mpPortalTexture, NULL, &mpPortalRenderTarget))) return false;

		// We also need to send this texture (resource) to the shaders. To do that we must create a shader-resource "view"
		srDesc.Format = portalDesc.Format;
		srDesc.ViewDimension = D3D10_SRV_DIMENSION_TEXTURE2D;
		srDesc.Texture2D.MostDetailedMip = 0;
		srDesc.Texture2D.MipLevels = 1;
		if (FAILED(mpDevice->CreateShaderResourceView(mpPortalTexture, &srDesc, &mpPortalMap))) return false;

		//**** This depth buffer can be shared with any other portals of the same size
		portalDesc.Width = 2056;
		portalDesc.Height = 2056;
		portalDesc.MipLevels = 1;
		portalDesc.ArraySize = 1;
		portalDesc.Format = DXGI_FORMAT_D32_FLOAT;
		portalDesc.SampleDesc.Count = 1;
		portalDesc.SampleDesc.Quality = 0;
		portalDesc.Usage = D3D10_USAGE_DEFAULT;
		portalDesc.BindFlags = D3D10_BIND_DEPTH_STENCIL;
		portalDesc.CPUAccessFlags = 0;
		portalDesc.MiscFlags = 0;
		if (FAILED(mpDevice->CreateTexture2D(&portalDesc, NULL, &mpPortalDepthStencil))) return false;

		// Create the depth stencil view, i.e. indicate that the texture just created is to be used as a depth buffer
		D3D10_DEPTH_STENCIL_VIEW_DESC portalDescDSV;
		portalDescDSV.Format = portalDesc.Format;
		portalDescDSV.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
		portalDescDSV.Texture2D.MipSlice = 0;
		if (FAILED(mpDevice->CreateDepthStencilView(mpPortalDepthStencil, &portalDescDSV, &mpPortalDepthStencilView))) return false;

		return true;
	}

	void CStellaris3D::Update()
	{
		// Update entities in the entity manager
		mpEntityManager->Update();
	}

	void CStellaris3D::Render(CCamera* pCamera, CCamera* pPortalCamera)
	{
		// Use the basic shader to set the required variables
		mpBasicShader->GetFXAmbientColourVar()->SetFloatVector((float*)&mAmbientColour);
		mpBasicShader->GetFXShadowMapVar()->SetResource(mpShadowMap);

		// Clear show depth view & render to shadow map
		mViewport.Width = 2056;
		mViewport.Height = 2056;
		mViewport.MinDepth = 0.0f;
		mViewport.MaxDepth = 1.0f;
		mViewport.TopLeftX = 0;
		mViewport.TopLeftY = 0;
		mpDevice->RSSetViewports(1, &mViewport);

		mpDevice->OMSetRenderTargets(0, 0, mpShadowMapDepthView);
		mpDevice->ClearDepthStencilView(mpShadowMapDepthView, D3D10_CLEAR_DEPTH, 1.0f, 0.0f);
		mpEntityManager->RenderShadows(mpDevice, mpBasicShader);


		// CLEAR CURRENT SCENE TO PORTAL
		//---------------------------------
		mViewport.Width = 2056;
		mViewport.Height = 2056;
		mViewport.MinDepth = 0.0f;
		mViewport.MaxDepth = 1.0f;
		mViewport.TopLeftX = 0;
		mViewport.TopLeftY = 0;
		mpDevice->RSSetViewports(1, &mViewport);

		float ambientColour[4] = { mAmbientColour.x, mAmbientColour.y, mAmbientColour.z, 1.0f };
		mpDevice->OMSetRenderTargets(1, &mpPortalRenderTarget, mpPortalDepthStencilView);
		mpDevice->ClearRenderTargetView(mpPortalRenderTarget, ambientColour);
		mpDevice->ClearDepthStencilView(mpPortalDepthStencilView, D3D10_CLEAR_DEPTH, 1.0f, 0);

		// Only if there is an active portal, render from the portal's camera's perspective
		if (mpEntityManager->HasPortal() && pPortalCamera != nullptr)
		{
			mpEntityManager->Render(mpDevice, pPortalCamera, mpBasicShader);
		}


		// CLEAR CURRENT SCENE
		//---------------------------------
		mViewport.Width = static_cast<float>(mpWindow->GetWindowWidth());
		mViewport.Height = static_cast<float>(mpWindow->GetWindowHeight());
		mViewport.TopLeftX = 0.0f;
		mViewport.TopLeftY = 0.0f;
		mViewport.MinDepth = 0.0f;
		mViewport.MaxDepth = 1.0f;
		mpDevice->RSSetViewports(1, &mViewport);

		mpDevice->OMSetRenderTargets(1, &mpRenderTarget, mpDepthStencilView);

		mpDevice->ClearRenderTargetView(mpRenderTarget, ambientColour);
		mpDevice->ClearDepthStencilView(mpDepthStencilView, D3D10_CLEAR_DEPTH | D3D10_CLEAR_STENCIL, 1.0f, 0U);


		// RENDER ENTITIES IN THE SCENE
		//---------------------------------
		// Only if there is a mirror active, render the mirror
		if (mpEntityManager->HasMirror())
		{
			mpEntityManager->RenderMirror(mpDevice, pCamera, mpBasicShader);
		}
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

		case FX_PARALLAX:
			pTechnique = mpBasicShader->GetParallaxMappingTechnique();
			needsTangents = true;
			break;

		case FX_CELL_SHADED:
			pTechnique = mpBasicShader->GetCellShadingTechnique();
			break;

		case FX_MIRROR:
			pTechnique = mpBasicShader->GetMirrorClearTechnique();
			break;
		}

		
		// Create new mesh & load it
		CMesh* pMesh = new CMesh();
		if (pMesh->LoadMesh(mpDevice, fileName, pTechnique, mpBasicShader->GetVertexLitTexMirrorTechnique(), renderType, needsTangents))
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

	CModel* CStellaris3D::CreateMirror(CMesh* pMesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, DirectX::XMFLOAT3 scale)
	{
		// Request a new model from the entity manager and return its reuslt
		CModel* pModel = mpEntityManager->CreateMirrorEntity(pMesh, pos, rot, scale);
		if (pModel) pModel->SetModelColour(mAmbientColour);
		return pModel;
	}

	CModel* CStellaris3D::CreatePortal(CMesh* pMesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, DirectX::XMFLOAT3 scale)
	{
		// Request a new model from the entity manager and return its reuslt
		CModel* pModel = mpEntityManager->CreatePortalEntity(pMesh, pos, rot, scale);
		mpEntityManager->SetPortalTexture(mpPortalMap);
		return pModel;
	}

	CLight* CStellaris3D::CreateLight(CMesh* pMesh, ELightTypes lightType, DirectX::XMFLOAT3 colour, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot,
		DirectX::XMFLOAT3 scale)
	{		
		return mpEntityManager->CreateLightEntity(pMesh, pos, rot, scale, colour, static_cast<int>(lightType));
	}

	CModel* CStellaris3D::RemoveModel(CModel* pModel)
	{
		mpEntityManager->DestroyEntity(pModel->GetID());
		return nullptr;
	}
}

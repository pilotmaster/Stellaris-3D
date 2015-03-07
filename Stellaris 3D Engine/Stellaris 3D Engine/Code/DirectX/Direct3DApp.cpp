//====================================================================================
// FILE:    Direct3DApp.cpp ----------------------------------------------------------
//------------------------------------------------------------------------------------

#include "Direct3DApp.h"


namespace sge
{
	//====================================================================================
	// DIRECT3D APPLICATION CLASS CONSTRUCTOR & DESTRUCTOR
	//------------------------------------------------------------------------------------
	CD3DApp::CD3DApp()
	{

	}

	CD3DApp::~CD3DApp()
	{

	}


	//====================================================================================
	// DIRECT3D APPLICATION CLASS RUN FUNCTION
	//------------------------------------------------------------------------------------
	bool CD3DApp::Run()
	{
		// Initialise message
		MSG msg = { 0 };

		// Loop through appplicaion
		// If there is a message, deal with it otherwise run engine
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			// Translate the message and dispatch it
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			// Check for instruction to quit
			if (msg.message == WM_QUIT)
			{
				// Stop running
				mpWindow->StopRunningWindow();

				// Return quit message - for error checking
				//return static_cast<int>(msg.wParam);
			}
		}
		else
		{
			// Check the window is not paused
			if (!mpWindow->WindowPaused())
			{
				CTimer::GetTimerInstace()->Tick();
				CalcFrameStats();

				// Update application
				Update();
			}
			else
			{
				// wait for application to become available again
				Sleep(100);
			}
		}

		return mpWindow->WindowRunning(); // No errors or quit message
	}


	//====================================================================================
	// DIRECT3D APPLICATION CLASS FRAMEWORK METHODS
	//------------------------------------------------------------------------------------
	bool CD3DApp::InitialiseApplication(HINSTANCE hInstance)
	{
		mpWindow = CWindow::GetInstance();
		
		// Create the window
		if (!mpWindow->InitialiseWindow(hInstance, 1600, 900, L"Stellaris-3D"))
		{
			// Failed to create window - show message & return false
			MessageBox(mpWindow->GetWindowHandle(), L"Failed to initialise window!", L"WINDOW ERROR", MB_OK);
			return false;
		}

		// Initialise Direct3D
		if (!InitialiseDirect3D())
		{
			MessageBox(mpWindow->GetWindowHandle(), L"Failed to initialise Direct3D", L"D3D ERROR", MB_OK);
			return false;
		}

		// Initialise input
		InitialiseInput();

		return true;
	}

	bool CD3DApp::InitialiseDirect3D()
	{
		// DEVICE FLAGS
		//------------------------------
		UINT deviceFlags = 0;

		// Check if in debug mode
#ifdef _DEBUG
		deviceFlags |= D3D10_CREATE_DEVICE_DEBUG;
#endif // _DEBUG

		// CREATE SWAP CHAIN DESC
		//------------------------------
		DXGI_SWAP_CHAIN_DESC swapDesc;
		ZeroMemory(&swapDesc, sizeof(swapDesc));
		swapDesc.BufferCount = 1;
		swapDesc.BufferDesc.Width = mpWindow->GetWindowWidth();
		swapDesc.BufferDesc.Height = mpWindow->GetWindowHeight();
		swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapDesc.BufferDesc.RefreshRate.Numerator = 1;
		swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
		swapDesc.OutputWindow = mpWindow->GetWindowHandle();
		swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapDesc.Windowed = true;
		swapDesc.SampleDesc.Count = 1;
		swapDesc.SampleDesc.Quality = 0;
		swapDesc.Flags = 0;

		// Create swap chain
		D3D_FEATURE_LEVEL fl = D3D_FEATURE_LEVEL_10_1;

		HRESULT result = D3D10CreateDeviceAndSwapChain(NULL, D3D10_DRIVER_TYPE_HARDWARE, NULL, 0,
			D3D10_SDK_VERSION, &swapDesc, &mpSwapChain, &mpDevice);


		// Check if failed
		if (FAILED(result))
		{
			// Failed to create the swap chain & device
			MessageBox(mpWindow->GetWindowHandle() , L"Failed to create Swap Chain & Device", L"SWAP CHAIN CREATION ERROR", MB_OK);
			return false;
		}


		// CREATE RENDER TARGET VIEW
		//------------------------------
		ID3D10Texture2D* backBufferTex = NULL;
		HR(mpSwapChain->GetBuffer(NULL, __uuidof(ID3D10Texture2D), (LPVOID*)&backBufferTex));
		HR(mpDevice->CreateRenderTargetView(backBufferTex, NULL, &mpRenderTarget));

		// Release back buffer pointer
		backBufferTex->Release();


		// CREATE DEPTH BUFFER
		//------------------------------
		D3D10_TEXTURE2D_DESC descDepth;
		descDepth.Width = mpWindow->GetWindowWidth();
		descDepth.Height = mpWindow->GetWindowHeight();
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D32_FLOAT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D10_USAGE_DEFAULT;
		descDepth.BindFlags = D3D10_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;

		result = mpDevice->CreateTexture2D(&descDepth, NULL, &mpDepthStencilBuffer);
		if (FAILED(result))
		{
			// Failed to create depth stencil buffer
			MessageBox(mpWindow->GetWindowHandle(), L"Failed to create the Depth Stencil Buffer", L"DEPTH STENCIL ERROR", MB_OK);
			return false;
		}


		// CREATE DEPTH STENCIL VIEW
		//------------------------------
		D3D10_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));

		descDSV.Format = DXGI_FORMAT_D32_FLOAT;
		descDSV.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;

		if (FAILED(mpDevice->CreateDepthStencilView(mpDepthStencilBuffer, &descDSV, &mpDepthStencilView)))
		{
			OutputDebugString(L"\nFAILED TO CREATE DEPTH STENCIL VIEW\n");
			return false;
		}


		// VIEWPORT CREATION
		//------------------------------
		mpDevice->OMSetRenderTargets(1, &mpRenderTarget, mpDepthStencilView);

		mViewport.Width = static_cast<float>(mpWindow->GetWindowWidth());
		mViewport.Height = static_cast<float>(mpWindow->GetWindowHeight());
		mViewport.TopLeftX = 0.0f;
		mViewport.TopLeftY = 0.0f;
		mViewport.MinDepth = 0.0f;
		mViewport.MaxDepth = 1.0f;

		// Create orthographic matrix
		DirectX::XMMATRIX ortho = DirectX::XMMatrixOrthographicOffCenterLH(0.0f, mViewport.Width, mViewport.Height, 0.0f, 0.0f, 1.0f);
		DirectX::XMStoreFloat4x4(&mOrthoMatrix, ortho);

		// Bind viewport
		mpDevice->RSSetViewports(1, &mViewport);

		// No problems Return true
		return true;
	}


	//====================================================================================
	// ADDITIONAL METHODS ----------------------------------------------------------------
	//------------------------------------------------------------------------------------
	void CD3DApp::CalcFrameStats()
	{
		static float timeElapsed = 0.0f;
		
		// Compute averages over a second
		if ((CTimer::GetTimerInstace()->GetTotalTime() - timeElapsed) >= 1.0f)
		{
			float mspf = CTimer::GetTimerInstace()->GetDeltaTime();
			int fps = (int)(1.0f / mspf);

			// Display data on title bar
			std::wstringstream outp;
			outp.precision(6);
			outp << *mpWindow->GetWindowTitle() << "      "
				<< "FPS: " << fps << "      "
				<< "FT: " << mspf;
			SetWindowText(mpWindow->GetWindowHandle(), outp.str().c_str());

			timeElapsed += 1.0f;
		}
	}
}

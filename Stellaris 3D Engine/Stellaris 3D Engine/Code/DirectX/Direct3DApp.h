//====================================================================================
// FILE:    Direct3DApp.h ------------------------------------------------------------
//------------------------------------------------------------------------------------

#include <dxgi.h>
#include <dxerr.h>
#include <d3dx10.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

#include "..\Windows\Window.h"


namespace sge
{
	//====================================================================================
	// DIRECT3D APPLICATION CLASS --------------------------------------------------------
	//------------------------------------------------------------------------------------
	class CD3DApp
	{
	public:
		// CONSTRUCTORS & DESTRUCTORS
		//---------------------------------
		CD3DApp();
		virtual ~CD3DApp();


		// APPLICATION FUNCTION
		//---------------------------------
		int Run();


		// FRAMEWORK METHODS
		//---------------------------------
		bool InitialiseApplication(HINSTANCE hInstance);
		virtual void Update() = 0;
		virtual void Render() = 0;


		// ACCESSORS
		//---------------------------------
		inline HWND GetWindowHandle()
		{
			return mpWindow->GetWindowHandle();
		}

		inline bool EngineRunning()
		{
			return mpWindow->WindowRunning();
		}

		inline UINT GetWindowWidth()
		{
			return mpWindow->GetWindowWidth();
		}

		inline UINT GetWindowHeight()
		{
			return mpWindow->GetWindowHeight();
		}


		// MUTATORS
		//---------------------------------



	protected:
		// WINDOW
		//---------------------------------
		CWindow* mpWindow;


		// DIRECTX
		//---------------------------------
		IDXGISwapChain* mpSwapChain;
		ID3D10Texture2D* mpDepthStencilBuffer;
		ID3D10DepthStencilState* mpDepthStencilState;
		ID3D10DepthStencilView* mpDepthStencilView;
		ID3D10RenderTargetView* mpRenderTarget;

		D3D10_VIEWPORT mViewport;


		// ORTHOGRAPHIC MATRIX
		//---------------------------------
		DirectX::XMFLOAT4X4 mOrthoMatrix;


		// PRIVATE FRAMEWORK METHODS
		//---------------------------------
		bool InitialiseWindow();
		bool InitialiseDirect3D();
		void CalcFrameStats();
	};
}

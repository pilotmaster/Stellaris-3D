//====================================================================================
// FILE:    Direct3DApp.h ------------------------------------------------------------
//------------------------------------------------------------------------------------

#ifndef _DIRECT_3D_APP_H_
#define _DIRECT_3D_APP_H_

#include <dxgi.h>
#include <dxerr.h>
#include <d3dx10.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

#include "..\Windows\Window.h"


//====================================================================================
// HELPER MACROS 
//------------------------------------------------------------------------------------
//#ifndef HR
//#define HR(x) \
//{ \
//	HRESULT hr = x; \
//if (FAILED(hr)) \
//{ \
//	DXTraceW(__FILE__, __LINE__, hr, L#x, TRUE); \
//} \
//}
//#endif


namespace sge
{
	//====================================================================================
	// DIRECT3D APPLICATION CLASS 
	//------------------------------------------------------------------------------------
	class CD3DApp
	{
	public:
		// CONSTRUCTORS & DESTRUCTORS
		//---------------------------------
		CD3DApp();
		virtual ~CD3DApp();


		// APPLICATION RUN FUNCTION
		//---------------------------------
		int Run();


		// FRAMEWORK METHODS
		//---------------------------------
		bool InitialiseApplication(HINSTANCE hInstance);
		virtual void Update() = 0;
		virtual void Render() = 0;

		bool InitialiseDirect3D();
		void CalcFrameStats();


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
		ID3D10Device* mpDevice;
		IDXGISwapChain* mpSwapChain;
		ID3D10Texture2D* mpDepthStencilBuffer;
		ID3D10DepthStencilState* mpDepthStencilState;
		ID3D10DepthStencilView* mpDepthStencilView;
		ID3D10RenderTargetView* mpRenderTarget;

		D3D10_VIEWPORT mViewport;


		// ORTHOGRAPHIC MATRIX
		//---------------------------------
		DirectX::XMFLOAT4X4 mOrthoMatrix;


	private:
		// PRIVATE FRAMEWORK METHODS
		//---------------------------------
		
	};
}


#endif /* _DIRECT_3D_APP_H_ */

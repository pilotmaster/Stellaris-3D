//====================================================================================
// FILE:    Camera.h -----------------------------------------------------------------
//------------------------------------------------------------------------------------

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Entity.h"


namespace sge
{
	//====================================================================================
	// CAMERA CLASS : CHILD OF ENTITY CLASS
	//------------------------------------------------------------------------------------
	class CCamera : public CEntity
	{
	public:
		// CONSTRUCTOR & DESTRUCTOR
		//---------------------------------
		CCamera(UINT id, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot,float fov = DirectX::XM_PI / 4.0f,
			float nearClip = 0.1f, float farClip = 1000.0f);
		~CCamera();


		// METHODS
		//---------------------------------
		void UpdateMatrices();
		void Update();


		// ACCESSORS
		//---------------------------------


		// MUTATORS
		//---------------------------------



	private:
		// MATRICES
		//---------------------------------
		DirectX::XMFLOAT4X4 mViewMatrix;
		DirectX::XMFLOAT4X4 mProjMatrix;
		DirectX::XMFLOAT4X4 mViewProj;


		// CAMERA SETTINGS
		//---------------------------------
		float mNearClip;
		float mFarClip;
		float mFOV;
	};
}


#endif /* _CAMERA_H_ */

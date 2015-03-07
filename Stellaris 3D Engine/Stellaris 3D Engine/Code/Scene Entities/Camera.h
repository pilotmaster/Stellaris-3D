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
		CCamera(UINT id, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot,float fov, float nearClip, float farClip);
		~CCamera();


		// METHODS
		//---------------------------------
		void UpdateMatrices();
		void Update();
		void Render(ID3D10Device* pDevice, ID3D10EffectTechnique* pTech);


		// ACCESSORS
		//---------------------------------


		// MUTATORS
		//---------------------------------



	private:
		// MATRICES
		//---------------------------------
		DirectX::XMFLOAT4X4 mViewMatrix;
		DirectX::XMFLOAT4X4 mProjMatrix;


		// CAMERA SETTINGS
		//---------------------------------
		float mNearClip;
		float mFarClip;
		float mFOV;
	};
}


#endif /* _CAMERA_H_ */

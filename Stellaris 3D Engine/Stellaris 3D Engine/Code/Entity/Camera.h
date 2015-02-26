//====================================================================================
// FILE:    Camera.h -----------------------------------------------------------------
//------------------------------------------------------------------------------------

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Entity.h"


class CCamera
{
public:
	// CONSTRUCTOR & DESTRUCTOR
	//---------------------------------
	CCamera(DirectX::XMFLOAT3 pos = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3 rot = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
		float fov = DirectX::XM_PI / 4.0f, float nearClip = 0.1f, float farClip = 1000.0f);
	~CCamera();


	// METHODS
	//---------------------------------
	void UpdateMatrices();
	void Control();


	// ACCESSORS
	//---------------------------------


	// MUTATORS
	//---------------------------------



private:
	// MATRICES
	//---------------------------------
	DirectX::XMFLOAT4X4 mWorldMatrix;
	DirectX::XMFLOAT4X4 mViewMatrix;
	DirectX::XMFLOAT4X4 mProjMatrix;
	DirectX::XMFLOAT4X4 mViewProj;


	// POSITIONS, ROTATIONS & SETTINGS
	//---------------------------------
	DirectX::XMFLOAT3 mPosition;
	DirectX::XMFLOAT3 mRotation;

	float mNearClip;
	float mFarClip;
	float mFOV;
};


#endif /* _CAMERA_H_ */

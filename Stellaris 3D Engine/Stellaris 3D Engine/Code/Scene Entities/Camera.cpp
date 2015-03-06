//====================================================================================
// FILE:    Camera.cpp ---------------------------------------------------------------
//------------------------------------------------------------------------------------

#include "Camera.h"


namespace sge
{
	//====================================================================================
	// CAMERA CLASS CONSTRUCTOR & DESTRUCTOR
	//------------------------------------------------------------------------------------
	CCamera::CCamera(UINT id, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, float fov, float nearClip, float farClip) :
		CEntity(id, pos, rot, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f)), mFOV(fov), mNearClip(nearClip), mFarClip(farClip)
	{

	}

	CCamera::~CCamera()
	{

	}


	//====================================================================================
	// CAMERA CLASS METHODS
	//------------------------------------------------------------------------------------
	void CCamera::UpdateMatrices()
	{
		// Call parent's function as this overrides it by default
		CEntity::UpdateMatrices();

		// Update the camera's matrices
	}

	void CCamera::Update()
	{
		// First update the matrices
		UpdateMatrices();
	}
}

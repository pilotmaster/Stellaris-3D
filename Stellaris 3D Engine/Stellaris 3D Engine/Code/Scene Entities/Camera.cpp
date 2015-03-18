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
		CEntity(id, pos, rot, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f)), mFOV(DirectX::XM_PI / 4.0f), mNearClip(1.0f), mFarClip(10000.0f)
	{
		// Call the update function to initialise the matrices first values
		Update();
	}

	CCamera::~CCamera()
	{

	}


	//====================================================================================
	// CAMERA CLASS METHODS
	//------------------------------------------------------------------------------------
	void CCamera::UpdateMatrices()
	{
		// Update camera's matrices WITHOUT SCALE
		DirectX::XMMATRIX world, rotation, rotationX, rotationY, rotationZ, translation;

		world = DirectX::XMMatrixIdentity();
		translation = world;

		rotationX = DirectX::XMMatrixRotationX(mRotation.x);
		rotationY = DirectX::XMMatrixRotationY(mRotation.y);
		rotationZ = DirectX::XMMatrixRotationZ(mRotation.z);

		//rotation = DirectX::XMMatrixRotationRollPitchYaw(mRotation.x, mRotation.y, mRotation.z);
		translation = DirectX::XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);

		world = rotationZ * rotationX * rotationY * translation;
		DirectX::XMStoreFloat4x4(&mModelMatrix, world);

		// Create and store the camera's view matrix
		DirectX::XMMATRIX matView = DirectX::XMMatrixInverse(NULL, DirectX::XMLoadFloat4x4(&mModelMatrix));
		DirectX::XMStoreFloat4x4(&mViewMatrix, matView);

		// Create and store camera's projection matrix
		DirectX::XMMATRIX matProj = DirectX::XMMatrixPerspectiveFovLH(mFOV, 1.33f, mNearClip, mFarClip);
		DirectX::XMStoreFloat4x4(&mProjMatrix, matProj);
	}

	void CCamera::Update()
	{
		// First update the matrices
		UpdateMatrices();
	}

	void CCamera::Render(ID3D10Device* pDevice, CShader* pShader, bool mirrored, bool forShadow)
	{
		return;
	}
}

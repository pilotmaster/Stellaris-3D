//====================================================================================
// FILE:    Entity.cpp ---------------------------------------------------------------
//------------------------------------------------------------------------------------

#include "Entity.h"
#include "..\Utilities\UserInput.h"
#include "..\Shaders\Shader.h"


namespace sge
{
	//====================================================================================
	// ENTITY CLASS CONSTRUCTOR & DESTRUCTOR
	//------------------------------------------------------------------------------------
	CEntity::CEntity(UINT id, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, DirectX::XMFLOAT3 scale) :
		mEID(id), mPosition(pos), mScale(scale), mRotation(rot)
	{
		mHasGeometry = false;
	}

	CEntity::~CEntity()
	{

	}


	//====================================================================================
	// ENTITY CLASS METHODS
	//------------------------------------------------------------------------------------
	void CEntity::UpdateMatrices()
	{
		DirectX::XMMATRIX world, rotation, rotationX, rotationY, rotationZ, scale, translation;
		world = DirectX::XMMatrixIdentity();
		translation = world;

		//rotation = DirectX::XMMatrixRotationRollPitchYaw(mRotation.x, mRotation.y, mRotation.z);

		D3DXMATRIX rotationDX;
		D3DXMatrixRotationYawPitchRoll(&rotationDX, mRotation.y, mRotation.x, mRotation.z);
		DirectX::XMFLOAT4X4 newRotation = static_cast<DirectX::XMFLOAT4X4>(rotationDX);
		rotation = DirectX::XMLoadFloat4x4(&newRotation);

		scale = DirectX::XMMatrixScaling(mScale.x, mScale.y, mScale.z);
		translation = DirectX::XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);

		world = scale * rotation * translation;
		DirectX::XMStoreFloat4x4(&mModelMatrix, world);
	}
}

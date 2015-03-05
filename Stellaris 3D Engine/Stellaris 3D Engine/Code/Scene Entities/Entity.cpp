//====================================================================================
// FILE:    Entity.cpp ---------------------------------------------------------------
//------------------------------------------------------------------------------------

#include "Entity.h"


namespace sge
{
	//====================================================================================
	// ENTITY CLASS CONSTRUCTOR & DESTRUCTOR
	//------------------------------------------------------------------------------------
	CEntity::CEntity(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, DirectX::XMFLOAT3 scale) :
		mPosition(pos), mScale(scale), mRotation(rot)
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
		DirectX::XMMATRIX world, rotation, scale, translation;

		world = DirectX::XMMatrixIdentity();
		translation = world;
		rotation = DirectX::XMMatrixRotationRollPitchYaw(mRotation.x, mRotation.y, mRotation.z);
		scale = DirectX::XMMatrixScaling(mScale.x, mScale.y, mScale.z);
		translation = DirectX::XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);

		world = rotation * scale * translation;
		DirectX::XMStoreFloat4x4(&mModelMatrix, world);
	}

	void CEntity::Render()
	{

	}
}

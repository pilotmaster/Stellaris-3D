//====================================================================================
// FILE:    Entity.h -----------------------------------------------------------------
//------------------------------------------------------------------------------------

// TODO: LOCAL MOVEMENT

#ifndef _ENTITY_H_
#define _ENTITY_H_


#include <dxgi.h>
#include <dxerr.h>
#include <d3dx10.h>
#include <DirectXMath.h>
#include <DirectXColors.h>


namespace sge
{
	//====================================================================================
	// ENTITY CLASS
	//------------------------------------------------------------------------------------
	class CShader;

	class CEntity
	{
	public:
		// CONSTRUCTOR & DESTRUCTOR
		//---------------------------------
		CEntity(UINT id, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, DirectX::XMFLOAT3 scale);
		virtual ~CEntity();


		// METHODS
		//---------------------------------
		virtual void UpdateMatrices();

		virtual void Update() = 0;
		virtual void Render(ID3D10Device* pDevice, CShader* pShader, bool forShadow = false) = 0;


		// ACCESSORS
		//---------------------------------
		inline void GetPosition(DirectX::XMFLOAT3& outPosition)
		{
			outPosition = mPosition;
		}

		inline float GetXPosition()
		{
			return mPosition.x;
		}

		inline float GetYPosition()
		{
			return mPosition.y;
		}

		inline float GetZPosition()
		{
			return mPosition.z;
		}

		inline void GetRotation(DirectX::XMFLOAT3& outRotation)
		{
			outRotation = mRotation;
		}

		inline float GetXRotation()
		{
			return mRotation.x;
		}

		inline float GetYRotation()
		{
			return mRotation.y;
		}

		inline float GetZRotation()
		{
			return mRotation.z;
		}

		inline void GetScale(DirectX::XMFLOAT3& outScale)
		{
			outScale = mScale;
		}

		inline float GetXScale()
		{
			return mScale.x;
		}

		inline float GetYScale()
		{
			return mScale.y;
		}

		inline float GetZScale()
		{
			return mScale.z;
		}


		// MUTATORS
		//---------------------------------
		inline void SetPosition(DirectX::XMFLOAT3& inPosition)
		{
			mPosition = inPosition;
		}

		void Move(DirectX::XMFLOAT3& inAmount)
		{
			mPosition.x += inAmount.x;
			mPosition.y += inAmount.y;
			mPosition.z += inAmount.z;
		}

		inline void SetXPosition(float inX)
		{
			mPosition.x = inX;
		}

		inline void MoveX(float inX)
		{
			mPosition.x += inX;
		}

		void MoveLocalX(float inAmount)
		{
			mPosition.x += mModelMatrix._11 * inAmount;
			mPosition.y += mModelMatrix._12 * inAmount;
			mPosition.z += mModelMatrix._13 * inAmount;
		}

		inline void SetYPosition(float inY)
		{
			mPosition.y = inY;
		}

		inline void MoveY(float inY)
		{
			mPosition.x += inY;
		}

		void MoveLocalY(float inAmount)
		{
			mPosition.x += mModelMatrix._21 * inAmount;
			mPosition.y += mModelMatrix._22 * inAmount;
			mPosition.z += mModelMatrix._23 * inAmount;
		}

		inline void SetZPosition(float inZ)
		{
			mPosition.z = inZ;
		}

		inline void MoveZ(float inZ)
		{
			mPosition.x += inZ;
		}

		void MoveLocalZ(float inAmount)
		{
			mPosition.x += mModelMatrix._31 * inAmount;
			mPosition.y += mModelMatrix._32 * inAmount;
			mPosition.z += mModelMatrix._33 * inAmount;
		}

		inline void SetRotation(DirectX::XMFLOAT3& inRotation)
		{
			mRotation = inRotation;
		}

		void Rotate(DirectX::XMFLOAT3& inAmount)
		{
			mRotation.x += DirectX::XMConvertToRadians(inAmount.x);
			mRotation.y += DirectX::XMConvertToRadians(inAmount.y);
			mRotation.z += DirectX::XMConvertToRadians(inAmount.z);
		}

		inline void SetXRotation(float inX)
		{
			mRotation.x = DirectX::XMConvertToRadians(inX);
		}

		inline void RotateX(float inX)
		{
			mRotation.x += DirectX::XMConvertToRadians(inX);
		}

		inline void SetYRotation(float inY)
		{
			mRotation.y = DirectX::XMConvertToRadians(inY);
		}

		inline void RotateY(float inY)
		{
			mRotation.y += DirectX::XMConvertToRadians(inY);
		}

		inline void SetZRotation(float inZ)
		{
			mRotation.z = DirectX::XMConvertToRadians(inZ);
		}

		inline void RotateZ(float inZ)
		{
			mRotation.z += DirectX::XMConvertToRadians(inZ);
		}

		inline void SetScale(DirectX::XMFLOAT3& inScale)
		{
			mScale = inScale;
		}

		void Scale(DirectX::XMFLOAT3& inAmount)
		{
			mScale.x += inAmount.x;
			mScale.y += inAmount.y;
			mScale.z += inAmount.z;
		}

		inline void SetXScale(float inX)
		{
			mScale.x = inX;
		}

		inline void SetYScale(float inY)
		{
			mScale.y = inY;
		}

		inline void SetZScale(float inZ)
		{
			mScale.z = inZ;
		}


	protected:
		// MATRICES
		//---------------------------------
		DirectX::XMFLOAT4X4 mModelMatrix;


		// ENTITY DATA
		//---------------------------------
		UINT mEID;

		bool mHasGeometry;

		DirectX::XMFLOAT3 mPosition;
		DirectX::XMFLOAT3 mRotation;
		DirectX::XMFLOAT3 mScale;
	};
}


#endif /* _ENTITY_H_ */

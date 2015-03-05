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
	class CEntity
	{
	public:
		// CONSTRUCTOR & DESTRUCTOR
		//---------------------------------
		CEntity(UINT id, DirectX::XMFLOAT3 pos = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3 rot = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
			DirectX::XMFLOAT3 scale = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
		virtual ~CEntity();


		// METHODS
		//---------------------------------
		virtual void UpdateMatrices();

		virtual void Update() = 0;
		virtual void Render(ID3D10Device* pDevice, ID3D10EffectTechnique* pTech) = 0;


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

		inline void Move(DirectX::XMFLOAT3& inAmount)
		{
			mPosition.x += inAmount.x;
			mPosition.y += inAmount.y;
			mPosition.z += inAmount.z;
		}

		inline void SetXPosition(float inX)
		{
			mPosition.x = inX;
		}

		inline void SetYPosition(float inY)
		{
			mPosition.y = inY;
		}

		inline void SetZPosition(float inZ)
		{
			mPosition.z = inZ;
		}

		inline void SetRotation(DirectX::XMFLOAT3& inRotation)
		{
			mRotation = inRotation;
		}

		inline void Rotate(DirectX::XMFLOAT3& inAmount)
		{
			mRotation.x += inAmount.x;
			mRotation.y += inAmount.y;
			mRotation.z += inAmount.z;
		}

		inline void SetXRotation(float inX)
		{
			mRotation.x = inX;
		}

		inline void SetYRotation(float inY)
		{
			mRotation.y = inY;
		}

		inline void SetZRotation(float inZ)
		{
			mRotation.z = inZ;
		}

		inline void SetScale(DirectX::XMFLOAT3& inScale)
		{
			mScale = inScale;
		}

		inline void Scale(DirectX::XMFLOAT3& inAmount)
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


	private:
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

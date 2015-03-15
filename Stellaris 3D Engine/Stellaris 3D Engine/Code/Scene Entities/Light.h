//====================================================================================
// FILE:    Light.h ------------------------------------------------------------------
//------------------------------------------------------------------------------------

#ifndef _LIGHT_H_
#define _LIGHT_H_


#include "Model.h"


namespace sge
{
	// Enumeration of light types and their associated values
	enum ELightTypes
	{
		POINT_LIGHT = 0, SPOT_LIGHT = 1
	};

	//====================================================================================
	// LIGHT CLASS : CHILD OF MODEL CLASS
	//------------------------------------------------------------------------------------
	class CLight : public CModel
	{
	public:
		// CONSTRUCTOR & DESTRUCTOR
		//---------------------------------
		CLight(UINT id, CMesh* pMesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 orientation, DirectX::XMFLOAT3 scale,
			DirectX::XMFLOAT3 lightColour, int lightType);
		~CLight();


		// METHODS
		//---------------------------------
		// Render the model using its mesh data & matrix data
		void Render(ID3D10Device* pDevice, CShader* pShader);


		// ACCESSORS
		//---------------------------------
		inline DirectX::XMFLOAT3 GetLightColour()
		{
			return mBrightnessColour;
		}

		inline DirectX::XMFLOAT3 GetLightFacing()
		{
			return DirectX::XMFLOAT3(mModelMatrix._31, mModelMatrix._32, mModelMatrix._33);
		}

		inline int GetType()
		{
			return mLightType;
		}

		inline float GetCosHalfAngle()
		{
			return mCosHalfAngle;
		}


		// MUTATORS
		//---------------------------------
		void SetLightColour(DirectX::XMFLOAT3 colour);
		void SetLightBrightness(float brightness);
		void SetSpotLightAngle(float angle);


	private:
		// LIGHT INFORMATION
		//---------------------------------
		DirectX::XMFLOAT3 mColour;
		DirectX::XMFLOAT3 mBrightnessColour;
		float mBrightness;
		float mConeAngle;
		float mCosHalfAngle;
		int mLightType;
	};
}


#endif /* _LIGHT_H_ */

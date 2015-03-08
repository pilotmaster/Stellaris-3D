//====================================================================================
// FILE:    Light.h ------------------------------------------------------------------
//------------------------------------------------------------------------------------

#ifndef _LIGHT_H_
#define _LIGHT_H_


#include "Model.h"


namespace sge
{
	//====================================================================================
	// LIGHT CLASS : CHILD OF MODEL CLASS
	//------------------------------------------------------------------------------------
	class CLight : public CModel
	{
	public:
		// CONSTRUCTOR & DESTRUCTOR
		//---------------------------------
		CLight(UINT id, CMesh* pMesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 orientation, DirectX::XMFLOAT3 scale,
			DirectX::XMFLOAT3 lightColour);
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


		// MUTATORS
		//---------------------------------
		void SetLightColour(DirectX::XMFLOAT3 colour);
		void SetLightBrightness(float brightness);


	private:
		// LIGHT INFORMATION
		//---------------------------------
		DirectX::XMFLOAT3 mColour;
		DirectX::XMFLOAT3 mBrightnessColour;
		float mBrightness;
	};
}


#endif /* _LIGHT_H_ */

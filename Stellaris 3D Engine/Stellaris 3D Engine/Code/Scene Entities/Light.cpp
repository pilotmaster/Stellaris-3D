//====================================================================================
// FILE:    Light.cpp ----------------------------------------------------------------
//------------------------------------------------------------------------------------

#include "Light.h"


namespace sge
{
	//====================================================================================
	// LIGHT CLASS CONSTRUCTOR & DESTRUCTOR
	//------------------------------------------------------------------------------------
	CLight::CLight(UINT id, CMesh* pMesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, DirectX::XMFLOAT3 scale,
		DirectX::XMFLOAT3 lightColour) :
		CModel(id, pMesh, pos, rot, scale), mColour(lightColour), mBrightness(1.0f)
	{
		mBrightnessColour.x = mColour.x * mBrightness;
		mBrightnessColour.y = mColour.y * mBrightness;
		mBrightnessColour.z = mColour.z * mBrightness;
	}

	CLight::~CLight()
	{

	}


	//====================================================================================
	// LIGHT CLASS METHODS
	//------------------------------------------------------------------------------------
	void CLight::SetLightColour(DirectX::XMFLOAT3 colour)
	{
		// Store the colour
		mColour = colour;

		// Use the brightness of the light to determine new colour
		mBrightnessColour.x = mColour.x * mBrightness;
		mBrightnessColour.y = mColour.y * mBrightness;
		mBrightnessColour.z = mColour.z * mBrightness;
	}

	void CLight::SetLightBrightness(float brightness)
	{
		// Store the brightness
		mBrightness = brightness;

		// Use brightness as a scale modifier
		SetScale(DirectX::XMFLOAT3(mBrightness, mBrightness, mBrightness));

		// Adjust light colour based on brightness
		mBrightnessColour.x = mColour.x * mBrightness;
		mBrightnessColour.y = mColour.y * mBrightness;
		mBrightnessColour.z = mColour.z * mBrightness;
	}

	void CLight::Render(ID3D10Device* pDevice, CShader* pShader)
	{
		// Before rendering, ensure model has geometry
		if (mpMesh)
		{
			// Send model data over required by model's shader
			pShader->GetFXWorldVar()->SetMatrix((float*)&mModelMatrix);

			// Check if the model contains colour values
			if (mpMesh->GetRenderType() == R_LIGHT)
			{
				// Send over colour data
				pShader->GetFXModelColour()->SetRawValue(&mBrightnessColour, 0U, 12U);
			}

			// If it has a texture, send it over
			if (mpMesh->GetMaterial())
			{
				pShader->GetFXDiffuseMapVar()->SetResource(mpMesh->GetMaterial()->GetDiffuseMap());
			}

			// Render the mesh
			mpMesh->Render(pDevice);
		}
	}
}
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
		DirectX::XMFLOAT3 lightColour, int lightType) :
		CModel(id, pMesh, pos, rot, scale), mLightType(lightType), mColour(lightColour), mBrightness(2.0f), mConeAngle(90.0f)
	{
		mBrightnessColour.x = mColour.x * mBrightness;
		mBrightnessColour.y = mColour.y * mBrightness;
		mBrightnessColour.z = mColour.z * mBrightness;

		// Set default angle & find the half cos angle for spotlight use
		mCosHalfAngle = cosf(DirectX::XMConvertToRadians(mConeAngle / 2.0f));
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
		SetScale(DirectX::XMFLOAT3(mBrightness / 5.0f, mBrightness / 5.0f, mBrightness / 5.0f));

		// Adjust light colour based on brightness
		mBrightnessColour.x = mColour.x * mBrightness;
		mBrightnessColour.y = mColour.y * mBrightness;
		mBrightnessColour.z = mColour.z * mBrightness;
	}

	void CLight::SetSpotLightAngle(float angle)
	{
		mConeAngle = angle;
		mCosHalfAngle = cosf(DirectX::XMConvertToRadians(mConeAngle / 2.0f));
	}

	void CLight::Update()
	{
		// Call paren'ts update function
		CModel::Update();

		// If it is a spot light, update the view and projection matrices
		if (mLightType == SPOT_LIGHT)
		{
			// View matrix
			DirectX::XMMATRIX matView = DirectX::XMMatrixInverse(nullptr, DirectX::XMLoadFloat4x4(&mModelMatrix));
			DirectX::XMStoreFloat4x4(&mViewMatrix, matView);

			// Projection matrix
			DirectX::XMMATRIX matProj = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(mConeAngle), 1.0f, 0.01f, 1000.0f);
			DirectX::XMStoreFloat4x4(&mProjMatrix, matProj);
		}
	}

	void CLight::Render(ID3D10Device* pDevice, CShader* pShader, bool mirrored, bool forShadow)
	{
		// If rendering for a shadow - do not take into consideration the lights
		if (forShadow) return;

		// Before rendering, ensure model has geometry
		if (mpMesh)
		{
			// Send model data over required by model's shader
			pShader->GetFXWorldVar()->SetMatrix((float*)&mModelMatrix);

			// Send over colour data
			pShader->GetFXModelColourVar()->SetFloatVector((float*)&mBrightnessColour);

			// If it has a texture, send it over
			if (mpMesh->GetMaterial())
			{
				pShader->GetFXDiffuseMapVar()->SetResource(mpMesh->GetMaterial()->GetDiffuseMap());
			}

			// Render the mesh
			if (mirrored)
			{
				mpMesh->Render(pDevice, pShader->GetAdditiveTintTexMirrorTechnique());
			}
			else
			{
				mpMesh->Render(pDevice, mpMesh->GetRenderTechnique());
			}
		}
	}
}
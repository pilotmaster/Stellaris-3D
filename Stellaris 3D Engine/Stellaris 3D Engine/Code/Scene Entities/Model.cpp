//====================================================================================
// FILE:    Model.cpp ----------------------------------------------------------------
//------------------------------------------------------------------------------------

#include "Model.h"


namespace sge
{
	//====================================================================================
	// MODEL CLASS CONSTRUCTOR & DESTRUCTOR
	//------------------------------------------------------------------------------------
	CModel::CModel(UINT id, CMesh* pMesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, DirectX::XMFLOAT3 scale) :
		CEntity(id, pos, rot, scale), mWiggle(0.0f), mParallaxDepth(0.08f), mOutlineThickness(0.01f),
		mModelColour(DirectX::XMFLOAT3(0.1f, 0.3f, 0.9f))
	{
		if (pMesh)
		{
			mpMesh = pMesh;
		}
		else
		{
			mpMesh = nullptr;
		}
	}

	CModel::~CModel()
	{

	}


	//====================================================================================
	// MODEL CLASS METHODS
	//------------------------------------------------------------------------------------
	void CModel::Update()
	{
		// Update the model's matrices
		UpdateMatrices();
	}

	void CModel::Render(ID3D10Device* pDevice, CShader* pShader)
	{
		// Before rendering, ensure model has geometry
		if (mpMesh)
		{
			// Send model data over required by model's shader
			pShader->GetFXWorldVar()->SetMatrix((float*)&mModelMatrix);

			// If it has a texture, send it over
			if (mpMesh->GetMaterial())
			{
				pShader->GetFXDiffuseMapVar()->SetResource(mpMesh->GetMaterial()->GetDiffuseMap());
			}

			// If it has a normal map, send it over
			if (mpMesh->GetMaterial()->GetNormalMap())
			{
				pShader->GetFXNormalMapVar()->SetResource(mpMesh->GetMaterial()->GetNormalMap());
			}

			// Set model colour & wiggle values
			pShader->GetFXWiggleVar()->SetFloat(mWiggle);
			pShader->GetFXModelColourVar()->SetRawValue(&mModelColour, 0U, 12U);
			pShader->GetFXParallaxDepthVar()->SetFloat(mParallaxDepth);
			pShader->GetFXOutlineThicknessVar()->SetFloat(mOutlineThickness);

			// Render the mesh
			mpMesh->Render(pDevice);
		}
	}
}

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
		CEntity(id, pos, rot, scale)
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

			// Render the mesh
			mpMesh->Render(pDevice);
		}
	}
}

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

	void CModel::Render(ID3D10Device* pDevice, ID3D10EffectTechnique* pTech)
	{
		// Before rendering, ensure model has geometry
		if (mpMesh)
		{
			// Render the mesh
			mpMesh->Render(pDevice, pTech);
		}
	}
}

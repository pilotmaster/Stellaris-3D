//====================================================================================
// FILE:    Model.cpp ----------------------------------------------------------------
//------------------------------------------------------------------------------------

#include "Model.h"


namespace sge
{
	//====================================================================================
	// MODEL CLASS CONSTRUCTOR & DESTRUCTOR
	//------------------------------------------------------------------------------------
	CModel::CModel()
	{
		mHasGeometry = false;
		mpMesh = nullptr;
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
		if (mHasGeometry)
		{
			return;
		}

		// Select vertex and index buffer - assuming all data will be as triangle lists
		UINT offset = 0;
		pDevice->IASetVertexBuffers(0, 1, mpMesh->GetVertexBuffer(), mpMesh->GetVertexSize(), &offset);
		pDevice->IASetInputLayout(mpMesh->GetInputLayout());
		pDevice->IASetIndexBuffer(mpMesh->GetIndexBuffer(), DXGI_FORMAT_R16_UINT, 0);
		pDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Render the model. All the data and shader variables are prepared, now select the technique to use and draw.
		// The loop is for advanced techniques that need multiple passes - we will only use techniques with one pass
		D3D10_TECHNIQUE_DESC techDesc;
		pTech->GetDesc(&techDesc);
		for (UINT p = 0; p < techDesc.Passes; ++p)
		{
			pTech->GetPassByIndex(p)->Apply(0);
			pDevice->DrawIndexed(mpMesh->GetNumIndices(), 0, 0);
		}
		pDevice->DrawIndexed(mpMesh->GetNumIndices(), 0, 0);
	}
}

//====================================================================================
// FILE:    Model.h ------------------------------------------------------------------
//------------------------------------------------------------------------------------

#ifndef _MODEL_H_
#define _MODEL_H_


#include "Entity.h"
#include "..\Utilities\Mesh.h"
#include "..\Shaders\Shader.h"


namespace sge
{
	//====================================================================================
	// MODEL CLASS : CHILD OF ENTITY CLASS
	//------------------------------------------------------------------------------------
	class CModel : public CEntity
	{
	public:
		// CONSTRUCTOR & DESTRUCTOR
		//---------------------------------
		CModel(UINT id, CMesh* pMesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, DirectX::XMFLOAT3 scale);
		~CModel();


		// METHODS
		//---------------------------------
		// Update the matrices, position, scale, etc. of the model
		void Update();
		// Render the model using its mesh data & matrix data
		void Render(ID3D10Device* pDevice, CShader* pShader);


	private:
		// MODEL DATA
		//---------------------------------
		CMesh* mpMesh;
	};
}

#endif /* _MODEL_H_ */

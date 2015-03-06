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
		CModel(UINT id, DirectX::XMFLOAT3 pos = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3 rot = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
			DirectX::XMFLOAT3 scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
		~CModel();


		// METHODS
		//---------------------------------
		// Update the matrices, position, scale, etc. of the model
		void Update();
		// Render the model using its mesh data & matrix data
		void Render(ID3D10Device* pDevice, ID3D10EffectTechnique* pTech);


	private:
		// MODEL DATA
		//---------------------------------
		bool mHasGeometry;
		CMesh* mpMesh;
	};
}

#endif /* _MODEL_H_ */

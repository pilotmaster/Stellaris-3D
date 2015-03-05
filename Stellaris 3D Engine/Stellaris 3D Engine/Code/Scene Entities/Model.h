//====================================================================================
// FILE:    Model.h ------------------------------------------------------------------
//------------------------------------------------------------------------------------

#ifndef _MODEL_H_
#define _MODEL_H_


#include "Entity.h"
#include "..\Utilities\Mesh.h"


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
		CModel();
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

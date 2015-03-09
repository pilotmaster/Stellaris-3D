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
		virtual void Render(ID3D10Device* pDevice, CShader* pShader);


		// ACCESSORS
		//---------------------------------
		inline float GetWiggle()
		{
			return mWiggle;
		}

		inline float GetParallaxDepth()
		{
			return mParallaxDepth;
		}

		inline float GetOutlineThickness()
		{
			return mOutlineThickness;
		}

		inline DirectX::XMFLOAT3 GetModelColour()
		{
			return mModelColour;
		}


		// MUTATORS
		//---------------------------------
		inline void IncrementWiggle(float amount)
		{
			mWiggle += amount;
		}

		inline void SetParallaxDepth(float amount)
		{
			mParallaxDepth = amount;
		}

		inline void SetOutlineThickness(float amount)
		{
			mOutlineThickness = amount;
		}

		inline void SetModelColour(DirectX::XMFLOAT3 colour)
		{
			mModelColour = colour;
		}


	protected:
		// MODEL DATA
		//---------------------------------
		CMesh* mpMesh;
		DirectX::XMFLOAT3 mModelColour;
		float mWiggle;
		float mParallaxDepth;
		float mOutlineThickness;
	};
}

#endif /* _MODEL_H_ */

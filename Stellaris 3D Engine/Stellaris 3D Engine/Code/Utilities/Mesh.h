//====================================================================================
// FILE:    Mesh.h -------------------------------------------------------------------
//------------------------------------------------------------------------------------

#ifndef _MESH_H_
#define _MESH_H_


#include "Material.h"
#include <sstream>


namespace sge
{
	//====================================================================================
	// ENUMERATION OF ENTITY RENDERING TYPES
	//------------------------------------------------------------------------------------
	enum ERenderTypes
	{
		FX_COLOURED, FX_TEXTURED, FX_WIGGLE, FX_LIT_TEXTURED, FX_LIGHT,	FX_NORMAL_MAPPED,
		FX_PARALLAX, FX_CELL_SHADED
	};

	
	//====================================================================================
	// MESH CLASS
	//------------------------------------------------------------------------------------
	class CStellaris3D;

	class CMesh
	{
	public:
		// CONSTRUCTOR & DESTRUCTOR
		//---------------------------------
		CMesh();
		~CMesh();


		// METHODS
		//---------------------------------
		// Release all of the resources used by the mesh
		void ReleaseResources();
		// Load in a mesh for use as a model
		bool LoadMesh(ID3D10Device* pDevice, std::string& fileName, ID3D10EffectTechnique* pTech, ERenderTypes renderType, bool tangents = false);

		// Render the model using its mesh data & matrix data
		void Render(ID3D10Device* pDevice);
		void Render(ID3D10Device* pDevice, ID3D10EffectTechnique* pTech);


		// ACCESSORS
		//---------------------------------
		inline ID3D10Buffer** GetVertexBuffer()
		{
			return &mpVertexBuffer;
		}

		inline ID3D10Buffer* GetIndexBuffer()
		{
			return mpIndexBuffer;
		}

		inline UINT GetNumVertices()
		{
			return mNumVertices;
		}

		inline UINT* GetVertexSize()
		{
			return &mVertexSize;
		}

		inline UINT GetNumIndices()
		{
			return mNumIndices;
		}

		inline ID3D10InputLayout* GetInputLayout()
		{
			return mpInputLayout;
		}

		inline CMaterial* GetMaterial()
		{
			return mpMaterial;
		}

		inline ERenderTypes GetRenderType()
		{
			return mRenderType;
		}


		// MUTATORS
		//---------------------------------



	private:
		// VERTEX DATA
		//---------------------------------
		ID3D10Buffer* mpVertexBuffer;
		UINT mNumVertices;


		// VERTEX ELEMENTS DATA
		//---------------------------------
		static const UINT MAX_VERT_ELMNTS = 64;
		D3D10_INPUT_ELEMENT_DESC mVertexElmnts[MAX_VERT_ELMNTS];
		ID3D10InputLayout* mpInputLayout;
		UINT mVertexSize;


		// INDEX DATA
		//---------------------------------
		ID3D10Buffer* mpIndexBuffer;
		UINT mNumIndices;


		// MATERIAL & RENDERING DATA
		//---------------------------------
		CMaterial* mpMaterial;
		ERenderTypes mRenderType;
		ID3D10EffectTechnique* mpRenderTech;
	};
}


#endif /* _MESH_H_ */

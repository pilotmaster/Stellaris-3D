//====================================================================================
// FILE:    Mesh.h -------------------------------------------------------------------
//------------------------------------------------------------------------------------

#ifndef _MESH_H_
#define _MESH_H_


#include <D3DX10.h>
#include <d3d10.h>
#include <DirectXMath.h>
#include <string>


namespace sge
{
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
		bool LoadMesh(ID3D10Device* pDevice, std::string& fileName, ID3D10EffectTechnique* pTech, bool tangents = false);

		// Render the model using its mesh data & matrix data
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

		// FOR LATER POTENTIAL TEXTURE CLASS
		ID3D10ShaderResourceView* mpDiffuseMap;
		ID3D10ShaderResourceView* mpNormalMap;
	};
}


#endif /* _MESH_H_ */

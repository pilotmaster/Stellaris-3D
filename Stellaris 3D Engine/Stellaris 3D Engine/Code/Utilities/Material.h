//====================================================================================
// FILE:    Material.h ---------------------------------------------------------------
//------------------------------------------------------------------------------------

#ifndef _MATERIAL_H_
#define _MATERIAL_H_


#include <D3DX10.h>
#include <d3d10.h>
#include <DirectXMath.h>
#include <string>


namespace sge
{
	//====================================================================================
	// MATERIAL CLASS
	//------------------------------------------------------------------------------------
	class CMaterial
	{
	public:
		// CONSTRUCTOR & DESTRUCTOR
		//---------------------------------
		CMaterial();
		~CMaterial();


		// METHODS
		//---------------------------------
		// Creates a diffuse map from the mesh file
		void CreateDiffuseMap(ID3D10Device* pDevice, std::wstring &wTexName);
		// Creates a normal map from the mesh file
		void CreateNormalMap(ID3D10Device* pDevice, std::wstring &wTexName);


		// ACCESSORS
		//---------------------------------
		ID3D10ShaderResourceView* GetDiffuseMap()
		{
			return mpDiffuseMap;
		}

		ID3D10ShaderResourceView* GetNormalMap()
		{
			return mpNormalMap;
		}


		// MUTATORS
		//---------------------------------
		inline void SetDiffuseMap(ID3D10ShaderResourceView* pShaderResource)
		{
			// If there is already a diffuse map, release it
			mpDiffuseMap = pShaderResource;
		}


	private:
		// TEXTURE DATA
		//---------------------------------
		ID3D10ShaderResourceView* mpDiffuseMap;
		ID3D10ShaderResourceView* mpNormalMap;
	};
}

#endif /* _MATERIAL_H_ */

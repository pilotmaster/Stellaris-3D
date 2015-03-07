//====================================================================================
// FILE:    Material.cpp -------------------------------------------------------------
//------------------------------------------------------------------------------------

#include "Material.h"


namespace sge
{
	//====================================================================================
	// MATERIAL CLASS CONSTRUCTOR & DESTRUCTOR
	//------------------------------------------------------------------------------------
	CMaterial::CMaterial()
	{
		mpDiffuseMap = nullptr;
		mpNormalMap = nullptr;
	}

	CMaterial::~CMaterial()
	{

	}


	//====================================================================================
	// MATERIAL CLASS METHODS
	//------------------------------------------------------------------------------------
	void CMaterial::CreateDiffuseMap(ID3D10Device* pDevice, std::wstring &wTexName)
	{
		D3DX10CreateShaderResourceViewFromFile(pDevice, wTexName.c_str(), NULL, NULL, &mpDiffuseMap, NULL);
	}
}

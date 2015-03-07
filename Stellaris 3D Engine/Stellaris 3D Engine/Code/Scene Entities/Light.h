//====================================================================================
// FILE:    Light.h ------------------------------------------------------------------
//------------------------------------------------------------------------------------

#ifndef _LIGHT_H_
#define _LIGHT_H_


#include "Model.h"


namespace sge
{
	//====================================================================================
	// LIGHT CLASS : CHILD OF MODEL CLASS
	//------------------------------------------------------------------------------------
	class CLight : public CModel
	{
	public:
		// CONSTRUCTOR & DESTRUCTOR
		//---------------------------------
		CLight(UINT id, CMesh* pMesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 orientation, DirectX::XMFLOAT3 scale,
			DirectX::XMFLOAT3 lightColour);
		~CLight();


		// METHODS
		//---------------------------------



		// ACCESSORS
		//---------------------------------
		inline DirectX::XMFLOAT3 SetLightColour()
		{
			return mColour;
		}


		// MUTATORS
		//---------------------------------
		inline void SetLightColour(DirectX::XMFLOAT3& colour)
		{
			mColour = colour;
		}


	private:
		// LIGHT INFORMATION
		//---------------------------------
		DirectX::XMFLOAT3 mColour;
	};
}


#endif /* _LIGHT_H_ */

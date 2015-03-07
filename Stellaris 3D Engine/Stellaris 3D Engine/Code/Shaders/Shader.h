//====================================================================================
// FILE:    Shader.h -----------------------------------------------------------------
//------------------------------------------------------------------------------------

#ifndef _SHADER_H_
#define _SHADER_H_

#include <D3DX10.h>
#include <d3d10.h>
#include <DirectXMath.h>
#include <atlbase.h>


namespace sge
{
	//====================================================================================
	// SHADER CLASS
	//------------------------------------------------------------------------------------
	class CShader
	{
	public:
		// CONSTRUCTOR & DESTRUCTOR
		//---------------------------------
		CShader(ID3D10Device* pDevice);
		~CShader();


		// ACCESSORS
		//---------------------------------
		ID3D10EffectTechnique* GetTechnique()
		{
			return mpFXTech;
		}

		ID3D10EffectMatrixVariable* GetFXWorldVar()
		{
			return mpFXVarWorldMat;
		}

		ID3D10EffectMatrixVariable* GetFXViewVar()
		{
			return mpFXVarViewMat;
		}

		ID3D10EffectMatrixVariable* GetFXProjVar()
		{
			return mpFXVarProjMat;
		}

		ID3D10EffectShaderResourceVariable* GetFXDiffuseMapVar()
		{
			return mpFXVarDiffuseMap;
		}


	protected:
		// EFFECT VARIABLES
		//---------------------------------
		ID3D10Effect* mpFX;
		ID3D10EffectTechnique* mpFXTech;
		ID3D10EffectMatrixVariable* mpFXVarWorldMat;
		ID3D10EffectMatrixVariable* mpFXVarViewMat;
		ID3D10EffectMatrixVariable* mpFXVarProjMat;
		ID3D10EffectShaderResourceVariable* mpFXVarDiffuseMap;
	};
}


#endif /* _SHADER_H_ */

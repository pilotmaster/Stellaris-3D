//====================================================================================
// FILE:    Shader.cpp ---------------------------------------------------------------
//------------------------------------------------------------------------------------

#include "Shader.h"


namespace sge
{
	//====================================================================================
	// MODEL CLASS CONSTRUCTOR & DESTRUCTOR
	//------------------------------------------------------------------------------------
	CShader::CShader(ID3D10Device* pDevice)
	{
		// Give variables default values
		mpFX = nullptr;
		mpFXTech = nullptr;
		mpFXVarDiffuseMap = nullptr;
		mpFXVarProjMat = nullptr;
		mpFXVarViewMat = nullptr;
		mpFXVarWorldMat = nullptr;


		// Load in the shader
		// COMPILE SHADER INTO MEMORY
		//---------------------------------
		DWORD shaderFlags = 0;
#if defined( DEBUG ) || defined( _DEBUG )
		shaderFlags |= D3D10_SHADER_DEBUG;
		shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif

		// CREATE SHADER
		//---------------------------------
		ID3D10Blob* compShader;
		ID3D10Blob* compMsgs;
		HRESULT hr;
		hr = D3DX10CreateEffectFromFile(L"GraphicsAssign1.fx", NULL, NULL, "fx_4_0", shaderFlags, 0,
			pDevice, NULL, NULL, &mpFX, &compShader, NULL);

		if (FAILED(hr))
		{
			if (compShader != 0)
			{
				MessageBox(NULL, CA2CT(reinterpret_cast<char*>(compShader->GetBufferPointer())), L"Error", MB_OK); // Compiler error: display error message
			}
			else
			{
				// No error message, which probably means the fx file was not found
				MessageBox(NULL, L"Error loading FX file. Ensure you run this executable from the directory that contains the FX file.", L"Error", MB_OK);
			}
			return;
		}


		// GET SHADER TECHNIQUES & VARIABLES
		//---------------------------------
		mpFXTech = mpFX->GetTechniqueByName("PlainColour");

		mpFXVarDiffuseMap = mpFX->GetVariableByName("DiffuseMap")->AsShaderResource();
		mpFXVarProjMat = mpFX->GetVariableByName("ProjMatrix")->AsMatrix();
		mpFXVarViewMat = mpFX->GetVariableByName("ViewMatrix")->AsMatrix();
		mpFXVarWorldMat = mpFX->GetVariableByName("WorldMatrix")->AsMatrix();
	}

	CShader::~CShader()
	{
		if (mpFX) mpFX->Release();
	}
}

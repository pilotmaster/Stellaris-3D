//====================================================================================
// FILE:    Shader.cpp ---------------------------------------------------------------
//------------------------------------------------------------------------------------

#include "Shader.h"


namespace sge
{
	//====================================================================================
	// SHADER CLASS CONSTRUCTOR & DESTRUCTOR
	//------------------------------------------------------------------------------------
	CShader::CShader(ID3D10Device* pDevice)
	{
		// Give variables default values
		mpFX = nullptr;
		mpFXLitTexTech = nullptr;
		mpFXLightDrawTech = nullptr;

		mpFXVarDiffuseMap = nullptr;
		mpFXVarProjMat = nullptr;
		mpFXVarViewMat = nullptr;
		mpFXVarWorldMat = nullptr;

		mpFXVarLightPositions = nullptr;
		mpFXVarLightColours = nullptr;
		mpFXVarModelColour = nullptr;
		mpFXVarAmbientColour = nullptr;
		mpFXVarCameraPosition = nullptr;

		mpFXVarSpecularPower = nullptr;


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
			PostQuitMessage(1);
			return;
		}


		// GET SHADER TECHNIQUES & VARIABLES
		//---------------------------------
		mpFXLitTexTech = mpFX->GetTechniqueByName("LitTextureTech");
		mpFXLightDrawTech = mpFX->GetTechniqueByName("LightDrawTech");

		mpFXVarDiffuseMap = mpFX->GetVariableByName("DiffuseMap")->AsShaderResource();

		mpFXVarProjMat = mpFX->GetVariableByName("ProjMatrix")->AsMatrix();
		mpFXVarViewMat = mpFX->GetVariableByName("ViewMatrix")->AsMatrix();
		mpFXVarWorldMat = mpFX->GetVariableByName("WorldMatrix")->AsMatrix();

		mpFXVarLightPositions = mpFX->GetVariableByName("LightPos")->AsVector();
		mpFXVarLightColours = mpFX->GetVariableByName("LightCol")->AsVector();
		mpFXVarModelColour = mpFX->GetVariableByName("ModelColour")->AsVector();
		mpFXVarAmbientColour = mpFX->GetVariableByName("AmbientColour")->AsVector();
		mpFXVarCameraPosition = mpFX->GetVariableByName("CameraPos")->AsVector();

		mpFXVarSpecularPower = mpFX->GetVariableByName("SpecularPower")->AsScalar();
	}

	CShader::~CShader()
	{
		if (mpFX) mpFX->Release();
	}


	//====================================================================================
	// SHADER CLASS CONSTRUCTOR & DESTRUCTOR
	//------------------------------------------------------------------------------------
	ID3D10Effect* CShader::GetEffect()
	{
		return mpFX;
	}

	ID3D10EffectTechnique* CShader::GetLitTexTechnique()
	{
		return mpFXLitTexTech;
	}

	ID3D10EffectTechnique* CShader::GetLightDrawTechnique()
	{
		return mpFXLightDrawTech;
	}

	ID3D10EffectMatrixVariable* CShader::GetFXWorldVar()
	{
		return mpFXVarWorldMat;
	}

	ID3D10EffectMatrixVariable* CShader::GetFXViewVar()
	{
		return mpFXVarViewMat;
	}

	ID3D10EffectMatrixVariable* CShader::GetFXProjVar()
	{
		return mpFXVarProjMat;
	}

	ID3D10EffectShaderResourceVariable* CShader::GetFXDiffuseMapVar()
	{
		return mpFXVarDiffuseMap;
	}

	ID3D10EffectVectorVariable* CShader::GetFXLightPositions()
	{
		return mpFXVarLightPositions;
	}

	ID3D10EffectVectorVariable* CShader::GetFXLightColours()
	{
		return mpFXVarLightColours;
	}

	ID3D10EffectVectorVariable* CShader::GetFXModelColour()
	{
		return mpFXVarModelColour;
	}

	ID3D10EffectVectorVariable* CShader::GetFXAmbientColour()
	{
		return mpFXVarAmbientColour;
	}

	ID3D10EffectVectorVariable* CShader::GetFXCameraPosition()
	{
		return mpFXVarCameraPosition;
	}

	ID3D10EffectScalarVariable* CShader::GetFXSpecularPower()
	{
		return mpFXVarSpecularPower;
	}
}

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
		mpFXWiggleTech = nullptr;
		mpFXNormalMappingTech = nullptr;
		mpFXParallaxMappingTech = nullptr;
		mpFXCellShadingTech = nullptr;

		mpFXVertexLitTexMirrorTech = nullptr;
		mpFXAdditiveTintTexMirrorTech = nullptr;
		mpFXMirrorClearTech = nullptr;
		mpFXMirrorSurfaceTech = nullptr;

		mpFXVarDiffuseMap = nullptr;
		mpFXVarNormalMap = nullptr;
		mpFXVarShadowMap = nullptr;

		mpFXVarProjMat = nullptr;
		mpFXVarViewMat = nullptr;
		mpFXVarWorldMat = nullptr;
		mpFXVarLightViewMat = nullptr;
		mpFXVarLightProjMat = nullptr;

		mpFXVarLightPositions = nullptr;
		mpFXVarLightColours = nullptr;
		mpFXVarModelColour = nullptr;
		mpFXVarCosHalfAngle = nullptr;
		mpFXVarLightType = nullptr;
		mpFXVarLightFacings = nullptr;
		mpFXVarAmbientColour = nullptr;
		mpFXVarCameraPosition = nullptr;
		mpFXVarClipPlane = nullptr;

		mpFXVarSpecularPower = nullptr;
		mpFXVarWiggle = nullptr;
		mpFXVarParallaxDepth = nullptr;
		mpFXVarOutlineThickness = nullptr;
		mpFXVarLightType = nullptr;
		mpFXVarCosHalfAngle = nullptr;


		// Load in the shader
		// COMPILE SHADER INTO MEMORY
		//---------------------------------
		DWORD shaderFlags = 0;
#if defined( DEBUG ) || defined( _DEBUG )
		shaderFlags |= D3D10_SHADER_ENABLE_STRICTNESS;
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
		mpFXWiggleTech = mpFX->GetTechniqueByName("WiggleTech");
		mpFXNormalMappingTech = mpFX->GetTechniqueByName("NormalMappingTech"); 
		mpFXParallaxMappingTech = mpFX->GetTechniqueByName("ParallaxMappingTech");
		mpFXCellShadingTech = mpFX->GetTechniqueByName("CellShadingTech");
		mpFXDepthOnlyTech = mpFX->GetTechniqueByName("DepthOnlyTech");

		mpFXVertexLitTexMirrorTech = mpFX->GetTechniqueByName("VertexLitTexMirrorTech");
		mpFXAdditiveTintTexMirrorTech = mpFX->GetTechniqueByName("AdditiveTexTintMirrorTech");
		mpFXMirrorClearTech = mpFX->GetTechniqueByName("MirrorClearTech");
		mpFXMirrorSurfaceTech = mpFX->GetTechniqueByName("MirrorSurfaceTech");

		mpFXVarDiffuseMap = mpFX->GetVariableByName("DiffuseMap")->AsShaderResource();
		mpFXVarNormalMap = mpFX->GetVariableByName("NormalMap")->AsShaderResource();
		mpFXVarShadowMap = mpFX->GetVariableByName("ShadowMap")->AsShaderResource();

		mpFXVarProjMat = mpFX->GetVariableByName("ProjMatrix")->AsMatrix();
		mpFXVarViewMat = mpFX->GetVariableByName("ViewMatrix")->AsMatrix();
		mpFXVarWorldMat = mpFX->GetVariableByName("WorldMatrix")->AsMatrix();
		mpFXVarLightViewMat = mpFX->GetVariableByName("LightViewMatrix")->AsMatrix();
		mpFXVarLightProjMat = mpFX->GetVariableByName("LightProjMatrix")->AsMatrix();

		mpFXVarLightPositions = mpFX->GetVariableByName("LightPos")->AsVector(); 
		mpFXVarLightColours = mpFX->GetVariableByName("LightCol")->AsVector();
		mpFXVarLightFacings = mpFX->GetVariableByName("LightFacing")->AsVector();
		mpFXVarModelColour = mpFX->GetVariableByName("ModelColour")->AsVector();
		mpFXVarAmbientColour = mpFX->GetVariableByName("AmbientColour")->AsVector();
		mpFXVarCameraPosition = mpFX->GetVariableByName("CameraPos")->AsVector();
		mpFXVarClipPlane = mpFX->GetVariableByName("ClipPlane")->AsVector();

		mpFXVarSpecularPower = mpFX->GetVariableByName("SpecularPower")->AsScalar();
		mpFXVarWiggle = mpFX->GetVariableByName("Wiggle")->AsScalar();
		mpFXVarParallaxDepth = mpFX->GetVariableByName("ParallaxDepth")->AsScalar();
		mpFXVarOutlineThickness = mpFX->GetVariableByName("OutlineThickness")->AsScalar();
		mpFXVarLightType = mpFX->GetVariableByName("LightType")->AsScalar();
		mpFXVarCosHalfAngle = mpFX->GetVariableByName("LightCosHalfAngle")->AsScalar();
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

	ID3D10EffectTechnique* CShader::GetWiggleTechnique()
	{
		return mpFXWiggleTech;
	}

	ID3D10EffectTechnique* CShader::GetNormalMappingTechnique()
	{
		return mpFXNormalMappingTech;
	}

	ID3D10EffectTechnique* CShader::GetParallaxMappingTechnique()
	{
		return mpFXParallaxMappingTech;
	}

	ID3D10EffectTechnique* CShader::GetCellShadingTechnique()
	{
		return mpFXCellShadingTech;
	}

	ID3D10EffectTechnique* CShader::GetDepthOnlyTechnique()
	{
		return mpFXDepthOnlyTech;
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

	ID3D10EffectMatrixVariable* CShader::GetFXLightViewVar()
	{
		return mpFXVarLightViewMat;
	}

	ID3D10EffectMatrixVariable* CShader::GetFXLightProjVar()
	{
		return mpFXVarLightProjMat;
	}

	ID3D10EffectShaderResourceVariable* CShader::GetFXDiffuseMapVar()
	{
		return mpFXVarDiffuseMap;
	}

	ID3D10EffectShaderResourceVariable* CShader::GetFXNormalMapVar()
	{
		return mpFXVarNormalMap;
	}

	ID3D10EffectShaderResourceVariable* CShader::GetFXShadowMapVar()
	{
		return mpFXVarShadowMap;
	}

	ID3D10EffectVectorVariable* CShader::GetFXLightPositionsVar()
	{
		return mpFXVarLightPositions;
	}

	ID3D10EffectVectorVariable* CShader::GetFXLightColoursVar()
	{
		return mpFXVarLightColours;
	}

	ID3D10EffectVectorVariable* CShader::GetFXLightFacingsVar()
	{
		return mpFXVarLightFacings;
	}

	ID3D10EffectVectorVariable* CShader::GetFXModelColourVar()
	{
		return mpFXVarModelColour;
	}

	ID3D10EffectVectorVariable* CShader::GetFXAmbientColourVar()
	{
		return mpFXVarAmbientColour;
	}

	ID3D10EffectVectorVariable* CShader::GetFXCameraPositionVar()
	{
		return mpFXVarCameraPosition;
	}

	ID3D10EffectVectorVariable* CShader::GetFXClipPlaneVar()
	{
		return mpFXVarClipPlane;
	}

	ID3D10EffectScalarVariable* CShader::GetFXSpecularPowerVar()
	{
		return mpFXVarSpecularPower;
	}

	ID3D10EffectScalarVariable* CShader::GetFXWiggleVar()
	{
		return mpFXVarWiggle;
	}

	ID3D10EffectScalarVariable* CShader::GetFXParallaxDepthVar()
	{
		return mpFXVarParallaxDepth;
	}

	ID3D10EffectScalarVariable* CShader::GetFXOutlineThicknessVar()
	{
		return mpFXVarOutlineThickness;
	}

	ID3D10EffectScalarVariable* CShader::GetFXLightTypeVar()
	{
		return mpFXVarLightType;
	}

	ID3D10EffectScalarVariable* CShader::GetFXCosHalfAngleVar()
	{
		return mpFXVarCosHalfAngle;
	}

	// Effect techniques for mirror rendering
	ID3D10EffectTechnique* CShader::GetVertexLitTexMirrorTechnique()
	{
		return mpFXVertexLitTexMirrorTech;
	}

	ID3D10EffectTechnique* CShader::GetAdditiveTintTexMirrorTechnique()
	{
		return mpFXAdditiveTintTexMirrorTech;
	}

	ID3D10EffectTechnique* CShader::GetMirrorClearTechnique()
	{
		return mpFXMirrorClearTech;
	}

	ID3D10EffectTechnique* CShader::GetMirrorSurfaceTechnique()
	{
		return mpFXMirrorSurfaceTech;
	}
}

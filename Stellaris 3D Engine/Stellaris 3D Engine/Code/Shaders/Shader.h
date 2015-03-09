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
		// Effects
		ID3D10Effect* GetEffect();

		// Effect techniques
		ID3D10EffectTechnique* GetLitTexTechnique();
		ID3D10EffectTechnique* GetLightDrawTechnique();
		ID3D10EffectTechnique* GetWiggleTechnique();
		ID3D10EffectTechnique* GetNormalMappingTechnique();
		ID3D10EffectTechnique* GetParallaxMappingTechnique();
		ID3D10EffectTechnique* GetCellShadingTechnique();

		// Matrices
		ID3D10EffectMatrixVariable* GetFXWorldVar();
		ID3D10EffectMatrixVariable* GetFXViewVar();
		ID3D10EffectMatrixVariable* GetFXProjVar();

		// Textures
		ID3D10EffectShaderResourceVariable* GetFXDiffuseMapVar();
		ID3D10EffectShaderResourceVariable* GetFXNormalMapVar();

		// Vectors
		ID3D10EffectVectorVariable* GetFXLightPositionsVar();
		ID3D10EffectVectorVariable* GetFXLightColoursVar();
		ID3D10EffectVectorVariable* GetFXModelColourVar();
		ID3D10EffectVectorVariable* GetFXAmbientColourVar();
		ID3D10EffectVectorVariable* GetFXCameraPositionVar();

		// Scalars
		ID3D10EffectScalarVariable* GetFXSpecularPowerVar();
		ID3D10EffectScalarVariable* GetFXWiggleVar();
		ID3D10EffectScalarVariable* GetFXParallaxDepthVar();
		ID3D10EffectScalarVariable* GetFXOutlineThicknessVar();


	protected:
		// EFFECT VARIABLES
		//---------------------------------
		ID3D10Effect* mpFX;

		ID3D10EffectTechnique* mpFXLitTexTech;					// Technique for lighting a given texture
		ID3D10EffectTechnique* mpFXLightDrawTech;				// Technique for rendering light models
		ID3D10EffectTechnique* mpFXWiggleTech;					// Technique for the wiggle effect required by the assignment
		ID3D10EffectTechnique* mpFXNormalMappingTech;			// Technique for rendering normal maps with lighting
		ID3D10EffectTechnique* mpFXParallaxMappingTech;			// Technique for rendering normal maps with lighting & parallax effect
		ID3D10EffectTechnique* mpFXCellShadingTech;				// Technique for rendering a cartoonish effect on models

		ID3D10EffectMatrixVariable* mpFXVarWorldMat;
		ID3D10EffectMatrixVariable* mpFXVarViewMat;
		ID3D10EffectMatrixVariable* mpFXVarProjMat;

		ID3D10EffectVectorVariable* mpFXVarLightPositions;
		ID3D10EffectVectorVariable* mpFXVarLightColours;
		ID3D10EffectVectorVariable* mpFXVarModelColour;
		ID3D10EffectVectorVariable* mpFXVarAmbientColour;
		ID3D10EffectVectorVariable* mpFXVarCameraPosition;

		ID3D10EffectScalarVariable* mpFXVarSpecularPower;
		ID3D10EffectScalarVariable* mpFXVarWiggle;
		ID3D10EffectScalarVariable* mpFXVarParallaxDepth;
		ID3D10EffectScalarVariable* mpFXVarOutlineThickness;

		ID3D10EffectShaderResourceVariable* mpFXVarDiffuseMap;
		ID3D10EffectShaderResourceVariable* mpFXVarNormalMap;
	};
}


#endif /* _SHADER_H_ */

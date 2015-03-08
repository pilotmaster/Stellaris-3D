//--------------------------------------------------------------------------------------
// File: GraphicsAssign1.fx
//
//	Shaders Graphics Assignment
//	Add further models using different shader techniques
//	See assignment specification for details
//--------------------------------------------------------------------------------------


//====================================================================================
// STRUCTURES
//------------------------------------------------------------------------------------
// Standard input geometry data, more complex techniques (e.g. normal mapping) may need more
struct VS_BASIC_INPUT
{
    float3 Pos    : POSITION;
    float3 Normal : NORMAL;
	float2 UV     : TEXCOORD0;
};

// Data output from vertex shader to pixel shader for simple techniques. Again different techniques have different requirements
struct VS_BASIC_OUTPUT
{
    float4 ProjPos : SV_POSITION;
    float2 UV      : TEXCOORD0;
};


//====================================================================================
// GLOBAL VARIABLES
//------------------------------------------------------------------------------------
// The matrices (4x4 matrix of floats) for transforming from 3D model to 2D projection (used in vertex shader)
float4x4 WorldMatrix;
float4x4 ViewMatrix;
float4x4 ProjMatrix;

// Information required for lighting
static const int NUM_LIGHTS = 2;
float3 lightPos[NUM_LIGHTS];
float3 lightCol[NUM_LIGHTS];
float3 AmbientColour;

float3 CameraPos;

float specularPower;


// Diffuse texture map (the main texture colour) - may contain specular map in alpha channel
Texture2D DiffuseMap;


//====================================================================================
// SAMPLER STATE DEFINITIONS
//------------------------------------------------------------------------------------
// Sampler to use with the above texture map. Specifies texture filtering and addressing mode to use when accessing texture pixels
SamplerState TrilinearWrap
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};


//====================================================================================
// VERTEX SHADERS
//------------------------------------------------------------------------------------
// Basic vertex shader to transform 3D model vertices to 2D and pass UVs to the pixel shader
VS_BASIC_OUTPUT BasicTransform(VS_BASIC_INPUT vIn)
{
	VS_BASIC_OUTPUT vOut;
	
	// Use world matrix passed from C++ to transform the input model vertex position into world space
	float4 modelPos = float4(vIn.Pos, 1.0f); // Promote to 1x4 so we can multiply by 4x4 matrix, put 1.0 in 4th element for a point (0.0 for a vector)
	float4 worldPos = mul(modelPos, WorldMatrix);
	float4 viewPos = mul(worldPos, ViewMatrix);
	vOut.ProjPos = mul(viewPos, ProjMatrix);
	
	// Pass texture coordinates (UVs) on to the pixel shader
	vOut.UV = vIn.UV;

	return vOut;
}


//====================================================================================
// PIXEL SHADERS
//------------------------------------------------------------------------------------
// A pixel shader that just outputs a single fixed colour
//
float4 OneColour(VS_BASIC_OUTPUT vOut) : SV_Target
{
	// Calculate colour of texel based on the sampling of the texture
	float4 diffuseColour = DiffuseMap.Sample(TrilinearWrap, vOut.UV);

	return diffuseColour;
}


//====================================================================================
// TECHNIQUES
//------------------------------------------------------------------------------------
// Techniques are used to render models in our scene. They select a combination of vertex, geometry and pixel shader from those provided above. Can also set states.

// Render models unlit in a single colour
technique10 PlainColour
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_4_0, BasicTransform()));
        SetGeometryShader(NULL);                                   
        SetPixelShader(CompileShader(ps_4_0, OneColour()));
	}
}

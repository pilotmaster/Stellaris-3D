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

struct VS_NORMAL_MAP_INPUT
{
	float3 Pos     : POSITION;
	float3 Normal  : NORMAL;
	float3 Tangent : TANGENT;
	float2 UV      : TEXCOORD0;
};

struct VS_POS_ONLY_OUTPUT
{
	float4 ProjPos : SV_POSITION;
};

// Data output from vertex shader to pixel shader for simple techniques. Again different techniques have different requirements
struct VS_BASIC_OUTPUT
{
    float4 ProjPos     : SV_POSITION;
    float2 UV          : TEXCOORD0;
};

// Output for if there is going to be lighting on the item (without tangent)
struct VS_LIGHTING_OUTPUT
{
	float4 ProjPos     : SV_POSITION;
	float3 WorldPos    : POSITION;
	float3 WorldNormal : NORMAL;
	float2 UV          : TEXCOORD0;
};

// Output required for normal mapping & possibly parallax mapping
struct VS_NORMAL_MAP_OUTPUT
{
	float4 ProjPos      : SV_POSITION;
	float3 WorldPos     : POSITION;
	float3 ModelNormal  : NORMAL;
	float3 ModelTangent : TANGENT;
	float2 UV           : TEXCOORD0;
};


//====================================================================================
// GLOBAL VARIABLES
//------------------------------------------------------------------------------------
// The matrices (4x4 matrix of floats) for transforming from 3D model to 2D projection (used in vertex shader)
float4x4 WorldMatrix;
float4x4 ViewMatrix;
float4x4 ProjMatrix;

// For additional effects
float3 ModelColour;
float Wiggle;
float ParallaxDepth;
float OutlineThickness;

// Information required for lighting
static const int NUM_LIGHTS = 2;
float3 LightPos[NUM_LIGHTS];
float3 LightCol[NUM_LIGHTS];
float3 AmbientColour;

float3 CameraPos;

float SpecularPower;


// Diffuse texture map (the main texture colour) - may contain specular map in alpha channel
Texture2D DiffuseMap;
Texture2D NormalMap;


//====================================================================================
// SAMPLER STATE DEFINITIONS
//------------------------------------------------------------------------------------
SamplerState PointSampleClamp
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Clamp;
	AddressV = Clamp;
};

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
VS_BASIC_OUTPUT VSBasicTransform(VS_BASIC_INPUT vIn)
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

VS_POS_ONLY_OUTPUT VSExpandOutline(VS_BASIC_INPUT vIn)
{
	VS_POS_ONLY_OUTPUT vOut;

	// Transform model-space vertex position to world-space
	float4 modelPos = float4(vIn.Pos, 1.0f); // Promote to 1x4 so we can multiply by 4x4 matrix, put 1.0 in 4th element for a point (0.0 for a vector)
	float4 worldPos = mul(modelPos, WorldMatrix);

	// Next the usual transform from world space to camera space - but we don't go any further here - this will be used to help expand the outline
	// The result "viewPos" is the xyz position of the vertex as seen from the camera. The z component is the distance from the camera - that's useful...
	float4 viewPos = mul(worldPos, ViewMatrix);

	// Transform model normal to world space, using the normal to expand the geometry, not for lighting
	float4 modelNormal = float4(vIn.Normal, 0.0f); // Set 4th element to 0.0 this time as normals are vectors
	float4 worldNormal = normalize(mul(modelNormal, WorldMatrix)); // Normalise in case of world matrix scaling

	// Now we return to the world position of this vertex and expand it along the world normal - that will expand the geometry outwards.
	// Use the distance from the camera to decide how much to expand. Use this distance together with a sqrt to creates an outline that
	// gets thinner in the distance, but always remains clear. Overall thickness is also controlled by the constant "OutlineThickness"
	worldPos += OutlineThickness * sqrt(viewPos.z) * worldNormal;

	// Transform new expanded world-space vertex position to viewport-space and output
	viewPos = mul(worldPos, ViewMatrix);
	vOut.ProjPos = mul(viewPos, ProjMatrix);

	return vOut;
}

// Vertex shader for fulfilling the 'wiggle' requirement of the assignment
VS_BASIC_OUTPUT VSWiggleTransform(VS_BASIC_INPUT vIn)
{
	VS_BASIC_OUTPUT vOut;

	// Use world matrix passed from C++ to transform the input model vertex position into world space
	float4 modelPos = float4(vIn.Pos, 1.0f); // Promote to 1x4 so we can multiply by 4x4 matrix, put 1.0 in 4th element for a point (0.0 for a vector)
	float4 worldPos = mul(modelPos, WorldMatrix);

	// Normalise normal & determine worldNormal
	float4 normal = float4(vIn.Normal, 0.0f);
	float4 worldNormal = mul(normal, WorldMatrix);
	worldNormal = normalize(worldNormal);

	// Calculate new world positions based on the wiggle value
	worldPos.x += sin(modelPos.y + Wiggle) * 0.1f;
	worldPos.y += sin(modelPos.z + Wiggle) * 0.1f;
	worldPos.z += sin(modelPos.x + Wiggle) * 0.1f;
	worldPos += worldNormal * (sin(Wiggle) + 1.0f) * 0.1f;

	// Calculate remaining values
	float4 viewPos = mul(worldPos, ViewMatrix);
	vOut.ProjPos = mul(viewPos, ProjMatrix);

	// Pass texture coordinates (UVs) on to the pixel shader
	vOut.UV = vIn.UV;

	return vOut;
}

VS_NORMAL_MAP_OUTPUT VSNormalMapTransform(VS_NORMAL_MAP_INPUT vIn)
{
	VS_NORMAL_MAP_OUTPUT vOut;

	// Use world matrix passed from C++ to transform the input model vertex position into world space
	float4 modelPos = float4(vIn.Pos, 1.0f); // Promote to 1x4 so we can multiply by 4x4 matrix, put 1.0 in 4th element for a point (0.0 for a vector)
	float4 worldPos = mul(modelPos, WorldMatrix);
	vOut.WorldPos = worldPos.xyz;

	// Use camera matrices to further transform the vertex from world space into view space (camera's point of view) and finally into 2D "projection" space for rendering
	float4 viewPos = mul(worldPos, ViewMatrix);
	vOut.ProjPos = mul(viewPos, ProjMatrix);

	// Just send the model's normal and tangent untransformed (in model space). The pixel shader will do the matrix work on normals
	vOut.ModelNormal = vIn.Normal;
	vOut.ModelTangent = vIn.Tangent;

	// Pass texture coordinates (UVs) on to the pixel shader, the vertex shader doesn't need them
	vOut.UV = vIn.UV;

	return vOut;
}

VS_LIGHTING_OUTPUT VSLightingTransform(VS_BASIC_INPUT vIn)
{
	VS_LIGHTING_OUTPUT vOut;

	// Use world matrix passed from C++ to transform the input model vertex position into world space
	float4 modelPos = float4(vIn.Pos, 1.0f); // Promote to 1x4 so we can multiply by 4x4 matrix, put 1.0 in 4th element for a point (0.0 for a vector)
	float4 worldPos = mul(modelPos, WorldMatrix);
	float4 viewPos = mul(worldPos, ViewMatrix);
	vOut.ProjPos = mul(viewPos, ProjMatrix);

	vOut.WorldPos = worldPos.xyz;

	// Transform the vertex normal from model space into world space (almost same as first lines of code above)
	float4 modelNormal = float4(vIn.Normal, 0.0f); // Set 4th element to 0.0 this time as normals are vectors
	vOut.WorldNormal = mul(modelNormal, WorldMatrix).xyz;

	// Pass texture coordinates (UVs) on to the pixel shader
	vOut.UV = vIn.UV;

	return vOut;
}


//====================================================================================
// PIXEL SHADERS
//------------------------------------------------------------------------------------
// A pixel shader that always outputs a fixed colour - used for the cell-shading outlines
float4 PSOneColour(VS_POS_ONLY_OUTPUT vOut) : SV_Target
{
	return float4(ModelColour, 1.0f); // Use alpha = 1
}

// A pixel shader that just outputs a single fixed colour
float4 PSBasicTexture(VS_BASIC_OUTPUT vOut) : SV_Target
{
	// Calculate colour of texel based on the sampling of the texture
	float4 diffuseColour = DiffuseMap.Sample(TrilinearWrap, vOut.UV);

	return diffuseColour;
}

// Tints a texture toward a given model colour
float4 PSTintedTexture(VS_BASIC_OUTPUT vOut) : SV_Target
{
	// Calculate colour of texel based on the sampling of the texture
	float4 diffuseColour = DiffuseMap.Sample(TrilinearWrap, vOut.UV);

	diffuseColour.rgb *= ModelColour;

	return diffuseColour;
}

// Tints a texture toward a given model colour
float4 PSTintedTextureWithWiggle(VS_BASIC_OUTPUT vOut) : SV_Target
{
	// Scroll UV co-ordinates based on the value of Wiggle
	vOut.UV.r += Wiggle / 8.0f;
	vOut.UV.g += Wiggle / 8.0f;
	
	// Calculate colour of texel based on the sampling of the texture
	float4 diffuseColour = DiffuseMap.Sample(TrilinearWrap, vOut.UV);

	diffuseColour.rgb += ModelColour;

	return diffuseColour;
}

// A pixel shader that just calculates lighting from multiple lights and adds that lighting to
// a provided texture
float4 PSLitTexture(VS_LIGHTING_OUTPUT vOut) : SV_Target
{
	// Can't guarantee the normals are length 1 now (because the world matrix may contain scaling), so renormalise
	// If lighting in the pixel shader, this is also because the interpolation from vertex shader to pixel shader will also rescale normals
	vOut.WorldNormal = normalize(vOut.WorldNormal);


	// CALCULATE LIGHTING
	//---------------------------------
	// Calculate direction of camera
	float3 CameraDir = normalize(CameraPos - vOut.WorldPos.xyz); // Position of camera - position of current vertex (or pixel) (in world space)

	//// LIGHT 1
	float3 Light1Dir = normalize(LightPos[0] - vOut.WorldPos.xyz);   // Direction for each light is different
	float3 Light1Dist = length(LightPos[0] - vOut.WorldPos.xyz);
	float3 DiffuseLight1 = LightCol[0] * max(dot(vOut.WorldNormal.xyz, Light1Dir), 0.0f) / Light1Dist;
	float3 halfway = normalize(Light1Dir + CameraDir);
	float3 SpecularLight1 = LightCol[0] * pow(max(dot(vOut.WorldNormal.xyz, halfway), 0.0f), SpecularPower);

	//// LIGHT 2
	float3 Light2Dir = normalize(LightPos[1] - vOut.WorldPos.xyz);
	float3 Light2Dist = length(LightPos[1] - vOut.WorldPos.xyz);
	float3 DiffuseLight2 = LightCol[1] * max(dot(vOut.WorldNormal.xyz, Light2Dir), 0.0f) / Light2Dist;
	halfway = normalize(Light2Dir + CameraDir);
	float3 SpecularLight2 = LightCol[1] * pow(max(dot(vOut.WorldNormal.xyz, halfway), 0.0f), SpecularPower);

	// Sum the effect of the two lights - add the ambient at this stage rather than for each light (or we will get twice the ambient level)
	float3 DiffuseLight = AmbientColour + DiffuseLight1 + DiffuseLight2;
	float3 SpecularLight = SpecularLight1 + SpecularLight2;


	// SAMPLE THE PROVIDED TEXTURE
	//---------------------------------
	// Extract diffuse material colour for this pixel from a texture (using float3, so we get RGB - i.e. ignore any alpha in the texture)
	float4 DiffuseMaterial = DiffuseMap.Sample(TrilinearWrap, vOut.UV);

	// There is no material for a lit texture, so set a custom amount quite low
	float SpecularMaterial = DiffuseMaterial.a / 10.0f;


	// COMBINE THE COLOURS
	//---------------------------------
	// Combine maps and lighting for final pixel colour
	float4 combinedColour;
	combinedColour.rgb = DiffuseMaterial * DiffuseLight + SpecularMaterial * SpecularLight;
	combinedColour.a = 1.0f; // No alpha processing in this shader, so just set it to 1

	return combinedColour;
}

// Performs normal mapping calculations as well as lighting
float4 PSLitNormalMap(VS_NORMAL_MAP_OUTPUT vOut) : SV_Target
{
	// CALCULATE WORLD NORMAL FROM TANGENTS
	//---------------------------------
	// Normalize the model's normals and tangents
	float3 modelNormal = normalize(vOut.ModelNormal);
	float3 modelTangent = normalize(vOut.ModelTangent);

	// Calculate bi-tangent to complete the three axes of tangent space - then create the *inverse* tangent matrix to convert *from*
	// tangent space into model space. This is just a matrix built from the three axes (very advanced note - by default shader matrices
	// are stored as columns rather than in rows as in the C++. This means that this matrix is created "transposed" from what we would
	// expect. However, for a 3x3 rotation matrix the transpose is equal to the inverse, which is just what we require)
	float3 modelBiTangent = cross(modelNormal, modelTangent);
	float3x3 invTangentMatrix = float3x3(modelTangent, modelBiTangent, modelNormal);

	// Get the texture normal from the normal map. The r,g,b pixel values actually store x,y,z components of a normal. However, r,g,b
	// values are stored in the range 0->1, whereas the x, y & z components should be in the range -1->1. So some scaling is needed
	float3 textureNormal = 2.0f * NormalMap.Sample(TrilinearWrap, vOut.UV) - 1.0f;

	// Now convert the texture normal into model space using the inverse tangent matrix, and then convert into world space using the world
	// matrix. Normalise, because of the effects of texture filtering and in case the world matrix contains scaling
	float3 WorldNormal = normalize(mul(mul(textureNormal, invTangentMatrix), WorldMatrix));


	// CALCULATE LIGHTING
	//---------------------------------
	// Calculate direction of camera
	float3 CameraDir = normalize(CameraPos - vOut.WorldPos.xyz); // Position of camera - position of current vertex (or pixel) (in world space)

	// Light1
	float3 Light1Dir = normalize(LightPos[0] - vOut.WorldPos.xyz);   // Direction for each light is different
	float3 Light1Dist = length(LightPos[0] - vOut.WorldPos.xyz);
	float3 DiffuseLight1 = LightCol[0] * max(dot(WorldNormal.xyz, Light1Dir), 0.0f) / Light1Dist;
	float3 halfway = normalize(Light1Dir + CameraDir);
	float3 SpecularLight1 = LightCol[0] * pow(max(dot(WorldNormal.xyz, halfway), 0.0f), SpecularPower);
	
	// Light2
	float3 Light2Dir = normalize(LightPos[1] - vOut.WorldPos.xyz);
	float3 Light2Dist = length(LightPos[1] - vOut.WorldPos.xyz);
	float3 DiffuseLight2 = LightCol[1] * max(dot(WorldNormal.xyz, Light2Dir), 0.0f) / Light2Dist;
	halfway = normalize(Light2Dir + CameraDir);
	float3 SpecularLight2 = LightCol[1] * pow(max(dot(WorldNormal.xyz, halfway), 0.0f), SpecularPower);

	// Sum the effect of the two lights - add the ambient at this stage rather than for each light (or we will get twice the ambient level)
	float3 DiffuseLight = AmbientColour + DiffuseLight1 + DiffuseLight2;
	float3 SpecularLight = SpecularLight1 + SpecularLight2;


	// SAMPLE THE PROVIDED TEXTURE
	//---------------------------------
	// Extract diffuse material colour for this pixel from a texture (using float3, so we get RGB - i.e. ignore any alpha in the texture)
	float4 DiffuseMaterial = DiffuseMap.Sample(TrilinearWrap, vOut.UV);
	
	// There is no material for a lit texture, so set a custom amount quite low
	float SpecularMaterial = DiffuseMaterial.a / 10.0f;


	// COMBINE THE COLOURS
	//---------------------------------
	// Combine maps and lighting for final pixel colour
	float4 combinedColour;
	combinedColour.rgb = DiffuseMaterial * DiffuseLight + SpecularMaterial * SpecularLight;
	combinedColour.a = 1.0f; // No alpha processing in this shader, so just set it to 1

	return combinedColour;
}

// Performs parallax mapping calculations as well as lighting
float4 PSLitParallaxMap(VS_NORMAL_MAP_OUTPUT vOut) : SV_Target
{
	// CALCULATE WORLD NORMAL FROM TANGENTS
	//---------------------------------
	// Normalize the model's normals and tangents
	float3 modelNormal = normalize(vOut.ModelNormal);
	float3 modelTangent = normalize(vOut.ModelTangent);

	// Calculate bi-tangent to complete the three axes of tangent space - then create the *inverse* tangent matrix to convert *from*
	// tangent space into model space. This is just a matrix built from the three axes (very advanced note - by default shader matrices
	// are stored as columns rather than in rows as in the C++. This means that this matrix is created "transposed" from what we would
	// expect. However, for a 3x3 rotation matrix the transpose is equal to the inverse, which is just what we require)
	float3 modelBiTangent = cross(modelNormal, modelTangent);
	float3x3 invTangentMatrix = float3x3(modelTangent, modelBiTangent, modelNormal);


	// CALCULATE PARALLAX MAPPING
	//---------------------------------
	// Get normalised vector to camera for parallax mapping and specular equation (this vector was calculated later in previous shaders)
	float3 CameraDir = normalize(CameraPos - vOut.WorldPos.xyz);

	// Transform camera vector from world into model space. Need *inverse* world matrix for this.
	// Only need 3x3 matrix to transform vectors, to invert a 3x3 matrix we transpose it (flip it about its diagonal)
	float3x3 invWorldMatrix = transpose(WorldMatrix);
	float3 cameraModelDir = normalize(mul(CameraDir, invWorldMatrix)); // Normalise in case world matrix is scaled

	// Then transform model-space camera vector into tangent space (texture coordinate space) to give the direction to offset texture
	// coordinate, only interested in x and y components. Calculated inverse tangent matrix above, so invert it back for this step
	float3x3 tangentMatrix = transpose(invTangentMatrix);
	float2 textureOffsetDir = mul(cameraModelDir, tangentMatrix);

	// Get the depth info from the normal map's alpha channel at the given texture coordinate
	// Rescale from 0->1 range to -x->+x range, x determined by ParallaxDepth setting
	float texDepth = ParallaxDepth * (NormalMap.Sample(TrilinearWrap, vOut.UV).a - 0.5f);

	// Use the depth of the texture to offset the given texture coordinate - this corrected texture coordinate will be used from here on
	float2 offsetTexCoord = vOut.UV + texDepth * textureOffsetDir;


	// Get the texture normal from the normal map. The r,g,b pixel values actually store x,y,z components of a normal. However, r,g,b
	// values are stored in the range 0->1, whereas the x, y & z components should be in the range -1->1. So some scaling is needed
	float3 textureNormal = 2.0f * NormalMap.Sample(TrilinearWrap, offsetTexCoord) - 1.0f;

	// Now convert the texture normal into model space using the inverse tangent matrix, and then convert into world space using the world
	// matrix. Normalise, because of the effects of texture filtering and in case the world matrix contains scaling
	float3 WorldNormal = normalize(mul(mul(textureNormal, invTangentMatrix), WorldMatrix));


	// CALCULATE LIGHTING
	//---------------------------------
	// Light1
	float3 Light1Dir = normalize(LightPos[0] - vOut.WorldPos.xyz);
	float3 Light1Dist = length(LightPos[0] - vOut.WorldPos.xyz);
	float3 DiffuseLight1 = LightCol[0] * max(dot(WorldNormal.xyz, Light1Dir), 0.0f) / Light1Dist;
	float3 halfway = normalize(Light1Dir + CameraDir);
	float3 SpecularLight1 = LightCol[0] * pow(max(dot(WorldNormal.xyz, halfway), 0.0f), SpecularPower);

	// Light2
	float3 Light2Dir = normalize(LightPos[1] - vOut.WorldPos.xyz);
	float3 Light2Dist = length(LightPos[1] - vOut.WorldPos.xyz);
	float3 DiffuseLight2 = LightCol[1] * max(dot(WorldNormal.xyz, Light2Dir), 0.0f) / Light2Dist;
	halfway = normalize(Light2Dir + CameraDir);
	float3 SpecularLight2 = LightCol[1] * pow(max(dot(WorldNormal.xyz, halfway), 0.0f), SpecularPower);

	// Sum the effect of the two lights - add the ambient at this stage rather than for each light (or we will get twice the ambient level)
	float3 DiffuseLight = AmbientColour + DiffuseLight1 + DiffuseLight2;
	float3 SpecularLight = SpecularLight1 + SpecularLight2;


	// SAMPLE THE PROVIDED TEXTURE
	//---------------------------------
	// Extract diffuse material colour for this pixel from a texture (using float3, so we get RGB - i.e. ignore any alpha in the texture)
	float4 DiffuseMaterial = DiffuseMap.Sample(TrilinearWrap, vOut.UV);

	// There is no material for a lit texture, so set a custom amount quite low
	float SpecularMaterial = DiffuseMaterial.a / 10.0f;


	// COMBINE THE COLOURS
	//---------------------------------
	// Combine maps and lighting for final pixel colour
	float4 combinedColour;
	combinedColour.rgb = DiffuseMaterial * DiffuseLight + SpecularMaterial * SpecularLight;
	combinedColour.a = 1.0f; // No alpha processing in this shader, so just set it to 1

	return combinedColour;
}

float4 PSLitCartoonify(VS_LIGHTING_OUTPUT vOut) : SV_Target
{
	// CALCULATE LIGHTING
	//---------------------------------
	// Calculate the world normal
	float3 worldNormal = normalize(vOut.WorldNormal);
	// Calculate direction of camera
	float3 CameraDir = normalize(CameraPos - vOut.WorldPos.xyz); // Position of camera - position of current vertex (or pixel) (in world space)

	//// LIGHT 1
	float3 Light1Dir = normalize(LightPos[0] - vOut.WorldPos.xyz);   // Direction for each light is different
	float3 Light1Dist = length(LightPos[0] - vOut.WorldPos.xyz);
	float DiffuseLevel1 = max(dot(worldNormal.xyz, Light1Dir), 0);
	float CellDiffuseLevel1 = NormalMap.Sample(PointSampleClamp, DiffuseLevel1).r;
	float3 DiffuseLight1 = LightCol[0] * CellDiffuseLevel1 / Light1Dist;

	// Do same for specular light and further lights
	float3 halfway = normalize(Light1Dir + CameraDir);
	float SpecularLevel1 = pow(max(dot(worldNormal.xyz, halfway), 0), SpecularPower);
	float CellSpecularLevel1 = NormalMap.Sample(PointSampleClamp, SpecularLevel1).r;
	float3 SpecularLight1 = DiffuseLight1 * CellSpecularLevel1;


	//// LIGHT 2
	float3 Light2Dir = normalize(LightPos[1] - vOut.WorldPos.xyz);
	float3 Light2Dist = length(LightPos[1] - vOut.WorldPos.xyz);
	float DiffuseLevel2 = max(dot(worldNormal.xyz, Light2Dir), 0);
	float CellDiffuseLevel2 = NormalMap.Sample(PointSampleClamp, DiffuseLevel2).r;
	float3 DiffuseLight2 = LightCol[1] * CellDiffuseLevel2 / Light2Dist;

	halfway = normalize(Light2Dir + CameraDir);
	float SpecularLevel2 = pow(max(dot(worldNormal.xyz, halfway), 0), SpecularPower);
	float CellSpecularLevel2 = NormalMap.Sample(PointSampleClamp, SpecularLevel2).r;
	float3 SpecularLight2 = DiffuseLight2 * CellSpecularLevel2;


	// Sum the effect of the two lights - add the ambient at this stage rather than for each light (or we will get twice the ambient level)
	float3 DiffuseLight = AmbientColour + DiffuseLight1 + DiffuseLight2;
	float3 SpecularLight = SpecularLight1 + SpecularLight2;


	// SAMPLE THE PROVIDED TEXTURE
	//---------------------------------
	// Extract diffuse material colour for this pixel from a texture (using float3, so we get RGB - i.e. ignore any alpha in the texture)
	float4 DiffuseMaterial = DiffuseMap.Sample(TrilinearWrap, vOut.UV);

	// Assume specular material colour is white (i.e. highlights are a full, untinted reflection of light)
	float3 SpecularMaterial = DiffuseMaterial.a / 10.0f;


	// COMBINE THE COLOURS
	//---------------------------------
	// Combine maps and lighting for final pixel colour
	float4 combinedColour;
	combinedColour.rgb = DiffuseMaterial * DiffuseLight + SpecularMaterial * SpecularLight;
	combinedColour.a = 1.0f; // No alpha processing in this shader, so just set it to 1

	return combinedColour;
}

//====================================================================================
// RASTERIZER STATES 
//------------------------------------------------------------------------------------
RasterizerState CullNone
{
	FillMode = Solid;
	CullMode = None;
	MultisampleEnable = true;
	AntialiasedLineEnable = true;
};

RasterizerState CullBack
{
	FillMode = Solid;
	CullMode = Back;
	MultisampleEnable = true;
	AntialiasedLineEnable = true;
};

RasterizerState CullFront
{
	FillMode = Solid;
	CullMode = Front;
	MultisampleEnable = true;
	AntialiasedLineEnable = true;
};


//====================================================================================
// DEPTH STENCIL STATES 
//------------------------------------------------------------------------------------
DepthStencilState DepthWritesOff // Don't write to the depth buffer - polygons rendered will not obscure other polygons
{
	DepthEnable = true;
	StencilEnable = true;
	DepthWriteMask = ZERO;
};

DepthStencilState DepthWritesOn  // Write to the depth buffer - normal behaviour 
{
	DepthEnable = true;
	StencilEnable = true;
	DepthWriteMask = ALL;
};


//====================================================================================
// BLENDING STATES 
//------------------------------------------------------------------------------------
BlendState NoBlending // Switch off blending - pixels will be opaque
{
	BlendEnable[0] = FALSE;
};

BlendState AdditiveBlending // Additive blending is used for lighting effects
{
	BlendEnable[0] = TRUE;
	SrcBlend = ONE;
	DestBlend = ONE;
	BlendOp = ADD;
};

BlendState AlphaBlending // Additive blending is used for lighting effects
{
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
	DestBlend = SRC_ALPHA;
	BlendOp = ADD;
};


//====================================================================================
// TECHNIQUES
//------------------------------------------------------------------------------------
// Techniques are used to render models in our scene. They select a combination of vertex, geometry and pixel shader from those provided above. Can also set states.

// Render models unlit with a provided simple texture
technique10 PlainTextureTech
{
    pass P0
    {
		SetVertexShader(CompileShader(vs_4_0, VSBasicTransform()));
        SetGeometryShader(NULL);                                   
		SetPixelShader(CompileShader(ps_4_0, PSBasicTexture()));

		SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetRasterizerState(CullBack);
		SetDepthStencilState(DepthWritesOn, 0);
	}
}

// Render models with lighting and simple texture
technique10 LitTextureTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_4_0, VSLightingTransform()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PSLitTexture()));

		SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetRasterizerState(CullBack);
		SetDepthStencilState(DepthWritesOn, 0);
	}
}

// Custom technique for doing the wiggle effect & scrolling UVs
technique10 WiggleTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_4_0, VSWiggleTransform()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PSTintedTextureWithWiggle()));
		
		SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetRasterizerState(CullBack);
		SetDepthStencilState(DepthWritesOn, 0);
	}
}

// Technique for normal mapping
technique10 NormalMappingTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_4_0, VSNormalMapTransform()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PSLitNormalMap()));

		SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetRasterizerState(CullBack);
		SetDepthStencilState(DepthWritesOn, 0);
	}
}

// Technique for parallax mapping
technique10 ParallaxMappingTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_4_0, VSNormalMapTransform()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PSLitParallaxMap()));

		SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetRasterizerState(CullBack);
		SetDepthStencilState(DepthWritesOn, 0);
	}
}

// Draws the lights and does addative blending with the background with tints of the lights colour
technique10 LightDrawTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_4_0, VSBasicTransform()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PSTintedTexture()));
		
		SetBlendState(AdditiveBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetRasterizerState(CullNone);
		SetDepthStencilState(DepthWritesOff, 0);
	}
}

technique10 CellShadingTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_4_0, VSExpandOutline()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PSOneColour()));

		// Cull (remove) the polygons facing us - i.e. draw the inside of the model
		SetRasterizerState(CullFront);

		// Switch off other blending states
		SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetDepthStencilState(DepthWritesOn, 0);
	}
	pass P1
	{
		SetVertexShader(CompileShader(vs_4_0, VSLightingTransform()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PSLitCartoonify()));

		// Return to standard culling (remove polygons facing away from us)
		SetRasterizerState(CullBack);
	}
}

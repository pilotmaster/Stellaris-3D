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
    float4 ProjPos  : SV_POSITION;
    float2 UV       : TEXCOORD0;
	float  ClipDist : SV_ClipDistance;
};

// Output for if there is going to be lighting on the item (without tangent)
struct VS_LIGHTING_OUTPUT
{
	float4 ProjPos     : SV_POSITION;
	float3 WorldPos    : POSITION;
	float3 WorldNormal : NORMAL;
	float2 UV          : TEXCOORD0;
	float  ClipDist    : SV_ClipDistance;
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

struct PS_DEPTH_OUTPUT
{
	float4 Colour : SV_Target;
	float  Depth : SV_Depth;
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
static const int NUM_LIGHTS = 5;
int LightType[NUM_LIGHTS];
float3 LightPos[NUM_LIGHTS];
float3 LightCol[NUM_LIGHTS];
float3 LightFacing[NUM_LIGHTS];
float LightCosHalfAngle[NUM_LIGHTS];
float4x4 LightViewMatrix[NUM_LIGHTS];
float4x4 LightProjMatrix[NUM_LIGHTS];
float3 AmbientColour;
float SpecularPower;

float3 CameraPos;

// We view a reflection using a camera that is reflected in the mirror. However, the near clip plane for a reflected camera
// is different - it needs to be the plane of the mirror itself. Otherwise, we would see objects *behind* the mirror in the
// reflection. So we use a custom clip plane, a special output (new in DX10) and a single line of code at the end of each
// vertex shader that allows us to use any arbitrary plane in space as the near clip plane. This is an advanced subtle detail.
float4 ClipPlane;

// Diffuse texture map (the main texture colour) - may contain specular map in alpha channel
Texture2D DiffuseMap;
Texture2D NormalMap;
Texture2D ShadowMap;


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
// LIGHT CALCULATION FUNCTIONS
//------------------------------------------------------------------------------------
void DoPointLight(int lIndex, float3 camDir, float3 worldPos, float3 worldNorm, out float3 diffuse, out float3 specular)
{
	float3 LightDir = normalize(LightPos[lIndex] - worldPos);
	float3 LightDist = length(LightPos[lIndex] - worldPos);
	diffuse = LightCol[lIndex] * max(dot(worldNorm, LightDir), 0.0f) / LightDist;
	float3 halfway = normalize(LightDir + camDir);
	specular = LightCol[lIndex] * pow(max(dot(worldNorm, halfway), 0.0f), SpecularPower);
}

void DoSpotLight(int lIndex, float3 camDir, float3 worldPos, float3 worldNorm, out float3 diffuse, out float3 specular)
{
	// Calculate the direction of the light along with the light's view position and projection position
	// which are all required for the shadow effect
	float4 LightViewPos = mul(float4(worldPos, 1.0f), LightViewMatrix[lIndex]);
	float4 LightProjPos = mul(LightViewPos, LightProjMatrix[lIndex]);
	float3 LightDir = normalize(LightPos[lIndex] - worldPos);

	// Calculate the dot product between light-pixel vector and the light-facing vector
	float FDotL = dot(-LightDir, normalize(LightFacing[lIndex]));

	// Check if within cone of light
	if (FDotL > LightCosHalfAngle[lIndex])
	{
		// Convert 2D pixel position as viewed from light into texture coordinates for shadow map - an advanced topic related to the projection step
		// Detail: 2D position x & y get perspective divide, then converted from range -1->1 to UV range 0->1. Also flip V axis
		float2 shadowUV = 0.5f * LightProjPos.xy / LightProjPos.w + float2(0.5f, 0.5f);
		shadowUV.y = 1.0f - shadowUV.y;

		// Get depth of this pixel if it were visible from the light (another advanced projection step)
		float depthFromLight = LightProjPos.z / LightProjPos.w - 0.00001f;

		// Compare pixel depth from light with depth held in shadow map of the light. If shadow map depth is less than something is nearer
		// to the light than this pixel - so the pixel gets no effect from this light
		if (depthFromLight < ShadowMap.Sample(PointSampleClamp, shadowUV).r)
		{
			float3 lightDist = length(LightPos[lIndex] - worldPos);
			diffuse = LightCol[lIndex] * max(dot(worldNorm, LightDir), 0) / lightDist;
			float3 halfway = normalize(LightDir + camDir);
			specular = diffuse * pow(max(dot(worldNorm, halfway), 0), SpecularPower);
		}
	}
}


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

	// Custom clip plane for mirrors
	vOut.ClipDist = dot(worldPos, ClipPlane);

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

	// Custom clip plane for mirrors - see comment near top
	vOut.ClipDist = dot(worldPos, ClipPlane);

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

float4 PSTintDiffuseMap(VS_BASIC_OUTPUT vOut) : SV_Target
{
	// Extract diffuse material colour for this pixel from a texture
	float4 diffuseMapColour = DiffuseMap.Sample(TrilinearWrap, vOut.UV);

	// Tint by global colour (set from C++)
	diffuseMapColour.rgb *= ModelColour / 10;

	return diffuseMapColour;
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

	// LIGHTING VARIABLES
	float3 TotalDiffuse = float3(0.0f, 0.0f, 0.0f);
	float3 TotalSpecular = float3(0.0f, 0.0f, 0.0f);

	float3 LightDif;
	float3 LightSpec;

	// CALCULATE LIGHTING FOR ALL LIGHTS
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		// Set light variables to 0
		LightDif = float3(0.0f, 0.0f, 0.0f);
		LightSpec = float3(0.0f, 0.0f, 0.0f);
		
		// Check value of light
		if (LightType[i] == 1)
		{
			// Point light
			DoPointLight(i, CameraDir, vOut.WorldPos.xyz, vOut.WorldNormal.xyz, LightDif, LightSpec);
		}
		else if (LightType[i] == 2)
		{
			// Spot light
			DoSpotLight(i, CameraDir, vOut.WorldPos.xyz, vOut.WorldNormal.xyz, LightDif, LightSpec);
		}
		else if (LightType[i] == 3)
		{
			// Directional Light
		}

		// Accumulate new light
		TotalDiffuse += LightDif;
		TotalSpecular += LightSpec;
	}

	// Sum the effect of the two lights - add the ambient at this stage rather than for each light (or we will get twice the ambient level)
	float3 DiffuseLight = AmbientColour + TotalDiffuse;
	float3 SpecularLight = TotalSpecular;


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

	// LIGHTING VARIABLES
	float3 TotalDiffuse = float3(0.0f, 0.0f, 0.0f);
	float3 TotalSpecular = float3(0.0f, 0.0f, 0.0f);

	float3 LightDif;
	float3 LightSpec;

	// CALCULATE LIGHTING FOR ALL LIGHTS
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		// Set light variables to 0
		LightDif = float3(0.0f, 0.0f, 0.0f);
		LightSpec = float3(0.0f, 0.0f, 0.0f);

		// Check value of light
		if (LightType[i] == 1)
		{
			// Point light
			DoPointLight(i, CameraDir, vOut.WorldPos.xyz, WorldNormal.xyz, LightDif, LightSpec);
		}
		else if (LightType[i] == 2)
		{
			// Spot light
			DoSpotLight(i, CameraDir, vOut.WorldPos.xyz, WorldNormal.xyz, LightDif, LightSpec);
		}
		else if (LightType[i] == 3)
		{
			// Directional Light
		}

		// Accumulate new light
		TotalDiffuse += LightDif;
		TotalSpecular += LightSpec;
	}

	// Sum the effect of the two lights - add the ambient at this stage rather than for each light (or we will get twice the ambient level)
	float3 DiffuseLight = AmbientColour + TotalDiffuse;
	float3 SpecularLight = TotalSpecular;


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
	// LIGHTING VARIABLES
	float3 TotalDiffuse = float3(0.0f, 0.0f, 0.0f);
	float3 TotalSpecular = float3(0.0f, 0.0f, 0.0f);

	float3 LightDif;
	float3 LightSpec;

	// CALCULATE LIGHTING FOR ALL LIGHTS
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		// Set light variables to 0
		LightDif = float3(0.0f, 0.0f, 0.0f);
		LightSpec = float3(0.0f, 0.0f, 0.0f);

		// Check value of light
		if (LightType[i] == 1)
		{
			// Point light
			DoPointLight(i, CameraDir, vOut.WorldPos.xyz, WorldNormal.xyz, LightDif, LightSpec);
		}
		else if (LightType[i] == 2)
		{
			// Spot light
			DoSpotLight(i, CameraDir, vOut.WorldPos.xyz, WorldNormal.xyz, LightDif, LightSpec);
		}
		else if (LightType[i] == 3)
		{
			// Directional Light
		}

		// Accumulate new light
		TotalDiffuse += LightDif;
		TotalSpecular += LightSpec;
	}

	// Sum the effect of the two lights - add the ambient at this stage rather than for each light (or we will get twice the ambient level)
	float3 DiffuseLight = AmbientColour + TotalDiffuse;
	float3 SpecularLight = TotalSpecular;


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
	float3 WorldNormal = normalize(vOut.WorldNormal);
	// Calculate direction of camera
	float3 CameraDir = normalize(CameraPos - vOut.WorldPos.xyz); // Position of camera - position of current vertex (or pixel) (in world space)


	// LIGHTING VARIABLES
	float3 TotalDiffuse = float3(0.0f, 0.0f, 0.0f);
	float3 TotalSpecular = float3(0.0f, 0.0f, 0.0f);

	float3 LightDif;
	float3 LightSpec;
	float CellDiffuseLevel;
	float CellSpecularLevel;

	// CALCULATE LIGHTING FOR ALL LIGHTS
	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		// Set light variables to 0
		LightDif = float3(0.0f, 0.0f, 0.0f);
		LightSpec = float3(0.0f, 0.0f, 0.0f);

		// Check value of light
		if (LightType[i] == 1)
		{
			// Point light
			DoPointLight(i, CameraDir, vOut.WorldPos.xyz, WorldNormal.xyz, LightDif, LightSpec);
		}
		else if (LightType[i] == 2)
		{
			// Spot light
			DoSpotLight(i, CameraDir, vOut.WorldPos.xyz, WorldNormal.xyz, LightDif, LightSpec);
		}
		else if (LightType[i] == 3)
		{
			// Directional Light
		}

		// Accumulate new light & apply cell shading
		CellDiffuseLevel = NormalMap.Sample(PointSampleClamp, LightDif).r;
		TotalDiffuse += LightDif * CellDiffuseLevel;

		CellSpecularLevel = NormalMap.Sample(PointSampleClamp, LightSpec).r;
		LightSpec = LightDif * CellDiffuseLevel;
		TotalSpecular += LightSpec;
	}

	// Sum the effect of the two lights - add the ambient at this stage rather than for each light (or we will get twice the ambient level)
	float3 DiffuseLight = AmbientColour + TotalDiffuse;
	float3 SpecularLight = TotalSpecular;


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

PS_DEPTH_OUTPUT PSClearDepth(VS_BASIC_OUTPUT vOut)
{
	PS_DEPTH_OUTPUT pOut;

	pOut.Colour = float4(ModelColour, 1.0f);
	pOut.Depth = 1.0f;

	return pOut;
}

float4 PSPlainColour(VS_BASIC_OUTPUT vOut) : SV_Target
{
	return float4(ModelColour, 1.0f);
}

float4 PSPixelDepth(VS_BASIC_OUTPUT vOut) : SV_Target
{
	// Output the value that would go in the depth puffer to the pixel colour (greyscale)
	return vOut.ProjPos.z / vOut.ProjPos.w;
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

DepthStencilState DepthWritesOn
{
	DepthEnable = true;
	StencilEnable = true;
	DepthWriteMask = ALL;
};

DepthStencilState MirrorStencil
{
	// Use depth buffer normally
	DepthFunc = Less;
	DepthWriteMask = ALL;

	// Enable stencil buffer and replace all pixel stencil values with the reference value (value specified in the technique)
	StencilEnable = true;
	FrontFaceStencilFunc = Always;
	FrontFaceStencilPass = Replace;
	BackFaceStencilFunc = Always;
	BackFaceStencilPass = Replace;
};

DepthStencilState AffectStencilArea
{
	// Use depth buffer normally
	DepthFunc = Less;
	DepthWriteMask = ALL;

	// Only render those pixels whose stencil value is equal to the reference value (value specified in the technique)
	StencilEnable = true;
	FrontFaceStencilFunc = Equal; // Only render on matching stencil
	FrontFaceStencilPass = Keep;  // But don't change the stencil values
	BackFaceStencilFunc = Equal;
	BackFaceStencilPass = Keep;
};

// Only render to the area with a given stencil value but don't write to depth-buffer, used for rendering transparent polygons within the mirror (similar to state above)
DepthStencilState AffectStencilAreaDepthWritesOff
{
	// Test the depth buffer, but don't write anything new to it
	DepthFunc = Less;
	DepthWriteMask = ZERO;

	// Only render those pixels whose stencil value is equal to the reference value (value specified in the technique)
	StencilEnable = true;
	FrontFaceStencilFunc = Equal; // Only render on matching stencil
	FrontFaceStencilPass = Keep;  // But don't change the stencil values
	BackFaceStencilFunc = Equal;
	BackFaceStencilPass = Keep;
};

// Only render to the area with a given stencil value and completely ignore the depth buffer - used to clear the mirror before rendering the scene inside it
DepthStencilState AffectStencilAreaIgnoreDepth
{
	// Disable depth buffer - we're going to fill the mirror/portal with distant z-values as we will want to render a new scene in there. So we must
	// ignore the z-values of the mirror surface - or the depth buffer would think the mirror polygon was obscuring our distant z-values
	DepthFunc = Always;

	// Only render those pixels whose stencil value is equal to the reference value (value specified in the technique)
	StencilEnable = True;
	FrontFaceStencilFunc = Equal; // Only render on matching stencil
	FrontFaceStencilPass = Keep;  // But don't change the stencil values
	BackFaceStencilFunc = Equal;
	BackFaceStencilPass = Keep;
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

BlendState NoColourOutput // Use blending to prevent drawing pixel colours, but still allow depth/stencil updates
{
	BlendEnable[0] = TRUE;
	SrcBlend = ZERO;
	DestBlend = ONE;
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

technique10 DepthOnlyTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_4_0, VSBasicTransform()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PSPixelDepth()));

		// Switch off blending states
		SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetRasterizerState(CullBack);
		SetDepthStencilState(DepthWritesOn, 0);
	}
}


// MIRROR RENDER TECHNIQUES

technique10 VertexLitTexMirrorTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_4_0, VSLightingTransform()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PSLitTexture()));

		// No blending
		SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);

		// Reverse culling - the mirror will reverse the clockwise/anti-clockwise ordering of the polygons
		SetRasterizerState(CullFront);

		// Only render in stencil area (the mirror surface)
		SetDepthStencilState(AffectStencilArea, 1);
	}
}

// Additive blended texture. No lighting, but uses a global colour tint. Used for light models
technique10 AdditiveTexTintMirrorTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_4_0, VSBasicTransform()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PSTintDiffuseMap()));

		// Additive blending states plus only rendering to stencil area
		SetBlendState(AdditiveBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetRasterizerState(CullNone);
		SetDepthStencilState(AffectStencilAreaDepthWritesOff, 1);
	}
}


//**** Mirror Surface Techniques ****//

// Managing the mirror surface requires precise stencil and depth buffer work as we are rendering two scenes into the same viewport

// First stage for stenciled mirror/portal polygons - clear the pixel colours and the depth buffer within. Prior to rendering the scene inside the mirror
technique10 MirrorClearTech
{
	pass P0 // Set the stencil values to 1, do nothing to the pixels
	{
		SetVertexShader(CompileShader(vs_4_0, VSBasicTransform()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PSPlainColour()));

		// Switch off colour output (only updating stencil in this pass), normal culling
		SetBlendState(NoColourOutput, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetRasterizerState(CullBack);

		// Set the stencil to 1 in the visible area of this polygon
		SetDepthStencilState(MirrorStencil, 1);
	}
	pass P1 // Set the pixel colour to background colour, set z-value to furthest distance - but do this only where stencil was set to 1 above
	{
		SetVertexShader(CompileShader(vs_4_0, VSBasicTransform()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PSClearDepth()));

		// Switch off blending, normal culling
		SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetRasterizerState(CullBack);

		// Only affect the area where the stencil was set to 1 in the pass above
		SetDepthStencilState(AffectStencilAreaIgnoreDepth, 1);
	}
}

// Final stage for stenciled mirror/portal polygons - put a transparent layer over the mirror surface. Clear the stencil values to normal
// and set the correct depth-buffer values for the mirror surface, so further rendering won't go "inside" the mirrored scene
technique10 MirrorSurfaceTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_4_0, VSBasicTransform()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PSPlainColour()));

		// Use a special blending state to disable any changes to the viewport pixels as we're only updating the stencil/depth
		// buffer in this pass (for now).
		SetBlendState(NoColourOutput, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);

		// Standard culling
		SetRasterizerState(CullBack);

		// Set the stencil back to 0 in the surface of the polygon
		SetDepthStencilState(MirrorStencil, 0);
	}
}
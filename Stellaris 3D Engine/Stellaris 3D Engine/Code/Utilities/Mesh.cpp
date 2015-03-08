//====================================================================================
// FILE:    Mesh.cpp -----------------------------------------------------------------
//------------------------------------------------------------------------------------

#include "Mesh.h"
#include "..\..\Import\CImportXFile.h"
#include "..\DirectX\Engine.h"


namespace sge
{
	//====================================================================================
	// MESH CLASS CONSTRUCTOR & DESTRUCTOR
	//------------------------------------------------------------------------------------
	CMesh::CMesh()
	{
		// Initialise all private data
		mpVertexBuffer = nullptr;
		mNumVertices = 0U;
		mVertexSize = 0U;
		mpInputLayout = nullptr;

		mpIndexBuffer = nullptr;
		mNumIndices = 0U;

		mpMaterial = nullptr;
	}

	CMesh::~CMesh()
	{
		ReleaseResources();
	}


	//====================================================================================
	// MESH CLASS METHODS
	//------------------------------------------------------------------------------------
	void CMesh::ReleaseResources()
	{
		// Check if each COM resource exists, and remove as necessary
		if (mpIndexBuffer) mpIndexBuffer->Release();
		if (mpVertexBuffer) mpVertexBuffer->Release();
		if (mpInputLayout) mpInputLayout->Release();
	}

	bool CMesh::LoadMesh(ID3D10Device* pDevice, std::string& fileName, ID3D10EffectTechnique* pTech, ERenderTypes renderType, bool tangents)
	{
		// Release any existing geometry in this object
		ReleaseResources();

		// Store render type & asscoated technique
		mRenderType = renderType;
		mpRenderTech = pTech;

		// Use CImportXFile class (from another application) to load the given file. The import code is wrapped in the namespace 'gen'
		gen::CImportXFile mesh;
		if (mesh.ImportFile(fileName.c_str()) != gen::kSuccess)
		{
			return false;
		}

		// Get first sub-mesh from loaded file
		gen::SSubMesh subMesh;
		if (mesh.GetSubMesh(0, &subMesh, tangents) != gen::kSuccess)
		{
			return false;
		}


		// Create vertex element list & layout. We need a vertex layout to say what data we have per vertex in this model (e.g. position, normal, uv, etc.)
		// In previous projects the element list was a manually typed in array as we knew what data we would provide. However, as we can load models with
		// different vertex data this time we need flexible code. The array is built up one element at a time: ask the import class if it loaded normals, 
		// if so then add a normal line to the array, then ask if it loaded UVS...etc
		UINT numElts = 0U;
		UINT offset = 0U;

		// Position is always required
		mVertexElmnts[numElts].SemanticName = "POSITION";   // Semantic in HLSL (what is this data for)
		mVertexElmnts[numElts].SemanticIndex = 0;           // Index to add to semantic (a count for this kind of data, when using multiple of the same type, e.g. TEXCOORD0, TEXCOORD1)
		mVertexElmnts[numElts].Format = DXGI_FORMAT_R32G32B32_FLOAT; // Type of data - this one will be a float3 in the shader. Most data communicated as though it were colours
		mVertexElmnts[numElts].AlignedByteOffset = offset;  // Offset of element from start of vertex data (e.g. if we have position (float3), uv (float2) then normal, the normal's offset is 5 floats = 5*4 = 20)
		mVertexElmnts[numElts].InputSlot = 0;               // For when using multiple vertex buffers (e.g. instancing - an advanced topic)
		mVertexElmnts[numElts].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA; // Use this value for most cases (only changed for instancing)
		mVertexElmnts[numElts].InstanceDataStepRate = 0;                     // --"--
		offset += 12;
		++numElts;
		// Repeat for each kind of vertex data
		if (subMesh.hasNormals)
		{
			mVertexElmnts[numElts].SemanticName = "NORMAL";
			mVertexElmnts[numElts].SemanticIndex = 0;
			mVertexElmnts[numElts].Format = DXGI_FORMAT_R32G32B32_FLOAT;
			mVertexElmnts[numElts].AlignedByteOffset = offset;
			mVertexElmnts[numElts].InputSlot = 0;
			mVertexElmnts[numElts].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
			mVertexElmnts[numElts].InstanceDataStepRate = 0;
			offset += 12;
			++numElts;
		}
		if (subMesh.hasTangents)
		{
			mVertexElmnts[numElts].SemanticName = "TANGENT";
			mVertexElmnts[numElts].SemanticIndex = 0;
			mVertexElmnts[numElts].Format = DXGI_FORMAT_R32G32B32_FLOAT;
			mVertexElmnts[numElts].AlignedByteOffset = offset;
			mVertexElmnts[numElts].InputSlot = 0;
			mVertexElmnts[numElts].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
			mVertexElmnts[numElts].InstanceDataStepRate = 0;
			offset += 12;
			++numElts;
		}
		if (subMesh.hasTextureCoords)
		{
			mVertexElmnts[numElts].SemanticName = "TEXCOORD";
			mVertexElmnts[numElts].SemanticIndex = 0;
			mVertexElmnts[numElts].Format = DXGI_FORMAT_R32G32_FLOAT;
			mVertexElmnts[numElts].AlignedByteOffset = offset;
			mVertexElmnts[numElts].InputSlot = 0;
			mVertexElmnts[numElts].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
			mVertexElmnts[numElts].InstanceDataStepRate = 0;
			offset += 8;
			++numElts;
		}
		if (subMesh.hasVertexColours)
		{
			mVertexElmnts[numElts].SemanticName = "COLOR";
			mVertexElmnts[numElts].SemanticIndex = 0;
			mVertexElmnts[numElts].Format = DXGI_FORMAT_R8G8B8A8_UNORM; // A RGBA colour with 1 byte (0-255) per component
			mVertexElmnts[numElts].AlignedByteOffset = offset;
			mVertexElmnts[numElts].InputSlot = 0;
			mVertexElmnts[numElts].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
			mVertexElmnts[numElts].InstanceDataStepRate = 0;
			offset += 4;
			++numElts;
		}
		mVertexSize = offset;


		// Get the textures from the .x file
		if (mesh.GetNumMaterials() > 0)
		{
			// Create a texture
			mpMaterial = new CMaterial();
			
			// Get the texture name
			gen::SMeshMaterial pNewMat;
			mesh.GetMaterial(0, &pNewMat);
			std::string texName = pNewMat.textureFileNames[0];

			// Convert to wstring & create resource file from the file
			std::wstring wTexName(texName.begin(), texName.end());
			std::wstringstream wStrStream;
			wStrStream << "Media\\" << wTexName;
			mpMaterial->CreateDiffuseMap(pDevice, wStrStream.str());
		}


		// Given the vertex element list, pass it to DirectX to create a vertex layout. We also need to pass an example of a technique that will
		// render this model. We will only be able to render this model with techniques that have the same vertex input as the example we use here
		D3D10_PASS_DESC PassDesc;
		pTech->GetPassByIndex(0)->GetDesc(&PassDesc);
		pDevice->CreateInputLayout(mVertexElmnts, numElts, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &mpInputLayout);


		// Create the vertex buffer and fill it with the loaded vertex data
		mNumVertices = subMesh.numVertices;
		D3D10_BUFFER_DESC bufferDesc;
		bufferDesc.BindFlags = D3D10_BIND_VERTEX_BUFFER;
		bufferDesc.Usage = D3D10_USAGE_DEFAULT; // Not a dynamic buffer
		bufferDesc.ByteWidth = mNumVertices * mVertexSize; // Buffer size
		bufferDesc.CPUAccessFlags = 0;   // Indicates that CPU won't access this buffer at all after creation
		bufferDesc.MiscFlags = 0;
		D3D10_SUBRESOURCE_DATA initData; // Initial data
		initData.pSysMem = subMesh.vertices;
		if (FAILED(pDevice->CreateBuffer(&bufferDesc, &initData, &mpVertexBuffer)))
		{
			return false;
		}


		// Create the index buffer - assuming 2-byte (WORD) index data
		mNumIndices = static_cast<unsigned int>(subMesh.numFaces) * 3;
		bufferDesc.BindFlags = D3D10_BIND_INDEX_BUFFER;
		bufferDesc.Usage = D3D10_USAGE_DEFAULT;
		bufferDesc.ByteWidth = mNumIndices * sizeof(WORD);
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		initData.pSysMem = subMesh.faces;
		if (FAILED(pDevice->CreateBuffer(&bufferDesc, &initData, &mpIndexBuffer)))
		{
			return false;
		}

		return true;
	}

	void CMesh::Render(ID3D10Device* pDevice)
	{
		// Select vertex and index buffer - assuming all data will be as triangle lists
		UINT offset = 0;
		pDevice->IASetVertexBuffers(0, 1, &mpVertexBuffer, &mVertexSize, &offset);
		pDevice->IASetInputLayout(mpInputLayout);
		pDevice->IASetIndexBuffer(mpIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
		pDevice->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// Render the model. All the data and shader variables are prepared, now select the technique to use and draw.
		// The loop is for advanced techniques that need multiple passes - we will only use techniques with one pass
		D3D10_TECHNIQUE_DESC techDesc;
		mpRenderTech->GetDesc(&techDesc);
		for (UINT p = 0; p < techDesc.Passes; ++p)
		{
			mpRenderTech->GetPassByIndex(p)->Apply(0);
			pDevice->DrawIndexed(mNumIndices, 0, 0);
		}
		pDevice->DrawIndexed(mNumIndices, 0, 0);
	}
}

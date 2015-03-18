//====================================================================================
// FILE:    EntityManager.cpp --------------------------------------------------------
//------------------------------------------------------------------------------------

#include "EntityManager.h"


namespace sge
{
	//====================================================================================
	// ENTITY MANAGER CLASS CONSTRUCTOR & DESTRUCTOR
	//------------------------------------------------------------------------------------
	CEntityManager::CEntityManager()
	{
		mNextEID = 0U;
		mNextLightNum = 0U;
	}

	CEntityManager::~CEntityManager()
	{
		DestroyAllEntities();
	}


	//====================================================================================
	// ENTITY MANAGER CLASS FACTORY FUNCTIONS
	//------------------------------------------------------------------------------------
	CCamera* CEntityManager::CreateCameraEntity(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 orientation, float fov, float nearClip, float farClip)
	{
		// Create an instance of the camera
		CCamera* pCamera = new CCamera(mNextEID, pos, orientation, fov, nearClip, farClip);

		// Store the camera in the hash map
		mEntityMap.insert(EntityMap::value_type(mNextEID, pCamera));

		// Increment next entity ID and return the new camera
		mNextEID++;
		return pCamera;
	}

	CModel* CEntityManager::CreateModelEntity(CMesh* pMesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 orientation,
		DirectX::XMFLOAT3 scale)
	{
		// Create an instance of the required model
		CModel* pModel = new CModel(mNextEID, pMesh, pos, orientation, scale);

		// Store the model in the hash map
		mEntityMap.insert(EntityMap::value_type(mNextEID, pModel));

		// Increment next entity ID and return the new model
		mNextEID++;
		return pModel;
	}

	CModel* CEntityManager::CreateMirrorEntity(CMesh* pMesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 orientation,
		DirectX::XMFLOAT3 scale)
	{
		// Check if a mirror has already been created
		if (!hasMirror)
		{
			mpMirror = new CModel(mNextEID, pMesh, pos, orientation, scale);

			// Store the model in the hash map
			mEntityMap.insert(EntityMap::value_type(mNextEID, mpMirror));

			mNextEID++;
			hasMirror = true;
			return mpMirror;
		}

		return nullptr;
	}

	CModel* CEntityManager::CreatePortalEntity(CMesh* pMesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 orientation,
		DirectX::XMFLOAT3 scale)
	{
		// Check if a mirror has already been created
		if (!hasPortal)
		{
			mpPortal = new CModel(mNextEID, pMesh, pos, orientation, scale);

			// Store the model in the hash map
			mEntityMap.insert(EntityMap::value_type(mNextEID, mpPortal));

			mNextEID++;
			hasPortal = true;
			return mpMirror;
		}

		return nullptr;
	}

	CLight* CEntityManager::CreateLightEntity(CMesh* pMesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 orientation, DirectX::XMFLOAT3 scale,
		DirectX::XMFLOAT3 lightColour, int lightType)
	{
		// Check if there are already the max number of lights
		if (mNextLightNum < MAX_LIGHTS)
		{
			// Enough lights - create new one. Store it in hash map & lights array
			CLight* pNewLight = new CLight(mNextEID, pMesh, pos, orientation, scale, lightColour, lightType);
			mpLights[mNextLightNum] = pNewLight;

			// Increment next ID, num lights & return new light model
			mNextLightNum++;
			return pNewLight;
		}

		// Too many lights - return nullptr
		return nullptr;
	}


	//====================================================================================
	// ENTITY MANAGER CLASS METHODS
	//------------------------------------------------------------------------------------
	void CEntityManager::SetPortalTexture(ID3D10ShaderResourceView* pTexture)
	{
		if (mpPortal)
		{
			mpPortal->SetTexture(pTexture);
		}
	}

	void CEntityManager::Update()
	{		
		// Update mirror modle
		mpMirror->Update();
		
		// Call the update function for each stored entity
		for (miterEntityMap = mEntityMap.begin(); miterEntityMap != mEntityMap.end(); miterEntityMap++)
		{
			miterEntityMap->second->Update();
		}

		// Render lights
		for (int i = 0; i < mNextLightNum; i++)
		{
			mpLights[i]->Update();
		}

		for (int i = 0; i < mNextLightNum; i++)
		{
			mpLightTypes[i] = mpLights[i]->GetType();
			mpLightColours[i] = mpLights[i]->GetLightColour();
			mpLights[i]->GetPosition(mpLightPositions[i]);
			mpLightFacings[i] = mpLights[i]->GetLightFacing();
			mpCosHalfAngles[i] = mpLights[i]->GetCosHalfAngle();
			mpLightViewMatrices[i] = mpLights[i]->GetViewMatrix();
			mpLightProjMatrices[i] = mpLights[i]->GetProjectionMatrix();
		}
	}

	void CEntityManager::RenderMirror(ID3D10Device* pDevice, CCamera* pCamera, CShader* pShader)
	{
		// Pass camera's data over to the shader
		DirectX::XMFLOAT4X4 viewMat;
		pCamera->GetViewMatrix(viewMat);

		pShader->GetFXViewVar()->SetMatrix((float*)&viewMat);
		pShader->GetFXProjVar()->SetMatrix((float*)&pCamera->GetProjectionMatrix());

		DirectX::XMFLOAT3 cameraPos;
		pCamera->GetPosition(cameraPos);
		pShader->GetFXCameraPositionVar()->SetRawValue(&cameraPos, 0U, 12U);
		pShader->GetFXLightTypeVar()->SetIntArray((int*)mpLightTypes, 0U, mNextLightNum);
		pShader->GetFXLightColoursVar()->SetFloatVectorArray((float*)mpLightColours, 0U, mNextLightNum);
		pShader->GetFXLightPositionsVar()->SetFloatVectorArray((float*)mpLightPositions, 0U, mNextLightNum);
		pShader->GetFXLightFacingsVar()->SetFloatVectorArray((float*)mpLightFacings, 0U, mNextLightNum);
		pShader->GetFXCosHalfAngleVar()->SetFloatArray((float*)mpCosHalfAngles, 0U, mNextLightNum);

		pShader->GetFXSpecularPowerVar()->SetFloat(100.0f);
		
		// Render just the mirror with the clear surface technique
		mpMirror->Render(pDevice, pShader->GetMirrorClearTechnique(), pShader);
		
		// Create the mirror's plane
		DirectX::XMFLOAT4X4 mirrorMat;
		mpMirror->GetModelMatrix(mirrorMat);
		DirectX::XMFLOAT3 mirrorPoint{ mirrorMat(3, 0), mirrorMat(3, 1), mirrorMat(3, 2) };
		DirectX::XMFLOAT3 mirrorNormal{ mirrorMat(2, 0), mirrorMat(2, 1), mirrorMat(2, 2) };
		DirectX::XMVECTOR vecMirrorPlane = DirectX::XMPlaneFromPointNormal(DirectX::XMLoadFloat3(&mirrorPoint), DirectX::XMLoadFloat3(&mirrorNormal));

		// Reflect camera's view matrix in the mirror plane
		DirectX::XMMATRIX reflectMat = DirectX::XMMatrixReflect(vecMirrorPlane);
		DirectX::XMFLOAT4X4 camViewMat;
		pCamera->GetViewMatrix(camViewMat);
		DirectX::XMMATRIX reflectViewMat = reflectMat * DirectX::XMLoadFloat4x4(&camViewMat);

		// Reflect camera's position in the mirror plane
		DirectX::XMFLOAT3 camPos;
		pCamera->GetPosition(camPos);
		DirectX::XMVECTOR reflectCamPosVec = DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&camPos), reflectMat);
		DirectX::XMFLOAT3 reflectCamPos;
		DirectX::XMStoreFloat3(&reflectCamPos, reflectCamPosVec);
		DirectX::XMFLOAT4X4 finalReflectViewMat;
		DirectX::XMStoreFloat4x4(&finalReflectViewMat, reflectViewMat);
		DirectX::XMFLOAT4 finalMirrorPlane;
		DirectX::XMStoreFloat4(&finalMirrorPlane, vecMirrorPlane);


		// Render models within the mirror
		pShader->GetFXViewVar()->SetMatrix((float*)&finalReflectViewMat);
		pShader->GetFXCameraPositionVar()->SetRawValue(&reflectCamPos, 0U, 12U);
		pShader->GetFXClipPlaneVar()->SetRawValue(&finalMirrorPlane, 0U, 16U);
		

		// Render all models and lights with the mirror technique
		for (miterEntityMap = mEntityMap.begin(); miterEntityMap != mEntityMap.end(); miterEntityMap++)
		{
			if (mpMirror != miterEntityMap->second)
			{
				miterEntityMap->second->Render(pDevice, pShader, true);
			}
		}

		for (int i = 0; i < mNextLightNum; i++)
		{
			mpLights[i]->Render(pDevice, pShader, true);
		}


		// Reset values in shader
		DirectX::XMFLOAT4 nullVec{0.0f, 0.0f, 0.0f, 0.0f};
		pShader->GetFXClipPlaneVar()->SetRawValue(&nullVec, 0U, 16U);
		pShader->GetFXViewVar()->SetMatrix((float*)&viewMat);
		pCamera->GetPosition(cameraPos);
		pShader->GetFXCameraPositionVar()->SetRawValue(&cameraPos, 0U, 12U);
	}

	void CEntityManager::RenderShadows(ID3D10Device* pDevice, CShader* pShader)
	{
		// Loop through each light & see if it is a spot light to render shadows for
		for (int i = 0; i < mNextLightNum; i++)
		{
			if (mpLightTypes[i] == ELightTypes::SPOT_LIGHT)
			{
				pShader->GetFXViewVar()->SetMatrix((float*)&mpLightViewMatrices[i]);
				pShader->GetFXProjVar()->SetMatrix((float*)&mpLightProjMatrices[i]);

				// Render shadows for this light
				for (miterEntityMap = mEntityMap.begin(); miterEntityMap != mEntityMap.end(); miterEntityMap++)
				{
					if (mpMirror != miterEntityMap->second)
					{
						miterEntityMap->second->Render(pDevice, pShader, false, true);
					}
				}
			}
		}
	}

	void CEntityManager::Render(ID3D10Device* pDevice, CCamera* pCamera, CShader* pShader)
	{
		SetShaderVariables(pCamera, pShader);

		// Render mirror
		mpMirror->Render(pDevice, pShader);

		// Call the update function for each stored entity
		for (miterEntityMap = mEntityMap.begin(); miterEntityMap != mEntityMap.end(); miterEntityMap++)
		{
			if (mpMirror != miterEntityMap->second)
			{
				miterEntityMap->second->Render(pDevice, pShader);
			}
		}

		// Render lights
		for (int i = 0; i < mNextLightNum; i++)
		{
			mpLights[i]->Render(pDevice, pShader);
		}
	}

	void CEntityManager::SetShaderVariables(CCamera* pCamera, CShader* pShader)
	{
		// Pass camera's data over to the shader
		DirectX::XMFLOAT4X4 viewMat;
		pCamera->GetViewMatrix(viewMat);

		pShader->GetFXViewVar()->SetMatrix((float*)&viewMat);
		pShader->GetFXProjVar()->SetMatrix((float*)&pCamera->GetProjectionMatrix());

		DirectX::XMFLOAT3 cameraPos;
		pCamera->GetPosition(cameraPos);
		pShader->GetFXCameraPositionVar()->SetRawValue(&cameraPos, 0U, 12U);
		pShader->GetFXLightTypeVar()->SetIntArray((int*)mpLightTypes, 0U, mNextLightNum);
		pShader->GetFXLightColoursVar()->SetFloatVectorArray((float*)mpLightColours, 0U, mNextLightNum);
		pShader->GetFXLightPositionsVar()->SetFloatVectorArray((float*)mpLightPositions, 0U, mNextLightNum);
		pShader->GetFXLightFacingsVar()->SetFloatVectorArray((float*)mpLightFacings, 0U, mNextLightNum);
		pShader->GetFXCosHalfAngleVar()->SetFloatArray((float*)mpCosHalfAngles, 0U, mNextLightNum);

		pShader->GetFXLightViewVar()->SetMatrixArray((float*)mpLightViewMatrices, 0U, mNextLightNum);
		pShader->GetFXLightProjVar()->SetMatrixArray((float*)mpLightProjMatrices, 0U, mNextLightNum);

		pShader->GetFXSpecularPowerVar()->SetFloat(100.0f);
	}

	bool CEntityManager::DestroyEntity(size_t key)
	{
		// Set the iterator to the searched entity
		miterEntityMap = mEntityMap.find(key);

		// Check if the entity key found an existing entity
		if (miterEntityMap != mEntityMap.end())
		{
			// Entity was found - deallocate & erase from map
			delete miterEntityMap->second;
			mEntityMap.erase(miterEntityMap);
			return true;
		}

		// An entity was not found
		return false;
	}

	void CEntityManager::DestroyAllEntities()
	{
		// Loop through all entities & remove them
		for (miterEntityMap = mEntityMap.begin(); miterEntityMap != mEntityMap.end(); miterEntityMap++)
		{
			delete miterEntityMap->second;
		}
		mEntityMap.clear();
	}
}

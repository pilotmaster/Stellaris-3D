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
	void CEntityManager::Update()
	{		
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

	void CEntityManager::RenderShadows(ID3D10Device* pDevice, CShader* pShader)
	{
		pShader->GetFXViewVar()->SetMatrix((float*)&mpLightViewMatrices[2]);
		pShader->GetFXProjVar()->SetMatrix((float*)&mpLightProjMatrices[2]);
		
		// Call the update function for each stored entity
		for (miterEntityMap = mEntityMap.begin(); miterEntityMap != mEntityMap.end(); miterEntityMap++)
		{
			miterEntityMap->second->Render(pDevice, pShader, true);
		}
	}

	void CEntityManager::Render(ID3D10Device* pDevice, CCamera* pCamera, CShader* pShader)
	{
		// Pass camera's data over to the shader
		pShader->GetFXViewVar()->SetMatrix((float*)&pCamera->GetViewMatrix());
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
		

		// Call the update function for each stored entity
		for (miterEntityMap = mEntityMap.begin(); miterEntityMap != mEntityMap.end(); miterEntityMap++)
		{
			miterEntityMap->second->Render(pDevice, pShader);
		}

		// Render lights
		for (int i = 0; i < mNextLightNum; i++)
		{
			mpLights[i]->Render(pDevice, pShader);
		}
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

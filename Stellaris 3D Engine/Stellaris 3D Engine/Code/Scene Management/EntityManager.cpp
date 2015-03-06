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
	}

	CEntityManager::~CEntityManager()
	{
		DestroyAllEntities();
	}


	//====================================================================================
	// ENTITY MANAGER CLASS FACTORY FUNCTIONS
	//------------------------------------------------------------------------------------
	CCamera* CEntityManager::CreateCameraEntity(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 orientation)
	{
		CCamera* pCamera = nullptr;
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


	//====================================================================================
	// ENTITY MANAGER CLASS METHODS
	//------------------------------------------------------------------------------------
	void CEntityManager::UpdateAndRenderEntities(ID3D10Device* pDevice, ID3D10EffectTechnique* pTech)
	{
		// Call the update function for each stored entity
		for (miterEntityMap = mEntityMap.begin(); miterEntityMap != mEntityMap.end(); miterEntityMap++)
		{
			miterEntityMap->second->Update();
			miterEntityMap->second->Render(pDevice, pTech);
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

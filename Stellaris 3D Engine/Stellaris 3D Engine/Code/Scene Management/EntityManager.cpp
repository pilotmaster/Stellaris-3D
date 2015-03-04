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


	//====================================================================================
	// ENTITY MANAGER CLASS METHODS
	//------------------------------------------------------------------------------------
	void CEntityManager::UpdateEntities()
	{
		// Call the update function for each stored entity
		for (miterEntityMap = mEntityMap.begin(); miterEntityMap != mEntityMap.end(); miterEntityMap++)
		{
			miterEntityMap->second->UpdateMatrices();
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

//====================================================================================
// FILE:    EntityManager.h ----------------------------------------------------------
//------------------------------------------------------------------------------------

#ifndef _ENTITY_MANAGER_H_
#define _ENTITY_MANAGER_H_


#include <unordered_map>

#include "..\Scene Entities\Entity.h"
#include "..\Scene Entities\Camera.h"


namespace sge
{
	typedef std::unordered_map<size_t, CEntity*> EntityMap;

	class CEntityManager
	{
	public:
		// CONSTRUCTOR & DESTRUCTOR
		//---------------------------------
		CEntityManager();
		~CEntityManager();


		// FACTORY FUNCTIONS
		//---------------------------------
		CCamera* CreateCameraEntity(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 orientation);


		// METHODS
		//---------------------------------
		void UpdateAndRenderEntities();
		bool DestroyEntity(size_t key);
		void DestroyAllEntities();


	private:
		// An instance of the hash map used to store the entities
		EntityMap mEntityMap;
		EntityMap::iterator miterEntityMap;

		// The next ID which will be used as a key for the next entity that is placed into
		// the hash map
		size_t mNextEID;
	};
}


#endif /* _ENTITY_MANAGER_H_ */

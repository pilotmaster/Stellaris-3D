//====================================================================================
// FILE:    EntityManager.h ----------------------------------------------------------
//------------------------------------------------------------------------------------

#ifndef _ENTITY_MANAGER_H_
#define _ENTITY_MANAGER_H_


#include <unordered_map>

#include "..\Scene Entities\Model.h"
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
		// Creates an entity of type camera and returns the created camera
		CCamera* CreateCameraEntity(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 orientation, float fov, float nearClip, float farClip);
		// Creates an entity of type model and returns the created model
		CModel* CreateModelEntity(CMesh* pMesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 orientation, DirectX::XMFLOAT3 scale);


		// METHODS
		//---------------------------------
		void Update();
		void Render(ID3D10Device* pDevice, CCamera* pCamera, CShader* pShader);
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

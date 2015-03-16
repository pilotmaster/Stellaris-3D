//====================================================================================
// FILE:    EntityManager.h ----------------------------------------------------------
//------------------------------------------------------------------------------------

#ifndef _ENTITY_MANAGER_H_
#define _ENTITY_MANAGER_H_


#include <unordered_map>

#include "..\Scene Entities\Light.h"
#include "..\Scene Entities\Camera.h"


namespace sge
{
	//====================================================================================
	// ENTITY MANAGER CLASS
	//------------------------------------------------------------------------------------
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
		// Create a new light
		CLight* CreateLightEntity(CMesh* pMesh, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 orientation, DirectX::XMFLOAT3 scale,
			DirectX::XMFLOAT3 lightColour, int lightType);


		// METHODS
		//---------------------------------
		void Update();
		// The base shader is passed in so that camera and light data can be passed to the effect file
		void Render(ID3D10Device* pDevice, CCamera* pCamera, CShader* pShader);
		bool DestroyEntity(size_t key);
		void DestroyAllEntities();


		// STATIC VARIABLES
		//---------------------------------
		static const int MAX_LIGHTS = 5;


	private:
		// An instance of the hash map used to store the entities
		EntityMap mEntityMap;
		EntityMap::iterator miterEntityMap;

		// Array of light objects
		CLight* mpLights[MAX_LIGHTS];
		int mpLightTypes[MAX_LIGHTS];
		DirectX::XMFLOAT3 mpLightColours[MAX_LIGHTS];
		DirectX::XMFLOAT3 mpLightPositions[MAX_LIGHTS];
		DirectX::XMFLOAT3 mpLightFacings[MAX_LIGHTS];
		DirectX::XMFLOAT4X4 mpLightViewMatrices[MAX_LIGHTS];
		DirectX::XMFLOAT4X4 mpLightProjMatrices[MAX_LIGHTS];
		float mpCosHalfAngles[MAX_LIGHTS];

		// The next ID which will be used as a key for the next entity that is placed into
		// the hash map
		size_t mNextEID;
		size_t mNextLightNum;
	};
}


#endif /* _ENTITY_MANAGER_H_ */

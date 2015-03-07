//====================================================================================
// FILE:    Engine.h -----------------------------------------------------------------
//------------------------------------------------------------------------------------

#ifndef _ENGINE_H_
#define _ENGINE_H_


#include "Direct3DApp.h"
#include "..\Shaders\Shader.h"
#include "..\Utilities\Mesh.h"


namespace sge
{
	//====================================================================================
	// STELLARIS 3D ENGINE CLASS 
	//------------------------------------------------------------------------------------
	class CStellaris3D : public CD3DApp
	{
	public:
		// CONSTRUCTORS & DESTRUCTOR
		//---------------------------------
		CStellaris3D();
		~CStellaris3D();


		// OVERRIDE METHODS
		//---------------------------------
		bool InitialiseEngine(HINSTANCE hInstance);
		void Update();
		void Render(CCamera* pCamera);


		// METHODS
		//---------------------------------
		// Creates a camera at the given co-ordinates
		CCamera* CreateCamera(DirectX::XMFLOAT3 pos = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3 rot = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
			float fov = DirectX::XM_PI / 4.0f, float nearClip = 0.1f, float farClip = 1000.0f);
		// Loads the mesh for a given .x file
		CMesh* LoadMesh(std::string fileName);
		// Creates a model from the passed in mesh instance
		CModel* CreateModel(CMesh* pMesh, DirectX::XMFLOAT3 pos = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3 rot = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f),
			DirectX::XMFLOAT3 scale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));


	private:
		// CLASS VARIABLES
		//---------------------------------
		CEntityManager* mpEntityManager;
		CShader* mpBasicShader;
	};
}


#endif /* _ENGINE_H_ */

//====================================================================================
// FILE:    Engine.h -----------------------------------------------------------------
//------------------------------------------------------------------------------------

#ifndef _ENGINE_H_
#define _ENGINE_H_


#include "Direct3DApp.h"
#include "..\Shaders\Shader.h"


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
		void Render();


	private:
		// CLASS VARIABLES
		//---------------------------------
		CEntityManager* mpEntityManager;
		CShader* mpBasicShader;
	};
}


#endif /* _ENGINE_H_ */

//====================================================================================
// FILE:    Engine.cpp ---------------------------------------------------------------
//------------------------------------------------------------------------------------

#include "Engine.h"

namespace sge
{
	//====================================================================================
	// ENGINE CLASS CONSTRUCTOR & DESTRUCTOR
	//------------------------------------------------------------------------------------
	CStellaris3D::CStellaris3D()
	{

	}

	CStellaris3D::~CStellaris3D()
	{

	}


	//====================================================================================
	// ENGINE CLASS OVERRIDE METHODS
	//------------------------------------------------------------------------------------
	bool CStellaris3D::InitialiseEngine(HINSTANCE hInstance)
	{
		// Call initialisation of application
		if (!CD3DApp::InitialiseApplication(hInstance))
		{
			return false;
		}

		// Initialise scene

		
		// Begin timer
		CTimer::GetTimerInstace()->Reset();

		return true;
	}

	void CStellaris3D::Update()
	{

	}

	void CStellaris3D::Render()
	{

	}
}

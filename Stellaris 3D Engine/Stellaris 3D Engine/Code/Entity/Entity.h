//====================================================================================
// FILE:    Entity.h -----------------------------------------------------------------
//------------------------------------------------------------------------------------

#ifndef _ENTITY_H_
#define _ENTITY_H_


#include <dxgi.h>
#include <dxerr.h>
#include <d3dx10.h>
#include <DirectXMath.h>
#include <DirectXColors.h>


//====================================================================================
// ENTITY CLASS
//------------------------------------------------------------------------------------
class CEntity
{
public:
	// CONSTRUCTOR & DESTRUCTOR
	//---------------------------------
	CEntity();
	~CEntity();


	// METHODS
	//---------------------------------
	virtual void Update() = 0;


	// ACCESSORS
	//---------------------------------


	// MUTATORS
	//---------------------------------


private:
	// MATRICES
	//---------------------------------
	DirectX::XMFLOAT4X4 mWorldMatrix;


	// ENTITY DATA
	//---------------------------------
	DirectX::XMFLOAT3 mPosition;
	DirectX::XMFLOAT3 mRotation;

};


#endif /* _ENTITY_H_ */

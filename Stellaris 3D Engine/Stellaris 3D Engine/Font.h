//====================================================================================
// FILE:	Font.h -------------------------------------------------------------------
//------------------------------------------------------------------------------------

#ifndef _FONT_H_
#define _FONT_H_


#include <D3DX10.h>
#include <d3d10.h>
#include <DirectXMath.h>
#include <fstream>


namespace sge
{
	//====================================================================================
	// CONSTANTS
	//------------------------------------------------------------------------------------
	const UINT NUM_CHARACTERS = 95U;


	//====================================================================================
	// ENUMERATIONS 
	//------------------------------------------------------------------------------------
	enum EFontTypes
	{
		FONT_SUBWAY_TICKER
	};

	
	//====================================================================================
	// FONT-SPECIFIC STRUCTURES
	//------------------------------------------------------------------------------------
	struct SFontData
	{
		int   mASCII;			// The ASCII character code for the character
		float mLeftPos;			// Left-most UV position for character
		float mRightPos;		// Right-most UV position for character
		float mPixelThickness;	// Thickness of the character
		bool  mHanging;			// Determines whether part of the character falls below the bottom i.e. y, j, etc.
	};


	//====================================================================================
	// FONT CLASS 
	//------------------------------------------------------------------------------------
	class CFont
	{
	private:
		// FONT VARIABLES
		//---------------------------------
		// Store a list of all the details for all the characters
		SFontData mCharList[NUM_CHARACTERS];

		// Diffuse map of the font
		ID3D10ShaderResourceView* mpDiffuseMap;


	public:
		// CONSTRUCTOR & DESTRUCTOR
		//---------------------------------
		CFont(EFontTypes inType, ID3D10Device* pDevice);
		~CFont();


		// ACCESSORS
		//---------------------------------
		SFontData* GetCharacterData(int inASCII);

		inline ID3D10ShaderResourceView* GetDiffuseMap()
		{
			return mpDiffuseMap;
		}
	};
}


#endif

//====================================================================================
// FILE:	Font.cpp -----------------------------------------------------------------
//------------------------------------------------------------------------------------


#include "Font.h"


namespace sge
{
	//====================================================================================
	// FONT CLASS CONSTRUCTORS & DESRUCTOR 
	//------------------------------------------------------------------------------------
	CFont::CFont(EFontTypes inType, ID3D10Device* pDevice)
	{
		// LOAD FONT DATA BASED ON TYPE
		//---------------------------------
		switch (inType)
		{
		case FONT_SUBWAY_TICKER:
			// Load & open font file
			std::ifstream fontFile;

			fontFile.open("SubwayTickerFontData.txt");

			char tmp;

			// Loop through character list
			for (int i = 0; i < NUM_CHARACTERS; i++)
			{
				// Load ASCII code
				fontFile >> mCharList[i].mASCII;

				// Load character into tmp - it is not needed
				fontFile >> tmp;

				// Load left (start) & right (end) positions
				fontFile >> mCharList[i].mLeftPos;
				fontFile >> mCharList[i].mRightPos;

				// Load the width of the character in pixels
				fontFile >> mCharList[i].mPixelThickness;

				// Load whether it is a hanging character
				fontFile >> mCharList[i].mHanging;
			}

			// Load diffuse map for font
			D3DX10CreateShaderResourceViewFromFile(pDevice, L"SubwayTickerMap.dds", NULL, NULL, &mpDiffuseMap, NULL);

			break;
		}
	}

	CFont::~CFont()
	{
		mpDiffuseMap->Release();
	}


	//====================================================================================
	// FONT CLASS ACCESSOR METHODS -------------------------------------------------------
	//------------------------------------------------------------------------------------
	SFontData* CFont::GetCharacterData(int inASCII)
	{
		// Return pointer
		SFontData* pFontCharacter = nullptr;

		// Find character based on the ascii code provided
		for (int i = 0; i < NUM_CHARACTERS; i++)
		{
			// Check if the ascii code matches the current character's code
			if (mCharList[i].mASCII == inASCII)
			{
				// Found right character - set pointer
				pFontCharacter = &mCharList[i];
				// Exit loop
				break;
			}
		}

		// Return found character data
		return pFontCharacter;
	}
}

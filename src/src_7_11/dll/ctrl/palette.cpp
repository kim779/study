#include "stdafx.h"
#include "bitmap.h"
#include "palette.h"

CExPalette::CExPalette(CExBitmap* pBmp)
{
	ASSERT(pBmp);

	int cPaletteEntries = pBmp->GetPalEntries();
	int cPalette = sizeof(LOGPALETTE) +
		   sizeof(PALETTEENTRY) * cPaletteEntries;
	
	LOGPALETTE* pPal = (LOGPALETTE*)new BYTE[cPalette];
	RGBQUAD*    pColorTab = pBmp->GetColorTablePtr();
	pPal->palVersion = 0x300;
	pPal->palNumEntries = cPaletteEntries;

	for (int ii = 0; ii < cPaletteEntries; ii++)
	{
		pPal->palPalEntry[ii].peRed   = pColorTab[ii].rgbRed;
		pPal->palPalEntry[ii].peGreen = pColorTab[ii].rgbGreen;
		pPal->palPalEntry[ii].peBlue  = pColorTab[ii].rgbBlue;
		pPal->palPalEntry[ii].peFlags = NULL;
	}
	VERIFY(CreatePalette(pPal));
	delete[] (BYTE *)pPal;
}


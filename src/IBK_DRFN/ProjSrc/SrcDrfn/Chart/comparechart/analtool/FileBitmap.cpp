// FileBitmap.cpp : implementation file
//

#include "stdafx.h"
#include "FileBitmap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileBitmap

CFileBitmap::CFileBitmap()
{
	m_hDIB = NULL;
}

CFileBitmap::~CFileBitmap()
{
	ClearAll();
}

void CFileBitmap::ClearAll()
{
	if(m_hDIB) {
		::GlobalFree(m_hDIB);
		m_hDIB = NULL;
	}
//	m_Pal.DeleteObject();
//	m_pPal.DeleteObject();
}

BOOL CFileBitmap::LoadBMP(LPCTSTR sBMPFile)
{
	return LoadBMP(sBMPFile, &m_hDIB/*, &m_Pal*/);
}

/////////////////////////////////////////////////////////////////////////////
// CFileButton message handlers
// LoadBMP		- Loads a BMP file and creates a logical palette for it.
// Returns		- TRUE for success
// sBMPFile		- Full path of the BMP file
// phDIB		- Pointer to a HGLOBAL variable to hold the loaded bitmap
//			  Memory is allocated by this function but should be 
//			  released by the caller.
// pPal			- Will hold the logical palette
BOOL CFileBitmap::LoadBMP( LPCTSTR sBMPFile, HGLOBAL *phDIB/*, CPalette *pPal*/)
{
	if(!sBMPFile)
		return FALSE;

	CFile file;
	if( !file.Open( sBMPFile, CFile::modeRead) )
		return FALSE;

	BITMAPFILEHEADER bmfHeader;
	long nFileLen;

	nFileLen = file.GetLength();


	// Read file header
	if (file.Read((LPSTR)&bmfHeader, sizeof(bmfHeader)) != sizeof(bmfHeader))
		return FALSE;

	// File type should be 'BM'
	if (bmfHeader.bfType != ((WORD) ('M' << 8) | 'B'))
		return FALSE;

	HGLOBAL hDIB = ::GlobalAlloc(GMEM_FIXED, nFileLen);
	if (hDIB == 0)
		return FALSE;

	// Read the remainder of the bitmap file.
	if (file.Read((LPSTR)hDIB, nFileLen - sizeof(BITMAPFILEHEADER)) !=
		nFileLen - sizeof(BITMAPFILEHEADER) )
	{
		::GlobalFree(hDIB);
		hDIB = NULL;
		return FALSE;
	}
	
	BITMAPINFO &bmInfo = *(LPBITMAPINFO)hDIB ;
	m_nWidth = bmInfo.bmiHeader.biWidth;
	m_nHeight = bmInfo.bmiHeader.biHeight;

	int nColors = bmInfo.bmiHeader.biClrUsed ? bmInfo.bmiHeader.biClrUsed : 
						1 << bmInfo.bmiHeader.biBitCount;

	// Create the palette
/*
	if( nColors <= 256 )
	{
		UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * nColors);
		LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];

		pLP->palVersion = 0x300;
		pLP->palNumEntries = nColors;

		for( int i=0; i < nColors; i++)
		{
			pLP->palPalEntry[i].peRed = bmInfo.bmiColors[i].rgbRed;
			pLP->palPalEntry[i].peGreen = bmInfo.bmiColors[i].rgbGreen;
			pLP->palPalEntry[i].peBlue = bmInfo.bmiColors[i].rgbBlue;
			pLP->palPalEntry[i].peFlags = 0;
		}

		pPal->CreatePalette( pLP );

		delete[] pLP;
	}
*/

	*phDIB = hDIB;
	return TRUE;
}

BOOL CFileBitmap::LoadBMPImage( LPCTSTR sBMPFile)
{
	return LoadBMPImage(sBMPFile, m_bitmap/*, &m_pPal*/);
}

// LoadBMPImage	- Loads a BMP file and creates a bitmap GDI object
//		  also creates logical palette for it.
// Returns	- TRUE for success
// sBMPFile	- Full path of the BMP file
// bitmap	- The bitmap object to initialize
// pPal		- Will hold the logical palette. Can be NULL
BOOL CFileBitmap::LoadBMPImage( LPCTSTR sBMPFile, CBitmap& bitmap/*, CPalette *pPal*/)
{
	CFile file;
	if( !file.Open( sBMPFile, CFile::modeRead) )
		return FALSE;

	BITMAPFILEHEADER bmfHeader;

	// Read file header
	if (file.Read((LPSTR)&bmfHeader, sizeof(bmfHeader)) != sizeof(bmfHeader))
		return FALSE;

	// File type should be 'BM'
	if (bmfHeader.bfType != ((WORD) ('M' << 8) | 'B'))
		return FALSE;

	// Get length of the remainder of the file and allocate memory
	DWORD nPackedDIBLen = file.GetLength() - sizeof(BITMAPFILEHEADER);
	HGLOBAL hDIB = ::GlobalAlloc(GMEM_FIXED, nPackedDIBLen);
	if (hDIB == 0)
		return FALSE;

	// Read the remainder of the bitmap file.
	if (file.Read((LPSTR)hDIB, nPackedDIBLen) != nPackedDIBLen )
	{
		::GlobalFree(hDIB);
		m_hDIB = NULL;
		return FALSE;
	}


	BITMAPINFOHEADER &bmiHeader = *(LPBITMAPINFOHEADER)hDIB ;
	BITMAPINFO &bmInfo = *(LPBITMAPINFO)hDIB ;
	m_nWidth = bmInfo.bmiHeader.biWidth;
	m_nHeight = bmInfo.bmiHeader.biHeight;

	// If bmiHeader.biClrUsed is zero we have to infer the number
	// of colors from the number of bits used to specify it.
	int nColors = bmiHeader.biClrUsed ? bmiHeader.biClrUsed : 
						1 << bmiHeader.biBitCount;

	LPVOID lpDIBBits;
	if( bmInfo.bmiHeader.biBitCount > 8 )
		lpDIBBits = (LPVOID)((LPDWORD)(bmInfo.bmiColors + bmInfo.bmiHeader.biClrUsed) + 
			((bmInfo.bmiHeader.biCompression == BI_BITFIELDS) ? 3 : 0));
	else
		lpDIBBits = (LPVOID)(bmInfo.bmiColors + nColors);

	// Create the logical palette
/*
	if( pPal != NULL )
	{
		// Create the palette
		if( nColors <= 256 )
		{
			UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * nColors);
			LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];

			pLP->palVersion = 0x300;
			pLP->palNumEntries = nColors;

			for( int i=0; i < nColors; i++)
			{
				pLP->palPalEntry[i].peRed = bmInfo.bmiColors[i].rgbRed;
				pLP->palPalEntry[i].peGreen = bmInfo.bmiColors[i].rgbGreen;
				pLP->palPalEntry[i].peBlue = bmInfo.bmiColors[i].rgbBlue;
				pLP->palPalEntry[i].peFlags = 0;
			}

			pPal->CreatePalette( pLP );

			delete[] pLP;
		}
	}
*/

	CClientDC dc(NULL);
//	CPalette* pOldPalette = NULL;
//	if( pPal )
//	{
//		pOldPalette = dc.SelectPalette( pPal, FALSE );
//		dc.RealizePalette();
//	}

	HBITMAP hBmp = CreateDIBitmap( dc.m_hDC,		// handle to device context 
				&bmiHeader,	// pointer to bitmap size and format data 
				CBM_INIT,	// initialization flag 
				lpDIBBits,	// pointer to initialization data 
				&bmInfo,	// pointer to bitmap color-format data 
				DIB_RGB_COLORS);		// color-data usage 
	bitmap.Attach( hBmp );

/*
	if( pOldPalette )
		dc.SelectPalette( pOldPalette, FALSE );
*/

	::GlobalFree(hDIB);
	m_hDIB = NULL;
	return TRUE;
}

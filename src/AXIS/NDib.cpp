// NDib.cpp: implementation of the CNDib class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "axis.h"
#include "NDib.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#pragma warning (disable : 6011)
CNDib::CNDib()
{
	m_dib = NULL;
	m_bi  = NULL;

	m_bMemoryDIB = TRUE;
	m_bLegitDraw = FALSE;
}

CNDib::~CNDib()
{
	FreeNDib();
}

BOOL CNDib::InitNDIB(CString file)
{
	unsigned           fh{};
	BITMAPINFOHEADER   bi{};
	OFSTRUCT           of{};

	FreeNDib();

	/* Open the file and get a handle to it's BITMAPINFO */
	fh = OpenFile ((LPCSTR)file, (LPOFSTRUCT)&of, OF_READ);
	if (fh == -1)	return FALSE;
	m_bi = ReadDibBitmapInfo(fh);

	m_dwOffset = _llseek(fh, 0L, SEEK_CUR);
	_lclose (fh);

	if (!m_bi)	return FALSE;

	DibInfo(m_bi,&bi);

	m_bLegitDraw = TRUE;

	/*  If the input bitmap is not in RGB FORMAT the banding code will
	*  not work!  we need to load the DIB bits into memory.
	*  if memory DIB, load it all NOW!  This will avoid calling the
	*  banding code.
	*/
	if (m_bMemoryDIB || bi.biCompression != BI_RGB)
		m_dib = OpenDIB(file);

	/*  If the RLE could not be loaded all at once, exit gracefully NOW,
	*  to avoid calling the banding code
	*/
	if ((bi.biCompression != BI_RGB) && !m_dib)
	{
		FreeNDib();
		return FALSE;
	}
	return TRUE;
}

void CNDib::FreeNDib()
{
    if (m_dib)			GlobalFree(m_dib);
    if (m_bi && m_bi != m_dib)	GlobalFree(m_bi);
}

HANDLE CNDib::ReadDibBitmapInfo(int fh)
{
	DWORD     off{};
	HANDLE    hbi = NULL;
	int       size{};
	int       i{};
	WORD      nNumColors{};

	RGBQUAD FAR* pRgb{};
	BITMAPINFOHEADER   bi{};
	BITMAPCOREHEADER   bc{};
	LPBITMAPINFOHEADER lpbi{};
	BITMAPFILEHEADER   bf{};
	DWORD              dwWidth = 0;
	DWORD              dwHeight = 0;
	WORD               wPlanes{}, wBitCount{};

	if (fh == -1)	return NULL;

	/* Reset file pointer and read file header */
	off = _llseek(fh, 0L, SEEK_CUR);
	if (sizeof (bf) != _lread (fh, (LPSTR)&bf, sizeof (bf)))
		return FALSE;

	/* Do we have a RC HEADER? */
	if (!ISDIB (bf.bfType))
	{
		bf.bfOffBits = 0L;
		_llseek (fh, off, SEEK_SET);
	}

	if (sizeof (bi) != _lread (fh, (LPSTR)&bi, sizeof(bi)))
		return FALSE;

	nNumColors = DibNumColors(&bi);

	/* Check the nature (BITMAPINFO or BITMAPCORE) of the info. block
	* and extract the field information accordingly. If a BITMAPCOREHEADER,
	* transfer it's field information to a BITMAPINFOHEADER-style block
	*/
	switch (size = (int)bi.biSize)
	{
	case sizeof (BITMAPINFOHEADER):
	    break;
	case sizeof (BITMAPCOREHEADER):
	    bc = *(BITMAPCOREHEADER*)&bi;
	    dwWidth   = (DWORD)bc.bcWidth;
	    dwHeight  = (DWORD)bc.bcHeight;
	    wPlanes   = bc.bcPlanes;
	    wBitCount = bc.bcBitCount;

	    bi.biSize               = sizeof(BITMAPINFOHEADER);
	    bi.biWidth              = dwWidth;
	    bi.biHeight             = dwHeight;
	    bi.biPlanes             = wPlanes;
	    bi.biBitCount           = wBitCount;

	    bi.biCompression        = BI_RGB;
	    bi.biSizeImage          = 0;
	    bi.biXPelsPerMeter      = 0;
	    bi.biYPelsPerMeter      = 0;
	    bi.biClrUsed            = nNumColors;
	    bi.biClrImportant       = nNumColors;

	    _llseek (fh, (LONG)sizeof (BITMAPCOREHEADER) - sizeof (BITMAPINFOHEADER), SEEK_CUR);
	    break;
	default:
	    /* Not a DIB! */
	    return NULL;
	}

	/*  Fill in some default values if they are zero */
	if (bi.biSizeImage == 0)
		bi.biSizeImage = WIDTHBYTES ((DWORD)bi.biWidth * bi.biBitCount)*bi.biHeight;
	if (bi.biClrUsed == 0)
		bi.biClrUsed = DibNumColors(&bi);

	/* Allocate for the BITMAPINFO structure and the color table. */
	hbi = GlobalAlloc (GHND, (LONG)bi.biSize + nNumColors * sizeof(RGBQUAD));
	if (!hbi)	return NULL;
	lpbi = (LPBITMAPINFOHEADER)GlobalLock (hbi);
	*lpbi = bi;

	/* Get a pointer to the color table */
	pRgb = (RGBQUAD FAR *)((LPSTR)lpbi + bi.biSize);
	if (nNumColors)
	{
		if (size == sizeof(BITMAPCOREHEADER))
		{
			/* Convert a old color table (3 byte RGBTRIPLEs) to a new
			* color table (4 byte RGBQUADs)
			*/
			_lread (fh, (LPSTR)pRgb, nNumColors * sizeof(RGBTRIPLE));

			for (i = nNumColors - 1; i >= 0; i--)
			{
				RGBQUAD rgb;

				rgb.rgbRed      = ((RGBTRIPLE FAR *)pRgb)[i].rgbtRed;
				rgb.rgbBlue     = ((RGBTRIPLE FAR *)pRgb)[i].rgbtBlue;
				rgb.rgbGreen    = ((RGBTRIPLE FAR *)pRgb)[i].rgbtGreen;
				rgb.rgbReserved = (BYTE)0;

				pRgb[i] = rgb;
			}
		}
		else	_lread(fh,(LPSTR)pRgb,nNumColors * sizeof(RGBQUAD));
	}

	if (bf.bfOffBits != 0L)
	_llseek(fh,off + bf.bfOffBits,SEEK_SET);

	GlobalUnlock(hbi);
	return hbi;
}

BOOL CNDib::DibInfo(HANDLE hbi,LPBITMAPINFOHEADER lpbi)
{
	if (hbi)
	{
		*lpbi = *(LPBITMAPINFOHEADER)GlobalLock (hbi);

		/* fill in the default fields */
		if (lpbi->biSize != sizeof (BITMAPCOREHEADER))
		{
			if (lpbi->biSizeImage == 0L)
				lpbi->biSizeImage = WIDTHBYTES(lpbi->biWidth*lpbi->biBitCount) * lpbi->biHeight;
			if (lpbi->biClrUsed == 0L)
				lpbi->biClrUsed = DibNumColors (lpbi);
		}
		GlobalUnlock (hbi);
		return TRUE;
	}
	return FALSE;
}

#pragma warning (disable : 6001)
HANDLE CNDib::OpenDIB(LPCSTR szFile)
{
	unsigned            fh{};
	BITMAPINFOHEADER    bi{};
	LPBITMAPINFOHEADER  lpbi{};
	DWORD               dwLen = 0;
	DWORD               dwBits{};
	HANDLE              hdib{};
	HANDLE              h{};
	OFSTRUCT            of{};

	/* Open the file and read the DIB information */
	fh = OpenFile(szFile, &of, OF_READ);
	if (fh == -1)	return NULL;

	hdib = ReadDibBitmapInfo(fh);
	if (!hdib)	return NULL;
	DibInfo(hdib,&bi);

	/* Calculate the memory needed to hold the DIB */
	dwBits = bi.biSizeImage;
	dwLen  = bi.biSize + (DWORD)PaletteSize (&bi) + dwBits;

	/* Try to increase the size of the bitmap info. buffer to hold the DIB */
	h = GlobalReAlloc(hdib, dwLen, GHND);
	if (!h)
	{
		GlobalFree(hdib);
		hdib = NULL;
	}
	else	hdib = h;

	/* Read in the bits */
	if (hdib)
	{
		lpbi = (LPBITMAPINFOHEADER)GlobalLock(hdib);
		lread(fh, (LPSTR)lpbi + (WORD)lpbi->biSize + PaletteSize(lpbi), dwBits);
		GlobalUnlock(hdib);
	}
	_lclose(fh);
	return hdib;
}
#pragma warning (default : 6001)

WORD CNDib::PaletteSize (VOID FAR *pv)
{
	LPBITMAPINFOHEADER lpbi{};
	WORD               NumColors{};

	lpbi      = (LPBITMAPINFOHEADER)pv;
	NumColors = DibNumColors(lpbi);

	if (lpbi->biSize == sizeof(BITMAPCOREHEADER))
		return NumColors * sizeof(RGBTRIPLE);
	else	return NumColors * sizeof(RGBQUAD);
}

WORD CNDib::DibNumColors (VOID FAR *pv)
{
	int                 bits{};
	LPBITMAPINFOHEADER  lpbi{};
	LPBITMAPCOREHEADER  lpbc{};

	lpbi = ((LPBITMAPINFOHEADER)pv);
	lpbc = ((LPBITMAPCOREHEADER)pv);

	/*  With the BITMAPINFO format headers, the size of the palette
	*  is in biClrUsed, whereas in the BITMAPCORE - style headers, it
	*  is dependent on the bits per pixel ( = 2 raised to the power of
	*  bits/pixel).
	*/
	if (lpbi->biSize != sizeof(BITMAPCOREHEADER))
	{
		if (lpbi->biClrUsed != 0)
			return (WORD)lpbi->biClrUsed;
		bits = lpbi->biBitCount;
	}
	else	bits = lpbc->bcBitCount;

	switch (bits)
	{
	case 1:	return 2;
	case 4:	return 16;
	case 8:	return 256;
	default:
		/* A 24 bitcount DIB has no color table */
		return 0;
	}
}

DWORD CNDib::lread (int fh,VOID far * pv,DWORD  ul)
{
	const DWORD     ulT = ul;
	BYTE  *hp = (unsigned char  *)pv;

	while (ul > (DWORD)MAXREAD)
	{
		if (_lread(fh, (LPSTR)hp, (WORD)MAXREAD) != MAXREAD)
			return 0;
		ul -= MAXREAD;
		hp += MAXREAD;
	}
	if (_lread(fh, (LPSTR)hp, (WORD)ul) != (WORD)ul)
		return 0;
	return ulT;
}

BOOL CNDib::SetNDIB(CString file)
{
	if (InitNDIB(file))	return FALSE;
	return TRUE;
}

HANDLE CNDib::GetNDIB()
{
	return m_dib;
}

HANDLE CNDib::GetNBI()
{
	return m_bi;
}

#pragma warning (default : 6011)
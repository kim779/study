#include "stdafx.h"
#include "bitmap.h"
#include "palette.h"


#define PADWIDTH(x)	(((x)*8 + 31)  & (~31))/8

CExBitmap::CExBitmap()
	: m_pInfo(0)
	, m_pPixels(0)
	, m_pPal(0)
	, m_bIsPadded(FALSE)
{
}

CExBitmap::~CExBitmap()
{
	delete[] (BYTE *)m_pInfo;
	delete[] m_pPixels;
	delete m_pPal;
}

BITMAPINFO* CExBitmap::GetHeaderPtr() const 
{
	ASSERT(m_pInfo);
	ASSERT(m_pPixels);
	return m_pInfo;
}

BYTE* CExBitmap::GetPixelPtr() const
{
	ASSERT(m_pInfo);
	ASSERT(m_pPixels);
	return m_pPixels;
}

RGBQUAD* CExBitmap::GetColorTablePtr() const
{
	ASSERT(m_pInfo);
	ASSERT(m_pPixels);
	RGBQUAD* pColorTable = 0;
	if (m_pInfo != 0)
	{
		int cOffset = sizeof(BITMAPINFOHEADER);
		pColorTable = (RGBQUAD *)(((BYTE *)(m_pInfo)) + cOffset);
	}
	return pColorTable;
}

int CExBitmap::GetWidth() const
{
	ASSERT(m_pInfo);
	return m_pInfo->bmiHeader.biWidth;
}

int CExBitmap::GetHeight() const
{
	ASSERT(m_pInfo);
	return m_pInfo->bmiHeader.biHeight;
}

void CExBitmap::GetSize(int& width, int& height)
{
	ASSERT(m_pInfo);
	width = m_pInfo->bmiHeader.biWidth;
	height = m_pInfo->bmiHeader.biHeight;
}

BOOL CExBitmap::CreateFromBitmap(CDC* pDC, CBitmap* pSrcBitmap)
{
	ASSERT_VALID(pSrcBitmap);
	ASSERT_VALID(pDC);

	try {
		BITMAP bmHdr;

		pSrcBitmap->GetObject(sizeof(BITMAP), &bmHdr);

		if (m_pPixels)
		{
			delete [] m_pPixels;
			m_pPixels = 0;
		}

		DWORD dwWidth{};
		if (bmHdr.bmBitsPixel > 8)
			dwWidth = PADWIDTH(bmHdr.bmWidth * 3);
		else
			dwWidth = PADWIDTH(bmHdr.bmWidth);

		m_pPixels = new BYTE[dwWidth*bmHdr.bmHeight];
		if (!m_pPixels)
			throw TEXT("could not allocate data storage\n");

		WORD wColors{};
		switch (bmHdr.bmBitsPixel)
		{
			case 1 : 
				wColors = 2;
				break;
			case 4 :
				wColors = 16;
				break;
			case 8 :
				wColors = 256;
				break;
			default :
				wColors = 0;
				break;
		}

		if (m_pInfo)
		{
			delete [] (BYTE*)m_pInfo;
			m_pInfo = 0;
		}

		m_pInfo = (BITMAPINFO *)new BYTE[sizeof(BITMAPINFOHEADER) + wColors*sizeof(RGBQUAD)];
		if (!m_pInfo)
			throw TEXT("could not allocate BITMAPINFO struct\n");

		m_pInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		m_pInfo->bmiHeader.biWidth = bmHdr.bmWidth;
		m_pInfo->bmiHeader.biHeight = bmHdr.bmHeight;
		m_pInfo->bmiHeader.biPlanes = bmHdr.bmPlanes;
		
		if( bmHdr.bmBitsPixel > 8 )
			m_pInfo->bmiHeader.biBitCount = 24;
		else
			m_pInfo->bmiHeader.biBitCount = bmHdr.bmBitsPixel;

		m_pInfo->bmiHeader.biCompression = BI_RGB;
		m_pInfo->bmiHeader.biSizeImage = ((((bmHdr.bmWidth * bmHdr.bmBitsPixel) + 31) & ~31) >> 3) * bmHdr.bmHeight;
		m_pInfo->bmiHeader.biXPelsPerMeter = 0;
		m_pInfo->bmiHeader.biYPelsPerMeter = 0;
		m_pInfo->bmiHeader.biClrUsed = 0;
		m_pInfo->bmiHeader.biClrImportant = 0;

		const int test = ::GetDIBits(pDC->GetSafeHdc(), (HBITMAP)pSrcBitmap->GetSafeHandle(),
	 		0, (WORD)bmHdr.bmHeight, m_pPixels, m_pInfo, DIB_RGB_COLORS);

		if (test != (int)bmHdr.bmHeight)
			throw TEXT("call to GetDIBits did not return full number of requested scan lines\n");
		
		CreatePalette();
		m_bIsPadded = FALSE;
#ifdef _DEBUG
	} catch( TCHAR * psz ) {
		TRACE1("CExBitmap::CreateFromBitmap(): %s\n", psz);
#else
	} catch( TCHAR * ) {
#endif
		if (m_pPixels)
		{
			delete [] m_pPixels;
			m_pPixels = 0;
		}
		if (m_pInfo)
		{
			delete [] (BYTE*) m_pInfo;
			m_pInfo = 0;
		}
		return FALSE;
	}
	return TRUE;
}

void CExBitmap::DestroyBitmap()
{
	delete[] (BYTE *)m_pInfo;
	delete[] m_pPixels;
	delete	m_pPal;
	m_pInfo = 0;
	m_pPixels = 0;
	m_pPal = 0;
}

BOOL CExBitmap::Load(HBITMAP& hBitmap)
{
	HBITMAP hBmp = hBitmap;
	if (hBmp == 0) 
		return FALSE;

	CBitmap bmp;
	bmp.Attach(hBmp);
	CClientDC cdc(CWnd::GetDesktopWindow());
	const BOOL bRet = CreateFromBitmap(&cdc, &bmp);
	bmp.DeleteObject();
	return bRet;
	
}

BOOL CExBitmap::Load(const CString& strFilename)
{

	CFile file;
	if (file.Open(strFilename, CFile::modeRead))
		return Load(&file);
	
	return FALSE;
}

BOOL CExBitmap::Load(CFile* pFile)
{
	ASSERT(pFile);
	BOOL fReturn = TRUE;
	try 
	{
		delete[] (BYTE *)m_pInfo;
		delete[] m_pPixels;
		m_pInfo = 0;
		m_pPixels = 0;
		const DWORD dwStart = (DWORD)pFile->GetPosition(); //vc2019
		
		BITMAPFILEHEADER fileHeader;
		pFile->Read(&fileHeader, sizeof(fileHeader));
		if (fileHeader.bfType != 0x4D42)
		    throw TEXT("Error:Unexpected file type, not a DIB\n");

		BITMAPINFOHEADER infoHeader;
		pFile->Read(&infoHeader, sizeof(infoHeader));
		if (infoHeader.biSize != sizeof(infoHeader))
		    throw TEXT("Error:OS2 PM BMP Format not supported\n");

		const int cPaletteEntries = GetPalEntries(infoHeader);
		const int cColorTable = 256 * sizeof(RGBQUAD);
		const int cInfo = sizeof(BITMAPINFOHEADER) + cColorTable;
		const int cPixels = fileHeader.bfSize - fileHeader.bfOffBits;
		
		m_pInfo = (BITMAPINFO *)new BYTE[cInfo];
		memcpy(m_pInfo, &infoHeader, sizeof(BITMAPINFOHEADER));
		pFile->Read(((BYTE*)m_pInfo) + sizeof(BITMAPINFOHEADER), cColorTable);
		
		m_pPixels = new BYTE[cPixels];
		pFile->Seek(dwStart + fileHeader.bfOffBits, CFile::begin);
		pFile->Read(m_pPixels, cPixels);
		CreatePalette();
		m_bIsPadded = TRUE;
#ifdef _DEBUG
	}
	catch (TCHAR* psz)
	{
		TRACE(psz);
#else
	}
	catch (TCHAR *)
	{
#endif
		fReturn = FALSE;
	}
	return fReturn;
}

void CExBitmap::DrawDIB(CDC* pDC, int x, int y)
{
	DrawDIB(pDC, x, y, GetWidth(), GetHeight());
}


void CExBitmap::DrawDIB(CDC* pDC, int x, int y, int width, int height)
{
	ASSERT( pDC );
	HDC     hdc = pDC->GetSafeHdc();

	CPalette * pOldPal = 0;

	if (m_pPal)
	{
		pOldPal = pDC->SelectPalette(m_pPal, FALSE);
		pDC->RealizePalette();
		pDC->SetStretchBltMode(COLORONCOLOR);
	}

	if (m_pInfo)
//		StretchDIBits( hdc,
//		       x,
//		       y,
//		       width,
//		       height,
//		       0,
//		       0,
//		       GetWidth(),
//		       GetHeight(),
//		       GetPixelPtr(),
//		       GetHeaderPtr(),
//		       DIB_RGB_COLORS,
//		       SRCCOPY );
		StretchDIBits( hdc,
		       x,
		       y,
		       GetWidth(),
		       GetHeight(),
		       0,
		       0,
		       GetWidth(),
		       GetHeight(),
		       GetPixelPtr(),
		       GetHeaderPtr(),
		       DIB_RGB_COLORS,
		       SRCCOPY );

	if (m_pPal)
		pDC->SelectPalette(pOldPal, FALSE);
}

BOOL CExBitmap::CreatePalette()
{
	if (m_pPal)
		delete m_pPal;
	m_pPal = 0;
	ASSERT(m_pInfo);
	
	if (m_pInfo->bmiHeader.biBitCount <= 8)
		m_pPal = new CExPalette(this);
	return m_pPal ? TRUE : FALSE;
}

void CExBitmap::ClearPalette()
{
	if (m_pPal)
		delete m_pPal;
	m_pPal = 0;
}

int CExBitmap::GetPalEntries() const
{
	ASSERT(m_pInfo);
	return GetPalEntries(*(BITMAPINFOHEADER*)m_pInfo);
}

int CExBitmap::GetPalEntries(const BITMAPINFOHEADER& infoHeader) const
{
	int nReturn{};
	if (infoHeader.biClrUsed == 0)
		nReturn = (1 << infoHeader.biBitCount);
	else
		nReturn = infoHeader.biClrUsed;

	return nReturn;
}


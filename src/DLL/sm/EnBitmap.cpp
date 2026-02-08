// EnBitmap.cpp: implementation of the CEnBitmap class (c) daniel godson 2002.
//
// credits: Peter Hendrix's CPicture implementation for the original IPicture code 
//          Yves Maurer's GDIRotate implementation for the idea of working directly on 32 bit representations of bitmaps 
//          Karl Lager's 'A Fast Algorithm for Rotating Bitmaps' 
// 
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EnBitmap.h"
#include "imageprocessors.h"

#include <afxpriv.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const int HIMETRIC_INCH	= 2540;

enum 
{
	FT_BMP,
	FT_ICO,
	FT_JPG,
	FT_GIF,

	FT_UNKNOWN
};

///////////////////////////////////////////////////////////////////////

C32BitImageProcessor::C32BitImageProcessor(BOOL bEnableWeighting) : m_bWeightingEnabled(bEnableWeighting)
{
}

C32BitImageProcessor::~C32BitImageProcessor()
{
}

CSize C32BitImageProcessor::CalcDestSize(CSize sizeSrc) 
{ 
	return sizeSrc; // default
}

BOOL C32BitImageProcessor::ProcessPixels(RGBX* pSrcPixels, CSize sizeSrc, RGBX* pDestPixels, CSize sizeDest)
{ 
	CopyMemory(pDestPixels, pSrcPixels, sizeDest.cx * 4 * sizeDest.cy); // default
	return TRUE;
}
 
// C32BitImageProcessor::CalcWeightedColor(...) is inlined in EnBitmap.h

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEnBitmap::CEnBitmap(COLORREF crBkgnd) : m_crBkgnd(crBkgnd)
{

}

CEnBitmap::~CEnBitmap()
{

}

BOOL CEnBitmap::LoadImage(UINT uIDRes, LPCTSTR szResourceType, HMODULE hInst, COLORREF crBack)
{
	ASSERT(m_hObject == NULL);      // only attach once, detach on destroy

	if (m_hObject != NULL)
		return FALSE;

	return Attach(LoadImageResource(uIDRes, szResourceType, hInst, crBack == -1 ? m_crBkgnd : crBack));
}

BOOL CEnBitmap::LoadImage(LPCTSTR szImagePath, COLORREF crBack)
{
	ASSERT(m_hObject == NULL);      // only attach once, detach on destroy

	if (m_hObject != NULL)
		return FALSE;

	return Attach(LoadImageFile(szImagePath, crBack == -1 ? m_crBkgnd : crBack));
}

HBITMAP CEnBitmap::LoadImageFile(LPCTSTR szImagePath, COLORREF crBack)
{
	const int nType = GetFileType(szImagePath);
	CString slog;
	HBITMAP hbit;
	switch (nType)
	{
		case FT_BMP:
			// the reason for this is that i suspect it is more efficient to load
			// bmps this way since it avoids creating device contexts etc that the 
			// IPicture methods requires. that method however is still valuable
			// since it handles other image types and transparency
			/*hbit = (HBITMAP)::LoadImage(NULL, szImagePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			slog.Format("%d [%x]", GetLastError(), hbit);
			AfxMessageBox(slog);
			return hbit;*/
			return (HBITMAP)::LoadImage(NULL, szImagePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		case FT_UNKNOWN:
			return NULL;

		default: // all the rest
		{
#pragma warning(disable : 26494)
#pragma warning(disable : 26496)
			USES_CONVERSION;
#pragma warning(default : 26494)
#pragma warning(default : 26496)
			IPicture* pPicture = NULL;
			
			HBITMAP hbm = NULL;
			//HRESULT hr = OleLoadPicturePath(T2OLE(szImagePath), NULL, 0, crBack, IID_IPicture, (LPVOID*)&pPicture);
					
			if (pPicture)
			{
				hbm = ExtractBitmap(pPicture, crBack);
				pPicture->Release();
			}

			return hbm;
		}
	}

	return NULL; // can't get here
}

HBITMAP CEnBitmap::LoadImageResource(UINT uIDRes, LPCTSTR szResourceType, HMODULE hInst, COLORREF crBack)
{
#ifdef 	DF_USESTL_ENBITMAP
	std::unique_ptr<BYTE[]> pBuff;
#else
	BYTE* pBuff = NULL;
#endif

	int nSize = 0;
	HBITMAP hbm = NULL;

	// first call is to get buffer size
	if (GetResource(MAKEINTRESOURCE(uIDRes), szResourceType, hInst, 0, nSize))
	{
		if (nSize > 0)
		{
#ifdef DF_USESTL_ENBITMAP
			pBuff = std::make_unique<BYTE[]>(nSize);
#else 
			pBuff = new BYTE[nSize];
#endif
			
			
			// this loads it
#ifdef DF_USESTL_ENBITMAP
			if (GetResource(MAKEINTRESOURCE(uIDRes), szResourceType, hInst, pBuff.get(), nSize))
#else 
			if (GetResource(MAKEINTRESOURCE(uIDRes), szResourceType, hInst, pBuff, nSize))
#endif
			{
#ifdef DF_USESTL_ENBITMAP
				IPicture* pPicture = LoadFromBuffer(pBuff.get(), nSize);
#else 
				IPicture* pPicture = LoadFromBuffer(pBuff, nSize);
#endif
				
				if (pPicture)
				{
					hbm = ExtractBitmap(pPicture, crBack);
					pPicture->Release();
				}
			}

#ifndef DF_USESTL_ENBITMAP
			delete[] pBuff;
#endif			
			
		}
	}

	return hbm;
}

IPicture* CEnBitmap::LoadFromBuffer(BYTE* pBuff, int nSize)
{
	//bool bResult = false;
#ifdef DF_USESTL_ENBITMAP
	const gsl::not_null<HGLOBAL> hGlobal = GlobalAlloc(GMEM_MOVEABLE, nSize);
	const gsl::not_null<void*> pData = GlobalLock(hGlobal);
#else 
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, nSize);
	void* pData = GlobalLock(hGlobal);
#endif
	
	memcpy(pData, pBuff, nSize);
	GlobalUnlock(hGlobal);

	IStream* pStream = NULL;
	IPicture* pPicture = NULL;

	if (CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) == S_OK)
	{
		const HRESULT hr = OleLoadPicture(pStream, nSize, FALSE, IID_IPicture, (LPVOID *)&pPicture);
		pStream->Release();
	}

	return pPicture; // caller releases
}

BOOL CEnBitmap::GetResource(LPCTSTR lpName, LPCTSTR lpType, HMODULE hInst, void* pResource, int& nBufSize)
{ 
	HRSRC		hResInfo{};
	HANDLE		hRes{};
	LPSTR		lpRes	= NULL; 
	const int			nLen	= 0;
	bool		bResult	= FALSE;

	// Find the resource
	hResInfo = FindResource(hInst, lpName, lpType);

	if (hResInfo == NULL) 
		return false;

	// Load the resource
	hRes = LoadResource(hInst, hResInfo);

	if (hRes == NULL) 
		return false;

	// Lock the resource
	lpRes = (char*)LockResource(hRes);

	if (lpRes != NULL)
	{ 
		if (pResource == NULL)
		{
			nBufSize = SizeofResource(hInst, hResInfo);
			bResult = true;
		}
		else
		{
			if (nBufSize >= (int)SizeofResource(hInst, hResInfo))
			{
				memcpy(pResource, lpRes, nBufSize);
				bResult = true;
			}
		} 

		UnlockResource(hRes);  
	}

	// Free the resource
	FreeResource(hRes);

	return bResult;
}

HBITMAP CEnBitmap::ExtractBitmap(IPicture* pPicture, COLORREF crBack)
{
	ASSERT(pPicture);

	if (!pPicture)
		return NULL;

	CBitmap bmMem;
	CDC dcMem;
	CDC* pDC = CWnd::GetDesktopWindow()->GetDC();

	if (dcMem.CreateCompatibleDC(pDC))
	{
		long hmWidth;
		long hmHeight;

		pPicture->get_Width(&hmWidth);
		pPicture->get_Height(&hmHeight);
		
		const int nWidth	= MulDiv(hmWidth, pDC->GetDeviceCaps(LOGPIXELSX), HIMETRIC_INCH);
		const int nHeight	= MulDiv(hmHeight, pDC->GetDeviceCaps(LOGPIXELSY), HIMETRIC_INCH);

		if (bmMem.CreateCompatibleBitmap(pDC, nWidth, nHeight))
		{
			CBitmap* pOldBM = dcMem.SelectObject(&bmMem);

			if (crBack != -1)
				dcMem.FillSolidRect(0, 0, nWidth, nHeight, crBack);
			
			const HRESULT hr = pPicture->Render(dcMem, 0, 0, nWidth, nHeight, 0, hmHeight, hmWidth, -hmHeight, CRect(0, 0, nWidth, nHeight));
			dcMem.SelectObject(pOldBM);
			dcMem.DeleteDC();
		}
	}

	CWnd::GetDesktopWindow()->ReleaseDC(pDC);

	return (HBITMAP)bmMem.Detach();
}

int CEnBitmap::GetFileType(LPCTSTR szImagePath)
{
	CString sPath(szImagePath);
	sPath.MakeUpper();

	if (sPath.Find(".BMP") > 0)
		return FT_BMP;

	else if (sPath.Find(".ICO") > 0)
		return FT_ICO;

	else if (sPath.Find(".JPG") > 0 || sPath.Find(".JPEG") > 0)
		return FT_JPG;

	else if (sPath.Find(".GIF") > 0)
		return FT_GIF;

	// else
	return FT_UNKNOWN;
}

BOOL CEnBitmap::RotateImage(int nDegrees, BOOL bEnableWeighting)
{
	return ProcessImage(&CImageRotator(nDegrees, bEnableWeighting));
}

BOOL CEnBitmap::ShearImage(int nHorz, int nVert, BOOL bEnableWeighting)
{
	return ProcessImage(&CImageShearer(nHorz, nVert, bEnableWeighting));
}

BOOL CEnBitmap::GrayImage()
{
	return ProcessImage(&CImageGrayer());
}

BOOL CEnBitmap::BlurImage(int nAmount)
{
	return ProcessImage(&CImageBlurrer(nAmount));
}

BOOL CEnBitmap::SharpenImage(int nAmount)
{
	return ProcessImage(&CImageSharpener(nAmount));
}

BOOL CEnBitmap::ResizeImage(double dFactor)
{
	return ProcessImage(&CImageResizer(dFactor));
}

BOOL CEnBitmap::FlipImage(BOOL bHorz, BOOL bVert)
{
	return ProcessImage(&CImageFlipper(bHorz, bVert));
}

BOOL CEnBitmap::NegateImage()
{
	return ProcessImage(&CImageNegator());
}

BOOL CEnBitmap::ReplaceColor(COLORREF crFrom, COLORREF crTo)
{
	return ProcessImage(&CColorReplacer(crFrom, crTo));
}

BOOL CEnBitmap::ProcessImage(C32BitImageProcessor* pProcessor)
{
	C32BIPArray aProcessors;

	aProcessors.Add(pProcessor);

	return ProcessImage(aProcessors);
}

BOOL CEnBitmap::ProcessImage(C32BIPArray& aProcessors)
{
	ASSERT (GetSafeHandle());

	if (!GetSafeHandle())
		return FALSE;

	if (!aProcessors.GetSize())
		return TRUE;

	int nProcessor{};
	const int nCount = aProcessors.GetSize();

	// retrieve src and final dest sizes
	BITMAP BM;

	if (!GetBitmap(&BM))
		return FALSE;

	CSize sizeSrc(BM.bmWidth, BM.bmHeight);
	CSize sizeDest(sizeSrc), sizeMax(sizeSrc);

	for (nProcessor = 0; nProcessor < nCount; nProcessor++)
	{
		sizeDest = aProcessors[nProcessor]->CalcDestSize(sizeDest);
		sizeMax = CSize(max(sizeMax.cx, sizeDest.cx), max(sizeMax.cy, sizeDest.cy));
	}

	// prepare src and dest bits

#ifdef DF_USESTL_ENBITMAP
	std::unique_ptr<RGBX[]> pSrcPixels{};
	pSrcPixels.reset(GetDIBits32());
#else 
	RGBX* pSrcPixels = GetDIBits32();
#endif
	

	if (!pSrcPixels)
		return FALSE;

#ifdef DF_USESTL_ENBITMAP
	std::unique_ptr<RGBX[]> pDestPixels = std::make_unique<RGBX[]>(sizeMax.cx * sizeMax.cy);
#else 
	RGBX* pDestPixels = new RGBX[sizeMax.cx * sizeMax.cy];
#endif
	

	if (!pDestPixels)
		return FALSE;

#ifdef DF_USESTL_ENBITMAP
	Fill(pDestPixels.get(), sizeMax, m_crBkgnd);
#else 
	Fill(pDestPixels, sizeMax, m_crBkgnd);
#endif
	

	BOOL bRes = TRUE;
	sizeDest = sizeSrc;

	// do the processing
	for (nProcessor = 0; bRes && nProcessor < nCount; nProcessor++)
	{
		// if its the second processor or later then we need to copy
		// the previous dest bits back into source.
		// we also need to check that sizeSrc is big enough
		if (nProcessor > 0)
		{
			if (sizeSrc.cx < sizeDest.cx || sizeSrc.cy < sizeDest.cy)
			{
#ifdef DF_USESTL_ENBITMAP
				pSrcPixels.reset();
				pSrcPixels = std::make_unique<RGBX[]>(sizeDest.cx * sizeDest.cy);
#else	
				delete[] pSrcPixels;
				pSrcPixels = new RGBX[sizeDest.cx * sizeDest.cy];
#endif
				
			}
#ifdef DF_USESTL_ENBITMAP
			CopyMemory(pSrcPixels.get(), pDestPixels.get(), sizeDest.cx * 4 * sizeDest.cy); // default
			Fill(pDestPixels.get(), sizeDest, m_crBkgnd);
#else 
			CopyMemory(pSrcPixels, pDestPixels, sizeDest.cx * 4 * sizeDest.cy); // default
			Fill(pDestPixels, sizeDest, m_crBkgnd);
#endif
		
		}

		sizeSrc = sizeDest;
		sizeDest = aProcessors[nProcessor]->CalcDestSize(sizeSrc);
	
#ifdef DF_USESTL_ENBITMAP
		bRes = aProcessors[nProcessor]->ProcessPixels(pSrcPixels.get(), sizeSrc, pDestPixels.get(), sizeDest);
#else 
		bRes = aProcessors[nProcessor]->ProcessPixels(pSrcPixels, sizeSrc, pDestPixels, sizeDest);
#endif
		
	}

	// update the bitmap
	if (bRes)
	{
		// set the bits
		HDC hdc = GetDC(NULL);
		HBITMAP hbmSrc = ::CreateCompatibleBitmap(hdc, sizeDest.cx, sizeDest.cy);

		if (hbmSrc)
		{
			BITMAPINFO bi;

			if (PrepareBitmapInfo32(bi, hbmSrc))
			{

#ifdef DF_USESTL_ENBITMAP
				if (SetDIBits(hdc, hbmSrc, 0, sizeDest.cy, pDestPixels.get(), &bi, DIB_RGB_COLORS))
#else 
				if (SetDIBits(hdc, hbmSrc, 0, sizeDest.cy, pDestPixels, &bi, DIB_RGB_COLORS))
#endif
				
				{
					// delete the bitmap and attach new
					DeleteObject();
					bRes = Attach(hbmSrc);
				}
			}

			::ReleaseDC(NULL, hdc);

			if (!bRes)
				::DeleteObject(hbmSrc);
		}
	}

#ifndef DF_USESTL_ENBITMAP
	delete[] pSrcPixels;
	delete[] pDestPixels;
#endif
	
	return bRes;
}

RGBX* CEnBitmap::GetDIBits32()
{
	BITMAPINFO bi;

	const int nHeight = PrepareBitmapInfo32(bi);
	
	if (!nHeight)
		return FALSE;

#ifdef DF_USESTL_ENBITMAP
	std::unique_ptr<BYTE[]>pBits = std::make_unique<BYTE[]>(bi.bmiHeader.biSizeImage);
#else 
	BYTE* pBits = (BYTE*)new BYTE[bi.bmiHeader.biSizeImage];
#endif
	HDC hdc = GetDC(NULL);

#ifdef DF_USESTL_ENBITMAP
	if (!GetDIBits(hdc, (HBITMAP)GetSafeHandle(), 0, nHeight, pBits.get(), &bi, DIB_RGB_COLORS))
#else 
	if (!GetDIBits(hdc, (HBITMAP)GetSafeHandle(), 0, nHeight, pBits, &bi, DIB_RGB_COLORS))
#endif
	{
#ifndef DF_USESTL_ENBITMAP
		delete pBits;
		pBits = NULL;
#endif
	}

	::ReleaseDC(NULL, hdc);

#ifdef DF_USESTL_ENBITMAP
	return (RGBX*)pBits.get();
#else 
	return (RGBX*)pBits;
#endif

}

BOOL CEnBitmap::PrepareBitmapInfo32(BITMAPINFO& bi, HBITMAP hBitmap)
{
	if (!hBitmap)
		hBitmap = (HBITMAP)GetSafeHandle();

	BITMAP BM;

	if (!::GetObject(hBitmap, sizeof(BM), &BM))
		return FALSE;

	bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
	bi.bmiHeader.biWidth = BM.bmWidth;
	bi.bmiHeader.biHeight = -BM.bmHeight;
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 32; // 32 bit
	bi.bmiHeader.biCompression = BI_RGB; // 32 bit
	bi.bmiHeader.biSizeImage = BM.bmWidth * 4 * BM.bmHeight; // 32 bit
	bi.bmiHeader.biClrUsed = 0;
	bi.bmiHeader.biClrImportant = 0;

	return BM.bmHeight;
}

BOOL CEnBitmap::CopyImage(HBITMAP hBitmap)
{
	ASSERT (hBitmap);
	
	if (!hBitmap)
		return FALSE;
	
	BITMAPINFO bi;
	const int nHeight = PrepareBitmapInfo32(bi, hBitmap);

	if (!nHeight)
		return FALSE;


#ifdef DF_USESTL_ENBITMAP
	std::unique_ptr<BYTE[]> pBits = std::make_unique<BYTE[]>(bi.bmiHeader.biSizeImage);
#else 
	BYTE* pBits = (BYTE*)new BYTE[bi.bmiHeader.biSizeImage];
#endif
	
	HDC hdc = GetDC(NULL);
	BOOL bRes = FALSE;


#ifdef DF_USESTL_ENBITMAP
	if (GetDIBits(hdc, hBitmap, 0, nHeight, pBits.get(), &bi, DIB_RGB_COLORS))
	{
		const int nWidth = bi.bmiHeader.biSizeImage / (nHeight * 4);

		HBITMAP hbmDest = ::CreateCompatibleBitmap(hdc, nWidth, nHeight);

		if (hbmDest)
		{
			if (SetDIBits(hdc, hbmDest, 0, nHeight, pBits.get(), &bi, DIB_RGB_COLORS))
			{
				DeleteObject();
				bRes = Attach(hbmDest);
			}
		}
}
#else 
	if (GetDIBits(hdc, hBitmap, 0, nHeight, pBits, &bi, DIB_RGB_COLORS))
	{
		const int nWidth = bi.bmiHeader.biSizeImage / (nHeight * 4);

		HBITMAP hbmDest = ::CreateCompatibleBitmap(hdc, nWidth, nHeight);

		if (hbmDest)
		{
			if (SetDIBits(hdc, hbmDest, 0, nHeight, pBits, &bi, DIB_RGB_COLORS))
			{
				DeleteObject();
				bRes = Attach(hbmDest);
			}
		}
	}
#endif


	::ReleaseDC(NULL, hdc);

#ifndef DF_USESTL_ENBITMAP
	delete [] pBits;
#endif

	return bRes;
}

BOOL CEnBitmap::CopyImage(CBitmap* pBitmap)
{
	if (!pBitmap)
		return FALSE;

	return CopyImage((HBITMAP)pBitmap->GetSafeHandle());
}

BOOL CEnBitmap::Fill(RGBX* pPixels, CSize size, COLORREF color)
{
	if (!pPixels)
		return FALSE;

	if (color == -1 || color == RGB(255, 255, 255))
		FillMemory(pPixels, size.cx * 4 * size.cy, 255); // white

	else if (color == 0)
		FillMemory(pPixels, size.cx * 4 * size.cy, 0); // black

	else
	{
		// fill the first line with the color
		RGBX* pLine = &pPixels[0];
		int nSize = 1;

		pLine[0] = RGBX(color);

		while (1)
		{
			if (nSize > size.cx)
				break;

			// else
			const int nAmount = min(size.cx - nSize, nSize) * 4;

			CopyMemory(&pLine[nSize], pLine, nAmount);
			nSize *= 2;
		}

		// use that line to fill the rest of the block
		int nRow = 1;

		while (1)
		{
			if (nRow > size.cy)
				break;

			// else
			const int nAmount = min(size.cy - nRow, nRow) * size.cx * 4;

			CopyMemory(&pPixels[nRow * size.cx], pPixels, nAmount);
			nRow *= 2;
		}
	}

	return TRUE;
}

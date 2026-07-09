// MyBitmap.cpp: implementation of the CMyBitmap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyBitmap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyBitmap::CMyBitmap()
{
	m_nWidth = 0;
	m_nHeight = 0;
}

CMyBitmap::~CMyBitmap()
{
	DeleteObject();
}

BOOL CMyBitmap::LoadBitmap(UINT nIDResource)
{
	BOOL iResult = CBitmap::LoadBitmap(nIDResource);
	
	BITMAP bmp;
	
	GetBitmap(&bmp);
	
	if(iResult)
	{
		m_nHeight = bmp.bmHeight;
		m_nWidth = bmp.bmWidth;
	}

	return iResult;
}

BOOL CMyBitmap::LoadBitmap(CString strFilename)						// 파일에서 불러옴 
{
	HBITMAP hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), strFilename, IMAGE_BITMAP,
									0, 0, LR_CREATEDIBSECTION|LR_LOADFROMFILE);
	BITMAP bmp;

		if(GetSafeHandle())
		{
			DeleteObject();
			Detach();
		}

		Attach(hBitmap);

		this->GetBitmap(&bmp);

		m_nHeight = bmp.bmHeight;
		m_nWidth = bmp.bmWidth;

		::DeleteObject(hBitmap);
		return TRUE;
}

void CMyBitmap::Draw(CDC *pDC, int x, int y)
{
	CDC MemDC;
	CBitmap	*poldbitmap;
		
		MemDC.CreateCompatibleDC(pDC);

		poldbitmap = (CBitmap*)MemDC.SelectObject(this);

		pDC->BitBlt(x, y, m_nWidth, m_nHeight, &MemDC, 0, 0, SRCCOPY);

		MemDC.SelectObject(poldbitmap);
		
		DeleteDC(MemDC);
}

void CMyBitmap::Draw(CDC *pDC, int x, int y, int w, int h, POINT p, DWORD dwRop)
{
	Draw(pDC, x, y, w, h, p.x, p.y, dwRop);
}	

void CMyBitmap::Draw(CDC *pDC, int x, int y, int w, int h, int srcX, int srcY, DWORD dwRop)
{
	CDC MemDC;
	CBitmap *poldbitmap;

		MemDC.CreateCompatibleDC(pDC);
		poldbitmap = (CBitmap*)MemDC.SelectObject(this);

		pDC->BitBlt(x, y, w, h, &MemDC, srcX, srcY, dwRop);
		
		MemDC.SelectObject(poldbitmap);
		DeleteDC(MemDC);
	
}

void CMyBitmap::Draw(CDC *pDC, int x, int y, int srcX, int srcY, int esrcX, int esrcY)
{
	CDC MemDC;
	CBitmap *poldbitmap;
		
		MemDC.CreateCompatibleDC(pDC);

		poldbitmap = (CBitmap*)MemDC.SelectObject(this);

		pDC->StretchBlt(x, y, 20, 20, &MemDC,
			srcX, srcY, esrcX, esrcY, SRCCOPY);
		
		MemDC.SelectObject(poldbitmap);
		DeleteDC(MemDC);		
}

void CMyBitmap::Draw(CDC *pDC, RECT dest)
{
	CDC MemDC;
	CBitmap *poldbitmap;
		
		MemDC.CreateCompatibleDC(pDC);

		poldbitmap = (CBitmap*)MemDC.SelectObject(this);

		pDC->StretchBlt(dest.left, dest.top, dest.right - dest.left, dest.bottom - dest.top, &MemDC,
			0, 0, m_nWidth, m_nHeight, SRCCOPY);
		
		MemDC.SelectObject(poldbitmap);
		MemDC.DeleteDC();
}

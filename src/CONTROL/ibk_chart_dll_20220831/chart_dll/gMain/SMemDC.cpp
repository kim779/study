// FBMemDC.cpp: implementation of the SMemDC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <math.h>

#include "SMemDC.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// IMPLEMENT_DYNAMIC(SMemDC, CDC);

//////////////////////////////////////////////////////////////////////
// SMemDC - implementation

SMemDC::SMemDC(CDC* pDC, BOOL bMap)
{
	ASSERT(pDC != NULL);
	
	m_Width = 0;
	m_Height = 0;

	m_pBitmap = m_pOldBitmap = NULL;

	CWnd *pWnd = CWnd::GetDesktopWindow();
	pWnd->GetWindowRect(&m_Rect);

	CreateCompatibleDC(pDC);
	SetMapMode(pDC->GetMapMode());

	m_Width = 1;
	m_Height = 1;
	m_pBitmap = new CBitmap;
	BOOL	res = m_pBitmap->CreateCompatibleBitmap(pDC, m_Width, m_Height);
	if(!res)
	{
		m_bValid = false;
		TRACE("Bitmap Create error.\n");
	}
	else
	{		
		m_pOldBitmap = SelectObject(m_pBitmap);
		m_bValid = true;
	}
}

SMemDC::~SMemDC()
{
	if(!m_bValid) return;
	
	m_pBitmap->DeleteObject();
	delete m_pBitmap;

}

void SMemDC::FreeDC()
{
	if (m_bValid)
		SelectObject(m_pOldBitmap);

	if(GetSafeHdc())	DeleteDC();
}

BOOL SMemDC::IsValid()	
{ 
	if (!m_bValid || m_Width == 1 || m_Height == 1)
		return FALSE;

	return TRUE; 
}

BOOL SMemDC::SetSize(int iWidth, int iHeight)
{
	if( iWidth<=0 || iHeight<=0 )
		return FALSE;

/*	if( iWidth <= m_Width && iHeight <= m_Height) // 메모리를 새로 할당하지않음
	{
		m_Width = iWidth;
		m_Height = iHeight;
		return TRUE;
	}
*/
	m_Width = iWidth;
	m_Height = iHeight;

	m_Rect = CRect(0,0,iWidth,iHeight);
	CBitmap* pNewBitmap = new CBitmap;
	CBitmap *pOldBitmap;
	pNewBitmap->CreateCompatibleBitmap(this,iWidth,iHeight);
	pOldBitmap = SelectObject(pNewBitmap);
	if(pOldBitmap)
	{
		m_pBitmap->DeleteObject();
		delete m_pBitmap;
		m_pBitmap = pNewBitmap;
	}
	else
	{
		pNewBitmap->DeleteObject();
		delete pNewBitmap;
	}

	return TRUE;
}

void SMemDC::SBitBlt(CDC *pDC)
{
	pDC->BitBlt(m_Rect.left, m_Rect.top, m_Rect.Width(), m_Rect.Height(),
			this, m_Rect.left, m_Rect.top, SRCCOPY);
}

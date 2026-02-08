#include "StdAfx.h"
#include "ExMemDC.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CExMemDC, CDC);

//////////////////////////////////////////////////////////////////////
// CExMemDC - implementation

CExMemDC::CExMemDC(CDC* pDC, const CRect& rect, COLORREF clrColor) :
	m_sizeImage(CSize(0, 0)), m_bStretch(FALSE)
{
	ASSERT(pDC != NULL);
	m_pDC = pDC;
	
	// If rect is NULL, use the device context's clip box.
	if (rect.IsRectEmpty())
		m_pDC->GetClipBox(&m_rc);
	else
		m_rc.CopyRect(&rect);

    // Create the memory DC, set Map Mode
	if (CreateCompatibleDC(m_pDC))
	{
		SetMapMode(m_pDC->GetMapMode());
		
		// Create a bitmap big enough to hold the window's image
		m_bitmap.CreateCompatibleBitmap(m_pDC, m_rc.Width(), m_rc.Height());
		
		// Select the bitmap into the memory DC
		m_hOldBitmap = (HBITMAP)SelectObject(&m_bitmap)->GetSafeHandle();
		
		// Repaint the background, this takes the place of WM_ERASEBKGND.
		if (clrColor != -1)
		{
			//P.Baranov
			CRect rc( m_rc );

			rc.left    = 0;
			rc.right  -= m_rc.left;
			rc.top     = 0;
			rc.bottom -= m_rc.top;

			FillSolidRect(rc, clrColor);
		}
		
		m_bValid = TRUE;
	}

	// Something bad happened, could be GDI leak, didn't create device context.
	else
	{
		m_bValid = FALSE;
		m_hOldBitmap = NULL;
	}
}

CExMemDC::~CExMemDC()
{
	if (m_bValid)
	{
		// Blt it
		if(m_bStretch)
			m_pDC->StretchBlt(m_rc.left, m_rc.top, m_rc.Width(), m_rc.Height(),
				this, 0, 0, m_sizeImage.cx, m_sizeImage.cy, SRCCOPY); 
		else
			m_pDC->BitBlt(m_rc.left, m_rc.top, m_rc.Width(), m_rc.Height(),
				this, 0, 0, SRCCOPY);            
	}

	// Select the original bitmap back in
	if (m_hOldBitmap != NULL)
	{
		SelectObject(m_hOldBitmap);
		m_bitmap.DeleteObject();
	}

	DeleteDC();
}

void CExMemDC::SetStretchMode(BOOL bMode, CSize sizeImage)
{
	if(sizeImage.cx == 0 || sizeImage.cy == 0)
		m_sizeImage = CSize(m_rc.Width(), m_rc.Height());
	else
		m_sizeImage = sizeImage;

	m_bStretch = bMode;
}
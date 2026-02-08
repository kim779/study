// BmpButton.cpp : implementation file
//

#include "stdafx.h"
#include "axisgview.h"
#include "BmpButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBmpButton

CBmpButton::CBmpButton(CWnd *pView)
{
	m_pView = pView;
	m_bLBDN = false;
}

CBmpButton::~CBmpButton()
{
}


BEGIN_MESSAGE_MAP(CBmpButton, CWnd)
	//{{AFX_MSG_MAP(CBmpButton)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBmpButton message handlers

void CBmpButton::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CAxisGViewApp* app = (CAxisGViewApp*)AfxGetApp();
	CString path = app->GetImgPath();
	
	CRect	crc, rc;
	GetClientRect(&crc);
	rc = crc;
//	rc.DeflateRect(2,2);

	
	CBitmap *pBitmap = NULL;
	if (m_bLBDN)
	{
//		dc.DrawFrameControl(&crc, DFC_BUTTON, DFCS_BUTTONPUSH|DFCS_PUSHED);

		path += "GTV_ROTATE_DN.bmp";
		pBitmap = (CBitmap *)m_pView->SendMessage(WM_USER, getBITMAP, long(path.operator LPCTSTR()));
	}
	else
	{
//		dc.DrawFrameControl(&crc, DFC_BUTTON, DFCS_BUTTONPUSH);

		path += "GTV_ROTATE.bmp";
		pBitmap = (CBitmap *)m_pView->SendMessage(WM_USER, getBITMAP, long(path.operator LPCTSTR()));
	}

	if (pBitmap)
	{
		CDC		memDC;
		CBitmap		*pSBitmap;

		BITMAP stBitmapInfo;
		pBitmap->GetBitmap(&stBitmapInfo);

		memDC.CreateCompatibleDC(&dc);	
		if (memDC.m_hDC != NULL) 
			pSBitmap = (CBitmap*)memDC.SelectObject(pBitmap);
		dc.StretchBlt(rc.left, rc.top, rc.Width(), rc.Height(), &memDC, 0, 0, stBitmapInfo.bmWidth, stBitmapInfo.bmHeight, SRCCOPY);
		memDC.SelectObject(pSBitmap);
		memDC.DeleteDC();
	}
}

void CBmpButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_bLBDN = true;
	Invalidate();

	SetCapture();
		
	CWnd::OnLButtonDown(nFlags, point);
}

void CBmpButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();

	m_bLBDN = false;
	Invalidate();

	m_pView->SendMessage(WM_USER, autoDLL, 0);

	CWnd::OnLButtonUp(nFlags, point);
}

void CBmpButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnMouseMove(nFlags, point);
}

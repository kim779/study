// PopWnd1.cpp : implementation file
//

#include "stdafx.h"
#include "IB420300.h"
#include "PopWnd1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DOWNRGB	RGB(0, 0, 255)
#define	UPRGB	RGB(255, 0, 0)

/////////////////////////////////////////////////////////////////////////////
// CPopWnd

CPopWnd::CPopWnd()
{
	m_pBitmap = NULL;
}

CPopWnd::~CPopWnd()
{
	if (m_pBitmap)
		delete m_pBitmap;
}


BEGIN_MESSAGE_MAP(CPopWnd, CWnd)
	//{{AFX_MSG_MAP(CPopWnd)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPopWnd message handlers

void CPopWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CDC		memDC;
	CBitmap*	pOldBitmap;
	CRect		clientRt;
	GetClientRect(&clientRt);
	
	memDC.CreateCompatibleDC(&dc);	
	if (memDC.m_hDC != NULL) 
		pOldBitmap = (CBitmap*)memDC.SelectObject(m_pBitmap);
	dc.BitBlt(0, 0, clientRt.Width(), clientRt.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
}

void CPopWnd::SetData(double jisu)
{
	m_jisu = jisu;

	drawBitmap();
	Invalidate();
}


void CPopWnd::drawBitmap()
{
	m_section.Lock();
	CDC	memDC, *pDC = GetDC();
	CBitmap*	pOldBitmap;
	CRect clientRt, tempRt;

	GetClientRect(&clientRt);

	if (m_pBitmap)
		delete m_pBitmap;
	m_pBitmap = new CBitmap();
	
	memDC.CreateCompatibleDC(pDC);
	m_pBitmap->CreateCompatibleBitmap(pDC, clientRt.Width(), clientRt.Height());
	pOldBitmap = (CBitmap*)memDC.SelectObject(m_pBitmap);
	
	memDC.FillSolidRect(clientRt, RGB(255, 255, 225));
	memDC.Draw3dRect(clientRt, RGB(190, 190, 190), RGB(190, 190, 190));
	memDC.SetTextColor(RGB(0, 0, 0));

	tempRt.SetRect(clientRt.left, clientRt.top, clientRt.right, clientRt.top + 20);
	memDC.Draw3dRect(tempRt, RGB(190, 190, 190), RGB(190, 190, 190));
	memDC.SetBkMode(TRANSPARENT);

	CString strtemp;
	strtemp.Format("%.2f%%", m_jisu);

	memDC.DrawText(strtemp, -1, CRect(0, 0, 70, 20), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC(); 
	ReleaseDC(pDC);
	m_section.Unlock();
}


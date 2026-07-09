// SelFrameWnd.cpp : implementation file
//

#include "stdafx.h"
#include "SelFrameWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelFrameWnd

CSelFrameWnd::CSelFrameWnd()
{
}

CSelFrameWnd::~CSelFrameWnd()
{
}


BEGIN_MESSAGE_MAP(CSelFrameWnd, CWnd)
	//{{AFX_MSG_MAP(CSelFrameWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSelFrameWnd message handlers

int CSelFrameWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

void CSelFrameWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rc;
	GetClientRect(&rc);
	dc.FillSolidRect(rc, RGB(0, 0, 0));	
	// Do not call CWnd::OnPaint() for painting messages
}

void CSelFrameWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	if (m_rectArr.GetSize() == 1)
	{
		CRect rc = m_rectArr.GetAt(0);
		rc.right = rc.left + cx;
		rc.bottom = rc.top + cy;
		m_rectArr.SetAt(0, rc);;
	}
	SetTP();
}

void CSelFrameWnd::SetTP()
{
	if (!m_rectArr.GetSize())
		return;

	CRgn	tmpRgn1, tmpRgn2;
	CRect tRc = CRect(0), rt;
	GetClientRect(&rt);
	m_wndRgn.DeleteObject();
	m_wndRgn.CreateRectRgn(0, 0, 0, 0);
	for (int ii = 0; ii < m_rectArr.GetSize(); ii++)
	{
		rt = m_rectArr.GetAt(ii);
		tRc.UnionRect(tRc, rt);
	}
	
	for (int ii = 0; ii < m_rectArr.GetSize(); ii++)
	{
		rt = m_rectArr.GetAt(ii);
		rt.OffsetRect(-tRc.left, -tRc.top);

		tmpRgn1.DeleteObject();
		tmpRgn1.CreateRectRgn(rt.left, rt.top, rt.right, rt.bottom);
		rt.DeflateRect(2, 2);
		tmpRgn2.CreateRectRgn(rt.left, rt.top, rt.right, rt.bottom);
		tmpRgn1.CombineRgn(&tmpRgn1, &tmpRgn2, RGN_XOR);
		tmpRgn2.DeleteObject();
		m_wndRgn.CombineRgn(&m_wndRgn, &tmpRgn1, RGN_OR);
		rt.InflateRect(2, 2);
	}
	GetClientRect(&rt);
	tRc.OffsetRect(-tRc.left, -tRc.top);
	if (!rt.EqualRect(tRc))
		MoveWindow(tRc);

	SetWindowRgn(m_wndRgn, TRUE);
}

void CSelFrameWnd::AddRect(CRect rc)
{
	m_rectArr.Add(rc);
}

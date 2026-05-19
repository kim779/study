// pointWnd.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "pointWnd.h"


// CPointWnd

IMPLEMENT_DYNAMIC(CPointWnd, CWnd)

CPointWnd::CPointWnd()
{

}

CPointWnd::~CPointWnd()
{
}


BEGIN_MESSAGE_MAP(CPointWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CPointWnd 메시지 처리기입니다.

void CPointWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect	rc;

	GetClientRect(&rc);
	dc.FillSolidRect(rc, RGB(0, 0, 0));
}

void CPointWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	if (m_rectArr.GetSize() == 1)
	{
		CRect	rc = m_rectArr.GetAt(0);
		rc.right = rc.left + cx;
		rc.bottom = rc.top + cy;
		m_rectArr.SetAt(0, rc);;
	}
	SetTP();
}

void CPointWnd::AddRect(CRect rc)
{
	m_rectArr.Add(rc);
}

void CPointWnd::SetTP()
{
	if (!m_rectArr.GetSize())
		return;

	CRgn	tmpRgn1, tmpRgn2;
	CRect	rt, tRc = CRect(0, 0, 0, 0);

	GetClientRect(&rt);
	m_rgn.DeleteObject();
	m_rgn.CreateRectRgn(0, 0, 0, 0);
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
		m_rgn.CombineRgn(&m_rgn, &tmpRgn1, RGN_OR);
		rt.InflateRect(2, 2);
	}
	GetClientRect(&rt);
	tRc.OffsetRect(-tRc.left, -tRc.top);
	if (!rt.EqualRect(tRc))
		MoveWindow(tRc);

	SetWindowRgn(m_rgn, TRUE);
}

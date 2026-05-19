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
	dc.FillSolidRect(rc, RGB(255, 255, 255));

	CPen pen, *open;
	LOGBRUSH logBrush;
	logBrush.lbStyle = BS_SOLID;
	logBrush.lbColor = RGB(0, 0, 0);
	pen.CreatePen(PS_DOT | PS_GEOMETRIC | PS_ENDCAP_ROUND, 1, &logBrush);
	open = dc.SelectObject(&pen);
	rc.SetRect(rc.left, rc.top, rc.right - 1, rc.bottom - 1);

	dc.MoveTo(rc.TopLeft());
	dc.LineTo(CPoint(rc.left, rc.bottom));

	dc.MoveTo(rc.TopLeft());
	dc.LineTo(CPoint(rc.right, rc.top));

	dc.MoveTo(CPoint(rc.right, rc.top));
	dc.LineTo(CPoint(rc.right, rc.bottom + 1));

	dc.MoveTo(CPoint(rc.left, rc.bottom));
	dc.LineTo(CPoint(rc.right, rc.bottom));

	dc.SelectObject(open);
	pen.DeleteObject();
	
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

	CRgn tmpRgn1, tmpRgn2, tmpRgn3;
	CRect rt, tRc = CRect(0, 0, 0, 0);

	GetClientRect(&rt);
	m_rgn.DeleteObject(); m_rgn.CreateRectRgn(0, 0, 0, 0);

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

		tmpRgn2.DeleteObject();
		tmpRgn2.CreateRectRgn(rt.left, rt.top, rt.right, rt.bottom);

		tmpRgn3.DeleteObject(); tmpRgn3.CreateRectRgn(0, 0, 0, 0);
		tmpRgn3.CombineRgn(&tmpRgn1, &tmpRgn2, RGN_XOR);

		tmpRgn1.CopyRgn(&m_rgn);
		m_rgn.CombineRgn(&tmpRgn1, &tmpRgn3, RGN_OR);
		rt.InflateRect(2, 2);
	}

	GetClientRect(&rt);
	tRc.OffsetRect(-tRc.left, -tRc.top);
	if (!rt.EqualRect(tRc)) MoveWindow(tRc);

	SetWindowRgn(m_rgn, TRUE);
}

void CPointWnd::debugX(CString gubn, CRect rect)
{
	CString string; string.Format(_T("[%10s][%03d %03d %03d %03d %03d %03d]\n"),
		gubn, rect.left, rect.top, rect.right, rect.bottom, rect.Height(), rect.Width());

#ifdef _DEBUG
	FILE* debug = fopen("c:/debugD.txt", "a+");
#else
	FILE* debug = fopen("c:/debugR.txt", "a+");
#endif
	fprintf(debug, "%s", string);
	fclose(debug);
} 

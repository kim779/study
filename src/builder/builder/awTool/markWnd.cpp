// markWnd.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "markWnd.h"


// CMarkWnd

IMPLEMENT_DYNAMIC(CMarkWnd, CWnd)

CMarkWnd::CMarkWnd()
{

}

CMarkWnd::~CMarkWnd()
{
}


BEGIN_MESSAGE_MAP(CMarkWnd, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CMarkWnd 메시지 처리기입니다.

void CMarkWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect	rc;

	GetClientRect(&rc);
	dc.FillSolidRect(rc, RGB(0, 0, 0));
}

void CMarkWnd::SetTP(int type)
{
	CRect	rc;
	CRgn	tmpRgn;
	int	ii;

	GetClientRect(&rc);
	m_rgn.DeleteObject();
	m_rgn.CreateRectRgn(0, 0, 0, 0);

	switch (type)
	{
	case 3:
		for (ii = 0; ii < rc.Width(); ii++)
		{
			tmpRgn.DeleteObject();
			tmpRgn.CreateRectRgn(ii, ii, ii + 1, rc.Height() - ii);
			m_rgn.CombineRgn(&m_rgn, &tmpRgn, RGN_OR);
		}
		break;
	case 4:
		for (ii = 0; ii < rc.Height(); ii++)
		{
			tmpRgn.DeleteObject();
			tmpRgn.CreateRectRgn(ii, ii, rc.Width() - ii, ii + 1);
			m_rgn.CombineRgn(&m_rgn, &tmpRgn, RGN_OR);
		}
		break;
	}
	SetWindowRgn(m_rgn, TRUE);
}

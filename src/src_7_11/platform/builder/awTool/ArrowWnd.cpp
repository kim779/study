// ArrowWnd.cpp : implementation file
//

#include "stdafx.h"
#include "ArrowWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CArrowWnd

CArrowWnd::CArrowWnd()
{
}

CArrowWnd::~CArrowWnd()
{
}


BEGIN_MESSAGE_MAP(CArrowWnd, CWnd)
	//{{AFX_MSG_MAP(CArrowWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CArrowWnd message handlers

void CArrowWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rc;
	GetClientRect(&rc);
	dc.FillSolidRect(rc, RGB(0, 0, 0));
	
	// Do not call CWnd::OnPaint() for painting messages
}

int CArrowWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	
	return 0;
}

void CArrowWnd::SetTP(int type)
{
	CRect rc;
	CRgn	tmpRgn;
	int	ii;

	GetClientRect(&rc);
	m_wndRgn.DeleteObject();
	m_wndRgn.CreateRectRgn(0, 0, 0, 0);

	switch(type)
	{
	case 3:
		for (ii = 0; ii < rc.Width(); ii++)
		{
			tmpRgn.DeleteObject();
			tmpRgn.CreateRectRgn(ii, ii, ii + 1, rc.Height() - ii);
			m_wndRgn.CombineRgn(&m_wndRgn, &tmpRgn, RGN_OR);
		}
		break;
	case 4:
		for (ii = 0; ii < rc.Height(); ii++)
		{
			tmpRgn.DeleteObject();
			tmpRgn.CreateRectRgn(ii, ii, rc.Width() - ii, ii + 1);
			m_wndRgn.CombineRgn(&m_wndRgn, &tmpRgn, RGN_OR);
		}
		break;
	}
	SetWindowRgn(m_wndRgn, TRUE);
}

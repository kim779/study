// TooltipDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IB0000A1.h"
#include "TooltipDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTooltipDlg dialog


CTooltipDlg::CTooltipDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTooltipDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTooltipDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_brush.CreateSolidBrush(RGB(255, 255, 225));
	m_brushBack.CreateSolidBrush(RGB(0, 0, 0));
	m_pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	m_bitmap.LoadBitmap(IDB_TITLE1);
}


void CTooltipDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTooltipDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTooltipDlg, CDialog)
	//{{AFX_MSG_MAP(CTooltipDlg)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTooltipDlg message handlers

void CTooltipDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	m_brush.DeleteObject();	
	m_brushBack.DeleteObject();	
	m_rgn.DeleteObject();
	m_pen.DeleteObject();
	m_rgnDraw.DeleteObject();
	SetWindowRgn(NULL, FALSE);
}

HBRUSH CTooltipDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	switch (nCtlColor)
	{
	case CTLCOLOR_DLG:
//		hbr = m_brush;		
		break;
	case CTLCOLOR_STATIC:
		pDC->SetBkMode(TRANSPARENT);
		hbr = m_brush;
		break;
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

LRESULT CTooltipDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_COMMAND)
	{
		switch (LOWORD(wParam))
		{
		case IDCANCEL:
		case IDOK:
			return 0;				
		}
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CTooltipDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	
	CRect	rect;
	CRgn	rgn;
	CPoint	pt[4];
	GetDlgItem(IDC_ST_TITLE)->GetWindowRect(m_rcBitmap);
	ScreenToClient(m_rcBitmap);
	
	GetClientRect(m_rect);		
	rect = m_rect;
	pt[0] = pt[3] = CPoint(rect.left, rect.top);
	rect.top += 20;
	pt[1].y = pt[2].y = rect.top;
	pt[1].x = rect.left + 20;
	pt[2].x = pt[1].x + 20;

	rgn.CreatePolygonRgn(pt, 4, ALTERNATE);
	m_rgn.CreateRoundRectRgn(rect.left, rect.top, rect.right, rect.bottom, 16, 16);
	m_rgn.CombineRgn(&m_rgn, &rgn, RGN_OR);
	rgn.DeleteObject();
	SetWindowRgn(m_rgn, TRUE);
	rect.DeflateRect(1, 1);
	pt[0].x += 2;
	pt[0].y += 2;

	pt[3] = pt[0];

	pt[1].x += 3;
	pt[1].y += 1;
	pt[2].x -= 2;
	pt[2].y += 1;
	rgn.CreatePolygonRgn(pt, 4, ALTERNATE);
	m_rgnDraw.CreateRoundRectRgn(rect.left, rect.top, rect.right, rect.bottom, 16, 16);
	m_rgnDraw.CombineRgn(&m_rgnDraw, &rgn, RGN_OR);
	rgn.DeleteObject();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CTooltipDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CPen*	oldpen = dc.SelectObject(&m_pen);
	dc.FillSolidRect(m_rect, RGB(0, 0, 0));
	dc.FillRgn(&m_rgnDraw, &m_brush);	
	dc.SelectObject(oldpen);
	m_bitmap.Draw(&dc, m_rcBitmap.left, m_rcBitmap.top);
	// Do not call CDialog::OnPaint() for painting messages
}

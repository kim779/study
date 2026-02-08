// MatrixSelecterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "./define/ChartMsgDef.h"
#include "MatrixSelecterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMatrixSelecterDlg dialog
#define WIDTHOFUNIT		(20)
#define HEIGHTOFUNIT	(20)

CMatrixSelecterDlg::CMatrixSelecterDlg(CWnd* pParent /*=NULL*/)
	: CRscDlg(CMatrixSelecterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMatrixSelecterDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nX = 0;
	m_nY = 0;
	m_nXOriginal = 0;
	m_nYOriginal = 0;
	m_nXSelected = 0;
	m_nYSelected = 0;
	m_pWndFrameDlg = NULL;
	m_pBesideWnd = NULL;
	m_bSetResized = TRUE;
}


void CMatrixSelecterDlg::DoDataExchange(CDataExchange* pDX)
{
	CRscDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMatrixSelecterDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMatrixSelecterDlg, CRscDlg)
	//{{AFX_MSG_MAP(CMatrixSelecterDlg)
	ON_WM_KILLFOCUS()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMatrixSelecterDlg message handlers

void CMatrixSelecterDlg::OnKillFocus(CWnd* pNewWnd) 
{
	CRscDlg::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	ShowWindow(SW_HIDE);
}

void CMatrixSelecterDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rectTitle;
	CString strTitle;
	GetClientRect(m_rectAll);	
	m_rectAll.DeflateRect(5,5);
	m_bSetResized = TRUE;
	rectTitle = m_rectAll;
	m_rectAll.top += 15;
	rectTitle.bottom = rectTitle.top + 15;
//	dc.Rectangle(m_rectAll);
	CRect rectUnit;
	
	m_nXSelected = 0;
	m_nYSelected = 0;
	COLORREF clrCurrent = dc.SetBkColor(RGB(110,110,220));

	for(int nPosX=0;nPosX<m_nX;nPosX++)
	{
		for(int nPosY=0;nPosY<m_nY;nPosY++)
		{
			rectUnit.SetRect(m_rectAll.left+ WIDTHOFUNIT*nPosX
							,m_rectAll.top+ WIDTHOFUNIT*nPosY
							,m_rectAll.left + WIDTHOFUNIT*(nPosX+1) +1
							,m_rectAll.top + WIDTHOFUNIT*(nPosY+1) +1 );
			dc.Rectangle(rectUnit);

			if(m_ptSelected.x >0 && m_ptSelected.y >0 
				&& rectUnit.left <= m_ptSelected.x && rectUnit.top <= m_ptSelected.y)
			{
				m_rectSelectedOne = rectUnit;
				rectUnit.DeflateRect(1,1);
				dc.FillSolidRect(rectUnit,RGB(110,110,220));
				m_nXSelected = nPosX + 1;
				m_nYSelected = nPosY + 1;
			}
			
		}
	}
	if(!m_nXSelected||!m_nYSelected)
	{
		strTitle = "Ãë¼Ò";
	}
	else
	{		
		strTitle.Format("%d X %d",m_nXSelected,m_nYSelected);
	}
	CFont *pftOld = (CFont *)dc.SelectObject(GetFont());	
	dc.SetBkColor(clrCurrent);
	dc.DrawText(strTitle,rectTitle,DT_CENTER);
	dc.SelectObject(pftOld);	
	// Do not call CDialog::OnPaint() for painting messages
}

void CMatrixSelecterDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if((point.x > m_rectAll.right || point.y > m_rectAll.bottom)&&m_bSetResized)
	{
		if(point.x > m_rectAll.right&&m_nX * m_nY<20)
		{
			m_nX++;
		}
		if(point.y > m_rectAll.bottom&&m_nX * m_nY<20)
		{
			m_nY++;
		}
		m_ptSelected = point;
		m_bSetResized = FALSE;

		ShowMatrix(FALSE);
	}
	else if(m_rectAll.PtInRect(point)&&!m_rectSelectedOne.PtInRect(point))
	{
		m_ptSelected = point;
		Invalidate();
	}
	else if(!m_rectAll.PtInRect(point))
	{
		m_ptSelected.x = 0;
		m_ptSelected.y = 0;
		m_rectSelectedOne.SetRectEmpty();
		Invalidate();
	}
	
	CRscDlg::OnMouseMove(nFlags, point);
}

void CMatrixSelecterDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	ShowWindow(SW_HIDE);
	if(m_nXSelected&&m_nYSelected&&m_pWndFrameDlg)
	{
		m_pWndFrameDlg->SendMessage(UMSG_UPDATEMATRIX,m_nXSelected,m_nYSelected);
	}	
	
	CRscDlg::OnLButtonUp(nFlags, point);
}

void CMatrixSelecterDlg::ShowMatrix(BOOL bInit /*=TRUE*/)
{
	VERIFY(m_pBesideWnd);
	if(bInit)
	{
		m_nX = m_nXOriginal;
		m_nY = m_nYOriginal;
	}
	CRect rect;
	m_pBesideWnd->GetWindowRect(rect);
	SetWindowPos(&CWnd::wndTop,rect.left,rect.bottom,WIDTHOFUNIT*m_nX+20,HEIGHTOFUNIT*m_nY+20+15,SWP_DRAWFRAME|SWP_SHOWWINDOW);
	//SetFocus();

	Invalidate();
}

void CMatrixSelecterDlg::SetBesideRect(CRect rect)
{
	m_rectOfBesideWnd = rect;
}

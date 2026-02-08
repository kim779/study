// HogaWindow.cpp : implementation file
//

#include "stdafx.h"
#include "IB100400.h"
#include "HogaWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHogaWindow

CHogaWindow::CHogaWindow(CWnd* pParent)
{
	m_pMain = pParent;
	m_conditioncolor[2] = m_txtcolor;
}

CHogaWindow::~CHogaWindow()
{
}


BEGIN_MESSAGE_MAP(CHogaWindow, CWnd)
	//{{AFX_MSG_MAP(CHogaWindow)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_NCHITTEST()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHogaWindow message handlers

int CHogaWindow::Create(CRect rect, CWnd* parent, UINT nID) 
{
	CString clsName = AfxRegisterWndClass(0);
	const UINT	dwStyle = WS_POPUP;
	const UINT	dwExStyle = WS_EX_TOOLWINDOW|WS_EX_TOPMOST;

	return CWnd::Create(NULL, NULL, WS_VISIBLE|WS_CHILD, rect, parent, nID, NULL); 
//	return CreateEx(dwExStyle, clsName, NULL, dwStyle, rect, NULL, NULL, NULL);
}

int CHogaWindow::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	LOGFONT lf = GetLogFont(12);
	m_pFont.CreateFontIndirect(&lf);
	
	return 0;
}

void CHogaWindow::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rect;
	GetClientRect(&rect);
	DrawGrid(&dc);
	DrawText(&dc);

	// Do not call CWnd::OnPaint() for painting messages
}

void CHogaWindow::DrawGrid(CDC* pDC)
{
	const COLORREF backcolor = 0xFFFFFF;
	const int ibottom = LINEH * 14;
	const int iright = (COLW+1) * 3;

	CBrush brush(backcolor);
	CBrush* pOldBrush = pDC->SelectObject(&brush); 
	CPen pen(PS_SOLID, 1, m_linecolor );
	CPen* pOldPen = pDC->SelectObject(&pen); 
	pDC->Rectangle(CRect(0,0,iright, ibottom)); 

	int iheight = LINEH;
	for(int i=0; i<3; i++)
	{
		pDC->MoveTo(1, iheight);
		pDC->LineTo(iright, iheight); 

		iheight += (LINEH);
	}

	iheight = (LINEH) * 8;
	pDC->MoveTo(1, iheight);
	pDC->LineTo(iright, iheight); 

	iheight = (LINEH) * 13;
	pDC->MoveTo(1, iheight);
	pDC->LineTo(iright, iheight); 

	int iw = 1+COLW;
	for(int i=0; i<2; i++)
	{
		pDC->MoveTo(iw, 1);
		pDC->LineTo(iw, ibottom); 

		iw += (COLW+1);
	}

	pDC->SelectObject(pOldPen); 
	pDC->SelectObject(pOldBrush); 

	DeleteObject(pOldPen);
	DeleteObject(pOldBrush);

	PaintGradiantRect(pDC, CRect(m_pRect[3][1].left+1, m_pRect[3][1].top,  m_pRect[3][1].right+1, m_pRect[7][1].bottom-1), 0xFFF0D7, 0xFFFFFF, false, false);
	PaintGradiantRect(pDC, CRect(m_pRect[3][1].left+1, m_pRect[8][1].top,  m_pRect[3][1].right+1, m_pRect[12][1].bottom-1), 0xEADEFF);
}

void CHogaWindow::DrawText(CDC* pDC)
{
	pDC->SetBkMode(TRANSPARENT); 
	

	CFont* pOldFont = pDC->SelectObject(&m_pFont); 
	CRect trect;
	for(int r=0; r<14; r++)
	{
		for(int c=0; c<3; c++)
		{
			trect = m_pRect[r][c];
			trect.DeflateRect(2, 1, 2, 1); 
			
			pDC->SetTextColor( GetTextFormat(m_astrHogaList[r][c], m_iDataFormat[r][c]) );
			if( m_iDataFormat[r][c] == 6) 
				pDC->DrawText(m_astrHogaList[r][c], trect, DT_CENTER|DT_SINGLELINE|DT_VCENTER); 
			else 
				pDC->DrawText(m_astrHogaList[r][c], trect, DT_RIGHT|DT_SINGLELINE|DT_VCENTER); 
		}
	}

	CString sTxt[] = {"상한/하한", "거래량", "매도잔량"};
	pDC->SetTextColor(m_lbltxtcolor);
	for(int r=0; r<3; r++)
	{
		trect = m_pRect[r][0];
		trect.DeflateRect(0, 0, 0, 1); 
		pDC->FillSolidRect(trect, m_lblcolor);
		trect.DeflateRect(2, 1, 2, 0); 
		pDC->DrawText(sTxt[r], trect, DT_CENTER|DT_SINGLELINE|DT_VCENTER); 
	}

	
	
	sTxt[0] = "매수잔량";
	trect = m_pRect[2][2];
	trect.DeflateRect(2, 0, -1, 1); 
	pDC->FillSolidRect(trect, m_lblcolor);

//	pDC->SetTextColor(m_txtcolor);
	trect.DeflateRect(2, 1, 2, 0); 
	pDC->DrawText(sTxt[0], trect, DT_CENTER|DT_SINGLELINE|DT_VCENTER); 

	pDC->SelectObject(pOldFont); 
	DeleteObject(pOldFont);
}

void CHogaWindow::OnDestroy() 
{
	CWnd::OnDestroy();
	
	// TODO: Add your message handler code here
}

void CHogaWindow::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	m_iWidth = cx;
	m_iHeight = cy;
	int iheight = LINEH, itop=1;
	int ileft = 1;

	for(int i=0; i<14; i++)
	{
		iheight = itop + LINEH;
		for(int c=0; c<3; c++)
		{
			m_pRect[i][c].top = itop;
			m_pRect[i][c].bottom = iheight;

			m_pRect[i][c].left = ileft;
			m_pRect[i][c].right = m_pRect[i][c].left + COLW;
			ileft = m_pRect[i][c].right;
			m_iDataFormat[i][c] = 1;
		}

		itop = iheight;
		ileft = 1;
	}

	m_iDataFormat[0][0] = m_iDataFormat[1][0] = m_iDataFormat[2][0] = m_iDataFormat[3][2] = 0;

	for(int i=3; i<14; i++)
		m_iDataFormat[i][1] = 2;

	m_iDataFormat[2][2] = 2;
	m_iDataFormat[0][1] = 3;
	m_iDataFormat[0][2] = 4;
	m_iDataFormat[2][1] = 6;
}

COLORREF CHogaWindow::GetTextFormat(CString& sData, int iFormat)
{
	if(iFormat == 3) return m_conditioncolor[0];
	if(iFormat == 4) return m_conditioncolor[1];
	if(iFormat != 2)
		return m_txtcolor;

	sData = Trim(sData);
	int isign = 2;
	if( sData.FindOneOf("+-")  == 0)
	{
		const char sign = sData.GetAt(0);
		if(sign == '+') isign = 0;
		else isign = 1;
		sData = sData.Mid(1); 
	}

	return m_conditioncolor[isign];
}


void CHogaWindow::ReDraw()
{
	m_strRealCode = Trim(m_astrHogaList[0][0]);
	m_astrHogaList[0][0] = "";
	if( !IsWindowVisible() ) return;
	RedrawWindow();
}

void CHogaWindow::ReDraw(int irow, int icol)
{
	if( !IsWindowVisible() ) return;
	InvalidateRect(m_pRect[irow][icol]);
}

void CHogaWindow::Show(bool bshow, CRect rect)
{
	UINT uiShow = SW_HIDE;
	if(bshow) uiShow = SW_SHOW;
	if(uiShow == SW_SHOW)
		MoveWindow(rect);
	ShowWindow(uiShow);
}

void CHogaWindow::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
//	ShowWindow(SW_HIDE);
	CWnd::OnLButtonDown(nFlags, point);
}

LRESULT CHogaWindow::OnNcHitTest(CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CWnd::OnNcHitTest(point);
}

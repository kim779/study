/************************************************/
/*																		          */
/*							DR Botton Control							  */
/*                                              */
/*																		          */
/*							작성일 : 2002. 05 ~							*/
/*						작성자 : 우동우 (Doori co.)		    */
/*																		          */
/************************************************/

#include "stdafx.h"			// PCH
#include "DrInfoTip.h"		// Class interface

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TIMER_HIDE				500		// Hide timer (ms)

#define CX_ROUNDED				  8		// Tip horizontal roundness
#define CY_ROUNDED				  6		// Tip vertical roundness
#define CX_LEADER				    12		// Width of tip lead
#define CY_LEADER				    12		// Height of tip lead
#define CX_ICON_MARGIN			30		//2// Width of margin between icon and tip text
#define DEFAULT_SHOW_DELAY	0		// Default show delay (ms)
#define CX_OFFSET					20

#define IDC_BTN_EXIT1		   1
#define IDC_BTN_EXIT2		   2
#define IDC_BTN_EXIT3		   3

CDrInfoTip::CDrInfoTip()
{
	// Register the class
	m_szClass		= AfxRegisterWndClass(0);
	
	m_hIcon			= NULL;
	
	// Set the delay defaults
	m_nShowDelay	= DEFAULT_SHOW_DELAY;
	
	m_IconSize		= CSize(0, 0);
	m_ptOrigin		= CPoint(0, 0);
	
	m_strShowType = "DOWN";

	LOGFONT	  logFont;
	GETFONT(logFont, 12, 400, m_fontDefault);
	m_nID = 0;
}

CDrInfoTip::~CDrInfoTip()
{
	m_fontDefault.DeleteObject();
}

// Message map
BEGIN_MESSAGE_MAP(CDrInfoTip, CWnd)
//{{AFX_MSG_MAP(CDrInfoTip)
ON_WM_PAINT()
ON_WM_CREATE()
ON_WM_TIMER()
ON_WM_DESTROY()
ON_WM_SIZE()
//}}AFX_MSG_MAP

ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_EXIT1, IDC_BTN_EXIT3, OnBtnExit)
END_MESSAGE_MAP()

int CDrInfoTip::OnCreate( LPCREATESTRUCT lpCreateStruct ) 
{
	if ( CWnd::OnCreate( lpCreateStruct ) == -1 )
		return -1;
	
	return 0;
}

BOOL CDrInfoTip::Create(CWnd* pParentWnd, CString strShowType, int nIndicatePos, int nID) 
{
	BOOL	bSuccess;
	
	// Must have a parent
	ASSERT(pParentWnd != NULL);
	
	bSuccess = CreateEx(NULL, m_szClass, NULL, WS_POPUP, 0, 0, 0, 0, pParentWnd->GetSafeHwnd(), NULL, NULL);
	
	// Use default GUI font for default font
	m_pFont = CFont::FromHandle((HFONT)::GetStockObject(DEFAULT_GUI_FONT));
	
	m_strShowType = strShowType;
	m_nIndicatePos = nIndicatePos;
	m_nID = nID;

	if(m_nID != 0)
	{	
		m_btnExit.Create(_T("X"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 
			CRect(0, 0, 0, 0), this, m_nID);
		m_btnExit.SetFont(&m_fontDefault);
		m_btnExit.SetBtnColor(2);
	}
	
	return bSuccess;
}

void CDrInfoTip::OnDestroy() 
{
	KillTimer(m_nTimer);
	
	CWnd::OnDestroy();
}

void CDrInfoTip::Show(CString szText, CPoint *pt /* = NULL */)
{
	if (pt != NULL)
		m_ptOrigin	= *pt;
	else
		GetCursorPos(&m_ptOrigin);
	
	if ((m_strShowType == "UP") || (m_strShowType == "BASE"))
		m_szText	= szText;
	else if (m_strShowType == "DOWN")
		m_szText	= "\n" + szText;
	
	// Start the show timer
	if(m_nID == 0) //종료 버튼 컨트롤 생성여부
		m_nTimer = SetTimer(timerShow, m_nShowDelay, NULL);
	else
		ShowTextBox();
}

void CDrInfoTip::RemoveTool(CWnd *pWnd)
{
	TipToolInfo * Info;
	if (m_ToolMap.Lookup(pWnd->m_hWnd, Info))
	{
		delete Info;
		m_ToolMap.RemoveKey(pWnd->m_hWnd);
	}	
}

void CDrInfoTip::OnPaint() 
{
	CPaintDC dc( this ); // device context for painting
	
	CRect	rc;
	CBrush	WindowBrush;
	CBrush	FrameBrush;
	CBrush	InnerFrameBrush;
	HRGN	hRegion;
	CRgn	*pRegion;
	CFont	*pSysFont;
	
	// Get the client rectangle
	GetClientRect(rc);
	
	// Create the brushes
	InnerFrameBrush.CreateSolidBrush(::GetSysColor(COLOR_SCROLLBAR));
	FrameBrush.CreateSolidBrush(::GetSysColor(COLOR_WINDOWTEXT));
	//COLOR_INFOTEXT 
	
	if (m_strShowType == "BASE")
		WindowBrush.CreateSolidBrush(::GetSysColor(COLOR_INFOBK));
	else
		WindowBrush.CreateSolidBrush(::GetSysColor(COLOR_WINDOW));
	
	// Get the window region
	GetWindowRegion(&dc, &hRegion);
	pRegion = CRgn::FromHandle(hRegion);
	
	// Draw the frame
	dc.FillRgn(pRegion, &WindowBrush);
	
	if (m_strShowType != "BASE")
		dc.FrameRgn(pRegion, &InnerFrameBrush, 3, 3);
	
	dc.FrameRgn(pRegion, &FrameBrush, 1, 1);
	
	// Adjust the area for the icon
	rc.DeflateRect(CX_ROUNDED, CY_ROUNDED, 0, 0);
	if (m_hIcon != NULL)
		rc.left = rc.left + m_IconSize.cx + CX_ICON_MARGIN;
	
	// Set the font
	pSysFont = (CFont *)dc.SelectObject(m_pFont);
	// Draw the tip text	
	dc.SetBkMode( TRANSPARENT );
	dc.DrawText(m_szText, &rc, DT_TOP | DT_LEFT);
	// Draw the icon
	if (m_hIcon != NULL)
		DrawIconEx(dc.m_hDC, CX_ROUNDED, CY_ROUNDED, m_hIcon, m_IconSize.cx, m_IconSize.cy, 0, NULL, DI_NORMAL);
	
	// Clean up GDI
	::DeleteObject(hRegion);
	dc.SelectObject(pSysFont);
	
	WindowBrush.DeleteObject();
	FrameBrush.DeleteObject();
	InnerFrameBrush.DeleteObject();
	
	pSysFont->DeleteObject();
}

BOOL CDrInfoTip::GetWindowRegion(CDC* pDC, HRGN* hRegion, CSize *Size /* = NULL */)
{
	CRect	rcWnd;
	POINT	ptLeader[3];
	CRgn	LeaderRegion;
	CRgn	CaptionRegion;
	CFont	*pSysFont;
	
	ASSERT(pDC != NULL);
	ASSERT(hRegion != NULL);
	
	// Calculate the are for the tip text
	pSysFont = (CFont *)pDC->SelectObject(m_pFont);
	pDC->DrawText(m_szText, &rcWnd, DT_CALCRECT);
	pDC->SelectObject(pSysFont);
	
	// Adjust for the rounded corners
	rcWnd.InflateRect(CX_ROUNDED, CY_ROUNDED);
	
	// Adjust for icon
	if (m_hIcon != NULL)
		rcWnd.right = rcWnd.right + m_IconSize.cx + CX_ICON_MARGIN;
	
	if (rcWnd.Height() < m_IconSize.cy)
		rcWnd.bottom = rcWnd.top + m_IconSize.cy;
	
	//# Calculate the leader triangle coordinates
	if (m_strShowType == "BASE")
	{
		ptLeader[0].x	= rcWnd.Width();
		ptLeader[0].y	= rcWnd.Height();
		
		ptLeader[1].x	= ptLeader[0].x;
		ptLeader[1].y	= ptLeader[0].y;
		
		ptLeader[2].x	= ptLeader[0].x;
		ptLeader[2].y	= ptLeader[0].y;
		
		//# Create the caption region
		CaptionRegion.CreateRoundRectRgn(0, 0, rcWnd.Width(), rcWnd.Height(), 0, 0);
	}
	else if (m_strShowType == "UP")
	{
		ptLeader[0].x	= rcWnd.Width() - CX_OFFSET;
		ptLeader[0].y	= rcWnd.Height() - CY_ROUNDED;
		
		ptLeader[1].x	= ptLeader[0].x;
		ptLeader[1].y	= ptLeader[0].y + CY_LEADER;
		
		ptLeader[2].x	= ptLeader[0].x - CX_LEADER;
		ptLeader[2].y	= ptLeader[0].y;
		
		//# Create the caption region
		CaptionRegion.CreateRoundRectRgn(0, 0, rcWnd.Width(), rcWnd.Height(), CX_ROUNDED, CY_ROUNDED);
	}
	else if (m_strShowType == "DOWN")
	{
		ptLeader[0].x	= rcWnd.Width() - CX_OFFSET - m_nTextMaxLength;
		ptLeader[0].y	= CY_LEADER;
		
		ptLeader[1].x	= ptLeader[0].x;
		ptLeader[1].y	= 0;
		
		ptLeader[2].x	= ptLeader[0].x - CX_LEADER;
		ptLeader[2].y	= ptLeader[0].y;
		
		//# Create the caption region
		CaptionRegion.CreateRoundRectRgn(0, CY_LEADER - 2, rcWnd.Width(), rcWnd.Height(), CX_ROUNDED, CY_ROUNDED);
	}
	
	//# Create the leader region
	LeaderRegion.CreatePolygonRgn(ptLeader, 3, ALTERNATE);
	
	//# Create window region
	*hRegion = ::CreateRectRgn(0, 0, rcWnd.Width(), rcWnd.Height() + CY_LEADER);
	
	//# Combine the regions
	CombineRgn(*hRegion, CaptionRegion.operator HRGN(), LeaderRegion.operator HRGN(), RGN_OR);
	
	//# Set the window size
	if (Size != NULL)
	{
		Size->cx	= rcWnd.Width();
		Size->cy	= rcWnd.Height() + CY_LEADER;
	}
	
	LeaderRegion.DeleteObject();
	CaptionRegion.DeleteObject();
	
	return TRUE;
}

void CDrInfoTip::OnTimer( UINT nIDEvent ) 
{
	HRGN	hRegion;
	CSize	WindowSize;
	CDC		*pDC;
	CPoint	ptCursor;
	
	switch (nIDEvent)
	{
		/////////////////////////////////////////////////////////////////////
		// Show the tip window
		/////////////////////////////////////////////////////////////////////
	case timerShow:
		KillTimer(m_nTimer);
		
		pDC = GetDC();
		GetWindowRegion(pDC, &hRegion, &WindowSize);
		ReleaseDC(pDC);
		
		::SetWindowRgn(m_hWnd, hRegion, TRUE);
		
		if (m_strShowType == "BASE")
			SetWindowPos(NULL, 
			m_ptOrigin.x, 
			m_ptOrigin.y + 20, 
			WindowSize.cx, 
			WindowSize.cy + 20, 
			SWP_NOZORDER | SWP_SHOWWINDOW);
		else if (m_strShowType == "UP")
			SetWindowPos(NULL, 
			m_ptOrigin.x - WindowSize.cx + CX_OFFSET, 
			m_ptOrigin.y - WindowSize.cy + CY_ROUNDED, 
			WindowSize.cx, 
			WindowSize.cy, 
			SWP_NOZORDER | SWP_SHOWWINDOW);
		else if (m_strShowType == "DOWN")		
		{
			SetWindowPos(NULL, 
				m_ptOrigin.x - WindowSize.cx + CX_OFFSET + m_nTextMaxLength,
				m_ptOrigin.y,
				WindowSize.cx, 
				WindowSize.cy + CY_ROUNDED,
				SWP_NOZORDER | SWP_SHOWWINDOW);
		}
		Invalidate();
		
		m_nTimer = SetTimer(timerHide, TIMER_HIDE, NULL);
		break;
		/////////////////////////////////////////////////////////////////////
		// Hide the tip window
		/////////////////////////////////////////////////////////////////////
	case timerHide:
		GetCursorPos(&ptCursor);
		
		//if (ptCursor != m_ptOrigin)
		int nOffset = 10;
		if (((ptCursor.x - nOffset <= m_ptOrigin.x) && (ptCursor.x + nOffset >= m_ptOrigin.x)) &&
			((ptCursor.y - nOffset <= m_ptOrigin.y) && (ptCursor.y + nOffset >= m_ptOrigin.y)))
		{
			
		}
		else
		{
			KillTimer(m_nTimer);
			ShowWindow(SW_HIDE);
		}
		
		break;
	}
	
	CWnd::OnTimer(nIDEvent);
}

void CDrInfoTip::RelayEvent(LPMSG lpMsg)
{
	CPoint			point;
	CWnd			*pWindow;
	CString			szTooltipText;
	TipToolInfo		* Info;
	
	switch(lpMsg->message)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
		ShowWindow(SW_HIDE);
		break;
	case WM_MOUSEMOVE:
		GetCursorPos(&point);
		
		if (point != m_ptOrigin)
		{
			// Find the tool
			pWindow = WindowFromPoint(point);
			if (pWindow != NULL)
			{
				if (m_ToolMap.Lookup(pWindow->m_hWnd, Info))
				{
					// Display the tooltip
					m_ptOrigin = point;
					SetIcon(Info->hIcon);
					Show(Info->szText, &point);
				}
			}
		}
		
		//if (point != m_ptOrigin)
		int nOffset = 10;
		if (((point.x - nOffset <= m_ptOrigin.x) && (point.x + nOffset >= m_ptOrigin.x)) &&
			((point.y - nOffset <= m_ptOrigin.y) && (point.y + nOffset >= m_ptOrigin.y)))
		{
		}
		else
			ShowWindow(SW_HIDE);
		
		break;
	}
}

void CDrInfoTip::SetIcon(HICON hIcon) 
{
	ICONINFO	IconInfo;
	
	m_hIcon = hIcon; 
	
	if (hIcon == NULL)
	{
		m_IconSize = CSize(0, 0);
		return;
	}
	
	ZeroMemory(&IconInfo, sizeof(ICONINFO));
	::GetIconInfo(m_hIcon, &IconInfo);
	
	m_IconSize.cx = (BYTE)(IconInfo.xHotspot * 2);
	m_IconSize.cy = (BYTE)(IconInfo.yHotspot * 2);
    
	::DeleteObject(IconInfo.hbmMask);
	::DeleteObject(IconInfo.hbmColor);
	
	if (IsWindow(m_hWnd))
		RedrawWindow();
}

void CDrInfoTip::AddTool(CWnd *pWnd, LPCTSTR szTooltipText, HICON hIcon /* = NULL */)
{
	ASSERT(pWnd != NULL);
	
	TipToolInfo * Info;
	Info = new TipToolInfo;
	
	Info->szText = szTooltipText;
	Info->hIcon	= hIcon;
	
	m_ToolMap.SetAt(pWnd->m_hWnd, Info);
}

void CDrInfoTip::UpdateToolTip(CWnd *pWnd, LPCTSTR szTooltipText, 
							   int nTextMaxLength,
							   HICON hIcon /* = NULL */)
{
	TipToolInfo* Info;
	
	CClientDC	dc(this);
	TEXTMETRIC	tm;
	
	dc.GetTextMetrics(&tm);
	int nFontWidth	=	tm.tmAveCharWidth;
	m_nTextMaxLength = (int)((nTextMaxLength * nFontWidth) / m_nIndicatePos);
	
	if (m_ToolMap.Lookup(pWnd->m_hWnd, Info))
	{
		Info->hIcon	= hIcon;
		if ((m_strShowType == "UP") || (m_strShowType == "BASE"))
			Info->szText = szTooltipText;
		else if (m_strShowType == "DOWN")
			Info->szText = "\n" + (CString)szTooltipText;
	}	
}

BOOL CDrInfoTip::GetShowState()
{
	BOOL bIsState = FALSE;
	if(GetSafeHwnd())
		bIsState = IsWindowVisible();
	return bIsState;
}

void CDrInfoTip::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	CRect rect;
	GetClientRect(rect);

	if (m_strShowType == "BASE")
	{
		rect.left = rect.right - 20;
		rect.right = rect.left + 15;
		rect.top = rect.top + 4;
		rect.bottom = rect.top + 15;
	}
	else if (m_strShowType == "UP")
	{
		rect.left = rect.right - 20;
		rect.right = rect.left + 15;
		rect.top = rect.top + 4;
		rect.bottom = rect.top + 15;
	}
	else if (m_strShowType == "DOWN")	
	{
		rect.left = rect.right - 20;
		rect.right = rect.left + 15;
		rect.top = rect.top + 14;
		rect.bottom = rect.top + 15;	
	}
	if(m_nID != 0)
		m_btnExit.MoveWindow(&rect);
}

void CDrInfoTip::OnBtnExit(UINT nID) 
{
	ShowWindow(SW_HIDE);
}

void CDrInfoTip::ShowTextBox()
{
	HRGN	hRegion;
	CSize	WindowSize;
	CDC		*pDC;
	CPoint	ptCursor;
		
	pDC = GetDC();
	GetWindowRegion(pDC, &hRegion, &WindowSize);
	ReleaseDC(pDC);
	
	::SetWindowRgn(m_hWnd, hRegion, TRUE);
	
	if (m_strShowType == "BASE")
		SetWindowPos(&wndNoTopMost      , 
		m_ptOrigin.x, 
		m_ptOrigin.y + 20, 
		WindowSize.cx, 
		WindowSize.cy + 20, 
		SWP_NOACTIVATE | SWP_SHOWWINDOW);
	else if (m_strShowType == "UP")
		SetWindowPos(&wndNoTopMost      , 
		m_ptOrigin.x - WindowSize.cx + CX_OFFSET, 
		m_ptOrigin.y - WindowSize.cy + CY_ROUNDED, 
		WindowSize.cx, 
		WindowSize.cy, 
		SWP_NOACTIVATE | SWP_SHOWWINDOW);
	else if (m_strShowType == "DOWN")		
	{
		SetWindowPos(&wndNoTopMost      , 
			m_ptOrigin.x - WindowSize.cx + CX_OFFSET + m_nTextMaxLength,
			m_ptOrigin.y,
			WindowSize.cx, 
			WindowSize.cy + CY_ROUNDED,
			SWP_NOACTIVATE | SWP_SHOWWINDOW);
	}
	Invalidate();
}



// CoolDialogBar.cpp : implementation file
//

#include "stdafx.h"
#include "afxpriv.h"    // for CDockContext
#include "CoolDialogBar.h"
#include "Dbar0.h"
#include "MenuSearch.h"
#include "../ctrl/fxTab.h"
#include "../../h/axisvar.h"
#include "../../axis/axMsg.hxx"

#define hHEAD		60
#define ID_TAB		0x01

#define COLOR_TAB_BK		22
#define COLOR_TAB_BK_TEXT	63
#define COLOR_TAB_SEL		23
#define COLOR_TAB_SEL_TEXT	63
#define COLOR_TAB_BKGRDOUND	27

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCoolDialogBar

CCoolDialogBar::CCoolDialogBar(CWnd* parent, bool (*callback)(int, WPARAM, LPARAM), CString home, class CTMenu* menu) : 
m_clrBtnHilight(::GetSysColor(COLOR_BTNHILIGHT)),
m_clrBtnShadow(::GetSysColor(COLOR_BTNSHADOW))
{
	m_parent	= parent;
	m_axiscall	= callback;
	m_home		= home;
	m_menu		= menu;

	m_cxGripper	= 15;
	m_cxEdge	= 4;
	m_cxBorder	= 3;
	m_cyBorder	= 3;		
	m_cCaptionSize	= GetSystemMetrics(SM_CYSMCAPTION);
	m_cMinWidth	= VARWIDTH + m_cxBorder*2;
	m_cMinHeight	= BANDHEIGHT*3 + CONTROLH + BANDGAP*2 + m_cCaptionSize + m_cyBorder*4;
	
	m_sizeMin	= CSize(m_cMinWidth, m_cMinHeight);
	m_sizeHorz	= CSize(m_cMinWidth, m_cMinHeight);
	m_sizeVert	= CSize(m_cMinWidth, m_cMinHeight);
	m_sizeFloat	= CSize(m_cMinWidth, m_cMinHeight*2);
	m_bTracking	= FALSE;
	m_bInRecalcNC	= FALSE;

	m_cDialog	= NULL;
	m_mDialog	= NULL;
	m_sDialog	= NULL;
	m_pTab		= NULL;

	m_tab		= 0;

	m_brushBkgd.CreateSolidBrush(RGB(242, 242, 242));

	CString	fontName;
	fontName.LoadString(STSM_FONTNAME_DEFAULT);
	m_font.CreatePointFont(90, fontName);

	m_bFloat = false;

	m_nDockBarID = AFX_IDW_DOCKBAR_LEFT;
}

CCoolDialogBar::~CCoolDialogBar()
{
	if (m_cDialog)	delete m_cDialog;
	if (m_mDialog)	delete m_mDialog;
	if (m_sDialog)	delete m_sDialog;
	if (m_pTab)	delete m_pTab;
}

BEGIN_MESSAGE_MAP(CCoolDialogBar, CControlBar)
    //{{AFX_MSG_MAP(CCoolDialogBar)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_NCPAINT()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCHITTEST()
	ON_WM_NCCALCSIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_CAPTURECHANGED()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_PAINT()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_NOTIFY(TCN_SELCHANGE, ID_TAB, OnTabChange)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCoolDialogBar message handlers

void CCoolDialogBar::OnUpdateCmdUI(class CFrameWnd *pTarget, int bDisableIfNoHndler)
{
	UpdateDialogControls(pTarget, bDisableIfNoHndler);
}

BOOL CCoolDialogBar::Create(CWnd* pParentWnd, UINT nID, DWORD dwStyle) 
{
	ASSERT_VALID(pParentWnd);   // must have a parent
	ASSERT (!((dwStyle & CBRS_SIZE_FIXED) && (dwStyle & CBRS_SIZE_DYNAMIC)));

	// save the style -- AMENDED by Holger Thiele - Thankyou
	m_dwStyle = dwStyle & CBRS_ALL;

	// create the base window
	CString wndclass = AfxRegisterWndClass(CS_DBLCLKS, LoadCursor(NULL, IDC_ARROW), m_brushBkgd, 0);
	if (!CWnd::Create(wndclass, "네비게이터", dwStyle|WS_CLIPCHILDREN,
		CRect(0,0,m_cMinWidth,m_cMinHeight), pParentWnd, 0))
		return FALSE;

	// create the navigator dialog
	m_cDialog = new CDbar0(m_axiscall, m_home, m_parent);
	m_cDialog->Create(nID, this);

	m_cDialog->SetCBar(this);
	m_cDialog->change_BandInfo();
	m_color = GetColor(-1);

	// create the menu dialog
	m_mDialog = new CMenuSearch(m_menu, m_home, m_axiscall);
	m_mDialog->Create(IDD_MENUSEARCH, this);

	// create the memo window
	m_sDialog = new CWnd;
	m_sDialog->Create(NULL, "memo", WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 0), this, -1);

	CString path;
	m_pTab = new CfxTab();
	m_pTab->Create(CRect(0, 0, 0, 0), this, ID_TAB);
	m_pTab->SetColor(GetColor(COLOR_TAB_BK_TEXT), GetColor(COLOR_TAB_BK), GetColor(COLOR_TAB_SEL_TEXT), GetColor(COLOR_TAB_SEL), GetColor(COLOR_TAB_BKGRDOUND));
		
	path.Format("%s\\%s", m_home, IMAGEDIR);
	m_pTab->SetImage(path, "L_Sintrotab.bmp", "Sintrotab.bmp", "R_Sintrotab.bmp", "L_introtab.bmp", "introtab.bmp", "R_introtab.bmp", "intortabBG.bmp");

	m_pTab->InsertItem(0, "메뉴");
	m_pTab->InsertItem(1, "일정");
	m_pTab->InsertItem(2, "자동");
	m_pTab->SetStyle(tsIMAGE, false);
	m_pTab->m_textUpDown = 2;

	ShowDialog();

	// use the dialog dimensions as default base dimensions
	m_sizeHorz.cy += m_cxEdge + m_cxBorder;
	m_sizeVert.cx += m_cxEdge + m_cxBorder;

	return TRUE;
}

CSize CCoolDialogBar::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
	CRect rc;

	m_pDockSite->GetControlBar(AFX_IDW_DOCKBAR_TOP)->GetWindowRect(rc);
	int nHorzDockBarWidth = bStretch ? 32767 : rc.Width() + 4;
	m_pDockSite->GetControlBar(AFX_IDW_DOCKBAR_LEFT)->GetWindowRect(rc);
	int nVertDockBarHeight = bStretch ? 32767 : rc.Height() + 4;

	if(IsFloating())
		return CSize(m_cMinWidth, m_sizeFloat.cy);
	else if (bHorz)
		return CSize(m_cMinWidth/*nHorzDockBarWidth*/, m_sizeHorz.cy);
	else
		return CSize(m_cMinWidth/*m_sizeVert.cx*/, nVertDockBarHeight);
}

CSize CCoolDialogBar::CalcDynamicLayout(int nLength, DWORD dwMode)
{
	// Rob Wolpov 10/15/98 Added support for diagonal sizing
	if (IsFloating())
	{
		// Enable diagonal arrow cursor for resizing
		GetParent()->GetParent()->ModifyStyle(MFS_4THICKFRAME,0);
	}    
	if (dwMode & (LM_HORZDOCK | LM_VERTDOCK))
	{
		SetWindowPos(NULL, 0, 0, 0, 0,
			SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER |
			SWP_NOACTIVATE | SWP_FRAMECHANGED);
		m_pDockSite->RecalcLayout();
	    return CControlBar::CalcDynamicLayout(nLength,dwMode);
	}

	if (dwMode & LM_MRUWIDTH || dwMode & LM_COMMIT)
		return CSize(m_cMinWidth, m_sizeFloat.cy);

	// Rob Wolpov 10/15/98 Added support for diagonal sizing
	if (IsFloating())
	{
		RECT	window_rect;
		POINT	cursor_pt;

		GetCursorPos(&cursor_pt);
		GetParent()->GetParent()->GetWindowRect(&window_rect);

		switch (m_pDockContext->m_nHitTest)
		{
		case HTTOPLEFT:
			m_sizeFloat.cx = max(window_rect.right - cursor_pt.x,
				m_cMinWidth) - m_cxBorder;
			m_sizeFloat.cy = max(window_rect.bottom - m_cCaptionSize - 
				cursor_pt.y,m_cMinHeight) - 1;
			m_pDockContext->m_rectFrameDragHorz.top = min(cursor_pt.y,
				window_rect.bottom - m_cCaptionSize - m_cMinHeight) - 
				m_cyBorder;
			m_pDockContext->m_rectFrameDragHorz.left = min(cursor_pt.x,
				window_rect.right - m_cMinWidth) - 1;
			return m_sizeFloat;

		case HTTOPRIGHT:
			m_sizeFloat.cx = max(cursor_pt.x - window_rect.left,
				m_cMinWidth);
			m_sizeFloat.cy = max(window_rect.bottom - m_cCaptionSize - 
				cursor_pt.y,m_cMinHeight) - 1;
			m_pDockContext->m_rectFrameDragHorz.top = min(cursor_pt.y,
				window_rect.bottom - m_cCaptionSize - m_cMinHeight) - 
				m_cyBorder;
			return m_sizeFloat;
			
		case HTBOTTOMLEFT:
			m_sizeFloat.cx = max(window_rect.right - cursor_pt.x,
				m_cMinWidth) - m_cxBorder;
			m_sizeFloat.cy = max(cursor_pt.y - window_rect.top - 
				m_cCaptionSize,m_cMinHeight);
			m_pDockContext->m_rectFrameDragHorz.left = min(cursor_pt.x,
				window_rect.right - m_cMinWidth) - 1;
			return m_sizeFloat;
			
		case HTBOTTOMRIGHT:
			m_sizeFloat.cx = max(cursor_pt.x - window_rect.left,
				m_cMinWidth);
			m_sizeFloat.cy = max(cursor_pt.y - window_rect.top - 
				m_cCaptionSize,m_cMinHeight);
			return m_sizeFloat;
		}
	}
	
	if (dwMode & LM_LENGTHY)
		return CSize(m_cMinWidth, m_sizeFloat.cy = max(m_sizeMin.cy, nLength));
	else
		return CSize(m_cMinWidth, m_sizeFloat.cy);
}

void CCoolDialogBar::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CControlBar::OnWindowPosChanged(lpwndpos);

	if(!::IsWindow(m_hWnd) || m_cDialog == NULL || m_mDialog == NULL || m_sDialog == NULL)
		return;
	if(!::IsWindow(m_cDialog->m_hWnd) || !::IsWindow(m_mDialog->m_hWnd) || !::IsWindow(m_sDialog->m_hWnd))
		return;

	if (m_bInRecalcNC) 
	{
		CRect rc;
		GetClientRect(rc);
		rc.top = hHEAD;
		m_cDialog->MoveWindow(rc);
		m_mDialog->MoveWindow(rc);
		m_sDialog->MoveWindow(rc);

		return;
	}

	// Find on which side are we docked
	UINT nDockBarID = GetParent()->GetDlgCtrlID();

	// Return if dropped at same location
	if (nDockBarID == m_nDockBarID // no docking side change
		&& (lpwndpos->flags & SWP_NOSIZE) // no size change
		&& ((m_dwStyle & CBRS_BORDER_ANY) != CBRS_BORDER_LEFT))//ANY))
		return; 

	m_nDockBarID = nDockBarID;

	// Force recalc the non-client area
	m_bInRecalcNC = TRUE;
	SetWindowPos(NULL, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE|SWP_NOZORDER|SWP_FRAMECHANGED);
	m_bInRecalcNC = FALSE;
}

BOOL CCoolDialogBar::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if ((nHitTest != HTSIZE) || m_bTracking)
		return CControlBar::OnSetCursor(pWnd, nHitTest, message);

	if (IsHorz())
		SetCursor(LoadCursor(NULL, IDC_SIZENS));
	else
		SetCursor(LoadCursor(NULL, IDC_SIZEWE));
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////
// Mouse Handling
//
void CCoolDialogBar::OnLButtonUp(UINT nFlags, CPoint point) 
{
    if (!m_bTracking)
        CControlBar::OnLButtonUp(nFlags, point);
    else
    {
        ClientToWnd(point);
        StopTracking(TRUE);
    }
}

void CCoolDialogBar::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (IsFloating() || !m_bTracking)
	{
		CControlBar::OnMouseMove(nFlags, point);
		return;
	}

	CPoint cpt = m_rectTracker.CenterPoint();
	ClientToWnd(point);

	if (IsHorz())
	{
		if (cpt.y != point.y)
		{
			OnInvertTracker(m_rectTracker);
			m_rectTracker.OffsetRect(0, point.y - cpt.y);
			OnInvertTracker(m_rectTracker);
		}
	}
	else 
	{
		if (cpt.x != point.x)
		{
			OnInvertTracker(m_rectTracker);
			m_rectTracker.OffsetRect(point.x - cpt.x, 0);
			OnInvertTracker(m_rectTracker);
		}
	}
}

void CCoolDialogBar::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
    // Compute the rectangle of the mobile edge
	GetWindowRect(m_rectBorder);
	m_rectBorder = CRect(0, 0, m_rectBorder.Width(), m_rectBorder.Height());

	DWORD dwBorderStyle = m_dwStyle | CBRS_BORDER_LEFT;//ANY;

	switch(m_nDockBarID)
	{
	case AFX_IDW_DOCKBAR_TOP:
		dwBorderStyle &= ~CBRS_BORDER_BOTTOM;
		lpncsp->rgrc[0].left += m_cxGripper;
		lpncsp->rgrc[0].bottom += -m_cxEdge;
		lpncsp->rgrc[0].top += m_cxBorder;
		lpncsp->rgrc[0].right += -m_cxBorder;
		m_rectBorder.top = m_rectBorder.bottom - m_cxEdge;
		break;
	case AFX_IDW_DOCKBAR_BOTTOM:
		dwBorderStyle &= ~CBRS_BORDER_TOP;
		lpncsp->rgrc[0].left += m_cxGripper;
		lpncsp->rgrc[0].top += m_cxEdge;
		lpncsp->rgrc[0].bottom += -m_cxBorder;
		lpncsp->rgrc[0].right += -m_cxBorder;
		m_rectBorder.bottom = m_rectBorder.top + m_cxEdge;
		break;
	case AFX_IDW_DOCKBAR_LEFT:
		dwBorderStyle &= ~CBRS_BORDER_RIGHT;
		lpncsp->rgrc[0].right += -m_cxEdge;
		lpncsp->rgrc[0].left += m_cxBorder;
		lpncsp->rgrc[0].bottom += -m_cxBorder;
		lpncsp->rgrc[0].top += m_cxGripper;
		m_rectBorder.left = m_rectBorder.right - m_cxEdge;
		break;
	case AFX_IDW_DOCKBAR_RIGHT:
		dwBorderStyle &= ~CBRS_BORDER_LEFT;
		lpncsp->rgrc[0].left += m_cxEdge;
		lpncsp->rgrc[0].right += -m_cxBorder;
		lpncsp->rgrc[0].bottom += -m_cxBorder;
		lpncsp->rgrc[0].top += m_cxGripper;
		m_rectBorder.right = m_rectBorder.left + m_cxEdge;
		break;
	default:
		m_rectBorder.SetRectEmpty();
		break;
	}

	SetBarStyle(dwBorderStyle);
}

void CCoolDialogBar::OnNcPaint() 
{
	EraseNonClient();

	CWindowDC dc(this);

	DrawGripper(dc);

	CRect pRect;
	GetClientRect(&pRect);
	InvalidateRect(&pRect, TRUE);
}

void CCoolDialogBar::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
	if (m_bTracking) 
		return;

	if((nHitTest == HTSYSMENU) && !IsFloating())
	{
		GetDockingFrame()->ShowControlBar(this, FALSE, FALSE);
		(*m_axiscall)(AXI_TOOLTIPCHANGE, 0, 0);
	}
	else if ((nHitTest == HTMINBUTTON) && !IsFloating())
		m_pDockContext->ToggleDocking();
	else if ((nHitTest == HTCAPTION) && !IsFloating() && (m_pDockBar != NULL))
	{
		// start the drag
		ASSERT(m_pDockContext != NULL);
		m_pDockContext->StartDrag(point);
	}
	else if ((nHitTest == HTSIZE) && !IsFloating())
		StartTracking();
	else    
		CControlBar::OnNcLButtonDown(nHitTest, point);
}

LRESULT CCoolDialogBar::OnNcHitTest(CPoint point) 
{
	if (IsFloating())
	{
		int ret = (int)CControlBar::OnNcHitTest(point);
TRACE("NcHitTest = %d\n", ret);
		return ret;
	}

	CRect rc;
	GetWindowRect(rc);
	point.Offset(-rc.left, -rc.top);
	if(m_rectClose.PtInRect(point))
		return HTSYSMENU;
	else if (m_rectUndock.PtInRect(point))
		return HTMINBUTTON;
	else if (m_rectGripper.PtInRect(point))
		return HTCAPTION;
	else if (m_rectBorder.PtInRect(point))
		return HTSIZE;
	else
		return CControlBar::OnNcHitTest(point);
}

void CCoolDialogBar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// only start dragging if clicked in "void" space
	if (m_pDockBar != NULL)
	{
		// start the drag
		ASSERT(m_pDockContext != NULL);
		ClientToScreen(&point);
//		m_pDockContext->StartDrag(point);
	}
	else
	{
		CWnd::OnLButtonDown(nFlags, point);
	}
}

void CCoolDialogBar::OnLButtonDblClk(UINT nFlags, CPoint point) 
{

	CWnd::OnLButtonDblClk(nFlags, point);
}

void CCoolDialogBar::StartTracking()
{
	SetCapture();

	// make sure no updates are pending
	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_UPDATENOW);
	m_pDockSite->LockWindowUpdate();

	m_ptOld = m_rectBorder.CenterPoint();
	m_bTracking = TRUE;

	m_rectTracker = m_rectBorder;
	if (!IsHorz()) m_rectTracker.bottom -= 4;

	OnInvertTracker(m_rectTracker);
}

void CCoolDialogBar::OnCaptureChanged(CWnd *pWnd) 
{
	if (m_bTracking && pWnd != this)
		StopTracking(FALSE); // cancel tracking

	CControlBar::OnCaptureChanged(pWnd);
}

void CCoolDialogBar::StopTracking(BOOL bAccept)
{
	OnInvertTracker(m_rectTracker);
	m_pDockSite->UnlockWindowUpdate();
	m_bTracking = FALSE;
	ReleaseCapture();

	if (!bAccept) return;

	int maxsize, minsize, newsize;
	CRect rcc;
	m_pDockSite->GetWindowRect(rcc);

	newsize = IsHorz() ? m_sizeHorz.cy : m_sizeVert.cx;
	maxsize = newsize + (IsHorz() ? rcc.Height() : rcc.Width());
	minsize = IsHorz() ? m_sizeMin.cy : m_sizeMin.cx;

	CPoint point = m_rectTracker.CenterPoint();
	switch (m_nDockBarID)
	{
	case AFX_IDW_DOCKBAR_TOP:	newsize += point.y - m_ptOld.y; break;
	case AFX_IDW_DOCKBAR_BOTTOM:	newsize += -point.y + m_ptOld.y; break;
	case AFX_IDW_DOCKBAR_LEFT:	newsize += point.x - m_ptOld.x; break;
	case AFX_IDW_DOCKBAR_RIGHT:	newsize += -point.x + m_ptOld.x; break;
	}

	newsize = max(minsize, min(maxsize, newsize));

	if (IsHorz())
		m_sizeHorz.cy = newsize;
	else	m_sizeVert.cx = newsize;

	m_pDockSite->RecalcLayout();
}

void CCoolDialogBar::OnInvertTracker(const CRect& rect)
{
	ASSERT_VALID(this);
	ASSERT(!rect.IsRectEmpty());
	ASSERT(m_bTracking);

	CRect rct = rect, rcc, rcf;
	GetWindowRect(rcc);
	m_pDockSite->GetWindowRect(rcf);

	rct.OffsetRect(rcc.left - rcf.left, rcc.top - rcf.top);
	rct.DeflateRect(1, 1);

	CDC *pDC = m_pDockSite->GetDCEx(NULL,
	DCX_WINDOW|DCX_CACHE|DCX_LOCKWINDOWUPDATE);

	CBrush* pBrush = CDC::GetHalftoneBrush();
	HBRUSH hOldBrush = NULL;
	if (pBrush != NULL)
		hOldBrush = (HBRUSH)SelectObject(pDC->m_hDC, pBrush->m_hObject);

	pDC->PatBlt(rct.left, rct.top, rct.Width(), rct.Height(), PATINVERT);

	if (hOldBrush != NULL)	SelectObject(pDC->m_hDC, hOldBrush);

	m_pDockSite->ReleaseDC(pDC);
}

BOOL CCoolDialogBar::IsHorz() const
{
	return (m_nDockBarID == AFX_IDW_DOCKBAR_TOP || m_nDockBarID == AFX_IDW_DOCKBAR_BOTTOM);
}

CPoint& CCoolDialogBar::ClientToWnd(CPoint& point)
{
	if (m_nDockBarID == AFX_IDW_DOCKBAR_BOTTOM)
		point.y += m_cxEdge;
	else if (m_nDockBarID == AFX_IDW_DOCKBAR_RIGHT)
		point.x += m_cxEdge;

	return point;
}

void CCoolDialogBar::DrawGripper(CDC & dc)
{
	// no gripper if floating
	if( m_dwStyle & CBRS_FLOATING )
		return;

	const int	gH = 20, gW = 40;
	// -==HACK==-
	// in order to calculate the client area properly after docking,
	// the client area must be recalculated twice (I have no idea why)
	m_pDockSite->RecalcLayout();
	// -==END HACK==-

	CRect gripper;
	GetWindowRect( gripper );
	ScreenToClient( gripper );
	gripper.OffsetRect( -gripper.left, -gripper.top );

	if( m_dwStyle & CBRS_ORIENT_HORZ )
	{
		// gripper at left
		m_rectGripper.top	= gripper.top + 40;
		m_rectGripper.bottom	= gripper.bottom;
		m_rectGripper.left	= gripper.left;
		m_rectGripper.right	= gripper.left + gH;

		// draw close box
		m_rectClose.left = gripper.left + 7;
		m_rectClose.right = m_rectClose.left + 12;
		m_rectClose.top = gripper.top + 10;
		m_rectClose.bottom = m_rectClose.top + 12;
		dc.DrawFrameControl(m_rectClose, DFC_CAPTION, DFCS_CAPTIONCLOSE);

		// draw docking toggle box
		m_rectUndock = m_rectClose;
		m_rectUndock.OffsetRect(0,13);
		dc.DrawFrameControl(m_rectUndock, DFC_CAPTION, DFCS_CAPTIONMAX);

		gripper.top += 38;
		gripper.bottom -= 10;
		gripper.left += 10;
		gripper.right = gripper.left+3;
		gripper.OffsetRect(0, -4);	
		dc.Draw3dRect( gripper, m_clrBtnHilight, m_clrBtnShadow );

		gripper.OffsetRect(4, 0);
		dc.Draw3dRect( gripper, m_clrBtnHilight, m_clrBtnShadow );
	}
	else
	{
		// gripper at top
		m_rectGripper.top	= gripper.top;
		m_rectGripper.bottom	= gripper.top + gH;
		m_rectGripper.left	= gripper.left;
		m_rectGripper.right	= gripper.right - 40;

		// draw close box
		m_rectClose.right = gripper.right - 10;
		m_rectClose.left = m_rectClose.right - 12;
		m_rectClose.top = gripper.top + 6;
		m_rectClose.bottom = m_rectClose.top + 12;
		m_rectClose.OffsetRect(2, -2);
		dc.DrawFrameControl(m_rectClose, DFC_CAPTION, DFCS_CAPTIONCLOSE);


		// draw docking toggle box
		m_rectUndock = m_rectClose;
		m_rectUndock.OffsetRect(-13,0);
		dc.DrawFrameControl(m_rectUndock, DFC_CAPTION, DFCS_CAPTIONMAX);

		gripper.right -= 38;
		gripper.left += 10;
		gripper.top += 10;
		gripper.bottom = gripper.top+3;
		gripper.OffsetRect(0, -4);
		dc.Draw3dRect( gripper, m_clrBtnHilight, m_clrBtnShadow );

		gripper.OffsetRect(0, 4);
		dc.Draw3dRect( gripper, m_clrBtnHilight, m_clrBtnShadow );
	}

}

void CCoolDialogBar::OnNcLButtonDblClk(UINT nHitTest, CPoint point) 
{
	if ((m_pDockBar != NULL) && (nHitTest == HTCAPTION))
	{
		// toggle docking
		ASSERT(m_pDockContext != NULL);
		m_pDockContext->ToggleDocking();
	}
	else
	{
		CWnd::OnNcLButtonDblClk(nHitTest, point);
	}
}

//////////////////////////////////////////////////
void CCoolDialogBar::make_Ctrl()
{
	m_cDialog->make_Ctrl();
}

void CCoolDialogBar::change_BandInfo()
{
	m_cDialog->change_BandInfo();
	m_color = GetColor(-1);

	Invalidate();
}

void CCoolDialogBar::set_MenuInfo(class CTMenu* menu, CString user)
{
	m_cDialog->set_MenuInfo(menu, user);
}

void CCoolDialogBar::reload_Code()
{
	m_cDialog->reload_Code();
	m_tab = 2;
	m_pTab->SetCurSel(2);
	ShowDialog();
	OnNcPaint();
}

CWnd* CCoolDialogBar::GetCategoryWnd()
{
	return (CWnd*) m_cDialog->GetCategoryWnd();
}

CWnd* CCoolDialogBar::GetMemoWnd()
{
	return m_sDialog;
}

void CCoolDialogBar::ShowDialog()
{
	if (!m_mDialog || !m_cDialog || !m_sDialog)	return;

	switch (m_tab)
	{
	case 0:
		m_mDialog->ShowWindow(SW_SHOW);
		m_cDialog->ShowWindow(SW_HIDE);
		m_sDialog->ShowWindow(SW_HIDE);
		break;
	case 1:
		m_mDialog->ShowWindow(SW_HIDE);
		m_cDialog->ShowWindow(SW_HIDE);
		m_sDialog->ShowWindow(SW_SHOW);
		break;
	case 2:
		m_mDialog->ShowWindow(SW_HIDE);
		m_cDialog->ShowWindow(SW_SHOW);
		m_sDialog->ShowWindow(SW_HIDE);
		break;
	}
}

void CCoolDialogBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	int bkMode = dc.SetBkMode(TRANSPARENT);

	CString	fontName;
	fontName.LoadString(STSM_FONTNAME_DEFAULT);
	CFont	font;
	font.CreatePointFont(90, fontName);
	CFont*	pFont = dc.SelectObject(&font);

	CRect	cRc, tRc;
	GetClientRect(cRc);
	cRc.DeflateRect(0, BANDGAP, BANDGAP-1, BANDGAP-1);
	tRc.CopyRect(&cRc);
	tRc.bottom = tRc.top + BANDHEIGHT;

	dc.FillSolidRect(tRc, m_color);
	COLORREF color = dc.SetTextColor(RGB(255, 255, 255));
	dc.DrawText("네 비 게 이 터", tRc, DT_VCENTER|DT_CENTER|DT_SINGLELINE);
	dc.Draw3dRect(tRc, RGB(255, 255, 255), RGB(108, 108, 108));

	dc.SetTextColor(color);
	dc.SetBkMode(bkMode);
	dc.SelectObject(pFont);
}

void CCoolDialogBar::OnSize(UINT nType, int cx, int cy) 
{
	CControlBar::OnSize(nType, cx, cy);

	if (!m_pTab)	return;

	if (IsFloating())
	{
		CRect	cRc, tRc;
		GetClientRect(cRc);
		cRc.DeflateRect(2, BANDGAP, BANDGAP-1, BANDGAP-1);
		tRc.CopyRect(&cRc);
		tRc.bottom = tRc.top + BANDHEIGHT;
		
		tRc.OffsetRect(0, BANDHEIGHT);
		tRc.bottom = tRc.top + 30;
		m_pTab->MoveWindow(tRc);

		m_bFloat = true;
	}
	else
	{
		CRect	cRc, tRc;
		GetClientRect(cRc);
		cRc.DeflateRect(0, BANDGAP, 0, BANDGAP-1);
		tRc.CopyRect(&cRc);
		tRc.bottom = tRc.top + BANDHEIGHT;
		
		tRc.OffsetRect(0, BANDHEIGHT);
		tRc.bottom = tRc.top + 30;
		m_pTab->MoveWindow(tRc);

		m_bFloat = false;
	}
}

void CCoolDialogBar::OnTabChange(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_tab = m_pTab->GetCurSel();

	ShowDialog();
}

COLORREF CCoolDialogBar::GetColor(int index)
{
	int		pos;
	char		buffer[128];
	CString		file, tmpS, dat, section, key;
	CStringArray	ary;

	file.Format("%s\\%s\\%s", m_home, TABDIR, PALETTE);
	GetPrivateProfileString(GENERALSN, "Palette", "Palette", buffer, sizeof(buffer), file);

	section = buffer;
	if (index < 0)
		key = "RGB";
	else
		key.Format("RGB%03d", index);
	DWORD dw = GetPrivateProfileString(section, key, "", buffer, sizeof(buffer), file);
	if (dw <= 0)
	{
		if (!section.Compare("Palette"))
			return RGB(215, 215, 215);
		section = _T("Palette");
		dw = GetPrivateProfileString(section, key, "", buffer, sizeof(buffer), file);
		if (dw <= 0)	return RGB(215, 215, 215);
	}

	dat = buffer;
	for (; !dat.IsEmpty();)
	{
		pos = dat.Find(',');
		if (pos == -1)
		{
			ary.Add(dat);
			break;
		}
		tmpS = dat.Left(pos++);
		dat = dat.Mid(pos);
		ary.Add(tmpS);
	}

	if (ary.GetSize() < 3)
		return RGB(215, 215, 215);
	return RGB(atoi(ary.GetAt(0)), atoi(ary.GetAt(1)), atoi(ary.GetAt(2)));
}
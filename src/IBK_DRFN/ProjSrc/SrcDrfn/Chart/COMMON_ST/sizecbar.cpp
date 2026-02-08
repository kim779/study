/////////////////////////////////////////////////////////////////////////
// Copyright (C) 1998, 1999 by Cristi Posea
// All rights reserved
//
// Use and distribute freely, except: don't remove my name from the
// source or documentation (don't take credit for my work), mark your
// changes (don't get me blamed for your possible bugs), don't alter
// or remove this notice.
// No warrantee of any kind, express or implied, is included with this
// software; use at your own risk, responsibility for damages (if any) to
// anyone resulting from the use of this software rests entirely with the
// user.
//
// This class is intended to be used as a base class. Do not simply add
// your code to this file - instead create a new class derived from
// CSizingControlBar and put there what you need.
// Modify this file only to fix bugs, and don't forget to send me a copy.
//
// Send bug reports, bug fixes, enhancements, requests, flames, etc.,
// and I'll try to keep a version up to date.  I can be reached at:
//    cristip@dundas.com
//
// More details at MFC Programmer's SourceBook
// http://www.codeguru.com/docking/docking_window.shtml or search
// www.codeguru.com for my name if the article was moved.
//
/////////////////////////////////////////////////////////////////////////
//
// Acknowledgements:
//  o   Thanks to Harlan R. Seymour (harlans@dundas.com) for his continuous
//      support during development of this code.
//  o   Thanks to Dundas Software for the opportunity to test this code
//      on real-life applications.
//      If you don't know who they are, visit them at www.dundas.com .
//      Their award winning components and development suites are
//      a pile of gold.
//  o   Thanks to Chris Maunder (chrism@dundas.com) who came with the
//      simplest way to query "Show window content while dragging" system
//      setting.
//  o   Thanks to Zafir Anjum (zafir@codeguru.com) for publishing this
//      code on his cool site (www.codeguru.com).
//  o   Some ideas for the gripper came from the CToolBarEx flat toolbar
//      by Joerg Koenig (Joerg.Koenig@rhein-neckar.de). Also he inspired
//      me on writing this notice:) . Thanks, Joerg!
//  o   Thanks to Jakawan Ratiwanich (jack@alpha.fsec.ucf.edu) and to
//      Udo Schaefer (Udo.Schaefer@vcase.de) for the dwStyle bug fix under
//      VC++ 6.0.
//  o   And, of course, many thanks to all of you who used this code,
//      for the invaluable feedback I received.
//      
/////////////////////////////////////////////////////////////////////////


// sizecbar.cpp : implementation file
//

#include "stdafx.h"
#include "sizecbar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////
// CSizingControlBar

CSCBArray CSizingControlBar::m_arrBars; // static member

IMPLEMENT_DYNAMIC(CSizingControlBar, baseCSizingControlBar);

CSizingControlBar::CSizingControlBar()
{
    m_szMin = CSize(33, 10);
    m_szHorz = CSize(200, 200);
    m_szVert = CSize(200, 200);
    m_szFloat = CSize(200, 200);
    m_bTracking = FALSE;
    m_bKeepSize = FALSE;
    m_bParentSizing = FALSE;
	m_bGradient = FALSE;
    m_cxEdge = 5;
	m_cyEdge = 5;
    m_bDragShowContent = FALSE;
    m_nDockBarID = 0;
    m_dwSCBStyle = 0;
	m_bExitBtn = FALSE;
	m_clrStart = RGB(232,232,232);
	m_clrEnd = RGB(199,199,199);
	m_nType = 0;
}

CSizingControlBar::~CSizingControlBar()
{
}

BEGIN_MESSAGE_MAP(CSizingControlBar, baseCSizingControlBar)
    //{{AFX_MSG_MAP(CSizingControlBar)
    ON_WM_CREATE()
    ON_WM_NCPAINT()
    ON_WM_NCCALCSIZE()
    ON_WM_NCHITTEST()
    ON_WM_CAPTURECHANGED()
    ON_WM_SETTINGCHANGE()
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
    ON_WM_NCLBUTTONDOWN()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_RBUTTONDOWN()
    ON_WM_NCLBUTTONUP()
    ON_WM_WINDOWPOSCHANGING()
    ON_WM_PAINT()
    ON_WM_NCMOUSEMOVE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CSizingControlBar::Create(LPCTSTR lpszWindowName, CWnd* pParentWnd,
                               CSize sizeDefault, BOOL bHasGripper,
                               UINT nID, DWORD dwStyle)
{
    // must have a parent
    ASSERT_VALID(pParentWnd);
    // cannot be both fixed and dynamic
    // (CBRS_SIZE_DYNAMIC is used for resizng when floating)
    ASSERT (!((dwStyle & CBRS_SIZE_FIXED) &&
              (dwStyle & CBRS_SIZE_DYNAMIC)));

    m_dwStyle = dwStyle & CBRS_ALL; // save the control bar styles

    m_szHorz = sizeDefault; // set the size members
    m_szVert = sizeDefault;
    m_szFloat = sizeDefault;

    m_cyGripper = bHasGripper ? 20 : 0; // set the gripper width

    // register and create the window - skip CControlBar::Create()
    CString wndclass = ::AfxRegisterWndClass(CS_DBLCLKS,
        ::LoadCursor(NULL, IDC_ARROW),
        ::GetSysColorBrush(COLOR_BTNFACE), 0);

    dwStyle &= ~CBRS_ALL; // keep only the generic window styles
    dwStyle |= WS_CLIPCHILDREN; // prevents flashing
    if (!CWnd::Create(wndclass, lpszWindowName, dwStyle,
        CRect(0, 0, 0, 0), pParentWnd, nID))
        return FALSE;

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////
// CSizingControlBar message handlers

int CSizingControlBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
    if (baseCSizingControlBar::OnCreate(lpCreateStruct) == -1)
        return -1;
    
    // querry SPI_GETDRAGFULLWINDOWS system parameter
    // OnSettingChange() will update m_bDragShowContent
    m_bDragShowContent = FALSE;
    ::SystemParametersInfo(SPI_GETDRAGFULLWINDOWS, 0,
        &m_bDragShowContent, 0);

    m_arrBars.Add(this);        // register
    
//    m_dwSCBStyle |= SCBS_SHOWEDGES;

    return 0;
}

BOOL CSizingControlBar::DestroyWindow() 
{
    int nPos = FindSizingBar(this);
    ASSERT(nPos >= 0);

    m_arrBars.RemoveAt(nPos);   // unregister

    return baseCSizingControlBar::DestroyWindow();
}

const BOOL CSizingControlBar::IsFloating() const
{
    return !IsHorzDocked() && !IsVertDocked();
}

const BOOL CSizingControlBar::IsHorzDocked() const
{
    return (m_nDockBarID == AFX_IDW_DOCKBAR_TOP ||
        m_nDockBarID == AFX_IDW_DOCKBAR_BOTTOM);
}

const BOOL CSizingControlBar::IsVertDocked() const
{
    return (m_nDockBarID == AFX_IDW_DOCKBAR_LEFT ||
        m_nDockBarID == AFX_IDW_DOCKBAR_RIGHT);
}

const BOOL CSizingControlBar::IsSideTracking() const
{
    // don't call this when not tracking
    ASSERT(m_bTracking && !IsFloating());

    return (m_htEdge == HTLEFT || m_htEdge == HTRIGHT) ?
        IsHorzDocked() : IsVertDocked();
}

CSize CSizingControlBar::CalcFixedLayout(BOOL bStretch, BOOL bHorz)
{
    if (bStretch) // the bar is stretched (is not the child of a dockbar)
        if (bHorz)
            return CSize(32767, m_szHorz.cy);
        else
            return CSize(m_szVert.cx, 32767);

    // dirty cast - using CSCBDockBar to access protected CDockBar members
    CSCBDockBar* pDockBar = (CSCBDockBar*) m_pDockBar;

    // force imediate RecalcDelayShow() for all sizing bars on the row
    // with delayShow/delayHide flags set to avoid IsVisible() problems
    CSCBArray arrSCBars;
    GetRowSizingBars(arrSCBars);
    AFX_SIZEPARENTPARAMS layout;
    layout.hDWP = pDockBar->m_bLayoutQuery ?
        NULL : ::BeginDeferWindowPos(arrSCBars.GetSize());
    for (int i = 0; i < arrSCBars.GetSize(); i++)
        arrSCBars[i]->RecalcDelayShow(&layout);
    if (layout.hDWP != NULL)
        ::EndDeferWindowPos(layout.hDWP);

    // get available length
    CRect rc = pDockBar->m_rectLayout;
    if (rc.IsRectEmpty())
        m_pDockSite->GetClientRect(&rc);
    int nLengthAvail = bHorz ? rc.Width() + 2 : rc.Height() - 2;

    if (IsVisible() && !IsFloating() &&
        m_bParentSizing && arrSCBars[0] == this)
        if (NegociateSpace(nLengthAvail, (bHorz != FALSE)))
            AlignControlBars();

    m_bParentSizing = FALSE;
    
    CSize szRet = bHorz ? m_szHorz : m_szVert;
    szRet.cx = max(m_szMin.cx, szRet.cx);
    szRet.cy = max(m_szMin.cy, szRet.cy);

    return szRet;
}

CSize CSizingControlBar::CalcDynamicLayout(int nLength, DWORD dwMode)
{
    if (dwMode & (LM_HORZDOCK | LM_VERTDOCK)) // docked ?
    {
        if (nLength == -1)
            m_bParentSizing = TRUE;

        return baseCSizingControlBar::CalcDynamicLayout(nLength, dwMode);
    }

    if (dwMode & LM_MRUWIDTH) return m_szFloat;
    if (dwMode & LM_COMMIT) return m_szFloat; // already committed

    ((dwMode & LM_LENGTHY) ? m_szFloat.cy : m_szFloat.cx) = nLength;

    m_szFloat.cx = max(m_szFloat.cx, m_szMin.cx);
    m_szFloat.cy = max(m_szFloat.cy, m_szMin.cy);

    return m_szFloat;
}

void CSizingControlBar::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos)
{
    // force non-client recalc if moved or resized
    lpwndpos->flags |= SWP_FRAMECHANGED;

    baseCSizingControlBar::OnWindowPosChanging(lpwndpos);

    // find on which side are we docked
    UINT nOldDockBarID = m_nDockBarID;
    m_nDockBarID = GetParent()->GetDlgCtrlID();

    if (!IsFloating())
        if (lpwndpos->flags & SWP_SHOWWINDOW)
            m_bKeepSize = TRUE;
}

/////////////////////////////////////////////////////////////////////////
// Mouse Handling
//
void CSizingControlBar::OnLButtonDown(UINT nFlags, CPoint point) 
{
    if (m_pDockBar != NULL)
    {
        // start the drag
        ASSERT(m_pDockContext != NULL);
        ClientToScreen(&point);
		m_ptCurrent = point;
        m_pDockContext->StartDrag(point);
    }
    else
        CWnd::OnLButtonDown(nFlags, point);
}

void CSizingControlBar::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
    if (m_pDockBar != NULL)
    {
        // toggle docking
        ASSERT(m_pDockContext != NULL);
        m_pDockContext->ToggleDocking();
    }
    else
        CWnd::OnLButtonDblClk(nFlags, point);
}

void CSizingControlBar::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
    if (IsFloating())
    {
        baseCSizingControlBar::OnNcLButtonDown(nHitTest, point);
        return;
    }

    if (m_bTracking) return;

    if ((nHitTest >= HTSIZEFIRST) && (nHitTest <= HTSIZELAST))
        StartTracking(nHitTest); // sizing edge hit
}

void CSizingControlBar::OnNcLButtonUp(UINT nHitTest, CPoint point) 
{
    if (nHitTest == HTCLOSE)
        m_pDockSite->ShowControlBar(this, FALSE, FALSE); // hide

    baseCSizingControlBar::OnNcLButtonUp(nHitTest, point);
}

void CSizingControlBar::OnLButtonUp(UINT nFlags, CPoint point) 
{
    if (m_bTracking)
        StopTracking();

    baseCSizingControlBar::OnLButtonUp(nFlags, point);
}

void CSizingControlBar::OnRButtonDown(UINT nFlags, CPoint point) 
{
    if (m_bTracking)
        StopTracking();
    
    baseCSizingControlBar::OnRButtonDown(nFlags, point);
}

void CSizingControlBar::OnMouseMove(UINT nFlags, CPoint point) 
{
    if (m_bTracking)
	    OnTrackUpdateSize(point);
		
    
    baseCSizingControlBar::OnMouseMove(nFlags, point);
}

void CSizingControlBar::OnCaptureChanged(CWnd *pWnd) 
{
    if (m_bTracking && (pWnd != this))
        StopTracking();

    baseCSizingControlBar::OnCaptureChanged(pWnd);
}

void CSizingControlBar::OnNcCalcSize(BOOL bCalcValidRects,
                                     NCCALCSIZE_PARAMS FAR* lpncsp) 
{
    // compute the the client area
    CRect rcClient = lpncsp->rgrc[0];
    rcClient.DeflateRect(m_cxEdge, m_cyEdge);

    m_dwSCBStyle &= ~SCBS_EDGEALL;

    switch(m_nDockBarID)
    {
    case AFX_IDW_DOCKBAR_TOP:
        m_dwSCBStyle |= SCBS_EDGEBOTTOM;
        rcClient.DeflateRect(m_cyGripper, 0, 0, 0);
        break;
    case AFX_IDW_DOCKBAR_BOTTOM:
        m_dwSCBStyle |= SCBS_EDGETOP;
        rcClient.DeflateRect(m_cyGripper, 0, 0, 0);
        break;
    case AFX_IDW_DOCKBAR_LEFT:
        m_dwSCBStyle |= SCBS_EDGERIGHT;
        rcClient.DeflateRect(0, m_cyGripper, 0, 0);
        break;
    case AFX_IDW_DOCKBAR_RIGHT:
        m_dwSCBStyle |= SCBS_EDGELEFT;
        rcClient.DeflateRect(0, m_cyGripper, 0, 0);
        break;
    default:
        break;
    }

    if (!IsFloating() && m_pDockBar != NULL)
    {
        CSCBArray arrSCBars;
        GetRowSizingBars(arrSCBars);

        for (int i = 0; i < arrSCBars.GetSize(); i++)
            if (arrSCBars[i] == this)
            {
                if (i > 0)
                    m_dwSCBStyle |= IsHorzDocked() ?
                        SCBS_EDGELEFT : SCBS_EDGETOP;
                if (i < arrSCBars.GetSize() - 1)
                    m_dwSCBStyle |= IsHorzDocked() ?
                        SCBS_EDGERIGHT : SCBS_EDGEBOTTOM;
            }
    }

    // make room for edges only if they will be painted
    if (m_dwSCBStyle & SCBS_SHOWEDGES)
        rcClient.DeflateRect(
            (m_dwSCBStyle & SCBS_EDGELEFT) ? m_cxEdge : 0,
            (m_dwSCBStyle & SCBS_EDGETOP) ? m_cyEdge : 0,
            (m_dwSCBStyle & SCBS_EDGERIGHT) ? m_cxEdge : 0,
            (m_dwSCBStyle & SCBS_EDGEBOTTOM) ? m_cyEdge : 0);

	if (m_bExitBtn)
	{
		// "hide" button positioning
		CPoint ptOrgBtn;
		if (IsHorzDocked())
			ptOrgBtn = CPoint(rcClient.left - m_cyGripper - 1,
				rcClient.top - 1);
		else
			ptOrgBtn = CPoint(rcClient.right - 11,
				rcClient.top - m_cyGripper - 1);

		m_biHide.Move(ptOrgBtn - CRect(lpncsp->rgrc[0]).TopLeft());
	}
    lpncsp->rgrc[0] = rcClient;
}

void CSizingControlBar::OnNcPaint() 
{
    // get window DC that is clipped to the non-client area
    CWindowDC dc(this);

    CRect rcClient, rcBar;
    GetClientRect(rcClient);
    ClientToScreen(rcClient);
    GetWindowRect(rcBar);
    rcClient.OffsetRect(-rcBar.TopLeft());
    rcBar.OffsetRect(-rcBar.TopLeft());

    // client area is not our bussiness :)
    dc.ExcludeClipRect(rcClient);

    // draw borders in non-client area
    CRect rcDraw = rcBar;
    DrawBorders(&dc, rcDraw);

    // erase parts not drawn
    dc.IntersectClipRect(rcDraw);

    // erase NC background the hard way
    HBRUSH hbr = (HBRUSH)GetClassLong(m_hWnd, GCL_HBRBACKGROUND);
	::FillRect(dc.m_hDC, rcDraw, hbr);		/*mingipa 05/12*/

    if (m_dwSCBStyle & SCBS_SHOWEDGES)
    {
        CRect rcEdge; // paint the sizing edges
        for (int i = 0; i < 4; i++)
            if (GetEdgeRect(rcBar, GetEdgeHTCode(i), rcEdge))
                dc.Draw3dRect(rcEdge, ::GetSysColor(COLOR_BTNHIGHLIGHT),
                    ::GetSysColor(COLOR_BTNSHADOW));
    }

	if(m_bGradient) DrawGradient(&dc, rcDraw);	

    if (m_cyGripper && !IsFloating())
       NcPaintGripper(&dc, rcClient);
	
    ReleaseDC(&dc);
}

void CSizingControlBar::NcPaintGripper(CDC* pDC, CRect rcClient)
{
    if (m_bExitBtn) {
		NcPaintGripperWithExitBtn(pDC, rcClient);
		return;
	}
    
    CRect gripper = rcClient;
    BOOL bHorz = IsHorzDocked();
    
    gripper.DeflateRect(1, 1);
    if (bHorz)
    {  
        gripper.left -= m_cyGripper;
        gripper.right = gripper.left + 3;
    }
    else
    {   // gripper at top
        gripper.top -= m_cyGripper;
        gripper.bottom = gripper.top + 3;
    }

    pDC->Draw3dRect(gripper, ::GetSysColor(COLOR_BTNHIGHLIGHT),
        ::GetSysColor(COLOR_BTNSHADOW));

/*
    gripper.OffsetRect(bHorz ? 3 : 0, bHorz ? 0 : 3);

    pDC->Draw3dRect(gripper, ::GetSysColor(COLOR_BTNHIGHLIGHT),
		::GetSysColor(COLOR_BTNSHADOW));
*/

}

void CSizingControlBar::OnPaint()
{
    // overridden to skip border painting based on clientrect
    CPaintDC dc(this);
	CRect rect;
	GetClientRect(rect);
	
	if(m_bGradient) DrawGradient(&dc, rect);
}

LRESULT CSizingControlBar::OnNcHitTest(CPoint point)
{
    if (IsFloating())
        return baseCSizingControlBar::OnNcHitTest(point);

    CRect rcBar, rcEdge;
    GetWindowRect(rcBar);

    for (int i = 0; i < 4; i++)
        if (GetEdgeRect(rcBar, GetEdgeHTCode(i), rcEdge))
            if (rcEdge.PtInRect(point)) return GetEdgeHTCode(i);

	if (m_bExitBtn)
	{
		CRect rc = m_biHide.GetRect();
		rc.OffsetRect(rcBar.TopLeft());
		if (rc.PtInRect(point)) 
			return HTCLOSE;
	}

    return HTCLIENT;
}

void CSizingControlBar::OnSettingChange(UINT uFlags, LPCTSTR lpszSection) 
{
    baseCSizingControlBar::OnSettingChange(uFlags, lpszSection);

    m_bDragShowContent = FALSE;
    ::SystemParametersInfo(SPI_GETDRAGFULLWINDOWS, 0,
        &m_bDragShowContent, 0); // update
}

/////////////////////////////////////////////////////////////////////////
// CSizingControlBar implementation helpers

void CSizingControlBar::StartTracking(UINT nHitTest)
{
    SetCapture();

    // make sure no updates are pending
    RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_UPDATENOW);
    
    BOOL bHorz = IsHorzDocked();

    m_szOld = bHorz ? m_szHorz : m_szVert;

    CRect rc;
    GetWindowRect(&rc);
    CRect rcEdge;
    VERIFY(GetEdgeRect(rc, nHitTest, rcEdge));
    m_ptOld = rcEdge.CenterPoint();

    m_htEdge = nHitTest;
    m_bTracking = TRUE;

    CSCBArray arrSCBars;
    GetRowSizingBars(arrSCBars);

    // compute the minsize as the max minsize of the sizing bars on row
    m_szMinT = m_szMin;
    int i = 0;
    for ( i = 0; i < arrSCBars.GetSize(); i++)
        if (bHorz)
            m_szMinT.cy = max(m_szMinT.cy, arrSCBars[i]->m_szMin.cy);
        else
            m_szMinT.cx = max(m_szMinT.cx, arrSCBars[i]->m_szMin.cx);

    if (!IsSideTracking())
    {
        // the control bar cannot grow with more than the size of 
        // remaining client area of the mainframe
        m_pDockSite->RepositionBars(0, 0xFFFF, AFX_IDW_PANE_FIRST,
            reposQuery, &rc, NULL, TRUE);
        m_szMaxT = m_szOld + rc.Size() - CSize(4, 4);
    }
    else
    {
        // side tracking: max size is the actual size plus the amount
        // the neighbour bar can be decreased to reach its minsize
        for ( i = 0; i < arrSCBars.GetSize(); i++)
            if (arrSCBars[i] == this) break;

        CSizingControlBar* pBar = arrSCBars[i +
            ((m_htEdge == HTTOP || m_htEdge == HTLEFT) ? -1 : 1)];

        m_szMaxT = m_szOld + (bHorz ? pBar->m_szHorz :
            pBar->m_szVert) - pBar->m_szMin;
    }
    OnTrackInvertTracker(); // draw tracker
}

void CSizingControlBar::StopTracking()
{
    OnTrackInvertTracker(); // erase tracker

    m_bTracking = FALSE;
    ReleaseCapture();
    
    m_pDockSite->DelayRecalcLayout();
}

void CSizingControlBar::OnTrackUpdateSize(CPoint& point)
{
    ASSERT(!IsFloating());

    CPoint pt = point;
    ClientToScreen(&pt);
    CSize szDelta = pt - m_ptOld;

    CSize sizeNew = m_szOld;
	if (m_htEdge == HTTOP || m_htEdge == HTBOTTOM)
		szDelta.cy = 0;		// mingipa 09/19 컨트롤바 Horz의 경우 사이징이 안되게 막음.
    switch (m_htEdge)
    {
    case HTLEFT:    sizeNew -= CSize(szDelta.cx, 0); break;
    case HTTOP:     sizeNew -= CSize(0, szDelta.cy); break;
    case HTRIGHT:   sizeNew += CSize(szDelta.cx, 0); break;
    case HTBOTTOM:  sizeNew += CSize(0, szDelta.cy); break;
    }

    // enforce the limits
    sizeNew.cx = max(m_szMinT.cx, min(m_szMaxT.cx, sizeNew.cx));
    sizeNew.cy = max(m_szMinT.cy, min(m_szMaxT.cy, sizeNew.cy));

    BOOL bHorz = IsHorzDocked();
    szDelta = sizeNew - (bHorz ? m_szHorz : m_szVert);
    
    if (szDelta == CSize(0, 0)) return; // no size change

    OnTrackInvertTracker(); // erase tracker

    (bHorz ? m_szHorz : m_szVert) = sizeNew; // save the new size

    CSCBArray arrSCBars;
    GetRowSizingBars(arrSCBars);

    for (int i = 0; i < arrSCBars.GetSize(); i++)
        if (!IsSideTracking())
        {   // track simultaneously
            CSizingControlBar* pBar = arrSCBars[i];
            (bHorz ? pBar->m_szHorz.cy : pBar->m_szVert.cx) =
                bHorz ? sizeNew.cy : sizeNew.cx;
        }
        else
        {   // adjust the neighbour's size too
            if (arrSCBars[i] != this) continue;

            CSizingControlBar* pBar = arrSCBars[i +
                ((m_htEdge == HTTOP || m_htEdge == HTLEFT) ? -1 : 1)];

            (bHorz ? pBar->m_szHorz.cx : pBar->m_szVert.cy) -=
                bHorz ? szDelta.cx : szDelta.cy;
        }

    OnTrackInvertTracker(); // redraw tracker at new pos

    if (m_bDragShowContent)
        m_pDockSite->DelayRecalcLayout();
}

void CSizingControlBar::OnTrackInvertTracker()
{
    ASSERT(m_bTracking);

    if (m_bDragShowContent)
        return; // don't show tracker if DragFullWindows is on

    BOOL bHorz = IsHorzDocked();
    CRect rc, rcBar, rcDock, rcFrame;
    GetWindowRect(rcBar);
    m_pDockBar->GetWindowRect(rcDock);
    m_pDockSite->GetWindowRect(rcFrame);
    VERIFY(GetEdgeRect(rcBar, m_htEdge, rc));
    if (!IsSideTracking())
        rc = bHorz ? 
            CRect(rcDock.left + 1, rc.top, rcDock.right - 1, rc.bottom) :
            CRect(rc.left, rcDock.top + 1, rc.right, rcDock.bottom - 1);

    rc.OffsetRect(-rcFrame.TopLeft());

    CSize sizeNew = bHorz ? m_szHorz : m_szVert;
    CSize sizeDelta = sizeNew - m_szOld;
    if (m_nDockBarID == AFX_IDW_DOCKBAR_LEFT && m_htEdge == HTTOP ||
        m_nDockBarID == AFX_IDW_DOCKBAR_RIGHT && m_htEdge != HTBOTTOM ||
        m_nDockBarID == AFX_IDW_DOCKBAR_TOP && m_htEdge == HTLEFT ||
        m_nDockBarID == AFX_IDW_DOCKBAR_BOTTOM && m_htEdge != HTRIGHT)
        sizeDelta = -sizeDelta;
    rc.OffsetRect(sizeDelta);

    CDC *pDC = m_pDockSite->GetDCEx(NULL,
        DCX_WINDOW | DCX_CACHE | DCX_LOCKWINDOWUPDATE);
    CBrush* pBrush = CDC::GetHalftoneBrush();
    CBrush* pBrushOld = pDC->SelectObject(pBrush);

    pDC->PatBlt(rc.left, rc.top, rc.Width(), rc.Height(), PATINVERT);
    
    pDC->SelectObject(pBrushOld);
    m_pDockSite->ReleaseDC(pDC);
}

BOOL CSizingControlBar::GetEdgeRect(CRect rcWnd, UINT nHitTest,
                                    CRect& rcEdge)
{
    rcEdge = rcWnd;
    if (m_dwSCBStyle & SCBS_SHOWEDGES)
        rcEdge.DeflateRect(1, 1);
    BOOL bHorz = IsHorzDocked();

    switch (nHitTest)
    {
    case HTLEFT:
        if (!(m_dwSCBStyle & SCBS_EDGELEFT)) return FALSE;
        rcEdge.right = rcEdge.left + m_cxEdge;
        rcEdge.DeflateRect(0, bHorz ? m_cxEdge: 0);
        break;
    case HTTOP:
        if (!(m_dwSCBStyle & SCBS_EDGETOP)) return FALSE;
        rcEdge.bottom = rcEdge.top + m_cyEdge;
        rcEdge.DeflateRect(bHorz ? 0 : m_cyEdge, 0);
        break;
    case HTRIGHT:
        if (!(m_dwSCBStyle & SCBS_EDGERIGHT)) return FALSE;
        rcEdge.left = rcEdge.right - m_cxEdge;
        rcEdge.DeflateRect(0, bHorz ? m_cxEdge: 0);
        break;
    case HTBOTTOM:
        if (!(m_dwSCBStyle & SCBS_EDGEBOTTOM)) return FALSE;
        rcEdge.top = rcEdge.bottom - m_cyEdge;
        rcEdge.DeflateRect(bHorz ? 0 : m_cyEdge, 0);
        break;
    default:
        ASSERT(FALSE); // invalid hit test code
    }
    return TRUE;
}

UINT CSizingControlBar::GetEdgeHTCode(int nEdge)
{
    if (nEdge == 0) return HTLEFT;
    if (nEdge == 1) return HTTOP;
    if (nEdge == 2) return HTRIGHT;
    if (nEdge == 3) return HTBOTTOM;
    ASSERT(FALSE); // invalid edge no
    return HTNOWHERE;
}

void CSizingControlBar::GetRowInfo(int& nFirst, int& nLast, int& nThis)
{
	ASSERT_VALID(m_pDockBar); // verify bounds

    nThis = m_pDockBar->FindBar(this);
    ASSERT(nThis != -1);

    int i, nBars = m_pDockBar->m_arrBars.GetSize();

    // find the first and the last bar in row
    for (nFirst = -1, i = nThis - 1; i >= 0 && nFirst == -1; i--)
        if (m_pDockBar->m_arrBars[i] == NULL)
            nFirst = i + 1;
    for (nLast = -1, i = nThis + 1; i < nBars && nLast == -1; i++)
        if (m_pDockBar->m_arrBars[i] == NULL)
            nLast = i - 1;

    ASSERT((nLast != -1) && (nFirst != -1));
}

void CSizingControlBar::GetRowSizingBars(CSCBArray& arrSCBars)
{
	arrSCBars.RemoveAll();

    int nFirst, nLast, nThis;
    GetRowInfo(nFirst, nLast, nThis);

    for (int i = nFirst; i <= nLast; i++)
    {
        CControlBar* pBar = (CControlBar*)m_pDockBar->m_arrBars[i];
        if (HIWORD(pBar) == 0) continue; // placeholder
        if (!pBar->IsVisible()) continue;
        if (FindSizingBar(pBar) >= 0 && ((CSizingControlBar*)pBar)->m_nType == m_nType)
            arrSCBars.Add((CSizingControlBar*)pBar);
    }
}

const int CSizingControlBar::FindSizingBar(CControlBar* pBar) const
{
    for (int nPos = 0; nPos < m_arrBars.GetSize(); nPos++)
        if (m_arrBars[nPos] == pBar)
            return nPos; // got it

    return -1; // not found
}

BOOL CSizingControlBar::NegociateSpace(int nLengthAvail, BOOL bHorz)
{
    ASSERT(bHorz == IsHorzDocked());

    int nFirst, nLast, nThis;
    GetRowInfo(nFirst, nLast, nThis);
    int i = 0;
    // step 1: subtract the visible fixed bars' lengths
    for ( i = nFirst; i <= nLast; i++)
    {
        CControlBar* pFBar = (CControlBar*)m_pDockBar->m_arrBars[i];
        if (HIWORD(pFBar) == 0) continue; // placeholder
        if (!pFBar->IsVisible() || (FindSizingBar(pFBar) >= 0)) continue;

        CRect rcBar;
        pFBar->GetWindowRect(&rcBar);

        nLengthAvail -= (bHorz ? rcBar.Width() - 2 : rcBar.Height() - 2);
    }

    CSCBArray arrSCBars;
    GetRowSizingBars(arrSCBars);
    CSizingControlBar* pBar;

    // step 2: compute actual and min lengths; also the common width
    int nActualLength = 0;
    int nMinLength = 2;
    int nWidth = 0;
    for (i = 0; i < arrSCBars.GetSize(); i++)
    {
        pBar = arrSCBars[i];
        nActualLength += bHorz ? pBar->m_szHorz.cx - 2 :
            pBar->m_szVert.cy - 2;
        nMinLength += bHorz ? pBar->m_szMin.cx - 2:
            pBar->m_szMin.cy - 2;
        nWidth = max(nWidth, bHorz ? pBar->m_szHorz.cy :
            pBar->m_szVert.cx);
    }
    
    // step 3: pop the bar out of the row if not enough room
    if (nMinLength > nLengthAvail)
    {
        if (nFirst < nThis || nThis < nLast)
        {   // not enough room - create a new row
            m_pDockBar->m_arrBars.InsertAt(nLast + 1, this);
            m_pDockBar->m_arrBars.InsertAt(nLast + 1, (CControlBar*) NULL);
            m_pDockBar->m_arrBars.RemoveAt(nThis);
        }
        return FALSE;
    }

    // step 4: make the bars same width
    for (i = 0; i < arrSCBars.GetSize(); i++)
        if (bHorz)
            arrSCBars[i]->m_szHorz.cy = nWidth;
        else
            arrSCBars[i]->m_szVert.cx = nWidth;

    if (nActualLength == nLengthAvail)
        return TRUE; // no change

    // step 5: distribute the difference between the bars, but
    //         don't shrink them below minsize
    int nDelta = nLengthAvail - nActualLength;

    while (nDelta != 0)
    {
        int nDeltaOld = nDelta;
        for (i = 0; i < arrSCBars.GetSize(); i++)
        {
            pBar = arrSCBars[i];
            int nLMin = bHorz ? pBar->m_szMin.cx : pBar->m_szMin.cy;
            int nL = bHorz ? pBar->m_szHorz.cx : pBar->m_szVert.cy;
            
            if ((nL == nLMin) && (nDelta < 0) || // already at min length
                pBar->m_bKeepSize) // or wants to keep its size
                continue;
            
            // sign of nDelta
            int nDelta2 = (nDelta == 0) ? 0 : ((nDelta < 0) ? -1 : 1);

            (bHorz ? pBar->m_szHorz.cx : pBar->m_szVert.cy) += nDelta2;
            nDelta -= nDelta2;
            if (nDelta == 0) break;
        }
        // clear m_bKeepSize flags
        if ((nDeltaOld == nDelta) || (nDelta == 0))
            for (i = 0; i < arrSCBars.GetSize(); i++)
                arrSCBars[i]->m_bKeepSize = FALSE;
    }

    return TRUE;
}

void CSizingControlBar::AlignControlBars()
{
    int nFirst, nLast, nThis;
    GetRowInfo(nFirst, nLast, nThis);

    BOOL bHorz = IsHorzDocked();
    BOOL bNeedRecalc = FALSE;
    int nPos, nAlign = bHorz ? -2 : 0;

    CRect rc, rcDock;
    m_pDockBar->GetWindowRect(&rcDock);

    for (int i = nFirst; i <= nLast; i++)
    {
        CControlBar* pBar = (CControlBar*)m_pDockBar->m_arrBars[i];
        if(HIWORD(pBar) == 0) continue; // placeholder
        if(!pBar->IsVisible()) continue;
//		if(((CSizingControlBar*)pBar)->m_nType != m_nType) continue;

        pBar->GetWindowRect(&rc);
        rc.OffsetRect(-rcDock.TopLeft());

        if ((nPos = FindSizingBar(pBar)) >= 0)
            rc = CRect(rc.TopLeft(), bHorz ?
                m_arrBars[nPos]->m_szHorz : m_arrBars[nPos]->m_szVert);

        if ((bHorz ? rc.left : rc.top) != nAlign)
        {
            if (!bHorz)
                rc.OffsetRect(0, nAlign - rc.top - 2);
            else if (m_nDockBarID == AFX_IDW_DOCKBAR_TOP)
                rc.OffsetRect(nAlign - rc.left, -2);
            else
                rc.OffsetRect(nAlign - rc.left, 0);
            pBar->MoveWindow(rc);
			
			TRACE("CTLID %D=> MOVEWINDOW:%d LEFT:%d, TOP:%d, RIGHT:%d, BOTTOM:%d\n",
				GetDlgCtrlID(),rc.left, rc.top, rc.right, rc.bottom);

            bNeedRecalc = TRUE;
        }
	
        nAlign += (bHorz ? rc.Width() : rc.Height()) - 2;
    }

    if (bNeedRecalc)
    {
        m_pDockSite->DelayRecalcLayout();
        TRACE("ccc\n");
    }
}

void CSizingControlBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler)
{
    BOOL bNeedPaint = FALSE;

    CPoint pt;
	if (m_bExitBtn)
	{
		::GetCursorPos(&pt);
		BOOL bHit = (OnNcHitTest(pt) == HTCLOSE);
		BOOL bLButtonDown = (::GetKeyState(VK_LBUTTON) < 0);

		BOOL bWasPushed = m_biHide.bPushed;
		m_biHide.bPushed = bHit && bLButtonDown;

		BOOL bWasRaised = m_biHide.bRaised;
		m_biHide.bRaised = bHit && !bLButtonDown;

		bNeedPaint |= (m_biHide.bPushed ^ bWasPushed) || 
					  (m_biHide.bRaised ^ bWasRaised);
	}
    if (bNeedPaint)
        SendMessage(WM_NCPAINT);
}

void CSizingControlBar::LoadState(LPCTSTR lpszProfileName, BOOL bInit/* = FALSE*/)
{
    ASSERT_VALID(this);
    ASSERT(GetSafeHwnd()); // must be called after Create()

    TCHAR szSection[256];
    wsprintf(szSection, _T("%s-SCBar-%d"), lpszProfileName,
        GetDlgCtrlID());

	char szFileName[MAX_PATH] = {0,};
	sprintf(szFileName, "%s\\Usr\\%s\\ToolBarState.ini", m_szRootPath, m_szUserID);

	char szFileNameSource[MAX_PATH] = {0,};
	//sprintf(szFileNameSource, "%s\\Data\\ToolBarState.ini", m_szRootPath);
	sprintf(szFileNameSource, "%s\\Icss\\ToolBarState.ini", m_szRootPath);	//@Solomon

	CFileFind ff;
	if(!ff.FindFile(szFileName))
		::CopyFile(szFileNameSource, szFileName, FALSE);

	if(bInit)
		//sprintf(szFileName, "%s\\Data\\ToolBarState.ini", m_szRootPath);
		sprintf(szFileName, "%s\\Icss\\ToolBarState.ini", m_szRootPath);	//@Solomon
	else
		sprintf(szFileName, "%s\\Usr\\%s\\ToolBarState.ini", m_szRootPath, m_szUserID);

	m_szHorz.cx = max(m_szMin.cx, (int) GetPrivateProfileInt(szSection,
        _T("sizeHorzCX"), m_szHorz.cx, szFileName));
    m_szHorz.cy = max(m_szMin.cy, (int) GetPrivateProfileInt(szSection, 
        _T("sizeHorzCY"), m_szHorz.cy, szFileName));

    m_szVert.cx = max(m_szMin.cx, (int) GetPrivateProfileInt(szSection, 
        _T("sizeVertCX"), m_szVert.cx, szFileName));
    m_szVert.cy = max(m_szMin.cy, (int) GetPrivateProfileInt(szSection, 
        _T("sizeVertCY"), m_szVert.cy, szFileName));

    m_szFloat.cx = max(m_szMin.cx, (int) GetPrivateProfileInt(szSection,
        _T("sizeFloatCX"), m_szFloat.cx, szFileName));
    m_szFloat.cy = max(m_szMin.cy, (int) GetPrivateProfileInt(szSection,
        _T("sizeFloatCY"), m_szFloat.cy, szFileName));

//	m_rect;
	m_rect.left = max(m_szMin.cx, (int) GetPrivateProfileInt(szSection, 
        _T("RECT_LEFT"), m_szVert.cx, szFileName));
    m_rect.top = max(m_szMin.cy, (int) GetPrivateProfileInt(szSection, 
        _T("RECT_TOP"), m_szVert.cy, szFileName));

    m_rect.right = max(m_szMin.cx, (int) GetPrivateProfileInt(szSection,
        _T("RECT_RIGHT"), m_szFloat.cx, szFileName));
    m_rect.bottom = max(m_szMin.cy, (int) GetPrivateProfileInt(szSection,
        _T("RECT_BOTTOM"), m_szFloat.cy, szFileName));
	
	m_bShow = (BOOL)::GetPrivateProfileInt(szSection,
        _T("VISIBLE"), 1, szFileName);

	char szTmp[10] = {0, };
	::GetPrivateProfileString(szSection, _T("DOCKPOSITION"), "", szTmp, sizeof(szTmp), szFileName);
	
	CString strTmp = szTmp;
	if(strTmp == "TOP")
		m_nDockBarID = AFX_IDW_DOCKBAR_TOP;
	else if(strTmp == "BOTTOM")
		m_nDockBarID = AFX_IDW_DOCKBAR_BOTTOM;
	else if(strTmp == "LEFT")
		m_nDockBarID = AFX_IDW_DOCKBAR_LEFT;
	else if(strTmp == "RIGHT")
		m_nDockBarID = AFX_IDW_DOCKBAR_RIGHT;
	else
	{
		if(::memcmp(lpszProfileName, "Ticker Bar", strlen(lpszProfileName)) == 0)
			m_nDockBarID = AFX_IDW_DOCKBAR_BOTTOM;			
	}

	if (!m_bShow)
		ShowWindow(SW_HIDE);
	else
		ShowWindow(SW_SHOW);
}

void CSizingControlBar::SaveState(LPCTSTR lpszProfileName)
{
    // place your SaveState or GlobalSaveState call in
    // CMainFrame::DestroyWindow(), not in OnDestroy()
    ASSERT_VALID(this);
    ASSERT(GetSafeHwnd());

    TCHAR szSection[256];
    wsprintf(szSection, _T("%s-SCBar-%d"), lpszProfileName,
        GetDlgCtrlID());

	char szFileName[100] = {0,};
	char szTmp[20] = {0,};

	sprintf(szFileName, "%s\\Usr\\%s\\ToolBarState.ini", m_szRootPath, m_szUserID);

	itoa(m_szHorz.cx, szTmp, 10);
	WritePrivateProfileString(szSection, _T("sizeHorzCX"), szTmp, szFileName);
	ZeroMemory(szTmp, sizeof(szTmp));
	itoa(m_szHorz.cy, szTmp, 10);
    WritePrivateProfileString(szSection, _T("sizeHorzCY"), szTmp, szFileName);
	ZeroMemory(szTmp, sizeof(szTmp));

	itoa(m_szVert.cx, szTmp, 10);
    WritePrivateProfileString(szSection, _T("sizeVertCX"), szTmp, szFileName);
	ZeroMemory(szTmp, sizeof(szTmp));
	itoa(m_szVert.cy, szTmp, 10);
    WritePrivateProfileString(szSection, _T("sizeVertCY"), szTmp, szFileName);
	ZeroMemory(szTmp, sizeof(szTmp));

	itoa(m_szFloat.cx, szTmp, 10);
    WritePrivateProfileString(szSection, _T("sizeFloatCX"), szTmp, szFileName);
	ZeroMemory(szTmp, sizeof(szTmp));
	
	itoa(m_szFloat.cy, szTmp, 10);
    WritePrivateProfileString(szSection, _T("sizeFloatCY"), szTmp, szFileName);

	CRect rect;
	GetWindowRect(&rect);

	itoa(rect.top, szTmp, 10);
	WritePrivateProfileString(szSection, _T("RECT_TOP"), szTmp, szFileName);
	ZeroMemory(szTmp, sizeof(szTmp));
	itoa(rect.left, szTmp, 10);
    WritePrivateProfileString(szSection, _T("RECT_LEFT"), szTmp, szFileName);
	ZeroMemory(szTmp, sizeof(szTmp));

	itoa(rect.bottom, szTmp, 10);
    WritePrivateProfileString(szSection, _T("RECT_BOTTOM"), szTmp, szFileName);
	ZeroMemory(szTmp, sizeof(szTmp));
	itoa(rect.right, szTmp, 10);
    WritePrivateProfileString(szSection, _T("RECT_RIGHT"), szTmp, szFileName);
	ZeroMemory(szTmp, sizeof(szTmp));

	itoa((int)IsWindowVisible(), szTmp, 10);
    WritePrivateProfileString(szSection, _T("VISIBLE"), szTmp, szFileName);
	ZeroMemory(szTmp, sizeof(szTmp));

	if(m_nDockBarID == AFX_IDW_DOCKBAR_TOP)
		strcpy(szTmp, "TOP");
	else if(m_nDockBarID == AFX_IDW_DOCKBAR_BOTTOM)
		strcpy(szTmp, "BOTTOM");
	else if(m_nDockBarID == AFX_IDW_DOCKBAR_LEFT)
		strcpy(szTmp, "LEFT");
	else if(m_nDockBarID == AFX_IDW_DOCKBAR_RIGHT)
		strcpy(szTmp, "RIGHT");
	
	WritePrivateProfileString(szSection, _T("DOCKPOSITION"), szTmp, szFileName);
}

void CSizingControlBar::GlobalLoadState(LPCTSTR lpszProfileName)
{
    for (int i = 0; i < m_arrBars.GetSize(); i++)
        ((CSizingControlBar*) m_arrBars[i])->LoadState(lpszProfileName);
}

void CSizingControlBar::GlobalSaveState(LPCTSTR lpszProfileName)
{
    for (int i = 0; i < m_arrBars.GetSize(); i++)
        ((CSizingControlBar*) m_arrBars[i])->SaveState(lpszProfileName);
}

void CSizingControlBar::NcPaintGripperWithExitBtn(CDC *pDC, CRect rcClient)
{
	// paints a simple "two raised lines" gripper
    // override this if you want a more sophisticated gripper
    CRect gripper = rcClient;
    CRect rcbtn = m_biHide.GetRect();
    BOOL bHorz = IsHorzDocked();
    
    gripper.DeflateRect(1, 1);
    if (bHorz)
    {   // gripper at left
        gripper.left -= m_cyGripper;
        gripper.right = gripper.left + 3;
        gripper.top = rcbtn.bottom + 3;
    }
    else
    {   // gripper at top
        gripper.top -= m_cyGripper;
        gripper.bottom = gripper.top + 3;
        gripper.right = rcbtn.left - 3;
    }

    pDC->Draw3dRect(gripper, ::GetSysColor(COLOR_BTNHIGHLIGHT),
        ::GetSysColor(COLOR_BTNSHADOW));

    gripper.OffsetRect(bHorz ? 3 : 0, bHorz ? 0 : 3);
    
    pDC->Draw3dRect(gripper, ::GetSysColor(COLOR_BTNHIGHLIGHT),
        ::GetSysColor(COLOR_BTNSHADOW));

    m_biHide.Paint(pDC);		//닫기 버튼(x) Draw
}

void CSizingControlBar::SetMinSize(int x, int y)
{
	m_szMin = CSize(x, y);
}

/////////////////////////////////////////////////////////////////////////
// CSCBButton  Start

CSCBButton::CSCBButton()
{
    bRaised = FALSE;
    bPushed = FALSE;
}

void CSCBButton::Paint(CDC* pDC)
{
    CRect rc = GetRect();

    if (bPushed)
        pDC->Draw3dRect(rc, ::GetSysColor(COLOR_BTNSHADOW),
            ::GetSysColor(COLOR_BTNHIGHLIGHT));
    else
        if (bRaised)
            pDC->Draw3dRect(rc, ::GetSysColor(COLOR_BTNHIGHLIGHT),
                ::GetSysColor(COLOR_BTNSHADOW));

    COLORREF clrOldTextColor = pDC->GetTextColor();
    pDC->SetTextColor(::GetSysColor(COLOR_BTNTEXT));
    int nPrevBkMode = pDC->SetBkMode(TRANSPARENT);
    CFont font;
    int ppi = pDC->GetDeviceCaps(LOGPIXELSX);
    int pointsize = MulDiv(60, 96, ppi); // 6 points at 96 ppi
    font.CreatePointFont(pointsize, _T("Marlett"));
    CFont* oldfont = pDC->SelectObject(&font);

    pDC->TextOut(ptOrg.x + 2, ptOrg.y + 2, CString(_T("r"))); // x-like
    
    pDC->SelectObject(oldfont);
    pDC->SetBkMode(nPrevBkMode);
    pDC->SetTextColor(clrOldTextColor);
}
/////////////////////////////////////////////////////////////////////////
// CSCBButton  End



BOOL CSizingControlBar::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return baseCSizingControlBar::OnEraseBkgnd(pDC);
}

void CSizingControlBar::SetBkColor(COLORREF color)
{
}

void CSizingControlBar::SetUseGradient(BOOL bUse, COLORREF clStart, COLORREF clEnd)
{
	m_bGradient = bUse;
	m_clrStart = clStart;
	m_clrEnd = clEnd;
}

void CSizingControlBar::DrawGradient(CDC *pDC, CRect rect)
{
	COLORREF Start, End;
	Start = m_clrStart;
	End   = m_clrEnd;

	CRect backup = rect;
		
	int r,g,b;//각 3가지 색상의 값의 차이..
	float rStep, gStep, bStep;//각 3가지 값들 각각의 색상값의 차이..
	float fStep;//최대 색상값 차이로 나누어서 한번에 이동할 옵셋..

	//각 생상 차이를 구한다..
	r = (GetRValue(End) - GetRValue(Start));
	g = (GetGValue(End) - GetGValue(Start));
	b = (GetBValue(End) - GetBValue(Start));

	//가장 큰차이가 나는것을 구한다..
	int Max = max( abs(r), max(abs(g), abs(b)) );

	//그릴 사각형을 차이가 나는것 만큼.. 분할할 길이를 구한다..
	fStep = (float)rect.Height()/(float)Max;     // 상-하 Gradation 

	//각 색상별 단계 차이값..
	rStep = r/(float)Max;
	gStep = g/(float)Max;
	bStep = b/(float)Max;

	//r,g,b를 시작 색으로 재설정...
	r = GetRValue(Start);
	g = GetGValue(Start);
	b = GetBValue(Start);

	//색깔이 변해야 하는 횟수만큼.. 반복 그린다.
	for(int iOnBand=0 ; iOnBand <Max; iOnBand++)  // 상-하 Gradation 
	{
		::SetRect( &rect, rect.left, int(fStep*iOnBand) + backup.top ,
		    rect.right, backup.bottom );  
		pDC->FillSolidRect( &rect,  RGB( r + rStep*iOnBand, g + gStep*iOnBand, b + bStep*iOnBand ) );
	}
}

void CSizingControlBar::SetHVEdges(CSize szEdges)
{
	m_cxEdge = szEdges.cx;
	m_cyEdge = szEdges.cy;
}
// ScrollWindow.cpp : implementation file
//

#include "pch.h"
#include "ScrollWindow.h"

#include <exception>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScrollWindow

#include <SDKDDKVer.h>
#include <Lm.h>
#pragma comment(lib, "netapi32.lib")
BOOL GetWindowsVersion(DWORD& dwMajor, DWORD& dwMinor)
{
    static DWORD dwMajorCache = 0, dwMinorCache = 0;
    if (0 != dwMajorCache)
    {
        dwMajor = dwMajorCache;
        dwMinor = dwMinorCache;
        return TRUE;
    }

    LPWKSTA_INFO_100 pBuf = NULL;
    if (NERR_Success != NetWkstaGetInfo(NULL, 100, (LPBYTE*)&pBuf))
        return FALSE;

    dwMajor = dwMajorCache = pBuf->wki100_ver_major;
    dwMinor = dwMinorCache = pBuf->wki100_ver_minor;
    NetApiBufferFree(pBuf);

    return TRUE;
}

BOOL CScrollWindow::m_bGotMouseScrollLines = FALSE;

UINT CScrollWindow::GetMouseScrollLines()
{
    static UINT uCachedScrollLines;
    
    // if we've already got it and we're not refreshing,
    // return what we've already got
    
    if (m_bGotMouseScrollLines)
        return uCachedScrollLines;
    
    // see if we can find the mouse window
    
    m_bGotMouseScrollLines = TRUE;
    
    static UINT msgGetScrollLines;
    static WORD nRegisteredMessage;
    
    if (nRegisteredMessage == 0)
    {
        msgGetScrollLines = ::RegisterWindowMessage(MSH_SCROLL_LINES);
        if (msgGetScrollLines == 0)
            nRegisteredMessage = 1;     // couldn't register!  never try again
        else
            nRegisteredMessage = 2;     // it worked: use it
    }
    
    if (nRegisteredMessage == 2)
    {
        HWND hwMouseWheel = NULL;
        hwMouseWheel = ::FindWindow(MSH_WHEELMODULE_CLASS, MSH_WHEELMODULE_TITLE);
        if (hwMouseWheel && msgGetScrollLines)
        {
            uCachedScrollLines = (UINT)
                ::SendMessage(hwMouseWheel, msgGetScrollLines, 0, 0);
            return uCachedScrollLines;
        }
    }
    
    // couldn't use the window -- try system settings
    uCachedScrollLines = 3; // reasonable default
    DWORD dwVersion = 10;//check (GetWindowsVersion() & 0x8000FFFF);// Get the window version and filter out the build number.
    if ((BYTE)dwVersion >= 4)// Windows NT 4.0
    {
        HKEY hKey;
        if (RegOpenKeyEx(HKEY_CURRENT_USER,  _T("Control Panel\\Desktop"),
            0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
        {
            TCHAR szData[128];
            DWORD dwKeyDataType;
            DWORD dwDataBufSize = sizeof(szData)/sizeof(szData[0]);
            
            if (RegQueryValueEx(hKey, _T("WheelScrollLines"), NULL, &dwKeyDataType,
                (LPBYTE) &szData, &dwDataBufSize) == ERROR_SUCCESS)
            {
                uCachedScrollLines = _tcstoul(szData, NULL, 10);
            }
            RegCloseKey(hKey);
        }
    }
    else if ((dwVersion & 0x80000000) != 0)// Windows 95
    {
        ::SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &uCachedScrollLines, 0);
    }
    
    return uCachedScrollLines;
}

/////////////////////////////////////////////////////////////////////////////
// CScrollWindow construction/destruction

CScrollWindow::CScrollWindow()
{
    m_bInsideUpdate = FALSE;

    DWORD m_dwVersion;
    BOOL m_bWin95;
    BOOL m_bWin4;
    BOOL m_bMarked4;

    m_dwVersion = 10;// GetWindowsVersion();
    m_bWin95 = (m_dwVersion & 0x80000000) != 0;
    m_bWin4 = (BYTE)m_dwVersion >= 4;

    // Determine various metrics based on EXE subsystem version mark    
    if (m_bWin4)
    {
        m_bMarked4 = (GetProcessVersion(0) >= 0x00040000);
    }
    else
    {
        m_bMarked4 = FALSE;
    }

    if(m_bMarked4)
    {
        m_cxVScroll = GetSystemMetrics(SM_CXVSCROLL) + CX_BORDER;
        m_cyHScroll = GetSystemMetrics(SM_CYHSCROLL) + CY_BORDER;
    }
    else
    {
        m_cxVScroll = GetSystemMetrics(SM_CXVSCROLL);
        m_cyHScroll = GetSystemMetrics(SM_CYHSCROLL);
    }

    SetScrollSizes(CSize(0, 0));
}

CScrollWindow::~CScrollWindow()
{
}


BEGIN_MESSAGE_MAP(CScrollWindow, CWnd)
    //{{AFX_MSG_MAP(CScrollWindow)
    ON_WM_SIZE()
    ON_WM_HSCROLL()
    ON_WM_VSCROLL()
    ON_WM_MOUSEWHEEL()
    ON_WM_SETTINGCHANGE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Other helpers

void CScrollWindow::FillOutsideRect(CDC* pDC, CBrush* pBrush)
{
    ASSERT_VALID(pDC);
    ASSERT_VALID(pBrush);
    
    // fill rect outside the image
    CRect rect;
    GetClientRect(rect);
    ASSERT(rect.left == 0 && rect.top == 0);
    rect.left = m_sizeTotal.cx;
    if (!rect.IsRectEmpty())
    {
        pDC->FillRect(rect, pBrush);    // vertical strip along the side
    }
    rect.left = 0;
    rect.right = m_sizeTotal.cx;
    rect.top = m_sizeTotal.cy;
    if (!rect.IsRectEmpty())
    {
        pDC->FillRect(rect, pBrush);    // horizontal strip along the bottom
    }
}

void CScrollWindow::ResizeParentToFit(BOOL bShrinkOnly)
{
    // Adjust parent rect so client rect is appropriate size
    
    // Determine current size of the client area as if no scrollbars present
    CRect rectClient;
    GetWindowRect(rectClient);
    CRect rect = rectClient;
    CalcWindowRect(rect);
    rectClient.left += rectClient.left - rect.left;
    rectClient.top += rectClient.top - rect.top;
    rectClient.right -= rect.right - rectClient.right;
    rectClient.bottom -= rect.bottom - rectClient.bottom;
    rectClient.OffsetRect(-rectClient.left, -rectClient.top);
    ASSERT(rectClient.left == 0 && rectClient.top == 0);
    
    // Determine desired size of the view
    CRect rectView(0, 0, m_sizeTotal.cx, m_sizeTotal.cy);
    if (bShrinkOnly)
    {
        if (rectClient.right <= m_sizeTotal.cx)
            rectView.right = rectClient.right;
        if (rectClient.bottom <= m_sizeTotal.cy)
            rectView.bottom = rectClient.bottom;
    }
    CalcWindowRect(rectView, CWnd::adjustOutside);
    rectView.OffsetRect(-rectView.left, -rectView.top);
    ASSERT(rectView.left == 0 && rectView.top == 0);
    if (bShrinkOnly)
    {
        if (rectClient.right <= m_sizeTotal.cx)
            rectView.right = rectClient.right;
        if (rectClient.bottom <= m_sizeTotal.cy)
            rectView.bottom = rectClient.bottom;
    }
    
    // Determine and set size of frame based on desired size of view
    CRect rectFrame;
    CFrameWnd* pFrame = GetParentFrame();
    ASSERT_VALID(pFrame);
    pFrame->GetWindowRect(rectFrame);
    CSize size = rectFrame.Size();
    size.cx += rectView.right - rectClient.right;
    size.cy += rectView.bottom - rectClient.bottom;
    pFrame->SetWindowPos(NULL, 0, 0, size.cx, size.cy,
        SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
}

/////////////////////////////////////////////////////////////////////////////

void CScrollWindow::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);

    // UpdateBars() handles locking out recursion
    UpdateBars();
}

/////////////////////////////////////////////////////////////////////////////
// Set mode and scaling/scrolling sizes

void CScrollWindow::SetScrollSizes(SIZE sizeTotal, const SIZE& sizePage, const SIZE& sizeLine)
{
    ASSERT(sizeTotal.cx >= 0 && sizeTotal.cy >= 0);

    m_sizeTotal = sizeTotal;
    m_sizePage = sizePage;
    m_sizeLine = sizeLine;
    
    // now adjust device specific sizes
    if (m_sizePage.cx == 0)
        m_sizePage.cx = m_sizeTotal.cx / 10;
    if (m_sizePage.cy == 0)
        m_sizePage.cy = m_sizeTotal.cy / 10;
    if (m_sizeLine.cx == 0)
        m_sizeLine.cx = m_sizePage.cx / 10;
    if (m_sizeLine.cy == 0)
        m_sizeLine.cy = m_sizePage.cy / 10;
    
    if (m_hWnd != NULL)
    {
        // window has been created, invalidate now
        UpdateBars();
    }
}

/////////////////////////////////////////////////////////////////////////////
// Getting information

CPoint CScrollWindow::GetScrollPosition() const
{
    CPoint pt(GetScrollPos(SB_HORZ), GetScrollPos(SB_VERT));

    ASSERT(pt.x >= 0 || pt.y >= 0);
    
    return pt;
}

void CScrollWindow::ScrollToPosition(POINT pt)
{
    // now in device coordinates - limit if out of range
    int xMax = GetScrollLimit(SB_HORZ);
    int yMax = GetScrollLimit(SB_VERT);
    if (pt.x < 0)
        pt.x = 0;
    else if (pt.x > xMax)
        pt.x = xMax;
    if (pt.y < 0)
        pt.y = 0;
    else if (pt.y > yMax)
        pt.y = yMax;
    
    // Note: ScrollToDevicePosition can and is used to scroll out-of-range
    //  areas as far as CScrollView is concerned -- specifically in
    //  the print-preview code.  Since OnScrollBy makes sure the range is
    //  valid, ScrollToDevicePosition does not vector through OnScrollBy.
    
    int xOrig = GetScrollPos(SB_HORZ);
    SetScrollPos(SB_HORZ, pt.x);
    int yOrig = GetScrollPos(SB_VERT);
    SetScrollPos(SB_VERT, pt.y);
    ScrollWindow(xOrig - pt.x, yOrig - pt.y);
}

CSize CScrollWindow::GetTotalSize() const
{
    return m_sizeTotal;
}

void CScrollWindow::GetScrollSizes(SIZE& sizeTotal,
            SIZE& sizePage, SIZE& sizeLine) const
{
    sizeTotal = m_sizeTotal;
    sizePage = m_sizePage;
    sizeLine = m_sizeLine;
}

/////////////////////////////////////////////////////////////////////////////
// Tie to scrollbars and CWnd behavior

void CScrollWindow::GetScrollBarSizes(CSize& sizeSb)
{
    sizeSb.cx = sizeSb.cy = 0;
    DWORD dwStyle = GetStyle();

    if (GetScrollBarCtrl(SB_VERT) == NULL)
    {
        // Vertical scrollbars will impact the client area of this window
        sizeSb.cx = m_cxVScroll;
        if (dwStyle & WS_BORDER)
        {
            sizeSb.cx -= CX_BORDER;
        }
    }
    if (GetScrollBarCtrl(SB_HORZ) == NULL)
    {
        // Horizontal scrollbars will impact the client area of this window
        sizeSb.cy = m_cyHScroll;
        if (dwStyle & WS_BORDER)
        {
            sizeSb.cy -= CY_BORDER;
        }
    }
}

BOOL CScrollWindow::GetTrueClientSize(CSize& size, CSize& sizeSb)
{
    CRect rect;
    GetClientRect(&rect);
    
    ASSERT(rect.top == 0 && rect.left == 0);

    size.cx = rect.right;
    size.cy = rect.bottom;
    DWORD dwStyle = GetStyle();
    
    // first get the size of the scrollbars for this window
    GetScrollBarSizes(sizeSb);
    
    // first calculate the size of a potential scrollbar
    // (scroll bar controls do not get turned on/off)
    if (sizeSb.cx != 0 && (dwStyle & WS_VSCROLL))
    {
        // Vertical scrollbars will impact client area of this window
        size.cx += sizeSb.cx;   // currently on - adjust now
    }
    if (sizeSb.cy != 0 && (dwStyle & WS_HSCROLL))
    {
        // Horizontal scrollbars will impact client area of this window
        size.cy += sizeSb.cy;   // currently on - adjust now
    }
    
    // return TRUE if enough room
    return (size.cx > sizeSb.cx && size.cy > sizeSb.cy);
}

void CScrollWindow::GetScrollBarState(CSize sizeClient, CSize& needSb,
                                       CSize& sizeRange, CPoint& ptMove, BOOL bInsideClient)
{
    // Get scroll bar sizes (the part that is in the client area)
    CSize sizeSb;
    GetScrollBarSizes(sizeSb);
    
    // Enough room to add scrollbars
    sizeRange = m_sizeTotal - sizeClient;
    // > 0 => need to scroll
    ptMove = GetScrollPosition();// Point to move to (start at current scroll position)
    
    BOOL bNeedH = sizeRange.cx > 0;
    if (!bNeedH)
        ptMove.x = 0;                       // jump back to origin
    else if (bInsideClient)
        sizeRange.cy += sizeSb.cy;          // need room for a scroll bar
    
    BOOL bNeedV = sizeRange.cy > 0;
    if (!bNeedV)
        ptMove.y = 0;                       // jump back to origin
    else if (bInsideClient)
        sizeRange.cx += sizeSb.cx;          // need room for a scroll bar
    
    if (bNeedV && !bNeedH && sizeRange.cx > 0)
    {
        ASSERT(bInsideClient);
        // need a horizontal scrollbar after all
        bNeedH = TRUE;
        sizeRange.cy += sizeSb.cy;
    }
    
    // If current scroll position will be past the limit, scroll to limit
    if (sizeRange.cx > 0 && ptMove.x >= sizeRange.cx)
        ptMove.x = sizeRange.cx;
    if (sizeRange.cy > 0 && ptMove.y >= sizeRange.cy)
        ptMove.y = sizeRange.cy;
    
    // Now update the bars as appropriate
    needSb.cx = bNeedH;
    needSb.cy = bNeedV;
    
    // needSb, sizeRange, and ptMove area now all updated
}

void CScrollWindow::UpdateBars()
{
    // UpdateBars may cause window to be resized - ignore those resizings
    if (m_bInsideUpdate)
        return;         // Do not allow recursive calls

    // Lock out recursion
    m_bInsideUpdate = TRUE;

    // Update the horizontal to reflect reality
    // NOTE: turning on/off the scrollbars will cause 'OnSize' callbacks
    ASSERT(m_sizeTotal.cx >= 0 && m_sizeTotal.cy >= 0);

    CRect rectClient;
    BOOL bCalcClient = TRUE;

    // allow parent to do inside-out layout first
    CWnd* pParentWnd = GetParent();
    if (pParentWnd != NULL)
    {
        // If parent window responds to this message, use just
        // client area for scroll bar calculation -- not "true" client area
        if ((BOOL)pParentWnd->SendMessage(WM_RECALCPARENT, 0,
            (LPARAM)(LPCRECT)&rectClient) != 0)
        {
            // Use rectClient instead of GetTrueClientSize for
            // client size calculation.
            bCalcClient = FALSE;
        }
    }

    CSize sizeClient;
    CSize sizeSb;

    if (bCalcClient)
    {
        // Get client rect
        if (!GetTrueClientSize(sizeClient, sizeSb))
        {
            // No room for scroll bars (common for zero sized elements)
            CRect rect;
            GetClientRect(&rect);
            if (rect.right > 0 && rect.bottom > 0)
            {
                // If entire client area is not invisible, assume we have
                // control over our scrollbars
                EnableScrollBarCtrl(SB_BOTH, FALSE);
            }
            m_bInsideUpdate = FALSE;
            return;
        }
    }
    else
    {
        // Let parent window determine the "client" rect
        GetScrollBarSizes(sizeSb);
        sizeClient.cx = rectClient.right - rectClient.left;
        sizeClient.cy = rectClient.bottom - rectClient.top;
    }

    // Enough room to add scrollbars
    CSize sizeRange;
    CPoint ptMove;
    CSize needSb;

    // Get the current scroll bar state given the true client area
    GetScrollBarState(sizeClient, needSb, sizeRange, ptMove, bCalcClient);
    if (needSb.cx)
        sizeClient.cy -= sizeSb.cy;
    if (needSb.cy)
        sizeClient.cx -= sizeSb.cx;

    // First scroll the window as needed
    ScrollToPosition(ptMove); // will set the scroll bar positions too

    // This structure needed to update the scrollbar page range
    SCROLLINFO info;
    info.fMask = SIF_PAGE|SIF_RANGE;
    info.nMin = 0;

    // Now update the bars as appropriate
    EnableScrollBarCtrl(SB_HORZ, needSb.cx);
    if (needSb.cx)
    {
        info.nPage = sizeClient.cx;
        info.nMax = m_sizeTotal.cx-1;
        if (!SetScrollInfo(SB_HORZ, &info, TRUE))
            SetScrollRange(SB_HORZ, 0, sizeRange.cx, TRUE);
    }
    EnableScrollBarCtrl(SB_VERT, needSb.cy);
    if (needSb.cy)
    {
        info.nPage = sizeClient.cy;
        info.nMax = m_sizeTotal.cy-1;
        if (!SetScrollInfo(SB_VERT, &info, TRUE))
            SetScrollRange(SB_VERT, 0, sizeRange.cy, TRUE);
    }

    // Prevent recursion lockout
    m_bInsideUpdate = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CScrollWindow scrolling

void CScrollWindow::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    if (pScrollBar != NULL && pScrollBar->SendChildNotifyLastMsg())
    {
        return;// Eat it
    }
    
    // Ignore scroll bar messages from other controls
    if (pScrollBar != GetScrollBarCtrl(SB_HORZ))
    {
        return;
    }
    
    OnScroll(MAKEWORD(nSBCode, -1), nPos);
}

void CScrollWindow::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    if (pScrollBar != NULL && pScrollBar->SendChildNotifyLastMsg())
    {
        return;// Eat it
    }
    
    // Ignore scroll bar messages from other controls
    if (pScrollBar != GetScrollBarCtrl(SB_VERT))
    {
        return;
    }
    
    OnScroll(MAKEWORD(-1, nSBCode), nPos);
}

BOOL CScrollWindow::OnMouseWheel(UINT fFlags, short zDelta, CPoint point)
{
    // We don't handle anything but scrolling just now
    if (fFlags & (MK_SHIFT | MK_CONTROL))
    {
        return FALSE;
    }
    
    // We can't get out of it--perform the scroll ourselves
    return DoMouseWheel(fFlags, zDelta, point);
}

BOOL CScrollWindow::DoMouseWheel(UINT fFlags, short zDelta, CPoint point)
{
    UNUSED_ALWAYS(point);
    UNUSED_ALWAYS(fFlags);
    
    // if we have a vertical scroll bar, the wheel scrolls that
    // if we have _only_ a horizontal scroll bar, the wheel scrolls that
    // otherwise, don't do any work at all
    
    DWORD dwStyle = GetStyle();
    CScrollBar* pBar = GetScrollBarCtrl(SB_VERT);
    BOOL bHasVertBar = ((pBar != NULL) && pBar->IsWindowEnabled()) ||
        (dwStyle & WS_VSCROLL);
    
    pBar = GetScrollBarCtrl(SB_HORZ);
    BOOL bHasHorzBar = ((pBar != NULL) && pBar->IsWindowEnabled()) ||
        (dwStyle & WS_HSCROLL);
    
    if (!bHasVertBar && !bHasHorzBar)
        return FALSE;
    
    BOOL bResult = FALSE;
    UINT uWheelScrollLines = GetMouseScrollLines();
    int nToScroll;
    int nDisplacement;
    
    if (bHasVertBar)
    {
        nToScroll = ::MulDiv(-zDelta, uWheelScrollLines, WHEEL_DELTA);
        if (nToScroll == -1 || uWheelScrollLines == WHEEL_PAGESCROLL)
        {
            nDisplacement = m_sizePage.cy;
            if (zDelta > 0)
                nDisplacement = -nDisplacement;
        }
        else
        {
            nDisplacement = nToScroll * m_sizeLine.cy;
            nDisplacement = min(nDisplacement, m_sizePage.cy);
        }
        bResult = OnScrollBy(CSize(0, nDisplacement), TRUE);
    }
    else if (bHasHorzBar)
    {
        nToScroll = ::MulDiv(-zDelta, uWheelScrollLines, WHEEL_DELTA);
        if (nToScroll == -1 || uWheelScrollLines == WHEEL_PAGESCROLL)
        {
            nDisplacement = m_sizePage.cx;
            if (zDelta > 0)
                nDisplacement = -nDisplacement;
        }
        else
        {
            nDisplacement = nToScroll * m_sizeLine.cx;
            nDisplacement = min(nDisplacement, m_sizePage.cx);
        }
        bResult = OnScrollBy(CSize(nDisplacement, 0), TRUE);
    }
    
    if (bResult)
        UpdateWindow();
    
    return bResult;
}

BOOL CScrollWindow::OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll)
{
    UNUSED_ALWAYS(nPos);// The actual value of nPos is only 16-bit.

    // Calculate new x position
    int x = GetScrollPos(SB_HORZ);
    int xOrig = x;
    SCROLLINFO si;
    
    switch (LOBYTE(nScrollCode))
    {
    case SB_TOP:
        x = 0;
        break;
    case SB_BOTTOM:
        x = INT_MAX;
        break;
    case SB_LINEUP:
        x -= m_sizeLine.cx;
        break;
    case SB_LINEDOWN:
        x += m_sizeLine.cx;
        break;
    case SB_PAGEUP:
        x -= m_sizePage.cx;
        break;
    case SB_PAGEDOWN:
        x += m_sizePage.cx;
        break;
    case SB_THUMBTRACK:
        si.fMask = SIF_TRACKPOS;
        if(GetScrollInfo(SB_HORZ, &si))
        {
            x = si.nTrackPos;
        }
        break;
    }
    
    // Calculate new y position
    int y = GetScrollPos(SB_VERT);
    int yOrig = y;

    switch (HIBYTE(nScrollCode))
    {
    case SB_TOP:
        y = 0;
        break;
    case SB_BOTTOM:
        y = INT_MAX;
        break;
    case SB_LINEUP:
        y -= m_sizeLine.cy;
        break;
    case SB_LINEDOWN:
        y += m_sizeLine.cy;
        break;
    case SB_PAGEUP:
        y -= m_sizePage.cy;
        break;
    case SB_PAGEDOWN:
        y += m_sizePage.cy;
        break;
    case SB_THUMBTRACK:
        si.fMask = SIF_TRACKPOS;
        if(GetScrollInfo(SB_VERT, &si))
        {
            y = si.nTrackPos;
        }
        break;
    }
    
    BOOL bResult = OnScrollBy(CSize(x - xOrig, y - yOrig), bDoScroll);
    if (bResult && bDoScroll)
        UpdateWindow();
    
    return bResult;
}

BOOL CScrollWindow::OnScrollBy(CSize sizeScroll, BOOL bDoScroll)
{
    int xOrig, x;
    int yOrig, y;
    
    // don't scroll if there is no valid scroll range (ie. no scroll bar)
    CScrollBar* pBar;
    DWORD dwStyle = GetStyle();
    pBar = GetScrollBarCtrl(SB_VERT);
    if ((pBar != NULL && !pBar->IsWindowEnabled()) ||
        (pBar == NULL && !(dwStyle & WS_VSCROLL)))
    {
        // vertical scroll bar not enabled
        sizeScroll.cy = 0;
    }
    pBar = GetScrollBarCtrl(SB_HORZ);
    if ((pBar != NULL && !pBar->IsWindowEnabled()) ||
        (pBar == NULL && !(dwStyle & WS_HSCROLL)))
    {
        // horizontal scroll bar not enabled
        sizeScroll.cx = 0;
    }
    
    // adjust current x position
    xOrig = x = GetScrollPos(SB_HORZ);
    int xMax = GetScrollLimit(SB_HORZ);
    x += sizeScroll.cx;
    if (x < 0)
        x = 0;
    else if (x > xMax)
        x = xMax;

    // adjust current y position
    yOrig = y = GetScrollPos(SB_VERT);
    int yMax = GetScrollLimit(SB_VERT);
    y += sizeScroll.cy;
    if (y < 0)
        y = 0;
    else if (y > yMax)
        y = yMax;
    
    // did anything change?
    if (x == xOrig && y == yOrig)
        return FALSE;
    
    if (bDoScroll)
    {
        // do scroll and update scroll positions
        ScrollWindow(-(x-xOrig), -(y-yOrig));
        if (x != xOrig)
            SetScrollPos(SB_HORZ, x);
        if (y != yOrig)
            SetScrollPos(SB_VERT, y);
    }
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Overridables
void CScrollWindow::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
    ASSERT_VALID(pDC);
    
    CPoint ptVpOrg(0, 0);// Assume no shift for printing
    
    if (!pDC->IsPrinting())
    {
        ASSERT(pDC->GetWindowOrg() == CPoint(0,0));
        
        // By default shift view port origin in negative direction of scroll
        ptVpOrg = -GetScrollPosition();   
    }
    
    pDC->SetViewportOrg(ptVpOrg);
}

void CScrollWindow::OnDraw(CDC* pDC)
{
    
}

/////////////////////////////////////////////////////////////////////////////
// CScrollWindow message handlers

void CScrollWindow::OnSettingChange(UINT uFlags, LPCTSTR lpszSection) 
{
    m_bGotMouseScrollLines = FALSE;    

    CWnd::OnSettingChange(uFlags, lpszSection);

}

void CScrollWindow::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	OnPrepareDC(&dc);
    OnDraw(&dc);
}

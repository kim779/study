// CNotePaneWnd.cpp: 구현 파일
//

#include "pch.h"
#include "TestDlg.h"
#include "CNotePaneWnd.h"
#include "CNoteWnd.h"


// CNotePaneWnd

IMPLEMENT_DYNAMIC(CNotePaneWnd, CWnd)

#define TM_TEST 9999
//CNotePaneWnd::CNotePaneWnd()
//{
//
//}

CNotePaneWnd::CNotePaneWnd(bool (*callback)(int, WPARAM, LPARAM))
{
    m_sticker_call = callback;
}

CNotePaneWnd::~CNotePaneWnd()
{

}

BEGIN_MESSAGE_MAP(CNotePaneWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
    ON_WM_TIMER()
    ON_WM_MOUSEHWHEEL()
    ON_WM_VSCROLL()
    ON_WM_HSCROLL()
    ON_WM_MOUSEWHEEL()
    ON_WM_RBUTTONUP()
END_MESSAGE_MAP()



// CNotePaneWnd 메시지 처리기




void CNotePaneWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.

	COLORREF startColor = RGB(255, 255, 255); // 파란색

	CRect clientRc;
	GetClientRect(&clientRc);
	dc.FillSolidRect(clientRc, startColor);
}


void CNotePaneWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_slog.Format("cx =[%d] cy=[%d]", cx, cy);
    WriteDataOnFunc(m_slog);

    for (int ii = m_vNoteWindows.size() - 1; ii >= 0; ii--)
    {
        CNoteWnd* pWnd = (CNoteWnd*)m_vNoteWindows.at(ii);
        CRect rec;
        pWnd->GetWindowRect(rec);
        ScreenToClient(rec);

        rec.right = cx - DF_PANE_RIGHTMARGIN - GetSystemMetrics(SM_CXVSCROLL);
        pWnd->MoveWindow(rec);
    }

}


int CNotePaneWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
    SetTimer(TM_TEST, 100, nullptr);
	return 0;
}

void CNotePaneWnd::ArrangNoteWnd(bool btop)
{
    if (btop) 
    {
        for (int ii = m_vNoteWindows.size() - 1; ii >= 0; ii--)
        {
            CNoteWnd* pWnd = (CNoteWnd*)m_vNoteWindows.at(ii);
            CRect rec;
            pWnd->GetWindowRect(rec);
            ScreenToClient(rec);

            int  idel = m_vNoteWindows.size() - 1 - ii;
            rec.top =idel * (DF_NOTEHEIGHT + DF_NOTEHMARGIN);
            rec.bottom = rec.top + DF_NOTEHEIGHT;

            pWnd->MoveWindow(rec);
        }
    }
    else
    {

    }
}

void CNotePaneWnd::AddNoteWnd(CString strData)
{
    CRect clientrect;
    GetClientRect(&clientrect);
    adjustScroll(SB_VERT);

    int itop = 0;
    CNoteWnd* pChildWnd = new CNoteWnd();
    pChildWnd->Create(NULL, _T("ChildWnd"), WS_CHILD | WS_VISIBLE,
                                     CRect(DF_PANE_RIGHTMARGIN, itop, clientrect.Width() - DF_PANE_RIGHTMARGIN  /*- GetSystemMetrics(SM_CXVSCROLL)*/,
                                        itop + DF_NOTEHEIGHT), this, 0x9999);

    if (strData.IsEmpty())
        pChildWnd->m_data.Format("[%p] [%d]", pChildWnd, m_vNoteWindows.size());
    else
        pChildWnd->m_data.Format("[%p] [%s]", pChildWnd, strData);

    m_vNoteWindows.push_back(pChildWnd);
    m_StartPnt.y++;

    SetScrollPos32(SB_VERT, 0);   //AddNoteWnd

    adjustScroll(SB_VERT);

    ArrangNoteWnd();
    InvalidateRect(clientrect);
}

void CNotePaneWnd::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    switch (nIDEvent)
    {
        case TM_TEST:
        {
            KillTimer(nIDEvent);
            for (int ii = 0; ii < 20; ii++)
                AddNoteWnd("");
        }
        break;
    }
    CWnd::OnTimer(nIDEvent);
}

void	CNotePaneWnd::adjustScroll(int wBar)
{
    CRect clientRc;
    GetClientRect(&clientRc);

    if (wBar == SB_HORZ || wBar == SB_BOTH)
    {
        if (DF_NOTEWIDTH > clientRc.Width())
        {
            SCROLLINFO info{};
            info.fMask = SIF_RANGE | SIF_PAGE | SIF_TRACKPOS;
            GetScrollInfo(SB_HORZ, &info);
            info.fMask |= (m_wDisableBar & SB_HORZ ? SIF_DISABLENOSCROLL : NULL);
            info.cbSize = sizeof(SCROLLINFO);
            info.nMin = 0;
            info.nMax = 1;
            info.nPos = m_StartPnt.x;
            info.nTrackPos = m_StartPnt.x;

            SetScrollInfo(SB_HORZ, &info);
            ShowScrollBar(SB_HORZ);
        }
        else
        {
            if (m_wDisableBar & SB_HORZ)
            {
                SCROLLINFO info{};
                info.fMask = SIF_RANGE | SIF_PAGE | SIF_TRACKPOS | SIF_DISABLENOSCROLL;
                SetScrollInfo(SB_HORZ, &info);
            }
            else
                ShowScrollBar(SB_HORZ, FALSE);
        }
    }

    if (wBar == SB_VERT || wBar == SB_BOTH)
    {
        // if (m_lHeadHeight + m_lDataHeight * m_childWindows.size() > clientRc.Height())
        {
            SCROLLINFO info{};
            info.fMask = SIF_RANGE | SIF_PAGE | SIF_TRACKPOS;
            GetScrollInfo(SB_VERT, &info);
            info.fMask |= (m_wDisableBar & SB_VERT ? SIF_DISABLENOSCROLL : NULL);
            info.cbSize = sizeof(SCROLLINFO);
            info.nMin = 0;
            info.nMax = ((int)m_vNoteWindows.size() - (clientRc.Height() / DF_NOTEHEIGHT)) * DF_NOSCROLLRATE;
            info.nPage = 1;
            info.nPos = m_StartPnt.y;
            info.nTrackPos = m_StartPnt.y;
            SetScrollInfo(SB_VERT, &info);
            ShowScrollBar(SB_VERT);
        }
        /*   else
           {
               if (m_wDisableBar & SB_VERT)
               {
                   SCROLLINFO info{};
                   info.fMask = SIF_RANGE | SIF_PAGE | SIF_TRACKPOS | SIF_DISABLENOSCROLL;
                   SetScrollInfo(SB_VERT, &info);
               }
               else
                   ShowScrollBar(SB_VERT, FALSE);
           }*/
    }
}

long CNotePaneWnd::GetScrollPos32(int nBar, BOOL bGetTrackPos)
{
    SCROLLINFO sinfo{};
    sinfo.cbSize = sizeof(SCROLLINFO);

    if (bGetTrackPos)
    {
        if (GetScrollInfo(nBar, &sinfo, SIF_TRACKPOS))
        {
            m_slog.Format("\r\n[NOTE][%s] nTrackPos=[%d] ", __FUNCTION__, sinfo.nTrackPos);
            OutputDebugString(m_slog);
            return sinfo.nTrackPos;
        }
    }
    else
    {
        if (GetScrollInfo(nBar, &sinfo, SIF_POS))
        {
            m_slog.Format("\r\n[NOTE][%s] nPos=[%d] ", __FUNCTION__, sinfo.nPos);
            OutputDebugString(m_slog);
            return sinfo.nPos;
        }
    }

    return 0;
}

long CNotePaneWnd::GetScrollMax32(int nBar)
{
    SCROLLINFO sinfo{};
    sinfo.cbSize = sizeof(SCROLLINFO);

    if (GetScrollInfo(nBar, &sinfo, SIF_RANGE))
    {
        m_slog.Format("\r\n[NOTE][%s] nMax=[%d] ", __FUNCTION__, sinfo.nMax);
        OutputDebugString(m_slog);

        return sinfo.nMax;
    }
    return 0;
}

BOOL CNotePaneWnd::SetScrollPos32(int nBar, int nPos, BOOL bRedraw)
{
    m_slog.Format("\r\n[NOTE][%s] nBar=[%d] nPos=[%d] bRedraw=[%d]", __FUNCTION__, nBar, nPos, bRedraw);
    OutputDebugString(m_slog);

    SCROLLINFO sinfo{};

    sinfo.cbSize = sizeof(SCROLLINFO);
    sinfo.fMask = SIF_POS;
    sinfo.nPos = nPos;

    if (nBar == SB_VERT)
        m_StartPnt.y = nPos;
    else if (nBar == SB_HORZ)
        m_StartPnt.x = nPos;

    return SetScrollInfo(nBar, &sinfo, bRedraw);
}

void CNotePaneWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    int	scrollPos{}, scrollMax{};
    CRect clientRC;

    GetClientRect(&clientRC);
    clientRC.top = 0;
    scrollPos = GetScrollPos32(SB_VERT);
    scrollMax = GetScrollMax32(SB_VERT);

    switch (nSBCode)
    {
        case SB_LINEDOWN:
        {
            if (scrollPos + 1 <= scrollMax)
            {
                scrollPos = min(scrollPos + 1, scrollMax);
                SetScrollPos32(SB_VERT, scrollPos); 
                m_StartPnt.y = scrollPos;
            
                for (int ii = 0; ii < m_vNoteWindows.size(); ii++)
                {
                    CNoteWnd* pWnd = (CNoteWnd*)m_vNoteWindows.at(ii);
                    CRect rec;
                    pWnd->GetWindowRect(rec);
                    ScreenToClient(rec);
                    rec.OffsetRect(0, -(DF_NOTEHEIGHT/ DF_NOSCROLLRATE));

                    m_slog.Format("\r\n [%s] [%d]", pWnd->m_data, rec.top);
                    TRACE(m_slog);

                    pWnd->MoveWindow(rec);
                }
            }

            if (scrollPos == scrollMax)
                 SendMessage(WM_VSCROLL, SB_BOTTOM, (LPARAM)scrollPos);
        }
        break;
        case SB_LINEUP:
        {
            if (scrollPos - 1 >= 0)
            {
                scrollPos = max(scrollPos - 1, 0);
                SetScrollPos32(SB_VERT, scrollPos);
                m_StartPnt.y = scrollPos;

                for (int ii = 0; ii < m_vNoteWindows.size(); ii++)
                {
                    CNoteWnd* pWnd = (CNoteWnd*)m_vNoteWindows.at(ii);
                    CRect rec;
                    pWnd->GetWindowRect(rec);
                    ScreenToClient(rec);
                    rec.OffsetRect(0, DF_NOTEHEIGHT/ DF_NOSCROLLRATE);

                    pWnd->MoveWindow(rec);
                }
            }

            if (scrollPos == 0)
                SendMessage(WM_VSCROLL, SB_TOP, (LPARAM)scrollPos);
        }
        break;
        case SB_THUMBPOSITION:
        case SB_THUMBTRACK:
        {
            if (nPos >= 0 && (long)nPos <= scrollMax)
            {
                const long nDelta = nPos - m_StartPnt.y;

                m_slog.Format("\r\n[NOTE][%s]  nPos=[%d]  m_StartPnt.y=[%d]  nDelta=[%d]  m_lDataHeight=[%d] ", __FUNCTION__, nPos, m_StartPnt.y, nDelta, DF_NOTEHEIGHT);
                OutputDebugString(m_slog);

                if (nDelta > 0)
                {
                    for (int ii = 0; ii < m_vNoteWindows.size(); ii++)
                    {
                        CNoteWnd* pWnd = (CNoteWnd*)m_vNoteWindows.at(ii);
                        CRect rec;
                        pWnd->GetWindowRect(rec);
                        ScreenToClient(rec);
                        rec.OffsetRect(0, -1 * nDelta * DF_NOTEHEIGHT / DF_NOSCROLLRATE);

                        pWnd->MoveWindow(rec);
                    }
                }
                else if (nDelta < 0)
                {
                    for (int ii = 0; ii < m_vNoteWindows.size(); ii++)
                    {
                        CNoteWnd* pWnd = (CNoteWnd*)m_vNoteWindows.at(ii);
                        CRect rec;
                        pWnd->GetWindowRect(rec);
                        ScreenToClient(rec);
                        rec.OffsetRect(0, -1 * nDelta * DF_NOTEHEIGHT / DF_NOSCROLLRATE);

                        pWnd->MoveWindow(rec);
                    }
                }

                m_StartPnt.y = nPos;
                SetScrollPos32(SB_VERT, nPos);

                if (nPos == scrollMax)
                    SendMessage(WM_VSCROLL, SB_BOTTOM, (LPARAM)scrollPos);

                if (nPos == 0)
                    SendMessage(WM_VSCROLL, SB_TOP, (LPARAM)scrollPos);

                InvalidateRect(&clientRC);
            }
        }
        break;
    case SB_PAGEDOWN:
    {
        m_slog.Format("\r\n[NOTE][%s]SB_PAGEDOWN  [%d] ", __FUNCTION__, GetScrollPos32(SB_VERT));
        OutputDebugString(m_slog);
        SendMessage(WM_VSCROLL, SB_LINEDOWN, (LPARAM)scrollPos);
    }
    break;
    case SB_PAGEUP:
    {
        m_slog.Format("\r\n[NOTE][%s]SB_PAGEUP  [%d]", __FUNCTION__, GetScrollPos32(SB_VERT));
        OutputDebugString(m_slog);
        SendMessage(WM_VSCROLL, SB_LINEUP, (LPARAM)scrollPos);
    }
    break;
    case SB_TOP:
    {
        SetScrollPos32(SB_VERT, 0);
        m_StartPnt.y = 0;
        ArrangNoteWnd();
    }
    break;
    case SB_BOTTOM:
    {
        SetScrollPos32(SB_VERT, scrollMax);
        m_StartPnt.y = scrollMax;

        int idel{};
        bool bfirst = true;
        for (int ii = 0 ;  ii < m_vNoteWindows.size() ; ii++)
        {
            CNoteWnd* pWnd = (CNoteWnd*)m_vNoteWindows.at(ii);
            CRect rec;
            pWnd->GetWindowRect(rec);
            ScreenToClient(rec);

            rec.bottom = clientRC.bottom - ( ii * (DF_NOTEHEIGHT + DF_NOTEHMARGIN));
            rec.top = rec.bottom -  DF_NOTEHEIGHT;

            pWnd->MoveWindow(rec);
        }
    }
    break;
    }

    CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CNotePaneWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

    CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}


BOOL CNotePaneWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    if (GetScrollMax32(SB_VERT))
    {
        if (zDelta < 0)
        {
            SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, 0), 0);
            SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, 0), 0);
        }
        else
        {
            SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEUP, 0), 0);
            SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEUP, 0), 0);
        }
    }
    return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CNotePaneWnd::WriteDataOnFunc(CString sdata)
{
    GetParent()->SendMessage(WM_USER, MAKEWPARAM(MAKEWORD(MSG_FUNCWND_LOG, 0), 0), (LPARAM)sdata.GetBuffer(0));
}


void CNotePaneWnd::OnRButtonUp(UINT nFlags, CPoint point)
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    //WriteDataOnFunc("test");

    CString stmp;
    stmp = "tsetsetsets";
   ( *m_sticker_call)(MSG_FUNCWND_LOG, 0, (LPARAM)stmp.GetBuffer(0));
    CWnd::OnRButtonUp(nFlags, point);
}

// CAutoOrderList.cpp: 구현 파일
//

#include "stdafx.h"
#include "axis.h"
#include "CSlideWnd.h"
//#include "afxdialogex.h"
#include "CNoteWnd.h"

#include "MainFrm.h"


// CAutoOrderList 대화 상자

IMPLEMENT_DYNAMIC(CSlideWnd, CDialog)

CSlideWnd::CSlideWnd(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DLG_SLIDEWND, pParent)
{
	m_pMainFrm = (CMainFrame*)pParent;
}

CSlideWnd::~CSlideWnd()
{
}

void CSlideWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSlideWnd, CDialog)
	ON_WM_TIMER()
    ON_WM_PAINT()
    ON_WM_LBUTTONUP()
    ON_WM_SIZE()
    ON_WM_VSCROLL()
    ON_WM_HSCROLL()
    ON_WM_MOUSEHWHEEL()
    ON_WM_MOUSEWHEEL()
    ON_WM_RBUTTONUP()
    ON_BN_CLICKED(IDOK, &CSlideWnd::OnBnClickedOk)
END_MESSAGE_MAP()

void CSlideWnd::SlideOpen(CRect baseRC)
{
    int nDialogWidth = baseRC.Width();
    int nDialogHeight = baseRC.Height();

    for (int x = -nDialogWidth, width = 0; x <= 0; x += 20, width += 20)
    {
        if (width > nDialogWidth) 
            width = nDialogWidth;
        SetWindowPos(&wndTop, baseRC.left, baseRC.top, width, nDialogHeight, SWP_NOZORDER | SWP_SHOWWINDOW);
        UpdateWindow();
        Sleep(10);
    }
    SetWindowPos(&wndTop, baseRC.left, baseRC.top, nDialogWidth, nDialogHeight, SWP_NOZORDER | SWP_SHOWWINDOW);
    Invalidate();
}

void CSlideWnd::SlideClose(CRect baseRC)
{
    int nDialogWidth = baseRC.Width();
    int nDialogHeight = baseRC.Height();

    for (int x = 0, width = nDialogWidth; x >= -nDialogWidth; x -= 20, width -= 20)
    {
        if (width < 0)
            width = 0;

        SetWindowPos(&wndTop, baseRC.left, baseRC.top, width, nDialogHeight, SWP_NOZORDER | SWP_SHOWWINDOW);
        UpdateWindow();
        Sleep(5);
    }
    ShowWindow(SW_HIDE);
}

// CAutoOrderList 메시지 처리기


void CSlideWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnTimer(nIDEvent);
}

BOOL CSlideWnd::DrawRow(CDC* pDC, long nRow, CRect visibleRc, BOOL ShowLOG)
{
    if (nRow + m_StartPnt.y >= m_childWindows.size())
        return FALSE;
    if (m_lHeadHeight + m_lDataHeight * nRow > visibleRc.bottom)
        return FALSE;

    CRect cellRc = CRect(0, m_lHeadHeight + (m_lDataHeight * nRow), 0, m_lHeadHeight + (m_lDataHeight * (nRow + 1)));

    if (m_childWindows.size() < nRow + m_StartPnt.y)
        return FALSE;
    CNoteWnd* pWnd = (CNoteWnd*)m_childWindows.at(nRow + m_StartPnt.y);

    m_slog.Format("[NOTE]][DRAWING] nRow =[%d]   m_StartPnt.y=[%d]  data=[%s]", nRow, m_StartPnt.y, pWnd->m_data);
    OutputDebugString(m_slog);

    pWnd->Invalidate();
}

void CSlideWnd::DrawBack(CDC* pdc)
{
    //COLORREF startColor = RGB(147, 204, 240); // 파란색
    COLORREF startColor = RGB(255, 255, 255); // 파란색
    COLORREF endColor = RGB(188, 221, 240); // 밝은 파란색

    CRect rect;
    GetClientRect(&rect);
    pdc->FillSolidRect(rect, startColor);
    return;

    // 전체 다이얼로그 크기로 라운드 처리된 사각형 생성
    CRgn roundRgn;
    int roundness = 20; // 모서리의 라운드 정도를 설정 (원의 반지름)
    roundRgn.CreateRoundRectRgn(rect.left, rect.top, rect.right, rect.bottom, roundness, roundness);

    // 라운드 처리된 사각형에 클리핑 영역 설정
    pdc->SelectClipRgn(&roundRgn);

    // 그라데이션 그리기
    for (int i = 0; i < rect.Height(); ++i)
    {
        int r = GetRValue(startColor) + (i * (GetRValue(endColor) - GetRValue(startColor)) / rect.Height());
        int g = GetGValue(startColor) + (i * (GetGValue(endColor) - GetGValue(startColor)) / rect.Height());
        int b = GetBValue(startColor) + (i * (GetBValue(endColor) - GetBValue(startColor)) / rect.Height());

        pdc->FillSolidRect(0, i, rect.Width(), 1, RGB(r, g, b));
    }
}

void CSlideWnd::OnPaint()
{
    CPaintDC dc(this); // device context for painting
                       // TODO: 여기에 메시지 처리기 코드를 추가합니다.
                       // 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.
    
    CRect clipRc, bkRect;

        // 그라데이션 색상 설정
    DrawBack(&dc);

   /* for (int ii = 0; ii < m_childWindows.size(); ii++)
        if (!DrawRow(&dc, ii, clipRc))
            break;*/

   

    dc.SetBkMode(TRANSPARENT);
}



BOOL CSlideWnd::OnInitDialog()
{
    CDialog::OnInitDialog();

    CWnd* pOKButton = GetDlgItem(IDOK);
    if (pOKButton)
        pOKButton->ShowWindow(SW_HIDE);
    CWnd* pCancelButton = GetDlgItem(IDCANCEL);
    if (pCancelButton)
        pCancelButton->ShowWindow(SW_HIDE);

   // InitScroll();

    for(int ii = 0 ; ii < 20; ii++)
        AddOrderDetail();

    return TRUE;  // return TRUE unless you set the focus to a control
                  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

int CSlideWnd::getNottY()
{
    return  DF_NOTEHMARGIN + (m_childWindows.size() * (DF_NOTEHMARGIN + DF_NOTEHEIGHT));
}

void CSlideWnd::AddOrderDetail(CString strData)
{
    CRect rect;
    GetClientRect(&rect);

    
    adjustScroll(SB_VERT);

    CNoteWnd* pChildWnd = new CNoteWnd();
    pChildWnd->Create(NULL, _T("ChildWnd"), WS_CHILD | WS_VISIBLE , 
        CRect(DF_NOTEHMARGIN, getNottY(), rect.Width() - DF_NOTEHMARGIN * 9 - GetSystemMetrics(SM_CXVSCROLL),
            getNottY() + DF_NOTEHEIGHT), this, 0x9999);

    if(strData.IsEmpty())
        pChildWnd->m_data.Format("[%p] [%d]", pChildWnd, m_childWindows.size());
    else
        pChildWnd->m_data.Format("[%p] [%s]", pChildWnd, strData);

    m_childWindows.push_back(pChildWnd);
    m_StartPnt.y++;

    SetScrollPos32(SB_VERT, GetScrollPos32(SB_VERT) + 1);
    //SetScrollPos32(SB_VERT, 0);

    m_totalHeight = DF_NOTEHMARGIN + (m_childWindows.size() * (DF_NOTEHMARGIN + DF_NOTEHEIGHT)); // 총 높이 업데이트
    
    adjustScroll(SB_VERT);
}

void CSlideWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    m_pMainFrm->ShowSlideWnd();
    CDialog::OnLButtonUp(nFlags, point);
}


void CSlideWnd::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);


}






void CSlideWnd::InitScroll()
{
    CRect rect;
    GetClientRect(&rect);
    m_scrollBar.Create(SBS_VERT | WS_CHILD | WS_VISIBLE, CRect(rect.right - GetSystemMetrics(SM_CXVSCROLL), rect.top, rect.right, rect.bottom), this, 1);
}

void	CSlideWnd::adjustScroll(int wBar)
{
    CRect clientRc;
    GetClientRect(&clientRc);
    
    if (wBar == SB_HORZ || wBar == SB_BOTH)
    {
        if (DF_NOTEWIDTH > clientRc.Width())
        {
            SCROLLINFO info;
            info.fMask = SIF_RANGE | SIF_PAGE | SIF_TRACKPOS;
            GetScrollInfo(SB_HORZ, &info);
            info.fMask |= (m_wDisableBar & SB_HORZ ? SIF_DISABLENOSCROLL : NULL);
            info.cbSize = sizeof(SCROLLINFO);
            info.nMin = 0; 
            info.nMax =1;
            info.nPos = m_StartPnt.x;
            info.nTrackPos = m_StartPnt.x;

            SetScrollInfo(SB_HORZ, &info);
            ShowScrollBar(SB_HORZ);
        }
        else
        {
            if (m_wDisableBar & SB_HORZ)
            {
                SCROLLINFO info;
                info.fMask = SIF_RANGE | SIF_PAGE | SIF_TRACKPOS | SIF_DISABLENOSCROLL;
                SetScrollInfo(SB_HORZ, &info);
            }
            else
                ShowScrollBar(SB_HORZ, FALSE);
        }
    }

    if (wBar == SB_VERT || wBar == SB_BOTH)
    {
        if (m_lHeadHeight + m_lDataHeight * m_childWindows.size() > clientRc.Height())
        {
            SCROLLINFO info;
            info.fMask = SIF_RANGE | SIF_PAGE | SIF_TRACKPOS;
            GetScrollInfo(SB_VERT, &info);
            info.fMask |= (m_wDisableBar & SB_VERT ? SIF_DISABLENOSCROLL : NULL);
            info.cbSize = sizeof(SCROLLINFO);
            info.nMin = 0;
            info.nMax = (int)m_childWindows.size() - (clientRc.Height() - m_lHeadHeight) / m_lDataHeight;
            info.nPage = 1;
            info.nPos = m_StartPnt.y;
            info.nTrackPos = m_StartPnt.y;
            SetScrollInfo(SB_VERT, &info);
            ShowScrollBar(SB_VERT);
        }
        else
        {
            if (m_wDisableBar & SB_VERT)
            {
                SCROLLINFO info;
                info.fMask = SIF_RANGE | SIF_PAGE | SIF_TRACKPOS | SIF_DISABLENOSCROLL;
                SetScrollInfo(SB_VERT, &info);
            }
            else
                ShowScrollBar(SB_VERT, FALSE);
        }
    }
}

long CSlideWnd::GetScrollPos32(int nBar, BOOL bGetTrackPos)
{
    SCROLLINFO sinfo;
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

long CSlideWnd::GetScrollMax32(int nBar)
{
    SCROLLINFO sinfo;
    sinfo.cbSize = sizeof(SCROLLINFO);

    if (GetScrollInfo(nBar, &sinfo, SIF_RANGE))
    {
m_slog.Format("\r\n[NOTE][%s] nMax=[%d] ", __FUNCTION__, sinfo.nMax);
OutputDebugString(m_slog);

        return sinfo.nMax;
    }
    return 0;
}

BOOL CSlideWnd::SetScrollPos32(int nBar, int nPos, BOOL bRedraw)
{
    m_slog.Format("\r\n[NOTE][%s] nBar=[%d] nPos=[%d] bRedraw=[%d]", __FUNCTION__, nBar, nPos, bRedraw);
    OutputDebugString(m_slog);

    SCROLLINFO sinfo;

    sinfo.cbSize = sizeof(SCROLLINFO);
    sinfo.fMask = SIF_POS;
    sinfo.nPos = nPos;

    return SetScrollInfo(nBar, &sinfo, bRedraw);
}

void CSlideWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    int	scrollPos{}, scrollMax{};
    CRect clientRc;

    GetClientRect(&clientRc);
    clientRc.top = m_lHeadHeight;
    scrollPos = GetScrollPos32(SB_VERT);
    scrollMax = GetScrollMax32(SB_VERT);

    m_slog.Format("\r\n[NOTE][%s] nSBCode=[%d] nPos=[%d] scrollPos=[%d] scrollMax=[%d]", __FUNCTION__, nSBCode, nPos, scrollPos, scrollMax);
    OutputDebugString(m_slog);

    switch (nSBCode)
    {
        case SB_LINEDOWN:
        {
            m_slog.Format("\r\n[NOTE][%s]SB_LINEDOWN", __FUNCTION__);
            OutputDebugString(m_slog);
            if (scrollPos + 1 <= scrollMax)
            {
                scrollPos = min(scrollPos + 1, scrollMax);
                SetScrollPos32(SB_VERT, scrollPos);
                m_StartPnt.y = scrollPos;
                clientRc.top += m_lDataHeight;
                ScrollWindow(0, -m_lDataHeight, &clientRc);
                clientRc.top = clientRc.bottom - m_lDataHeight;

                for (int ii = 0; ii < 20; ii++)
                {
                    CNoteWnd* pWnd = (CNoteWnd*)m_childWindows.at(ii);
                    CRect rec;
                    pWnd->GetWindowRect(rec);
                    ScreenToClient(rec);
                    rec.OffsetRect(0, -m_lDataHeight);

                    pWnd->MoveWindow(rec);
                }

                InvalidateRect(&clientRc);
            }

          //  if (scrollPos == scrollMax && m_parent)
           //     m_parent->SendMessage(WM_FROMGRID, SB_BOTTOM, (LPARAM)scrollPos);
        }
        break;
        case SB_LINEUP:
        {
            m_slog.Format("\r\n[NOTE][%s]SB_LINEUP", __FUNCTION__);
            OutputDebugString(m_slog);
            if (scrollPos - 1 >= 0)
            {
                scrollPos = max(scrollPos - 1, 0);
                SetScrollPos32(SB_VERT, scrollPos);
                m_StartPnt.y = scrollPos;
                clientRc.bottom -= m_lDataHeight;
                ScrollWindow(0, m_lDataHeight, &clientRc);
                clientRc.bottom = clientRc.top + m_lDataHeight;

                for (int ii = 0; ii < 20; ii++)
                {
                    CNoteWnd* pWnd = (CNoteWnd*)m_childWindows.at(ii);
                    CRect rec;
                    pWnd->GetWindowRect(rec);
                    ScreenToClient(rec);
                    rec.OffsetRect(0, m_lDataHeight);

                    pWnd->MoveWindow(rec);
                }

                InvalidateRect(&clientRc);
            }
         //   else if (scrollPos == 0)
          //      m_parent->SendMessage(WM_FROMGRID, SB_TOP, (LPARAM)scrollPos);
        }
        break;
        case SB_PAGEDOWN:
        {
            m_slog.Format("\r\n[NOTE][%s]SB_PAGEDOWN", __FUNCTION__);
            OutputDebugString(m_slog);
        }
        break;
        case SB_PAGEUP:
        {
            m_slog.Format("\r\n[NOTE][%s]SB_PAGEUP", __FUNCTION__);
            OutputDebugString(m_slog);
        }
        break;
        case SB_THUMBPOSITION:
        case SB_THUMBTRACK:
        {
            m_slog.Format("\r\n[NOTE][%s]SB_THUMBPOSITION SB_THUMBTRACK", __FUNCTION__);
            OutputDebugString(m_slog);
        }
        break;
        case SB_TOP:
        {
            m_slog.Format("\r\n[NOTE][%s]SB_TOP", __FUNCTION__);
            OutputDebugString(m_slog);
        }
            break;
        case SB_BOTTOM:
        {
            m_slog.Format("\r\n[NOTE][%s]SB_BOTTOM", __FUNCTION__);
            OutputDebugString(m_slog);
        }
            break;
        default:
        {
            m_slog.Format("\r\n[NOTE][%s]default", __FUNCTION__);
            OutputDebugString(m_slog);
        }
            break;
    }

    CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CSlideWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    OutputDebugString("\r\n ----HSCROLL---");
    CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CSlideWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    if (GetScrollMax32(SB_VERT))
    {
        if (zDelta < 0)
        {
            OutputDebugString("\r\n !!!!!  DOWN VSCROLL!!!!");
            SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, 0), 0);
            SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, 0), 0);
        }
        else
        {
            OutputDebugString("\r\n ---- UP VSCROLL---");
            SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEUP, 0), 0);
            SendMessage(WM_VSCROLL, MAKEWPARAM(SB_LINEUP, 0), 0);
        }
    }
    return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}


void CSlideWnd::OnRButtonUp(UINT nFlags, CPoint point)
{
    // TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
    CRect wrect;
    GetWindowRect(wrect);

    for (int ii = 0; ii < 20; ii++)
    {
        CNoteWnd* pWnd = (CNoteWnd*)m_childWindows.at(ii);
        CRect rec;
        pWnd->GetWindowRect(rec);

        pWnd->ShowWindow(SW_HIDE);
    }

    CNoteWnd* pWnd = (CNoteWnd*)m_childWindows.at(0);
    pWnd->ShowWindow(SW_HIDE);

    pWnd = (CNoteWnd*)m_childWindows.at(1);
    pWnd->ShowWindow(SW_HIDE);

    pWnd = (CNoteWnd*)m_childWindows.at(2);

    pWnd->ShowWindow(SW_HIDE);
    CDialog::OnRButtonUp(nFlags, point);
}


void CSlideWnd::OnBnClickedOk()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    CDialog::OnOK();
}

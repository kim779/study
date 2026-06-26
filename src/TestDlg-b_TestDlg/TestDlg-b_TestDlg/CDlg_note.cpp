// CDlg_note.cpp: 구현 파일
//

#include "pch.h"
#include "TestDlg.h"
#include "CDlg_note.h"
#include "afxdialogex.h"
#include "CNoteWnd.h"

#include "CNotePaneWnd.h"
#include "CFuncWnd.h"

// CDlg_note 대화 상자

IMPLEMENT_DYNAMIC(CDlg_note, CDialogEx)
static CDlg_note* m_pTopWnd = NULL;

bool sticker_call(int msg, WPARAM wParam, LPARAM lParam)
{
    CString sMsg;
    switch (msg)
    {
        case MSG_FUNCWND_LOG:
        {
            if (m_pTopWnd->m_pFuncWnd)
                m_pTopWnd->m_pFuncWnd->SendMessage(WM_USER, MAKEWPARAM(MAKEWORD(msg, 0), 0), lParam);
        }
        break;
    }

    return false;
}

CDlg_note::CDlg_note(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLGNOTE, pParent)
{

}

CDlg_note::~CDlg_note()
{
}

void CDlg_note::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDlg_note, CDialogEx)
    ON_WM_TIMER()
    ON_WM_PAINT()
    ON_WM_LBUTTONUP()
    ON_WM_SIZE()
    ON_WM_VSCROLL()
    ON_WM_HSCROLL()
    ON_WM_MOUSEHWHEEL()
    ON_WM_MOUSEWHEEL()
    ON_WM_RBUTTONUP()
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEMOVE()
    ON_MESSAGE(WM_USER, OnMessage)
END_MESSAGE_MAP()


// CDlg_note 메시지 처리기
LRESULT CDlg_note::OnMessage(WPARAM wParam, LPARAM lParam)
{
    int len = 0;
    switch (LOBYTE(LOWORD(wParam)))
    {
        case MSG_FUNCWND_LOG:
        {
            if (m_pFuncWnd)
                m_pFuncWnd->SendMessage(WM_USER, wParam, lParam);
        }
        break;
    }
    return FALSE;
}

BOOL CDlg_note::OnInitDialog()
{
	CDialogEx::OnInitDialog();

    CWnd* pOKButton = GetDlgItem(IDOK);
    if (pOKButton)
        pOKButton->ShowWindow(SW_HIDE);
    CWnd* pCancelButton = GetDlgItem(IDCANCEL);
    if (pCancelButton)
        pCancelButton->ShowWindow(SW_HIDE);

    m_pTopWnd = this;
    SetWindowText("스 티 커");

    CRect rec;
    GetClientRect(rec);

    m_pPaneWnd = std::make_unique<CNotePaneWnd>(sticker_call);
    m_pPaneWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL, CRect(0,0,0,0), this, 1000);

    m_pFuncWnd = std::make_unique<CFuncWnd>(sticker_call);
    m_pFuncWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL, CRect(0, 0, 0, 0), this, 1001);
    SetWindowPos(&wndTop, 0, 0, rec.Width(), rec.Height(), SWP_NOMOVE );


    SetTimer(9898, 10, nullptr);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

int CDlg_note::getNottY()
{
    return  DF_NOTEHMARGIN + (m_childWindows.size() * (DF_NOTEHMARGIN + DF_NOTEHEIGHT));
}

void CDlg_note::ArrangNoteWnd(bool btop)
{
   
}

void CDlg_note::AddOrderDetail(CString strData)
{
  
}
void CDlg_note::OnTimer(UINT_PTR nIDEvent)
{
    switch (nIDEvent)
    {
        case 9898:
        {
            KillTimer(nIDEvent);
            CRect rec;
            GetWindowRect(rec);
            SetWindowPos(NULL, rec.left, rec.top, DF_TOTALWIDTH, rec.Height(), SWP_NOZORDER | SWP_NOMOVE);
        }
        break;
    }
    CDialog::OnTimer(nIDEvent);
}

void CDlg_note::OnPaint()
{
    CPaintDC dc(this); // device context for painting
                       // 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.
    //COLORREF startColor = RGB(255, 255, 255); // 파란색
    //COLORREF endColor = RGB(188, 221, 240); // 밝은 파란색

    //CRect clientRc;
    //GetClientRect(&clientRc);
    //dc.FillSolidRect(clientRc, startColor);

    //bool bFrist = true;
    //int idel{};
    //for (int ii = m_childWindows.size() - 1 ; ii >= 0; ii--)
    //{
    //    CNoteWnd* pWnd = (CNoteWnd*)m_childWindows.at(ii);
    //    CRect rec;
    //    pWnd->GetWindowRect(rec);
    //    ScreenToClient(rec);

    //    idel = m_childWindows.size() - 1 - ii;
    //    if(ii == m_childWindows.size() - 1)
    //        idel *= m_lDataHeight;
    //    else
    //        idel *= m_lDataHeight + DF_NOTEHMARGIN;

    //    rec.top = idel;
    //    rec.bottom = rec.top +  m_lDataHeight;

    //    pWnd->MoveWindow(rec);
    //}
}

void CDlg_note::OnSize(UINT nType, int cx, int cy)
{
    CDialog::OnSize(nType, cx, cy);

    if (m_pPaneWnd && m_pFuncWnd)
    {
        m_pFuncWnd->SetWindowPos(NULL, 0, 0, cx, DF_FUNCHEIGHT, SWP_NOZORDER);
        m_pFuncWnd->SendMessage(WM_SIZE, nType, MAKELPARAM(cx, DF_FUNCHEIGHT));

        m_pPaneWnd->SetWindowPos(NULL, 0, DF_FUNCHEIGHT, cx, cy - DF_FUNCHEIGHT, SWP_NOZORDER );
        m_pPaneWnd->SendMessage(WM_SIZE, nType, MAKELPARAM(cx, cy - DF_FUNCHEIGHT));
     }
}

void CDlg_note::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{

}

BOOL CDlg_note::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
   
    return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CDlg_note::OnRButtonUp(UINT nFlags, CPoint point)
{
    CDialog::OnRButtonUp(nFlags, point);
   

   /* CRect rec;
    GetWindowRect(rec);
    m_pPaneWnd = std::make_unique<CNotePaneWnd>();
    m_pPaneWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL, CRect(0, 0, 0, 0), this, 1000);

    m_pFuncWnd = std::make_unique<CFuncWnd>();
    m_pFuncWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL, CRect(0, 0, 0, 0), this, 1001);
    SetWindowPos(&wndTop, 0, 0, rec.Width() + 1, rec.Height(), SWP_NOMOVE);*/
    
}

void CDlg_note::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{

    CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void	CDlg_note::adjustScroll(int wBar)
{
   
}

long CDlg_note::GetScrollPos32(int nBar, BOOL bGetTrackPos)
{
  
    return 0;
}

long CDlg_note::GetScrollMax32(int nBar)
{
 
    return 0;
}

BOOL CDlg_note::IsPointInScrollBarArea(CPoint point)
{
    CRect clientRect;
    GetClientRect(&clientRect);

    SCROLLBARINFO sbi;
    ZeroMemory(&sbi, sizeof(sbi));
    sbi.cbSize = sizeof(SCROLLBARINFO);

    if (!::GetScrollBarInfo(m_hWnd, OBJID_VSCROLL, &sbi))
        return FALSE;

    CRect scrollBarRect = sbi.rcScrollBar;
    return scrollBarRect.PtInRect(point);
}

void CDlg_note::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (IsPointInScrollBarArea(point))
    {
        m_bDragging = TRUE;
        m_nLastYPos = point.y;
        SetCapture();
    }
    CDialogEx::OnLButtonDown(nFlags, point);
}

void CDlg_note::OnLButtonUp(UINT nFlags, CPoint point)
{
    if (m_bDragging)
    {
        m_bDragging = FALSE;
        ReleaseCapture();
    }
    CDialog::OnLButtonUp(nFlags, point);
}

void CDlg_note::OnMouseMove(UINT nFlags, CPoint point)
{


    CDialogEx::OnMouseMove(nFlags, point);
}


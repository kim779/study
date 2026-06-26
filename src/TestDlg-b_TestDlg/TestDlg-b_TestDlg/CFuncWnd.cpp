// CFuncWnd.cpp: 구현 파일
//

#include "pch.h"
#include "TestDlg.h"
#include "CFuncWnd.h"

#include "NoteDefine.h"




// CFuncWnd

IMPLEMENT_DYNAMIC(CFuncWnd, CWnd)

CFuncWnd::CFuncWnd(bool (*callback)(int, WPARAM, LPARAM))
{
    m_sticker_call = callback;
}

CFuncWnd::~CFuncWnd()
{
}


BEGIN_MESSAGE_MAP(CFuncWnd, CWnd)
	ON_WM_PAINT()
    ON_MESSAGE(WM_USER, OnMessage)
    ON_WM_CREATE()
    ON_WM_SIZE()
END_MESSAGE_MAP()



// CFuncWnd 메시지 처리기


int CFuncWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  여기에 특수화된 작성 코드를 추가합니다.
    Init();
    return 0;
}

void CFuncWnd::Init()
{
    const DWORD dwStyle = WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
    m_pEdit = std::make_unique<CSedit>();
    if (!m_pEdit->Create(NULL, "SearchEdit", dwStyle, CRect(0, 0, 0, 0), this, -1))
        m_pEdit = nullptr;
    else
        m_pEdit->init(m_sticker_call);

}

LRESULT CFuncWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
    int len = 0;
    switch (LOBYTE(LOWORD(wParam)))
    {
        case MSG_FUNCWND_LOG:
        {
            m_strData.Format("%s", lParam);
            Invalidate();
        }
        break;
    }

    return FALSE;
}

void CFuncWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.

	CRect clientRC;
	GetClientRect(&clientRC);
    //dc.FillSolidRect(clientRC, RGB(255,255,255));
	//dc.FillSolidRect(m_Rect, COLOR_FUNC_BACK_COL);

    if (m_strData.GetLength() > 0)
    {
       // dc.SetBkMode(TRANSPARENT);
       
        clientRC.top += clientRC.Height() / 2;
        clientRC.top += DF_NOTEHMARGIN;
        clientRC.left += DF_PANE_RIGHTMARGIN;
        dc.FillSolidRect(clientRC, COLOR_FUNC_BACK_COL);
        dc.DrawText(m_strData, clientRC, DT_LEFT);
    }

}


void CFuncWnd::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);

    // TODO: 여기에 메시지 처리기 코드를 추가합니다.
    GetClientRect(m_Rect);
    m_Rect.DeflateRect(DF_NOTEHMARGIN, DF_NOTEHMARGIN);
    if (m_pEdit)
        m_pEdit->MoveWindow(CRect(m_Rect.left, m_Rect.top, m_Rect.right, m_Rect.top+  m_Rect.Height()/2));
}

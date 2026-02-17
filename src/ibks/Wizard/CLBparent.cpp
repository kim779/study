// CLBparent.cpp: 구현 파일
//

#include "pch.h"
#include "CLBparent.h"
#include "Client.h"

// CLBparent

IMPLEMENT_DYNAMIC(CLBparent, CWnd)

CLBparent::CLBparent()
{
    m_pbuf = new char[1000];
    memcpy(m_pbuf, "123456789012345678901234567890", 30);
 
}

CLBparent::~CLBparent()
{
    if (m_pbuf)
    {
        delete[] m_pbuf;
        m_pbuf = nullptr;
    }
}

BEGIN_MESSAGE_MAP(CLBparent, CWnd)
    ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CLBparent 메시지 처리기
BOOL CLBparent::CreatePopUpWindow(CWnd* pParent, CRect rec)
{
    CString className = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW,
        ::LoadCursor(NULL, IDC_ARROW),
        (HBRUSH)(COLOR_WINDOW + 1),
        NULL);

        // 윈도우 생성
    return CreateEx(WS_EX_TOPMOST | WS_EX_TOOLWINDOW,  // 확장 스타일: 최상위 및 도구 창
        className,                        // 클래스 이름
        _T("My Popup Window"),            // 윈도우 이름
        WS_POPUP | WS_VISIBLE | WS_BORDER, // 윈도우 스타일
        rec,                             // 위치와 크기
        pParent,                       // 부모 윈도우
        0);                               // 컨트롤 ID 없음

}

CWnd* CLBparent::CreateListBox(CClient* client, CString items)
{
    m_child = new CPopHistory(client, items);
    CRect rect;
    GetClientRect(rect);

    if (!((CPopHistory*)m_child)->Create(WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS , rect, this, 223)) //test codelist
    {
        delete m_child;
        return nullptr;
    }
   //m_child->SendMessage(WM_KILLFOCUS, 0, 0);
    return m_child;
}


void CLBparent::OnLButtonDown(UINT nFlags, CPoint point)
{
    CWnd::OnLButtonDown(nFlags, point);
}

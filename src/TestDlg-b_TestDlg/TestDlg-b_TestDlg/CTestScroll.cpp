// CTestScroll.cpp: 구현 파일
//

#include "pch.h"
#include "TestDlg.h"
#include "CTestScroll.h"


// CTestScroll

//IMPLEMENT_DYNAMIC(CTestScroll, CWnd)

CTestScroll::CTestScroll()
{
    NONCLIENTMETRICS pvParam;
    pvParam.cbSize = sizeof(NONCLIENTMETRICS);

    if (::SystemParametersInfo(SPI_GETNONCLIENTMETRICS, pvParam.cbSize, &pvParam, 0))
    {
        m_Font.CreateFontIndirect(&pvParam.lfMenuFont);
    }
}

CTestScroll::~CTestScroll()
{
}


BEGIN_MESSAGE_MAP(CTestScroll, CWnd)
    ON_WM_CREATE()
END_MESSAGE_MAP()



// CTestScroll 메시지 처리기




BOOL CTestScroll::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
    if (!CScrollWindow::PreCreateWindow(cs))
        return FALSE;

    cs.dwExStyle |= WS_EX_CLIENTEDGE;
    cs.style &= ~WS_BORDER;
    cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
        ::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW + 1), NULL);

    return TRUE;
  //  return CScrollWindow::PreCreateWindow(cs);
}

void CTestScroll::OnDraw(CDC* pDC)
{
    int dx = 128;
    int dy = 128;


}

int CTestScroll::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CScrollWindow::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  여기에 특수화된 작성 코드를 추가합니다.

    return 0;
}

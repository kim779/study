// CDlg_Edge.cpp: 구현 파일
//

#include "pch.h"
#include "TestDlg.h"
#include "CDlg_Edge.h"
#include "afxdialogex.h"


// CDlg_Edge 대화 상자

IMPLEMENT_DYNAMIC(CDlg_Edge, CDialogEx)

CDlg_Edge::CDlg_Edge(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_EDGE, pParent)
{
	BrowserWindowEdge::InitInstance(theApp.m_hInstance);
//	HINSTANCE hins{};
//	BrowserWindowEdge::InitInstance(hins);
}

CDlg_Edge::~CDlg_Edge()
{
}

void CDlg_Edge::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlg_Edge, CDialogEx)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlg_Edge 메시지 처리기

BOOL CDlg_Edge::OnInitDialog()
{
	CDialogEx::OnInitDialog();

//	browserEdge = std::make_unique<BrowserWindowEdge>();
//	CRect rect;
//	GetClientRect(rect);
//
//	if (!browserEdge->Create(nullptr, "BrowserWindowEdge", WS_CHILD | WS_VISIBLE, rect, this, 0)) {
//		m_slog.Format("[edge] [%s] create failure", __FUNCTION__);
//		OutputDebugString(m_slog);
//	}
//
////	std::wstring url = L"https://www.naver.com";
////	browserEdge->Init(url.c_str());
//	CString m_strurl;
//	//m_strurl.Format("%s", "https://www.ibks.com/LoadService.jsp?url=https://www.ibks.com/wts/OS/IOS00012_00.jsp&auth=_q579%3A%2B9%3E%3A%262%3A8%2C%3C1%2Ccai%3El%3C0%3E41j%60%3D0h%3A%24lu5%3A2%3A097181279%3B0%2C_f3hby3%3D%3A%2Cxs32y%3As9f&keyB_YN=Y&nPro_YN=N&aos_YN=N");
//
//	//m_strurl.Format("%s", "https://asp.zeroin.co.kr/ibk/");
//	 m_strurl.Format("%s", "https://naver.com");
//	std::wstring url;
//	std::string message_a = m_strurl;
//
//	url.assign(message_a.begin(), message_a.end());
//	//std::wstring url((LPCTSTR)m_strurl.GetBuffer(0));
//	browserEdge->Init(url);

	CRect rec;
	GetClientRect(&rec);
	rec.bottom -= 20;

if(1)
{ 
	m_pBrowserWnd = new CBrowserWnd();
	//pControlWnd->m_pParent = parent;

	//pControlWnd->SetParam((struct _param*)pParam);
	m_pBrowserWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, rec, this, 100);
}
else
{
	m_pBrowserCmp = new CBrowserComp();
	m_pBrowserCmp->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN, rec, this, 100);
}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlg_Edge::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (browserEdge) {
		CRect rect; // Gets resized later.		
		GetClientRect(&rect);
	
	//	browserEdge->MoveWindow(rect, TRUE);

	//	browserEdge->Resize(rect);
	}
}

void CDlg_Edge::Checkenv()
{
	
}
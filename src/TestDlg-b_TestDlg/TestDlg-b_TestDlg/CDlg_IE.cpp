// CDlg_IE.cpp: 구현 파일
//

#include "pch.h"
#include "TestDlg.h"
#include "CDlg_IE.h"
#include "afxdialogex.h"
#include "AxBrowser.h"

// CDlg_IE 대화 상자

IMPLEMENT_DYNAMIC(CDlg_IE, CDialogEx)

CDlg_IE::CDlg_IE(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_IE, pParent)
{
	m_pHtml = nullptr;
}

CDlg_IE::~CDlg_IE()
{
}

void CDlg_IE::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlg_IE, CDialogEx)
END_MESSAGE_MAP()


// CDlg_IE 메시지 처리기


BOOL CDlg_IE::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect rec;
	GetClientRect(&rec);
	if (m_bScroll)
	{
		m_Htmlrc.SetRect(-2, -2, rec.Width() + 2, rec.Height() + 2);
		m_pHtml = new CAxBrowser;
		m_pHtml->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, m_Htmlrc, this, 0);
	}
	else
	{
		std::unique_ptr<CWnd> pRefView = std::make_unique<CWnd>();
		pRefView->Create(NULL, NULL, WS_CHILD | WS_VSCROLL, CRect(0, 0, 50, 50), this, 0);

		CRect	crc;
		pRefView->GetClientRect(&crc);
		pRefView.reset();

		//m_scrGap = 50 - crc.Width();

		m_Htmlrc.SetRect(-2, -2, rec.Width() + 2 , rec.Height() + 2 );
		m_pHtml = new CAxBrowser;
		m_pHtml->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, m_Htmlrc, this, 0);
		m_pHtml->SetParentView(this);
		//m_pHtml->m_scrGap = m_scrGap;
	}
	//m_pHtml->Navigate2("https://download.ibks.com/customer/ibk_rp_main.pdf");

	CString url;
	url = "https://docs.google.com/viewer?url=https://download.ibks.com/customer/ibk_rp_main.pdf";
	m_pHtml->SetSilent(TRUE);
	//	SetBrowserReg();
	m_pHtml->Navigate2(url);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

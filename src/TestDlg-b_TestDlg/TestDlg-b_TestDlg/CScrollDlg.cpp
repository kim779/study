// CScrollDlg.cpp: 구현 파일
//

#include "pch.h"
#include "TestDlg.h"
#include "CScrollDlg.h"
#include "afxdialogex.h"


// CScrollDlg 대화 상자

IMPLEMENT_DYNAMIC(CScrollDlg, CDialog)

CScrollDlg::CScrollDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG1, pParent)
{

}

CScrollDlg::~CScrollDlg()
{
}

void CScrollDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CScrollDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CScrollDlg::OnBnClickedOk)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON1, &CScrollDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CScrollDlg 메시지 처리기


void CScrollDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialog::OnOK();
}


int CScrollDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	if (scrollControl.Create(NULL, NULL, WS_CHILD | WS_VISIBLE  | WS_VSCROLL, CRect(0, 0, 300, 300), this, 1000) == FALSE)
	{
		return -1;
	}

	//scrollControl.SetScrollSizes(CSize(1024, 768));

	return 0;
}


void CScrollDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	CRect rect;

	GetClientRect(rect);
	rect.bottom -= 20;
//	scrollControl.MoveWindow(rect);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


BOOL CScrollDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL CScrollDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return TRUE;
}


void CScrollDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

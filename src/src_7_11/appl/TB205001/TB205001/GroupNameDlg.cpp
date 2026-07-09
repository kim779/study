// GroupNameDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TB205001.h"
#include "GroupNameDlg.h"
#include "afxdialogex.h"


// CGroupNameDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CGroupNameDlg, CDialogEx)

CGroupNameDlg::CGroupNameDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGroupNameDlg::IDD, pParent)
{

}

CGroupNameDlg::~CGroupNameDlg()
{
}

void CGroupNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGroupNameDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CGroupNameDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CGroupNameDlg 메시지 처리기입니다.


BOOL CGroupNameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	UpdateData(FALSE);
	
	CRect rcCtrl;
	GetParent()->GetWindowRect(&rcCtrl);
	CPoint	point;		
	GetCursorPos(&point);
	CRect rcDlg;
	GetClientRect(&rcDlg);

	MoveWindow(point.x, point.y, rcDlg.Width(), rcDlg.Height());
	((CWnd*)GetDlgItem(IDC_EDIT1))->SetWindowText(m_strGroupName);
	((CWnd*)GetDlgItem(IDC_EDIT1))->SetFocus();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}



void CGroupNameDlg::SetGroupName(LPCTSTR lpszGroupName)
{
	CString tmpGrpName = lpszGroupName;

	tmpGrpName.TrimRight();

	m_strGroupName = tmpGrpName;
}

LPCTSTR CGroupNameDlg::GetGroupName()
{
	return (LPCTSTR)m_strGroupName;
}


void CGroupNameDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	((CWnd*)GetDlgItem(IDC_EDIT1))->GetWindowText(m_strGroupName);
	CDialogEx::OnOK();
}


BOOL CGroupNameDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	//if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)	
	//{
	//	OnBnClickedOk();
	//	return true;
	//}
		 
	return CDialogEx::PreTranslateMessage(pMsg);
}



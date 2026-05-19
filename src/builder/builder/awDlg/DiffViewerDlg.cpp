// DiffViewerDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DiffViewerDlg.h"
#include "afxdialogex.h"


// CDiffViewerDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDiffViewerDlg, CDialogEx)

CDiffViewerDlg::CDiffViewerDlg(CString strDiffViewer)
	: CDialogEx(CDiffViewerDlg::IDD, NULL)
{
	m_strDiffViewer = strDiffViewer;
}

CDiffViewerDlg::~CDiffViewerDlg()
{
}

void CDiffViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDiffViewerDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_PATH, &CDiffViewerDlg::OnBnClickedButtonPath)
	ON_BN_CLICKED(IDOK, &CDiffViewerDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CDiffViewerDlg 메시지 처리기입니다.


void CDiffViewerDlg::OnBnClickedButtonPath()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char name_filter[] = "All Files (*.*)|*.*|Exe Files (*.exe)|*.exe|";
	CFileDialog ins_dlg(TRUE, "", "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |
		OFN_NOCHANGEDIR, name_filter, NULL);

	if (ins_dlg.DoModal() != IDOK){
		return;
	}
	CString filePath;
	filePath = ins_dlg.GetPathName();

	GetDlgItem(IDC_EDIT_PATH)->SetWindowText(filePath);
}


void CDiffViewerDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItem(IDC_EDIT_PATH)->GetWindowText(m_strDiffViewer);
	CDialogEx::OnOK();
}


BOOL CDiffViewerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	GetDlgItem(IDC_EDIT_PATH)->SetWindowText(m_strDiffViewer);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

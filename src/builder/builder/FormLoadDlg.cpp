// FormLoadDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
//#include "axisMBuilder.h"
#include "FormLoadDlg.h"
#include "afxdialogex.h"
#include "awSock/download.h"


// CFormLoadDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CFormLoadDlg, CDialogEx)

CFormLoadDlg::CFormLoadDlg(CWnd* pParent, CString strID, CString strName)
	: CDialogEx(CFormLoadDlg::IDD, pParent)	
{	
	m_strID = strID;
	m_strName = strName;
	m_bFlag   = FALSE;
}

CFormLoadDlg::~CFormLoadDlg()
{
}

void CFormLoadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFormLoadDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CFormLoadDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RA_READONLY, &CFormLoadDlg::OnBnClickedRaReadonly)
	ON_MESSAGE(WM_USER, OnEnd)
END_MESSAGE_MAP()


// CFormLoadDlg 메시지 처리기입니다.
void CFormLoadDlg::OnBnClickedOk()
{	
	int imode = 0;
	if (((CButton*)GetDlgItem(IDC_RA_READONLY))->GetCheck())
		imode = 0;
	else if (((CButton*)GetDlgItem(IDC_RA_WRITE))->GetCheck())
		imode = 1;
	else if (((CButton*)GetDlgItem(IDC_RA_WRITETEMP))->GetCheck())
		imode = 2;

	m_iMode = imode;

	// send server
//	CString strMsg;
//	strMsg.Format("%8s%64s%d", m_strID, m_strName, imode);	



	CDialogEx::OnOK();
}


BOOL CFormLoadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	((CEdit*)GetDlgItem(IDC_EDIT_NAME))->SetWindowTextA(m_strName);
	((CButton*)GetDlgItem(IDC_RA_READONLY))->SetCheck(1);

	return TRUE;
}

LRESULT CFormLoadDlg::OnEnd(WPARAM wParam, LPARAM lParam)
{
	
	return 0;
}

void CFormLoadDlg::OnBnClickedRaReadonly()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	

	
}



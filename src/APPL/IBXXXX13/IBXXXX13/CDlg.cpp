// CDlg.cpp: 구현 파일
//

#include "pch.h"
#include "IBXXXX13.h"
#include "CDlg.h"
#include "afxdialogex.h"


// CDlg 대화 상자
#include "CMapWnd.h"
IMPLEMENT_DYNAMIC(CDlg, CDialog)

CDlg::CDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DLG, pParent)
{

}

CDlg::~CDlg()
{
}

void CDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void CDlg::AddTextEdit(CString sdata)
{
	CString sedit;
	((CWnd*)GetDlgItem(IDC_EDIT_LOG))->GetWindowText(sedit);

	sedit += "\r\n";
	sedit += sdata;

	((CWnd*)GetDlgItem(IDC_EDIT_LOG))->SetWindowText(sedit);
}

void CDlg::OnBnClickedBtnClear()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	((CWnd*)GetDlgItem(IDC_EDIT_LOG))->SetWindowText("");
}


BEGIN_MESSAGE_MAP(CDlg, CDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_GPSEARCH, &CDlg::OnBnClickedBtnGpsearch)
	ON_BN_CLICKED(IDC_BTN_GPCODE, &CDlg::OnBnClickedBtnGpcode)
	ON_BN_CLICKED(IDC_BTN_DB, &CDlg::OnBnClickedBtnDb)
	ON_BN_CLICKED(IDC_BTN_NEWGROUP, &CDlg::OnBnClickedBtnNewgroup)
	ON_BN_CLICKED(IDC_BTN_GROUPSAVE, &CDlg::OnBnClickedBtnGroupsave)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CDlg::OnBnClickedBtnClear)
	ON_BN_CLICKED(IDC_BTN_CODENAME, &CDlg::OnBnClickedBtnCodename)
	ON_BN_CLICKED(IDC_BTN_INTERADD, &CDlg::OnBnClickedBtnInteradd)
END_MESSAGE_MAP()


// CDlg 메시지 처리기


BOOL CDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CDlg::OnBnClickedBtnGpsearch()
{
	((CMapWnd*)GetParent())->OnBtnInterList();
}


void CDlg::OnBnClickedBtnGpcode()
{
	CString strname;
	((CWnd*)GetDlgItem(IDC_EDIT_SEARCHGROUP))->GetWindowText(strname);
	((CMapWnd*)GetParent())->OnBtnInterCode(atoi(strname));
}


void CDlg::OnBnClickedBtnDb()
{
	((CMapWnd*)GetParent())->OnBtnDBBackup();
}


void CDlg::OnBnClickedBtnNewgroup()
{
	//IDC_EDIT_NEWGROUP
	CString strname;
	((CWnd*)GetDlgItem(IDC_EDIT_NEWGROUP))->GetWindowText(strname);
	((CMapWnd*)GetParent())->OnBtnNewGroup(strname);
}


void CDlg::OnBnClickedBtnGroupsave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//수정하려는 그룹의 순서와 그룹명  그리고 종목을 올려줘야 한다.
	CString strname;
	((CWnd*)GetDlgItem(IDC_EDIT_GROUPSAVE))->GetWindowText(strname);
	((CMapWnd*)GetParent())->OnBtnGroupSave(atoi(strname));
}


void CDlg::OnBnClickedBtnCodename()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strcode;
	((CWnd*)GetDlgItem(IDC_EDIT_CODE))->GetWindowText(strcode);
	CString name = (char*)m_pWizrd->SendMessage(WM_USER, MAKEWPARAM(nameDLL, 0), (LPARAM)strcode.operator LPCSTR());
	AddTextEdit(name);
}


void CDlg::OnBnClickedBtnInteradd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strcode;
	((CWnd*)GetDlgItem(IDC_EDIT_CODE))->GetWindowText(strcode);

	CString sParam;
	sParam = "IB202201 /p5 /S/dappendITEMS\t02\t005930\t\n";
		m_pWizrd->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typePOPUP), (LPARAM)(LPCSTR)sParam);
	/*
	* sParam = "IB202201 /p5 /S/dappendITEMS\t02\t005930\t\n"

CString sParam;
sParam.Format("%s /p5 /S/d%s%s%c", "IB202201", "appendITEMS", strData, CH_LF);
HWND hWnd = pItem->m_hWinix;
::SendMessage(hWnd, WM_USER, MAKEWPARAM(viewDLL,typePOPUP), (LPARAM)(LPCSTR)sParam);
	*/

}

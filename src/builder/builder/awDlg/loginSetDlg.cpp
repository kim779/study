// loginSetDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "loginSetDlg.h"
#include "afxdialogex.h"

#include "../h/mainvar.h"
#include "../../h/axisvar.h"

// CLoginSetDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLoginSetDlg, CDialogEx)

CLoginSetDlg::CLoginSetDlg(CString path, CWnd* pParent /*=NULL*/)
	: CDialogEx(CLoginSetDlg::IDD, pParent)
{
	m_path = path;

	m_sProjectName = _T("");
	m_nPort = 0;
	m_bFloat = FALSE;
}

CLoginSetDlg::~CLoginSetDlg()
{
}

void CLoginSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDR, m_addr);
	DDX_Text(pDX, IDC_PROJECTNAME, m_sProjectName);
	DDX_Radio(pDX, IDC_LOCALRDO, m_nPort);
	DDX_Check(pDX, IDC_FLOAT_CHK, m_bFloat);
}


BEGIN_MESSAGE_MAP(CLoginSetDlg, CDialogEx)
END_MESSAGE_MAP()


// CLoginSetDlg 메시지 처리기입니다.


BOOL CLoginSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	char	wb[512];
	CString	project;

	GetPrivateProfileString(ENVIRONMENT, _T("PROJECTNAME"), _T("AXIS"), wb, sizeof(wb), m_path + _T("\\workshop.ini"));
	project = wb;

	GetPrivateProfileString(ENVIRONMENT, SHOPIPS, _T(""), wb, sizeof(wb), m_path + _T("\\workshop.ini"));
	m_ips = wb;

	m_nPort = GetPrivateProfileInt(ENVIRONMENT, "PROXY", 0, m_path + _T("\\workshop.ini"));
	m_bFloat = GetPrivateProfileInt(ENVIRONMENT, "FLOAT", 0, m_path + _T("\\workshop.ini"));

	m_sProjectName = project;
	if (m_ips.IsEmpty())
	{
		m_addr.SetFieldFocus(0);
		return TRUE;
	}

	BYTE	field0, field1, field2, field3;
	getAddress(field0, field1, field2, field3);
	m_addr.SetAddress(field0, field1, field2, field3);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CLoginSetDlg::OnOK()
{
	CString tmps;
	BYTE	field0, field1, field2, field3;

	UpdateData();
	m_addr.GetAddress(field0, field1, field2, field3);
	m_ips.Format(_T("%d.%d.%d.%d"), field0, field1, field2, field3);
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETIPS, (LPARAM)m_ips.operator LPCTSTR());

	WritePrivateProfileString(ENVIRONMENT, _T("PROJECTNAME"), m_sProjectName, m_path + _T("\\workshop.ini"));

	tmps.Format(_T("%d"), m_nPort);
	WritePrivateProfileString(ENVIRONMENT, _T("PROXY"), tmps, m_path + _T("\\workshop.ini"));

	WritePrivateProfileString(ENVIRONMENT, _T("FLOAT"), m_bFloat ? _T("1") : _T("0"), m_path + _T("\\workshop.ini"));

	CDialogEx::OnOK();
}

void CLoginSetDlg::getAddress(BYTE &field0, BYTE &field1, BYTE &field2, BYTE &field3)
{
	int	offs;
	CString	ips = m_ips;

	for (int ii = 0; ii < 4; ii++)
	{
		offs = ips.Find(_T('.'));
		if (offs != -1)
		{
			switch (ii)
			{
			case 0:
				field0 = (BYTE)atoi(ips.Left(offs));	break;
			case 1:
				field1 = (BYTE)atoi(ips.Left(offs));	break;
			case 2:
				field2 = (BYTE)atoi(ips.Left(offs));	break;
			case 3:
				field3 = (BYTE)atoi(ips.Left(offs));	break;
			}
			ips = (ips.GetLength() > offs+1) ? ips.Mid(offs+1) : _T("");
		}
		else
		{
			switch (ii)
			{
			case 0:
				field0 = (BYTE)atoi(ips);	break;
			case 1:
				field1 = (BYTE)atoi(ips);	break;
			case 2:
				field2 = (BYTE)atoi(ips);	break;
			case 3:
				field3 = (BYTE)atoi(ips);	break;
			}
			ips = _T("");
		}
	}
}
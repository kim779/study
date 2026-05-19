// addrDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "addrDlg.h"
#include "afxdialogex.h"

#include "../h/mainvar.h"
#include "../../h/axisvar.h"


// CAddrDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAddrDlg, CDialogEx)

CAddrDlg::CAddrDlg(CString path /*=_T("")*/, CWnd* pParent /*=NULL*/)
	: CDialogEx(CAddrDlg::IDD, pParent)
{
	m_ips  = _T("");
	m_path = path;
}

CAddrDlg::~CAddrDlg()
{
}

void CAddrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ADDR, m_addr);
}


BEGIN_MESSAGE_MAP(CAddrDlg, CDialogEx)
END_MESSAGE_MAP()


// CAddrDlg 메시지 처리기입니다.


BOOL CAddrDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	for (int ii = 0; ii < 4; ii++)
		m_addr.SetFieldRange(ii, 0, 255);

	char twb[512];
	GetPrivateProfileString(ENVIRONMENT, SHOPIPS, "", twb, 512, m_path + _T("\\workshop.ini"));
	m_ips = twb;

	if (m_ips.IsEmpty())
	{
		m_addr.SetFieldFocus(0);
		return TRUE;
	}

	BYTE	field0, field1, field2, field3;
	getAddress(field0, field1, field2, field3);
	m_addr.SetAddress(field0, field1, field2, field3);
	m_addr.SetFocus();
	m_addr.SetFieldFocus(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CAddrDlg::OnOK()
{
	if (m_addr.IsBlank())
	{
		AfxMessageBox(_T("IP 입력 오류"));
		return;
	}

	BYTE	field0, field1, field2, field3;

	m_addr.GetAddress(field0, field1, field2, field3);
	m_ips.Format(_T("%d.%d.%d.%d"), field0, field1, field2, field3);
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETIPS, (LPARAM)m_ips.operator LPCTSTR());

	CDialogEx::OnOK();
}

void CAddrDlg::getAddress(BYTE& field0, BYTE& field1, BYTE& field2, BYTE& field3)
{
	int	offs;
	CString	ips = m_ips;

	for (int ii = 0; ii < 4; ii++)
	{
		offs = ips.Find('.');
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

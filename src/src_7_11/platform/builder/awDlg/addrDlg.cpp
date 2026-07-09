// addrDlg.cpp : implementation file
//

#include <afxcmn.h>
#include "stdafx.h"
#include "addrDlg.h"
#include "../../h/axisvar.h"
#include "../mainvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddrDlg dialog


CAddrDlg::CAddrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddrDlg::IDD, pParent)
{
	m_ips = _T("");
	//{{AFX_DATA_INIT(CAddrDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAddrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddrDlg)
	DDX_Control(pDX, IDC_ADDR, m_addr);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddrDlg, CDialog)
	//{{AFX_MSG_MAP(CAddrDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddrDlg message handlers

BOOL CAddrDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	for (int ii = 0; ii < 4; ii++)
		m_addr.SetFieldRange(ii, 0, 255);

	char twb[512];
	GetPrivateProfileString(ENVIRONMENT, SHOPIPS, "", twb, 512, m_currentPath + "\\workshop.ini");
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
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAddrDlg::OnOK() 
{
	if (m_addr.IsBlank())
	{
		AfxMessageBox("IP 입력 오류");
		return;
	}

	BYTE	field0, field1, field2, field3;
	CWinApp* app = AfxGetApp();

	m_addr.GetAddress(field0, field1, field2, field3);
	m_ips.Format("%d.%d.%d.%d", field0, field1, field2, field3);
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETIPS, (LPARAM)m_ips.operator LPCTSTR());
	
	CDialog::OnOK();
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

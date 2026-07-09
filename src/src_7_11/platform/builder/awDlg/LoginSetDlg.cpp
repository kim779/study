// LoginSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LoginSetDlg.h"
#include "../mainvar.h"
#include "../../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoginSetDlg dialog


CLoginSetDlg::CLoginSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoginSetDlg)
	m_sProjectName = _T("");
	m_nPort = 0;
	m_bFloat = FALSE;
	//}}AFX_DATA_INIT
}


void CLoginSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoginSetDlg)
	DDX_Control(pDX, IDC_IPADDR, m_addr);
	DDX_Text(pDX, IDC_PROJECTNAME, m_sProjectName);
	DDX_Radio(pDX, IDC_LOCALRDO, m_nPort);
	DDX_Check(pDX, IDC_FLOAT_CHK, m_bFloat);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoginSetDlg, CDialog)
	//{{AFX_MSG_MAP(CLoginSetDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoginSetDlg message handlers

BOOL CLoginSetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	char twb[512];
	GetPrivateProfileString(ENVIRONMENT, "PROJECTNAME", "AXIS", twb, sizeof(twb), m_currentPath + "\\workshop.ini");
	m_ProjectName = twb;

	GetPrivateProfileString(ENVIRONMENT, SHOPIPS, "", twb, 512, m_currentPath + "\\workshop.ini");
	m_ips = twb;

	m_nPort = GetPrivateProfileInt(ENVIRONMENT, "PROXY", 0, m_currentPath + "\\workshop.ini");

	m_bFloat = GetPrivateProfileInt(ENVIRONMENT, "FLOAT", 0, m_currentPath + "\\workshop.ini");

	m_sProjectName = m_ProjectName;

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
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLoginSetDlg::getAddress(BYTE &field0, BYTE &field1, BYTE &field2, BYTE &field3)
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

void CLoginSetDlg::OnOK() 
{
	UpdateData();
	
	BYTE	field0, field1, field2, field3;
	m_addr.GetAddress(field0, field1, field2, field3);
	m_ips.Format("%d.%d.%d.%d", field0, field1, field2, field3);
	AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_SETIPS, (LPARAM)m_ips.operator LPCTSTR());

	m_ProjectName = m_sProjectName;
	WritePrivateProfileString(ENVIRONMENT, "PROJECTNAME", m_ProjectName, m_currentPath + "\\workshop.ini");

	CString sPort = "";
	sPort.Format("%d", m_nPort);
	WritePrivateProfileString(ENVIRONMENT, "PROXY", sPort, m_currentPath + "\\workshop.ini");

	WritePrivateProfileString(ENVIRONMENT, "FLOAT", m_bFloat?"1":"0", m_currentPath + "\\workshop.ini");

	CDialog::OnOK();
}



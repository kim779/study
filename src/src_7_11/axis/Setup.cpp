// Setup.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "Setup.h"

#include "../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetup dialog


CSetup::CSetup(CString home, int connectBy, CWnd* pParent /*=NULL*/)
	: CDialog(CSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetup)
	m_bReset = FALSE;
	m_bStep = FALSE;
	//}}AFX_DATA_INIT
	m_home = home;
	m_by = connectBy;
	m_parent = pParent;
}

CSetup::~CSetup()
{
}

void CSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetup)
	DDX_Control(pDX, IDC_SERVER, m_server);
	DDX_Check(pDX, IDC_Reset, m_bReset);
	DDX_Check(pDX, IDC_DataStep, m_bStep);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetup, CDialog)
	//{{AFX_MSG_MAP(CSetup)
	ON_BN_CLICKED(IDC_TCP, OnTcp)
	ON_BN_CLICKED(IDC_UDP, OnUdp)
	ON_BN_CLICKED(IDC_UDPD, OnUdpd)
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_IPINPUT, OnIpinput)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetup message handlers

BOOL CSetup::OnInitDialog() 
{
	CDialog::OnInitDialog();

	switch (m_by)
	{
	case byTCP:
		((CButton *)GetDlgItem(IDC_TCP))->SetCheck(1);
		((CButton *)GetDlgItem(IDC_UDP))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_UDPD))->SetCheck(0);
		break;
	case byUDP:
		((CButton *)GetDlgItem(IDC_TCP))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_UDP))->SetCheck(1);
		((CButton *)GetDlgItem(IDC_UDPD))->SetCheck(0);
		break;
	case byUDD:
		((CButton *)GetDlgItem(IDC_TCP))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_UDP))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_UDPD))->SetCheck(1);
		break;
	default:
		m_by    = byNONE;
		break;
	}
	loadServer(m_by);

	if (m_bSDI)
	{
		((CButton *)GetDlgItem(IDC_SDI))->SetCheck(1);
		((CButton *)GetDlgItem(IDC_MDI))->SetCheck(0);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_SDI))->SetCheck(0);
		((CButton *)GetDlgItem(IDC_MDI))->SetCheck(1);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CSetup::OnTcp() 
{
	m_by = byTCP;
	loadServer(byTCP);
}

void CSetup::loadServer(int type)
{
	char	wb[1024*4];
	int	value;
	int ii = 0;

	CString	path, keys, tmps, tmpx;
	CStringArray arr;

	m_ips.RemoveAll();
	arr.RemoveAll();
	m_server.ResetContent();
	path.Format("%s\\%s\\yellowPage.ini", m_home, TABDIR);
	tmpx  = _T("Server_IP");

	value = GetPrivateProfileSection(tmpx, wb, sizeof(wb), path);

	for (ii = 0; ii < value; ii++)
	{
		tmps = &wb[ii];
		if (tmps.IsEmpty())
			break;
		arr.Add(tmps);
		ii += tmps.GetLength();
	}

	CWinApp* app = AfxGetApp();

	keys = app->GetProfileString(INFORMATION, byTCPs);
	
	value = -1;

	for (ii = 0; ii < arr.GetSize(); ii++)
	{
		tmps = arr.GetAt(ii);
		getStrings(tmps, tmpx);
		m_server.AddString(tmps);
		m_ips.Add(tmpx);
		if (!keys.CompareNoCase(tmpx))
			value = ii;
	}

	if (value >= 0)
		m_server.SetCurSel(value);
	else if (!keys.IsEmpty())
	{
		int pos = keys.Find('|');
		if (pos >= 0)
			keys = keys.Left(pos);
		value = m_server.AddString(keys);
		m_server.SetCurSel(value);
		m_ips.Add(keys);
	}
	else
		m_server.SetCurSel(0);

	m_bStep = app->GetProfileInt(INFORMATION, "STEP", 0);
	m_bSDI = app->GetProfileInt(INFORMATION, "SDI", 1);

	UpdateData(FALSE);
}

void CSetup::getStrings(CString& str, CString& keys)
{
	int	pos = str.Find('=');
	if (pos < 0)
		keys = _T("");
	else
	{
		keys = str.Mid(pos+1);
		str  = str.Left(pos);
	}

	str.TrimLeft();  str.TrimRight();
	keys.TrimLeft(); keys.TrimRight();
}

void CSetup::OnOK() 
{
	CWinApp* app  = AfxGetApp();

	int value = m_server.GetCurSel();

	CString	ips = _T("");

	if (value >= 0 && value < m_server.GetCount())
		ips = m_ips.GetAt(value);

	if (!ips.IsEmpty())
		app->WriteProfileString(INFORMATION, byTCPs, ips);

	app->WriteProfileInt(INFORMATION, CONNECTBY, m_by);

	UpdateData(TRUE);

	app->WriteProfileInt(INFORMATION, "STEP", m_bStep);

	if (m_bReset)
	{
		CString path;
		path.Format("%s/%s/%s", m_home, TABDIR, "infoAXIS");
		DeleteFile(path);
		path.Format("%s/%s/%s", m_home, TABDIR, "infoRSC");
		DeleteFile(path);
	}
	
	if (((CButton *)GetDlgItem(IDC_SDI))->GetCheck())
		app->WriteProfileInt(INFORMATION, "SDI", 1);
	else
		app->WriteProfileInt(INFORMATION, "SDI", 0);
		

	CDialog::OnOK();
}

void CSetup::OnUdp() 
{
	m_by = byUDP;
	loadServer(byUDP);
}

void CSetup::OnUdpd() 
{
	m_by = byUDD;
	loadServer(byUDD);	
}

#include "InputIP.h"
void CSetup::OnIpinput() 
{
	// TODO: Add your control notification handler code here
	CInputIP dlg(m_home);
	dlg.DoModal();

	loadServer(m_by);
}

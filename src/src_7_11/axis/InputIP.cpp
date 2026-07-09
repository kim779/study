// InputIP.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "InputIP.h"
#include "../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputIP dialog


CInputIP::CInputIP(CString home, CWnd* pParent /*=NULL*/)
	: CDialog(CInputIP::IDD, pParent)
{
	m_home = home;

	//{{AFX_DATA_INIT(CInputIP)
	m_strConnectName = _T("");
	m_strIPNum = _T("");
	//}}AFX_DATA_INIT
}


void CInputIP::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputIP)
	DDX_Text(pDX, IDC_ConnectName, m_strConnectName);
	DDX_Text(pDX, IDC_IPNum, m_strIPNum);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputIP, CDialog)
	//{{AFX_MSG_MAP(CInputIP)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputIP message handlers

void CInputIP::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	if (m_strConnectName.IsEmpty() || m_strIPNum.IsEmpty()) return;

	CString path;
	path.Format("%s\\%s\\yellowPage.ini", m_home, TABDIR);

	WritePrivateProfileString("Server_IP", m_strConnectName, m_strIPNum, path);

	CDialog::OnOK();
}

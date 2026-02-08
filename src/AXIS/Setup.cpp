// Setup.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "Setup.h"
#include "axMsg.hxx"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	TM_CONNECT	100
#define	TMI_CONNECT	3000

#define	TS_FAIL		-1
#define	TS_READY	0
#define	TS_SUCCESS	1

/////////////////////////////////////////////////////////////////////////////
// CSetup dialog
CSetup::CSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetup)
	//}}AFX_DATA_INIT

	m_allResourceDownload = FALSE;
}

CSetup::~CSetup()
{
}

void CSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetup)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetup, CDialog)
	//{{AFX_MSG_MAP(CSetup)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_AOS, OnAos)
	ON_BN_CLICKED(IDC_KEYPROTECT, OnKeyprotect)
	ON_BN_CLICKED(IDC_PCFIREWALL, OnPcfirewall)
	//}}AFX_MSG_MAP
//	ON_MESSAGE(WM_AXIS, OnAXIS)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetup message handlers

BOOL CSetup::OnInitDialog() 
{
	CDialog::OnInitDialog();
#ifdef DF_USE_CPLUS17
	m_changer = std::make_unique<CControlChanger>(this);
#else
	m_changer = new CControlChanger(this);
#endif
	
	Init();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetup::OnOK()
{
	Save();
	CDialog::OnOK();
}

void CSetup::Init()
{
	CWinApp* app  = AfxGetApp();
	m_Port    = app->GetProfileString(INFORMATION, "Port");

	if (m_Port.IsEmpty())	
		m_Port.Format("%d", portProxy);

	switch (atoi(m_Port))
	{
	case portEmployee:
		GetDlgItem(IDC_EMP)->SendMessage(BM_SETCHECK, TRUE);
		break;
	case portCustomer:
		GetDlgItem(IDC_CUST)->SendMessage(BM_SETCHECK, TRUE);
		break;
	case portProxy: 
	default:
		//GetDlgItem(IDC_PROXY2)->SendMessage(BM_SETCHECK, TRUE);
		((CButton*)GetDlgItem(IDC_PROXY2))->SetCheck(true);
		m_Port.Format("%d", portProxy);
		break;
	}

	/**
	if (Axis::isCustomer && atoi(m_Port) == portEmployee)
	{
		m_Port.Format("%d", portCustomer);
		GetDlgItem(IDC_CUST)->SendMessage(BM_SETCHECK, TRUE);
	}
	**/

	/**
	if (Axis::isCustomer)
	{
		GetDlgItem(IDC_EMP)->SendMessage(BM_SETCHECK);
		GetDlgItem(IDC_EMP)->EnableWindow(FALSE);
	}
	**/
	if (app->GetProfileInt(INFORMATION, "INTERNAL", 0))
	{  //내부망 직원이거나 NOAOS.TXT 파일존재시
		if (app->GetProfileInt(INFORMATION, "AOS", 1))
			GetDlgItem(IDC_AOS)->SendMessage(BM_SETCHECK, TRUE);

		if (app->GetProfileInt(INFORMATION, "PCFirewall", 0))
			GetDlgItem(IDC_PCFIREWALL)->SendMessage(BM_SETCHECK, TRUE);

		if (app->GetProfileInt(ENVIRONMENT, "KeyProtect", 0))
			GetDlgItem(IDC_KEYPROTECT)->SendMessage(BM_SETCHECK, TRUE);

		if (app->GetProfileInt(WORKSTATION, "ServerTime", 0))
			GetDlgItem(IDC_USESERVERTIME)->SendMessage(BM_SETCHECK, TRUE);
	}
	else
	{
		GetDlgItem(IDC_AOS)->EnableWindow(FALSE);
		GetDlgItem(IDC_PCFIREWALL)->EnableWindow(FALSE);
		GetDlgItem(IDC_KEYPROTECT)->EnableWindow(FALSE);

		GetDlgItem(IDC_AOS)->SendMessage(BM_SETCHECK, TRUE);
		GetDlgItem(IDC_PCFIREWALL)->SendMessage(BM_SETCHECK, TRUE);
		GetDlgItem(IDC_KEYPROTECT)->SendMessage(BM_SETCHECK, TRUE);

		app->WriteProfileInt(INFORMATION, "AOS", GetDlgItem(IDC_AOS)->SendMessage(BM_GETCHECK));
		app->WriteProfileInt(INFORMATION, "PCFireWall", GetDlgItem(IDC_PCFIREWALL)->SendMessage(BM_GETCHECK));
		app->WriteProfileInt(ENVIRONMENT, "KeyProtect", GetDlgItem(IDC_KEYPROTECT)->SendMessage(BM_GETCHECK));
	}
}

void CSetup::Save()
{
	int port = portProxy;
	
	//CString tmp; tmp.Format("%d", port);
	//AfxMessageBox(tmp);
	if (!((CButton*)GetDlgItem(IDC_PROXY2))->GetCheck())
	{
		
		if (!Axis::isCustomer)
			port = portEmployee;
		else// if (GetDlgItem(IDC_CUST)->SendMessage(BM_GETCHECK))
			port = portCustomer;
		if (((CButton*)GetDlgItem(IDC_PROXY2))->GetCheck())
			port = portProxy;
	}
	m_Port.Format("%d", port);
	

	CWinApp* app  = AfxGetApp();
	app->WriteProfileString(INFORMATION, "Port", m_Port);

	app->WriteProfileInt(INFORMATION, "AOS", GetDlgItem(IDC_AOS)->SendMessage(BM_GETCHECK));
	app->WriteProfileInt(INFORMATION, "PCFireWall", GetDlgItem(IDC_PCFIREWALL)->SendMessage(BM_GETCHECK));
	app->WriteProfileInt(ENVIRONMENT, "KeyProtect", GetDlgItem(IDC_KEYPROTECT)->SendMessage(BM_GETCHECK));
	app->WriteProfileInt(WORKSTATION, "ServerTime", GetDlgItem(IDC_USESERVERTIME)->SendMessage(BM_GETCHECK));

	if (GetDlgItem(IDC_ALLRESOURCEDOWNLOAD)->SendMessage(BM_GETCHECK))
		m_allResourceDownload = TRUE;
}


BOOL CSetup::OnEraseBkgnd(CDC* pDC) 
{
	if (Axis::dialogBgColor > 0)
	{
		CRect rc;
		GetClientRect(&rc);
		
		CBrush* oldBrush = pDC->SelectObject(&Axis::dialogBgBrush);

		pDC->PatBlt(0, 0, rc.Width(), rc.Height(), PATCOPY);
		pDC->SelectObject(oldBrush);

		return TRUE;
	}
	else
	{
		return CDialog::OnEraseBkgnd(pDC);
	}
}

HBRUSH CSetup::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = Axis::GetCtlBrush(pWnd);

	pDC->SetBkMode(TRANSPARENT);
	if (hbr)
	{
		return hbr;
	}
	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CSetup::OnDestroy() 
{
	CDialog::OnDestroy();
#ifndef DF_USE_CPLUS17
	delete m_changer;	
#endif
}



void CSetup::OnAos() 
{
	// TODO: Add your control notification handler code here
	if(!GetDlgItem(IDC_AOS)->SendMessage(BM_GETCHECK))
		Axis::MessageBox(this,"메모리보안을 해제하면 해킹 될 우려가 있습니다.",MB_OK | MB_ICONWARNING);
}

void CSetup::OnKeyprotect() 
{
	// TODO: Add your control notification handler code here
	if(!GetDlgItem(IDC_KEYPROTECT)->SendMessage(BM_GETCHECK))
		Axis::MessageBox(this,"키보드보안을 해제하면 해킹 될 우려가 있습니다.",MB_OK | MB_ICONWARNING);
}

void CSetup::OnPcfirewall() 
{
	// TODO: Add your control notification handler code here
	if(!GetDlgItem(IDC_PCFIREWALL)->SendMessage(BM_GETCHECK))
		Axis::MessageBox(this,"방화벽을 해제하면 해킹 될 우려가 있습니다.",MB_OK | MB_ICONWARNING);
}

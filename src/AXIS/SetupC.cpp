// SetupC.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "SetupC.h"
#include "../dll/sm/smdefine.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	CTRL_OK			20
/////////////////////////////////////////////////////////////////////////////
// CSetupC dialog


CSetupC::CSetupC(CWnd* pParent /*=NULL*/)
	: CDialog(CSetupC::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetupC)
	//}}AFX_DATA_INIT
	m_nEmerge = 0;
	m_bitmap  = NULL;
	m_okBtn   = NULL;
}


void CSetupC::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetupC)
	DDX_Control(pDX, IDC_PROXY_CUST, m_ckProxy);
	DDX_Control(pDX, IDC_USE_FIREWALL, m_ckFirewall);
	DDX_Control(pDX, IDC_SERVERTIME, m_ckServerTime);
	DDX_Control(pDX, IDC_RECEIVE_RESOURCE, m_ckResource);
	DDX_Control(pDX, IDC_DBITMAP, m_static);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetupC, CDialog)
	//{{AFX_MSG_MAP(CSetupC)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_XMSG, OnXMSG)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetupC message handlers

BOOL CSetupC::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CWinApp* app  = AfxGetApp();
	CString port = app->GetProfileString("Information", "Port");
	if (atoi(port) != 80)
		((CButton *)GetDlgItem(IDC_PROXY_CUST))->SetCheck(1);

	// 2006. 7. 11 - 이인호 서버시간적용추가
	int	val = app->GetProfileInt(WORKSTATION, "ServerTime", 0);
	if (val)
		((CButton*)GetDlgItem(IDC_SERVERTIME))->SetCheck(1);
	
	val = app->GetProfileInt(WORKSTATION, "GetAllResource", 0);
	if (val)
		((CButton*)GetDlgItem(IDC_RECEIVE_RESOURCE))->SetCheck(1);

	val = app->GetProfileInt(WORKSTATION, "UseFirewall", 0);
	if (val)
		((CButton*)GetDlgItem(IDC_USE_FIREWALL))->SetCheck(1);

	CString imgN;
	imgN.Format("%s\\%s\\Setup_c.bmp", Axis::home, IMAGEDIR);
	m_bitmap = (HBITMAP)LoadImage(AfxGetApp()->m_hInstance,
			imgN,IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);

	if (m_bitmap)
	{
		m_static.SetBitmap(m_bitmap);
		m_ckFirewall.SetWindowText("                    ");
		m_ckServerTime.SetWindowText("                    ");
		m_ckResource.SetWindowText("                    ");
	}
	else
	{
		m_static.ShowWindow(SW_HIDE);
	}

	m_ckFirewall.SetBitmaps(IDB_CHECK_ON, Axis::maskColor, IDB_CHECK_OFF, Axis::maskColor);
	m_ckFirewall.DrawTransparent();

	m_ckServerTime.SetBitmaps(IDB_CHECK_ON, Axis::maskColor, IDB_CHECK_OFF, Axis::maskColor);
	m_ckServerTime.DrawTransparent();

	m_ckResource.SetBitmaps(IDB_CHECK_ON, Axis::maskColor, IDB_CHECK_OFF, Axis::maskColor);
	m_ckResource.DrawTransparent();

	m_ckProxy.SetBitmaps(IDB_CHECK_ON, Axis::maskColor, IDB_CHECK_OFF, Axis::maskColor);
	m_ckProxy.DrawTransparent();

	for (int ii = 0; ii <= 5; ii++)
	{
		m_ckFirewall.SetColor(ii, RGB(0, 0, 0));
		m_ckServerTime.SetColor(ii, RGB(0, 0, 0));
		m_ckResource.SetColor(ii, RGB(0, 0, 0));
		m_ckProxy.SetColor(ii, RGB(0, 0, 0));
	}

	const DWORD dwStyle = WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN;

#ifdef DF_USE_CPLUS17
	m_okBtn = std::make_unique<CNButton>(CTRL_OK);
	m_okBtn->set_Color(RGB(247, 251, 196));
	if (!m_okBtn->Create(NULL, "확인", dwStyle, CRect(105, 169, 159, 189), this, -1))
		m_okBtn = NULL;
	else
	{
		m_okBtn->set_ToolTipText("설정을 완료 합니다.");
		m_okBtn->set_Image(CAxButtonName("ok"));
	}
#else
	m_okBtn = new CNButton(CTRL_OK);
	m_okBtn->set_Color(RGB(247, 251, 196));
	if (!m_okBtn->Create(NULL, "확인", dwStyle, CRect(105, 169, 159, 189), this, -1))
	{
		delete m_okBtn;
		m_okBtn = NULL;
	}
	else	
	{
		m_okBtn->set_ToolTipText("설정을 완료 합니다.");
		m_okBtn->set_Image(CAxButtonName("ok"));
	}
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CSetupC::OnOK() 
{
	CWinApp* app  = AfxGetApp();
	if (((CButton *)GetDlgItem(IDC_PROXY_CUST))->GetCheck() == BST_CHECKED)
		app->WriteProfileString("Information", "Port", Format("%d", portCustomer));
	else	
		app->WriteProfileString("Information", "Port", Format("%d", portProxy));

	int	val = 0;
	if (((CButton*)GetDlgItem(IDC_SERVERTIME))->GetCheck())
		val = 1;
	app->WriteProfileInt(WORKSTATION, "ServerTime", val);

	val = 0;
	if (((CButton*)GetDlgItem(IDC_RECEIVE_RESOURCE))->GetCheck())
		val = 1;
	app->WriteProfileInt(WORKSTATION, "GetAllResource", val);

	val = 0;
	if (((CButton*)GetDlgItem(IDC_USE_FIREWALL))->GetCheck())
		val = 1;
	app->WriteProfileInt(WORKSTATION, "UseFirewall", val);

	CDialog::OnOK();
}

#include "EmergencyDlg.h"
void CSetupC::OnEmergency() 
{
	CEmergencyDlg	dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_nEmerge = dlg.GetVal();
		if (m_nEmerge)
			OnCancel();
	}
}

LONG CSetupC::OnXMSG(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case CTRL_OK:
		OnOK();
		break;
	}
	return 0;
}

void CSetupC::OnDestroy() 
{
	if (m_okBtn)
	{
		m_okBtn->DestroyWindow();
#ifndef DF_USE_CPLUS17
		delete m_okBtn;
#endif
	}
	if (m_bitmap)	DeleteObject(m_bitmap);	
	CDialog::OnDestroy();	
}

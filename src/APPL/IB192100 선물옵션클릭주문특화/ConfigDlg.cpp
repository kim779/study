// ConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IB192100.h"
#include "ConfigDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg dialog


CConfigDlg::CConfigDlg(CWnd* pParent, LPCSTR profile)
	: CDialog(CConfigDlg::IDD, pParent)
{
	m_profile = profile;
	//{{AFX_DATA_INIT(CConfigDlg)
	m_bEnterOrder = FALSE;
	m_bOrderConfirm = FALSE;
	m_iClickMode = 1;
	m_bPutReverse = FALSE;
	m_bShowBefore = FALSE;
	m_bSpaceOrder = TRUE;
	m_iCntWidth = 30;
	m_iHogaWidth = 53;
	m_iVolWidth = 40;
	m_iOdrWidth = 50;
	//}}AFX_DATA_INIT
}


void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigDlg)
	DDX_Check(pDX, IDC_ENTERORDER, m_bEnterOrder);
	DDX_Check(pDX, IDC_ODR_CONFIRM, m_bOrderConfirm);
	DDX_Radio(pDX, IDC_ONECLICK_ORDER, m_iClickMode);
	DDX_Check(pDX, IDC_PUTREVERSE, m_bPutReverse);
	DDX_Check(pDX, IDC_SHOW_BEFORE, m_bShowBefore);
	DDX_Check(pDX, IDC_SPACEORDER, m_bSpaceOrder);
	DDX_Text(pDX, IDC_CNT_WIDTH, m_iCntWidth);
	DDV_MinMaxUInt(pDX, m_iCntWidth, 25, 100);
	DDX_Text(pDX, IDC_HOGA_WIDTH, m_iHogaWidth);
	DDV_MinMaxUInt(pDX, m_iHogaWidth, 50, 100);
	DDX_Text(pDX, IDC_VOL_WIDTH, m_iVolWidth);
	DDV_MinMaxUInt(pDX, m_iVolWidth, 35, 100);
	DDX_Text(pDX, IDC_ORDR_WIDTH, m_iOdrWidth);
	DDV_MinMaxUInt(pDX, m_iOdrWidth, 45, 100);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CConfigDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg message handlers

void CConfigDlg::OnOK() 
{
	if (!UpdateData())
	{
		return;
	}
	CDialog::OnOK();
}

void CConfigDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

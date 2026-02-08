// ConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CX_FODepth.h"
#include "ConfigDlg.h"
#include "depth.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg dialog


CConfigDlg::CConfigDlg(struct _config *pConfig, CWnd* pParent /*=NULL*/)
	: CDialog(CConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigDlg)
	//}}AFX_DATA_INIT

	m_pConfig = pConfig;
}


void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigDlg)
	DDX_Control(pDX, IDC_CHKBOLD3, m_chkBOLD3);
	DDX_Control(pDX, IDC_CHKBOLD2, m_chkBOLD2);
	DDX_Control(pDX, IDC_CHKDBONG, m_chkDBONG);
	DDX_Control(pDX, IDC_CHKSGRP, m_chkSGRP);
	DDX_Control(pDX, IDC_CHKDYUL, m_chkDYUL);
	DDX_Control(pDX, IDC_CHKCURR, m_chkCURR);
	DDX_Control(pDX, IDC_CHKBOLD, m_chkBOLD);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CConfigDlg)
	ON_BN_CLICKED(IDC_CHKDYUL, OnChkdyul)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigDlg message handlers

void CConfigDlg::OnChkdyul() 
{
}

BOOL CConfigDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CPoint	pt;
	CRect	rc;

	GetCursorPos(&pt);
	GetWindowRect(&rc);
	MoveWindow(pt.x, pt.y, rc.Width(), rc.Height());
	
	if (m_pConfig->curr)	m_chkCURR.SetCheck(1);
	if (m_pConfig->sgrp)	m_chkSGRP.SetCheck(1);
	if (m_pConfig->hdyul)	m_chkDYUL.SetCheck(1);
	if (m_pConfig->hbold)	m_chkBOLD.SetCheck(1);
	if (m_pConfig->sbong)	m_chkDBONG.SetCheck(1);
	if (m_pConfig->hrbold)	m_chkBOLD2.SetCheck(1);
	if (m_pConfig->hcbold)	m_chkBOLD3.SetCheck(1);
	
	return TRUE;
}

void CConfigDlg::OnOK() 
{
	m_pConfig->curr   = m_chkCURR.GetCheck() ? 1 : 0;	//현재가
	m_pConfig->sgrp   = m_chkSGRP.GetCheck() ? 1 : 0;	//호가잔량 그래프
	m_pConfig->hdyul  = m_chkDYUL.GetCheck() ? 1 : 0;	//호가별 등락률
	m_pConfig->hbold  = m_chkBOLD.GetCheck() ? 1 : 0;	//호가 굵게
	m_pConfig->sbong  = m_chkDBONG.GetCheck() ? 1 : 0;	//당일 봉차트
	m_pConfig->hrbold = m_chkBOLD2.GetCheck() ? 1 : 0;
	m_pConfig->hcbold = m_chkBOLD3.GetCheck() ? 1 : 0;

	CDialog::OnOK();
}

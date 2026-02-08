// ConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CX_SFDepth.h"
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
	GetCursorPos(&pt);
	CRect	rc;
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
	if (m_chkCURR.GetCheck())	m_pConfig->curr = 1;	//현재가
	else				m_pConfig->curr = 0;
	if (m_chkSGRP.GetCheck())	m_pConfig->sgrp = 1;	//호가잔량 그래프
	else				m_pConfig->sgrp = 0;
	if (m_chkDYUL.GetCheck())	m_pConfig->hdyul = 1;	//호가별 등락률
	else				m_pConfig->hdyul = 0;
	if (m_chkBOLD.GetCheck())	m_pConfig->hbold = 1;	//호가 굵게
	else				m_pConfig->hbold = 0;
	if (m_chkDBONG.GetCheck())	m_pConfig->sbong = 1;	//당일 봉차트
	else				m_pConfig->sbong = 0;
	if (m_chkBOLD2.GetCheck())	m_pConfig->hrbold = 1;
	else				m_pConfig->hrbold = 0;
	if (m_chkBOLD3.GetCheck())	m_pConfig->hcbold = 1;
	else				m_pConfig->hcbold = 0;
	CDialog::OnOK();
}

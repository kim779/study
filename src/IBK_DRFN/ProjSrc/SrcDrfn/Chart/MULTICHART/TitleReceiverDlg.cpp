// TitleReceiverDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TitleReceiverDlg.h"
#include "./define/ChartMsgDef.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTitleReceiverDlg dialog


CTitleReceiverDlg::CTitleReceiverDlg(CWnd* pParent /*=NULL*/)
	: CRscDlg(CTitleReceiverDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTitleReceiverDlg)
	m_strNewTitle = _T("");
	//}}AFX_DATA_INIT
}


void CTitleReceiverDlg::DoDataExchange(CDataExchange* pDX)
{
	CRscDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTitleReceiverDlg)
	DDX_Text(pDX, IDC_EDITNEWTITLE, m_strNewTitle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTitleReceiverDlg, CRscDlg)
	//{{AFX_MSG_MAP(CTitleReceiverDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTitleReceiverDlg message handlers

BOOL CTitleReceiverDlg::OnInitDialog() 
{
	CRscDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	GetDlgItem(IDC_EDITNEWTITLE)->SetFocus();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTitleReceiverDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	m_strNewTitle.TrimLeft();
	m_strNewTitle.TrimRight();
	if(m_strNewTitle.IsEmpty())
	{
		MessageBox("저장될 이름을 입력하십시오.",DLLTITLENAME);
	}
	else
	{
		CRscDlg::OnOK();
	}
	
}

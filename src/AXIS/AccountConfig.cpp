// AccountConfig.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "AccountConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAccountConfig dialog


CAccountConfig::CAccountConfig(CWnd* pParent /*=NULL*/)
	: CDialog(CAccountConfig::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAccountConfig)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAccountConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAccountConfig)
	DDX_Control(pDX, IDC_TAB, m_tab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAccountConfig, CDialog)
	//{{AFX_MSG_MAP(CAccountConfig)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccountConfig message handlers

BOOL CAccountConfig::OnInitDialog() 
{
	CDialog::OnInitDialog();

	InitTabControl();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CAccountConfig::InitTabControl()
{
	m_tab.InsertItem(0, "계좌비밀번호저장");
	m_tab.InsertItem(1, "그룹계좌설정");
	m_tab.InsertItem(2, "계좌별칭설정");
}
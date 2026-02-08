// DlgItemMaxCount.cpp : implementation file
//

#include "stdafx.h"
#include "comparechart.h"
#include "DlgItemMaxCount.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgItemMaxCount dialog


CDlgItemMaxCount::CDlgItemMaxCount(int nMaxItemCount, int& nReturn, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgItemMaxCount::IDD, pParent),
	m_nMaxItemCount(nMaxItemCount),
	m_nReturn(nReturn)
{
	//{{AFX_DATA_INIT(CDlgItemMaxCount)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgItemMaxCount::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgItemMaxCount)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgItemMaxCount, CDialog)
	//{{AFX_MSG_MAP(CDlgItemMaxCount)
	ON_BN_CLICKED(ID_BTN_MODIFY, OnBtnModify)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgItemMaxCount message handlers

BOOL CDlgItemMaxCount::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString strItemMax;
	strItemMax.Format("최대 %d개 까지 추가 할 수 있습니다.", m_nMaxItemCount);

	SetDlgItemText(IDC_STATIC_MAXITEM, strItemMax);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgItemMaxCount::OnOK() 
{
	m_nReturn = IDOK;
	
	CDialog::OnOK();
}

void CDlgItemMaxCount::OnBtnModify() 
{
	m_nReturn = ID_BTN_MODIFY;

	CDialog::OnOK();
}

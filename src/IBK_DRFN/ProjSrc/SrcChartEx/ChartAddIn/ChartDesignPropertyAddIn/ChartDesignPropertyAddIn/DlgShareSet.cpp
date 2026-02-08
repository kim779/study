// DlgShareSet.cpp : implementation file
//

#include "stdafx.h"
#include "ChartDesignPropertyAddIn.h"
#include "DlgShareSet.h"

#include "UserDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgShareSet dialog


CDlgShareSet::CDlgShareSet(CWnd* pParent /*=NULL*/)
: CDialog(CDlgShareSet::IDD, pParent) ,m_bSName(TRUE)
{
	//{{AFX_DATA_INIT(CDlgShareSet)
	m_bGet = FALSE;
	m_bSet = FALSE;
	m_szSharedName = _T("");
	m_bStart = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgShareSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgShareSet)
	DDX_Control(pDX, IDC_SHARENAME, m_ctrlSName);
	DDX_Check(pDX, IDC_CHKGET, m_bGet);
	DDX_Check(pDX, IDC_CHKSET, m_bSet);
	DDX_Text(pDX, IDC_SHARENAME, m_szSharedName);
	DDX_Check(pDX, IDC_CHKSTART, m_bStart);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgShareSet, CDialog)
	//{{AFX_MSG_MAP(CDlgShareSet)
	ON_CBN_SELCHANGE(IDC_SHAREDTYPE, OnSelchangeSharedtype)
	ON_EN_CHANGE(IDC_INDEX, OnChangeIndex)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgShareSet message handlers

BOOL CDlgShareSet::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ctrlSName.EnableWindow(m_bSName);	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgShareSet::SetWndState(int nType)
{
	if(nType<0)
		return;

	WND_STATE(IDC_SHARENAME, nType==RDT_USER);		//UserDefine..

	if(nType==RDT_USER)
	{
		SetDlgItemText(IDC_SHARENAME, "");
	}
	else 
	{
		CComboBox *pCombo = (CComboBox *)GetDlgItem(IDC_SHAREDTYPE);
		CString rString;
		pCombo->GetLBText(nType, rString);

		int iFind = rString.Find("-");
		if(iFind<0) return;

		rString = rString.Mid(iFind+1);
		rString.TrimLeft();
		rString.TrimRight();

		CString szIndex;
		GetDlgItemText(IDC_INDEX,szIndex);

		if(szIndex.GetLength() > 0)
		{
			CString szUpdate;
			szUpdate.Format("%s-%s",rString,szIndex);
			SetDlgItemText(IDC_SHARENAME, szUpdate);
		}
		else
			SetDlgItemText(IDC_SHARENAME, rString);

	}

}

void CDlgShareSet::OnSelchangeSharedtype() 
{
	CComboBox *pCombo = (CComboBox *)GetDlgItem(IDC_SHAREDTYPE);
	int nIndex = pCombo->GetCurSel();

	SetWndState(nIndex);
	
}

void CDlgShareSet::OnChangeIndex() 
{
	CComboBox *pCombo = (CComboBox *)GetDlgItem(IDC_SHAREDTYPE);
	int nIndex = pCombo->GetCurSel();

	SetWndState(nIndex);	
	
}

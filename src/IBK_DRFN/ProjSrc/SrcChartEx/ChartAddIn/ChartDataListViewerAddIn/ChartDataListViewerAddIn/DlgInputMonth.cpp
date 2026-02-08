// DlgInputMonth.cpp : implementation file
//

#include "stdafx.h"
#include "ChartDataListViewerAddIn.h"
#include "DlgInputMonth.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// ML_SET_LANGUAGE_RES()

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgInputMonth dialog


CDlgInputMonth::CDlgInputMonth( HWND p_hOcxWnd, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInputMonth::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgInputMonth)
	m_strMonth = _T("");
	m_strYear = _T("");
	//}}AFX_DATA_INIT

	// (2008/1/22 - Seung-Won, Bae) for Multi-Language
	m_hOcxWnd = p_hOcxWnd;
}


void CDlgInputMonth::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgInputMonth)
	DDX_Text(pDX, IDC_EDT_MONTH_MONTH, m_strMonth);
	DDX_Text(pDX, IDC_EDT_MONTH_YEAR, m_strYear);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgInputMonth, CDialog)
	//{{AFX_MSG_MAP(CDlgInputMonth)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgInputMonth message handlers

void CDlgInputMonth::OnOK() 
{
	UpdateData();

	ML_SET_LANGUAGE_RES();

	CString s;
	int nY, nM;
	nY = atoi(m_strYear);
	nM = atoi(m_strMonth);


	if(nY < 1970 || 2037 < nY)
	{
		s.LoadString( IDS_MSG_ADD_Y_ERROR);
		AfxMessageBox(s);
		CEdit * pEdit = (CEdit *)GetDlgItem(IDC_EDT_MONTH_YEAR);
		if(pEdit && IsWindow(pEdit->GetSafeHwnd()))
		{
			pEdit->SetFocus();
			pEdit->SetSel(0, -1);
		}		
		return;
	}

	if(nM < 1 || 12 < nM)
	{
		s.LoadString(IDS_MSG_ADD_M_ERROR);
		AfxMessageBox(s);
		CEdit * pEdit = (CEdit *)GetDlgItem(IDC_EDT_MONTH_MONTH);
		if(pEdit && IsWindow(pEdit->GetSafeHwnd()))
		{
			pEdit->SetFocus();
			pEdit->SetSel(0, -1);
		}		
		return;
	}
	
	m_strCurrentDate.Format("%04d%02d", nY, nM);
	CDialog::OnOK();
}

BOOL CDlgInputMonth::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(m_strCurrentDate.GetLength() >= 6)
	{
		m_strYear.Format("%04d", atoi(m_strCurrentDate.Left(4)));
		m_strMonth.Format("%02d",atoi(m_strCurrentDate.Mid(4,2)));
	}

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// DlgInputDayWeek.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "DlgInputDayWeek.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgInputDayWeek dialog


CDlgInputDayWeek::CDlgInputDayWeek(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInputDayWeek::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgInputDayWeek)
	m_strDay = _T("");
	m_strMonth = _T("");
	m_strYear = _T("");
	//}}AFX_DATA_INIT
}


void CDlgInputDayWeek::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgInputDayWeek)
	DDX_Text(pDX, IDC_EDT_DAYWEEK_DAY, m_strDay);
	DDX_Text(pDX, IDC_EDT_DAYWEEK_MONTH, m_strMonth);
	DDX_Text(pDX, IDC_EDT_DAYWEEK_YEAR, m_strYear);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgInputDayWeek, CDialog)
	//{{AFX_MSG_MAP(CDlgInputDayWeek)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgInputDayWeek message handlers

void CDlgInputDayWeek::OnOK() 
{
	UpdateData();

	CString s;
	int nY, nM, nD;
	nY = atoi(m_strYear);
	nM = atoi(m_strMonth);
	nD = atoi(m_strDay);


	if(nY < 1970 || 2037 < nY)
	{
		s.LoadString(IDS_MSG_ADD_Y_ERROR);
		AfxMessageBox(s);
		CEdit * pEdit = (CEdit *)GetDlgItem(IDC_EDT_DAYWEEK_YEAR);
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
		CEdit * pEdit = (CEdit *)GetDlgItem(IDC_EDT_DAYWEEK_MONTH);
		if(pEdit && IsWindow(pEdit->GetSafeHwnd()))
		{
			pEdit->SetFocus();
			pEdit->SetSel(0, -1);
		}		
		return;
	}

	int nDayArray[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	//윤년계산으로 2월 마지막날짜 셋팅
	if((nY % 4 == 0 && nY % 100 != 0) || (nY % 400 == 0))
		nDayArray[1] = 28;

	if(nD < 1 || nDayArray[nM-1] < nD)
	{
		CString s1;
		s.LoadString(IDS_MSG_ADD_D_ERROR);
		s1.Format(s, nDayArray[nM-1]);
		AfxMessageBox(s1);
		CEdit * pEdit = (CEdit *)GetDlgItem(IDC_EDT_DAYWEEK_DAY);
		if(pEdit && IsWindow(pEdit->GetSafeHwnd()))
		{
			pEdit->SetFocus();
			pEdit->SetSel(0, -1);
		}		
		return;
	}
	
	m_strCurrentDate.Format("%04d%02d%02d", nY, nM, nD);

	CDialog::OnOK();
}

BOOL CDlgInputDayWeek::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(m_strCurrentDate.GetLength() >= 8)
	{
		m_strYear.Format("%04d", atoi(m_strCurrentDate.Left(4)));
		m_strMonth.Format("%02d",atoi(m_strCurrentDate.Mid(4,2)));
		m_strDay.Format("%02d",  atoi(m_strCurrentDate.Mid(6,2)));
	}

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

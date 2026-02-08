// DlgInputTick.cpp : implementation file
//

#include "stdafx.h"
#include "ChartDataListViewerAddIn.h"
#include "DlgInputTick.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgInputTick dialog


CDlgInputTick::CDlgInputTick(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInputTick::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgInputTick)
	m_strSecond = _T("");
	m_strDay = _T("");
	m_strHour = _T("");
	m_strMin = _T("");
	//}}AFX_DATA_INIT
}


void CDlgInputTick::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgInputTick)
	DDX_Text(pDX, IDC_EDT_TICK_SECOND, m_strSecond);
	DDX_Text(pDX, IDC_EDT_TICK_DAY, m_strDay);
	DDX_Text(pDX, IDC_EDT_TICK_HOUR, m_strHour);
	DDX_Text(pDX, IDC_EDT_TICK_MIN, m_strMin);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgInputTick, CDialog)
	//{{AFX_MSG_MAP(CDlgInputTick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDlgInputTick message handlers


BOOL CDlgInputTick::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(m_strCurrentDate.GetLength() >= 8)
	{
		m_strDay.Format("%02d",   atoi(m_strCurrentDate.Mid(0,2)));
		m_strHour.Format("%02d",  atoi(m_strCurrentDate.Mid(2,2)));
		m_strMin.Format("%02d",   atoi(m_strCurrentDate.Mid(4,2)));		
		m_strSecond.Format("%02d",atoi(m_strCurrentDate.Mid(6,2)));
	}

	UpdateData(FALSE);		
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgInputTick::OnOK() 
{
	UpdateData();

	CString s;
	int nD, nHour, nMin, nSecond;
	nD		= atoi(m_strDay);
	nHour	= atoi(m_strHour);
	nMin	= atoi(m_strMin);
	nSecond = atoi(m_strSecond);
	
	
	int nDayArray[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};	

	CTime t	= CTime::GetCurrentTime();
	int nM	= t.GetMonth();
	if(nD < 1 || nDayArray[nM-1] < nD)
	{
		CString s1;
		s.LoadString(IDS_MSG_ADD_D_ERROR);
		s1.Format(s, nDayArray[nM-1]);
		AfxMessageBox(s1);
		CEdit * pEdit = (CEdit *)GetDlgItem(IDC_EDT_TICK_DAY);
		if(pEdit && IsWindow(pEdit->GetSafeHwnd()))
		{
			pEdit->SetFocus();
			pEdit->SetSel(0, -1);
		}		
		return;
	}
	
	if(nHour < 0 || 23 < nHour)
	{
		CString s1;
		s.LoadString(IDS_MSG_ADD_D_ERROR);
		s1.Format(s, nDayArray[nM-1]);
		AfxMessageBox(s1);
		CEdit * pEdit = (CEdit *)GetDlgItem(IDC_EDT_TICK_DAY);
		if(pEdit && IsWindow(pEdit->GetSafeHwnd()))
		{
			pEdit->SetFocus();
			pEdit->SetSel(0, -1);
		}		
		return;
	}

	if(nMin < 0 || 60 <= nMin)
	{
		CString s1;
		s.LoadString(IDS_MSG_ADD_D_ERROR);
		s1.Format(s, nDayArray[nM-1]);
		AfxMessageBox(s1);
		CEdit * pEdit = (CEdit *)GetDlgItem(IDC_EDT_TICK_DAY);
		if(pEdit && IsWindow(pEdit->GetSafeHwnd()))
		{
			pEdit->SetFocus();
			pEdit->SetSel(0, -1);
		}		
		return;
	}

	if(nSecond < 0 || 60 <= nSecond)
	{
		CString s1;
		s.LoadString(IDS_MSG_ADD_D_ERROR);
		s1.Format(s, nDayArray[nM-1]);
		AfxMessageBox(s1);
		CEdit * pEdit = (CEdit *)GetDlgItem(IDC_EDT_TICK_DAY);
		if(pEdit && IsWindow(pEdit->GetSafeHwnd()))
		{
			pEdit->SetFocus();
			pEdit->SetSel(0, -1);
		}		
		return;
	}

	m_strCurrentDate.Format("%02d%02d%02d%02d", nD, nHour, nMin, nSecond);

	CDialog::OnOK();
}

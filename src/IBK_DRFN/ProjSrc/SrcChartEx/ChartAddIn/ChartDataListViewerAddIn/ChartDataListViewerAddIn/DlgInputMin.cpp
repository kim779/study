// DlgInputMin.cpp : implementation file
//

#include "stdafx.h"
#include "ChartDataListViewerAddIn.h"
#include "DlgInputMin.h"

#include "../Include_Chart/Dll_Load/IMetaTable.h"		// ML_SET_LANGUAGE_RES()

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgInputMin dialog


CDlgInputMin::CDlgInputMin( HWND p_hOcxWnd, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInputMin::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgInputMin)
	m_strDay	= _T("");
	m_strHour	= _T("");
	m_strMonth	= _T("");
	m_strMin	= _T("");
	//}}AFX_DATA_INIT

	// (2008/1/22 - Seung-Won, Bae) for Multi-Language
	m_hOcxWnd = p_hOcxWnd;
}


void CDlgInputMin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgInputMin)
	DDX_Text(pDX, IDC_EDT_MIN_DAY,	m_strDay);
	DDX_Text(pDX, IDC_EDT_MIN_HOUR, m_strHour);
	DDX_Text(pDX, IDC_EDT_MIN_MONTH,m_strMonth);
	DDX_Text(pDX, IDC_EDT_MIN_MIN,	m_strMin);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgInputMin, CDialog)
	//{{AFX_MSG_MAP(CDlgInputMin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgInputMin message handlers

void CDlgInputMin::OnOK() 
{
	UpdateData();

	ML_SET_LANGUAGE_RES();

	CString s;
	int nM, nD, nHour, nMin;
	nM		= atoi(m_strMonth);
	nD		= atoi(m_strDay);
	nHour	= atoi(m_strHour);
	nMin	= atoi(m_strMin);	
	
	if(nM < 1 || 12 < nM)
	{
		CString s1;
		s.LoadString( IDS_MSG_ADD_M_ERROR);
		AfxMessageBox(s);
		CEdit * pEdit = (CEdit *)GetDlgItem(IDC_EDT_MIN_MONTH);
		if(pEdit && IsWindow(pEdit->GetSafeHwnd()))
		{
			pEdit->SetFocus();
			pEdit->SetSel(0, -1);
		}		
		return;
	}
	
	int nDayArray[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};	
	if(nD < 1 || nDayArray[nM-1] < nD)
	{
		CString s1;
		s.LoadString( IDS_MSG_ADD_D_ERROR);
		s1.Format(s, nDayArray[nM-1]);
		AfxMessageBox(s1);
		CEdit * pEdit = (CEdit *)GetDlgItem(IDC_EDT_MIN_DAY);
		if(pEdit && IsWindow(pEdit->GetSafeHwnd()))
		{
			pEdit->SetFocus();
			pEdit->SetSel(0, -1);
		}		
		return;
	}
	
	if(nHour < 0 || 24 <= nHour)
	{
		CString s1;
		s.LoadString( IDS_MSG_ADD_HOUR_ERROR);		
		AfxMessageBox(s);
		CEdit * pEdit = (CEdit *)GetDlgItem(IDC_EDT_MIN_HOUR);
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
		s.LoadString( IDS_MSG_ADD_MIN_ERROR);		
		AfxMessageBox(s);
		CEdit * pEdit = (CEdit *)GetDlgItem(IDC_EDT_MIN_MIN);
		if(pEdit && IsWindow(pEdit->GetSafeHwnd()))
		{
			pEdit->SetFocus();
			pEdit->SetSel(0, -1);
		}		
		return;
	}
	
	m_strCurrentDate.Format("%02d%02d%02d%02d", nM, nD, nHour, nMin);

	CDialog::OnOK();
}

BOOL CDlgInputMin::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(m_strCurrentDate.GetLength() >= 8)
	{
		m_strMonth.Format("%02d", atoi(m_strCurrentDate.Mid(0,2)));
		m_strDay.Format("%02d",   atoi(m_strCurrentDate.Mid(2,2)));
		m_strHour.Format("%02d",  atoi(m_strCurrentDate.Mid(4,2)));
		m_strMin.Format("%02d",   atoi(m_strCurrentDate.Mid(6,2)));		
	}

	UpdateData(FALSE);	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

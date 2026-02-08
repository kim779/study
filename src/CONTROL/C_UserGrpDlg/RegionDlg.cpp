// RegionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "C_UserGrpDlg.h"
#include "RegionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegionDlg dialog
#define	MAX_CMB	9
static int CID[MAX_CMB] = 
{ IDC_COMBO01, IDC_COMBO02, IDC_COMBO03, IDC_COMBO04, IDC_COMBO05,
IDC_COMBO06, IDC_COMBO07, IDC_COMBO08, IDC_COMBO09};

CRegionDlg::CRegionDlg(int count, CString sRegion, CWnd* pParent /*=NULL*/)
	: CDialog(CRegionDlg::IDD, pParent)
{
	m_Count = count;
	m_sRegion = sRegion;

	//{{AFX_DATA_INIT(CRegionDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CRegionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRegionDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRegionDlg, CDialog)
	//{{AFX_MSG_MAP(CRegionDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegionDlg message handlers

BOOL CRegionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	Init();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRegionDlg::OnOK() 
{
	int	iRate = 0, iTotal = 0;
	CWnd	*pWnd = nullptr;
	CString	sTmp;
	CStringArray	arRate;

	m_sRegion = "";
	arRate.RemoveAll();
	for (int ii = 0; ii < m_Count; ii++)
	{
		pWnd = GetDlgItem(CID[ii]);
		pWnd->GetWindowText(sTmp);
		iRate = atoi(sTmp);
		if (iRate <= 0 || iRate >= 100)
		{
			AfxMessageBox("영역 비율은 모두 0과 100 사이의 값이어야 합니다.");
			return;
		}

		iTotal += iRate;
		sTmp.Format("%02d", iRate);
		arRate.Add(sTmp);
	}

	if (iTotal > 100)
	{
		AfxMessageBox("전체 비율의 합이 100%를 초과입니다.");
		return;
	}

	for (int ii = 0; ii < m_Count; ii++)
	{
		sTmp = arRate.GetAt(ii);
		if (ii == m_Count-1)
			sTmp.Format("%02d", atoi(sTmp) + 100 - iTotal);

		m_sRegion += sTmp;
	}

	CDialog::OnOK();
}

void CRegionDlg::Init()
{
	if (m_Count < 2 || m_Count > MAX_CMB)
		return;

	CWnd	*pWnd = nullptr;
	for (int ii = 0; ii < MAX_CMB; ii++)
	{
		pWnd = GetDlgItem(CID[ii]);
		((CComboBox*)pWnd)->SetCurSel(0);
	}

	for (int ii = 0; ii < m_Count; ii++)
	{
		pWnd = GetDlgItem(CID[ii]);
		pWnd->EnableWindow();
	}

	CString	sTmp;
	if (m_sRegion.GetLength() == m_Count*2)
	{
		for (int ii = 0; ii < m_Count; ii++)
		{
			sTmp = m_sRegion.Mid(ii*2, 2);
			pWnd = GetDlgItem(CID[ii]);
			pWnd->SetWindowText(sTmp);
		}
	}
	else
	{
		const	int	iRate = (int)(100.0/(double)m_Count);
		const	int	iMod = 100 - iRate * m_Count;

		for (int ii = 0; ii < m_Count; ii++)
		{
			if (ii == m_Count - 1)
				sTmp.Format("%d", iRate + iMod);
			else
				sTmp.Format("%d", iRate);

			pWnd = GetDlgItem(CID[ii]);
			pWnd->SetWindowText(sTmp);
		}
	}
}
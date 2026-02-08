// SortSignalDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IB202201.h"
#include "SortSignalDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSrotSignalDlg dialog


CSortSignalDlg::CSortSignalDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(CSortSignalDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSortSignalDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_strUp   = "¡ã";
	m_strDown = "¡å";
}


void CSortSignalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSortSignalDlg)
	DDX_Control(pDX, IDC_STATIC_SIGNAL, m_stcUpDown);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSortSignalDlg, CDialog)
	//{{AFX_MSG_MAP(CSortSignalDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSortSignalDlg Operators

void CSortSignalDlg::SetSortSignal(int nCol, BOOL bUpDown)
{
	if (bUpDown)
	{
		m_stcUpDown.SetWindowText(m_strUp);
	}
	else
	{
		m_stcUpDown.SetWindowText(m_strDown);
	}

	switch(nCol) {
	case 0:
		SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		break;
	case 1:
		SetWindowPos(&CWnd::wndTopMost, 100, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CSortSignalDlg message handlers

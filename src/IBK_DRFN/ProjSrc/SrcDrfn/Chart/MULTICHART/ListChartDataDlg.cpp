// ListChartDataDlg.cpp : implementation file
//

#include "stdafx.h"
#include "multichart.h"
#include "ListChartDataDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListChartDataDlg dialog


CListChartDataDlg::CListChartDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CListChartDataDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CListChartDataDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CListChartDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CListChartDataDlg)
	DDX_Control(pDX, IDC_LIST_CHARTDATA, m_listData);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CListChartDataDlg, CDialog)
	//{{AFX_MSG_MAP(CListChartDataDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListChartDataDlg message handlers

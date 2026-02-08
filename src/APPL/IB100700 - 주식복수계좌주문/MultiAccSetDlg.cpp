// MultiAccSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IB100700.h"
#include "MultiAccSetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMultiAccSetDlg dialog


CMultiAccSetDlg::CMultiAccSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMultiAccSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMultiAccSetDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMultiAccSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMultiAccSetDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMultiAccSetDlg, CDialog)
	//{{AFX_MSG_MAP(CMultiAccSetDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMultiAccSetDlg message handlers

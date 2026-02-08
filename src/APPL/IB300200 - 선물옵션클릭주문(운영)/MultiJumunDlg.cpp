// MultiJumunDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MultiJumunDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMultiJumunDlg dialog


CMultiJumunDlg::CMultiJumunDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMultiJumunDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMultiJumunDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CMultiJumunDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMultiJumunDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMultiJumunDlg, CDialog)
	//{{AFX_MSG_MAP(CMultiJumunDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMultiJumunDlg message handlers

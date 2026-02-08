// TargetItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TargetItemDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTargetItemDlg dialog


CTargetItemDlg::CTargetItemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTargetItemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTargetItemDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTargetItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTargetItemDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTargetItemDlg, CDialog)
	//{{AFX_MSG_MAP(CTargetItemDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTargetItemDlg message handlers

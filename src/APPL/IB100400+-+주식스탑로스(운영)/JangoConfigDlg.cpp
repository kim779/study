// JangoConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IB100400.h"
#include "JangoConfigDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJangoConfigDlg dialog


CJangoConfigDlg::CJangoConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CJangoConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CJangoConfigDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CJangoConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJangoConfigDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CJangoConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CJangoConfigDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJangoConfigDlg message handlers

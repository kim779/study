// Dlgtest.cpp : implementation file
//

#include "stdafx.h"
#include "cx_test.h"
#include "Dlgtest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgtest dialog


CDlgtest::CDlgtest(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgtest::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgtest)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgtest::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgtest)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgtest, CDialog)
	//{{AFX_MSG_MAP(CDlgtest)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgtest message handlers

// DlgSTSelector.cpp : implementation file
//

#include "stdafx.h"
#include "hwstrategyanalysis.h"
#include "DlgSTSelector.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSTSelector dialog


CDlgSTSelector::CDlgSTSelector(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSTSelector::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSTSelector)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgSTSelector::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSTSelector)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSTSelector, CDialog)
	//{{AFX_MSG_MAP(CDlgSTSelector)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSTSelector message handlers

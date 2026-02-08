// TEST.cpp : implementation file
//

#include "stdafx.h"
#include "CX_FundPrinter.h"
#include "TEST.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TEST dialog


TEST::TEST(CWnd* pParent /*=NULL*/)
	: CDialog(TEST::IDD, pParent)
{
	//{{AFX_DATA_INIT(TEST)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void TEST::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TEST)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TEST, CDialog)
	//{{AFX_MSG_MAP(TEST)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TEST message handlers

// Dbar5.cpp : implementation file
//

#include "stdafx.h"
//#include "axissm.h"
#include "Dbar5.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDbar5 dialog


CDbar5::CDbar5(CWnd* pParent /*=NULL*/)
	: CDialog(CDbar5::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDbar5)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDbar5::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDbar5)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDbar5, CDialog)
	//{{AFX_MSG_MAP(CDbar5)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDbar5 message handlers

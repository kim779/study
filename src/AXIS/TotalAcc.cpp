// TotalAcc.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "TotalAcc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTotalAcc dialog


CTotalAcc::CTotalAcc(CWnd* pParent /*=NULL*/)
	: CDialog(CTotalAcc::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTotalAcc)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


BEGIN_MESSAGE_MAP(CTotalAcc, CDialog)
	//{{AFX_MSG_MAP(CTotalAcc)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTotalAcc message handlers

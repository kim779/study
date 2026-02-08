// Warning.cpp : implementation file
//

#include "stdafx.h"
#include "IB885200.h"
#include "Warning.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWarning dialog


CWarning::CWarning(CWnd* pParent /*=NULL*/)
	: CDialog(CWarning::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWarning)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CWarning::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWarning)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWarning, CDialog)
	//{{AFX_MSG_MAP(CWarning)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWarning message handlers

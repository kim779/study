// MyFindDlg.cpp : implementation file
//

#include "stdafx.h"
#include "axiswork.h"
#include "MyFindDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyFindDlg dialog


CMyFindDlg::CMyFindDlg(CWnd* pParent /*=NULL*/)
	: CFindReplaceDialog()
{
	//{{AFX_DATA_INIT(CMyFindDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMyFindDlg::DoDataExchange(CDataExchange* pDX)
{
	CFindReplaceDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyFindDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyFindDlg, CFindReplaceDialog)
	//{{AFX_MSG_MAP(CMyFindDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyFindDlg message handlers

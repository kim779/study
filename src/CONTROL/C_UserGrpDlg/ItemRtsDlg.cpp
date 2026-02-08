// ItemRtsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "C_UserGrpDlg.h"
#include "ItemRtsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CItemRtsDlg dialog


CItemRtsDlg::CItemRtsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CItemRtsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CItemRtsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CItemRtsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CItemRtsDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CItemRtsDlg, CDialog)
	//{{AFX_MSG_MAP(CItemRtsDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CItemRtsDlg message handlers

// DlgCompileStatus.cpp : implementation file
//

#include "stdafx.h"
#include "systemtrading.h"
#include "DlgCompileStatus.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCompileStatus dialog


CDlgCompileStatus::CDlgCompileStatus(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCompileStatus::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCompileStatus)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgCompileStatus::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCompileStatus)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCompileStatus, CDialog)
	//{{AFX_MSG_MAP(CDlgCompileStatus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCompileStatus message handlers


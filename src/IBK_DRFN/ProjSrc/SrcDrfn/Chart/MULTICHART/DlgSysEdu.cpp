// DlgGlass.cpp : implementation file
//

#include "stdafx.h"
#include "DlgSysEdu.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSysEdu dialog

CDlgSysEdu::CDlgSysEdu(CWnd* pParent /*=NULL*/)
	: CRscDlg(CDlgSysEdu::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSysEdu)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CDlgSysEdu::~CDlgSysEdu()
{
}


void CDlgSysEdu::DoDataExchange(CDataExchange* pDX)
{
	CRscDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSysEdu)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSysEdu, CRscDlg)
	//{{AFX_MSG_MAP(CDlgSysEdu)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSysEdu message handlers

BOOL CDlgSysEdu::OnInitDialog() 
{
	CRscDlg::OnInitDialog();
	return TRUE;
}
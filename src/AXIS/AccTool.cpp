// AccTool.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "AccTool.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAccTool dialog


CAccTool::CAccTool(CWnd* pParent /*=NULL*/)
	: CDialog(CAccTool::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAccTool)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_parent = pParent;
}


void CAccTool::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAccTool)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAccTool, CDialog)
	//{{AFX_MSG_MAP(CAccTool)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_APPLYACC, OnApplyACC)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccTool message handlers

LONG CAccTool::OnApplyACC(WPARAM wp, LPARAM lp)
{
	if (!m_parent)	m_parent = GetParent();
	return m_parent->SendMessage(WM_APPLYACC, wp, lp);
}

// RscDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RscDlg.h"

// 2008.02.20 by LYH >>
#include "./Include_Chart/Dll_Load/IMetaTable.h"		// for g_iMetaTable
// 2008.02.20 by LYH <<

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRscDlg dialog
extern HINSTANCE g_hInstance;


CRscDlg::CRscDlg(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
	: CDialog(nIDTemplate , pParent)
{
	//{{AFX_DATA_INIT(CRscDlg)
		// NOTE: the ClassWizard will add member initialization here
// 2008.02.20 by LYH >>
	m_hOcxWnd = NULL;
// 2008.02.20 by LYH <<
	//}}AFX_DATA_INIT
}


void CRscDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRscDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRscDlg, CDialog)
	//{{AFX_MSG_MAP(CRscDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRscDlg message handlers

 

BOOL CRscDlg::Create(UINT nID,CWnd* pParentWnd)
{
	// TODO: Add your specialized code here and/or call the base class
	ML_SET_LANGUAGE_RES();
	BOOL bRetValue = CDialog::Create(nID, pParentWnd);
	return bRetValue;
}

int CRscDlg::DoModal() 
{
	// TODO: Add your specialized code here and/or call the base class
	ML_SET_LANGUAGE_RES();
	BOOL bRetValue = CDialog::DoModal();
	return bRetValue;

}

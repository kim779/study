// RscDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RscDlg.h"

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
	HINSTANCE hInstResourceClient = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);
	BOOL bRetValue = CDialog::Create(nID, pParentWnd);
	AfxSetResourceHandle(hInstResourceClient);
	return bRetValue;
}

int CRscDlg::DoModal() 
{
	// TODO: Add your specialized code here and/or call the base class
	HINSTANCE hInstResourceClient = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);	
	BOOL bRetValue = CDialog::DoModal();
	AfxSetResourceHandle(hInstResourceClient);
	return bRetValue;

}

// LRESULT CRscDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
// {
// 	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
// 	if( WM_IDLEUPDATECMDUI == message) return 0;
// 
// 	return CDialog::WindowProc(message, wParam, lParam);
// }
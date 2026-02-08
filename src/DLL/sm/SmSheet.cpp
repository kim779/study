// SmSheet.cpp : implementation file
//

#include "stdafx.h"
#include "SmSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSmSheet

IMPLEMENT_DYNAMIC(CSmSheet, CPropertySheet)

CSmSheet::CSmSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CSmSheet::CSmSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
}

CSmSheet::~CSmSheet()
{
}


BEGIN_MESSAGE_MAP(CSmSheet, CPropertySheet)
	//{{AFX_MSG_MAP(CSmSheet)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSmSheet message handlers

BOOL CSmSheet::OnInitDialog() 
{
	const BOOL bResult = CPropertySheet::OnInitDialog();
	CenterWindow(GetDesktopWindow());
	return bResult;
}

BOOL CSmSheet::PreTranslateMessage(MSG* pMsg) 
{
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		switch (pMsg->wParam)
		{
		case VK_RETURN:
		//case VK_ESCAPE:
			return TRUE;
		}
		break;
	}
	return CPropertySheet::PreTranslateMessage(pMsg);
}

 // XDateTime.cpp : implementation file
//

#include "stdafx.h"
#include "hwstrategyanalysis.h"
#include "XDateTime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXDateTime

CXDateTime::CXDateTime()
{
}

CXDateTime::~CXDateTime()
{
}


BEGIN_MESSAGE_MAP(CXDateTime, CDateTimeCtrl)
	//{{AFX_MSG_MAP(CXDateTime)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXDateTime message handlers

void CXDateTime::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default

	if ( VK_ESCAPE == nChar )
	{
		GetParent()->SendMessage( WM_XEDIT_VK_ESCAPE);
		return;
	}	
	CDateTimeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

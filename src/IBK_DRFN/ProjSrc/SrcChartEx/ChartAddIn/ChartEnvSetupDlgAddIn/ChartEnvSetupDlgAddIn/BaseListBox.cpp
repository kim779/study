// BaseListBox.cpp : implementation file
//

#include "stdafx.h"
#include "ChartEnvSetupDlgAddIn.h"
#include "BaseListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBaseListBox

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 25
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
CBaseListBox::CBaseListBox()
{

}


//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 25
// Modifier		: 
// Comments		: 
//-----------------------------------------------------------------------------
CBaseListBox::~CBaseListBox()
{
	
}


BEGIN_MESSAGE_MAP(CBaseListBox, CListBox)
	//{{AFX_MSG_MAP(CBaseListBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBaseListBox message handlers

//-----------------------------------------------------------------------------
// Author		: Sang-Woo, Cho		Date : 2005 / 7 / 25
// Return BOOL : 
// Param  MSG* pMsg : 
// Comments		: List에서 Delete key를 눌렀을때를 처리해준다.
//-----------------------------------------------------------------------------
BOOL CBaseListBox::PreTranslateMessage( MSG* pMsg ) 
{
	int nIndex = GetCurSel();
	
	if( pMsg->message == WM_KEYDOWN )
	{
		if( pMsg->wParam == VK_DELETE )
		{
			DeleteString( nIndex );
		}
	}
	
	return CListBox::PreTranslateMessage(pMsg);
}

// SEdit.cpp : implementation file
//

#include "stdafx.h"
#include "IB771100.h"
#include "SEdit.h"
#include "MapDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSEdit

CSEdit::CSEdit(CMapDlg* mapdlg)
{ 
	m_pMapDlg = mapdlg;
}

CSEdit::~CSEdit()
{
}


BEGIN_MESSAGE_MAP(CSEdit, CEdit)
	//{{AFX_MSG_MAP(CSEdit)
	ON_WM_KEYDOWN()
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSEdit message handlers

void CSEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
////	TRACE("KeyDown , %d\n",nChar);
	if( nChar == VK_RETURN ){
		m_pMapDlg->SendMessage(WM_COMBO_ENTERED); 
	}
		
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);

	CString szText = "";
	GetWindowText(szText);
	((CSComboBox*)GetParent())->SetWindowText(szText);
	
}
#include "SComboBox1.h"
void CSEdit::OnChange() 
{
	CString szText = "";
	GetWindowText(szText);
	((CSComboBox*)GetParent())->SetWindowText(szText);
	
//	TRACE("%s\n",szText);
}

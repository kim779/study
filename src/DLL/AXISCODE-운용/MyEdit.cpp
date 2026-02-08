// MyEdit.cpp : implementation file
//

#include "stdafx.h"
#include "axiscode.h"
#include "MyEdit.h"
#include "user.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyEdit

CMyEdit::CMyEdit()
{
	m_bComposition = FALSE;
}

CMyEdit::~CMyEdit()
{
}


BEGIN_MESSAGE_MAP(CMyEdit, CEdit)
	//{{AFX_MSG_MAP(CMyEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyEdit message handlers


BOOL CMyEdit::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN	&& pMsg->wParam == VK_RETURN)
		GetParent()->SendMessage(WM_USER, GetDlgCtrlID());
	return CEdit::PreTranslateMessage(pMsg);
}

LRESULT CMyEdit::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	switch(message)
	{
		//조합중
	case WM_IME_STARTCOMPOSITION:
		m_bComposition = TRUE;
		break;
		
		//조합완료
	case WM_IME_ENDCOMPOSITION:
		m_bComposition = FALSE;
		break;
		
	case WM_IME_COMPOSITION:
		break;
	}
	
	return CEdit::DefWindowProc(message, wParam, lParam);
}

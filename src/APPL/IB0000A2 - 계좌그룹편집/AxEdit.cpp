
#include "stdafx.h"
#include "AxEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CAxEdit::CAxEdit()
{
	m_bKeyDown = false;
}

CAxEdit::~CAxEdit()
{
}


BEGIN_MESSAGE_MAP(CAxEdit, CEdit)
	//{{AFX_MSG_MAP(CAxEdit)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CAxEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{	
	CString sTmp, sTmp2;
	GetWindowText(sTmp);

	CEdit::OnChar(nChar, nRepCnt, nFlags);

	GetWindowText(sTmp2);
	if ((sTmp.GetLength() < (int)GetLimitText() && (int)GetLimitText() == sTmp2.GetLength()) || 
		nChar == VK_RETURN || nChar == VK_TAB)
		GetParent()->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), EN_MAXTEXT), (long)this->m_hWnd);
}

#include "stdafx.h"
#include "exEdit.h"
#include "../libDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CExEdit::CExEdit()
{
	m_pParent = NULL;
}

CExEdit::~CExEdit()
{
}


BEGIN_MESSAGE_MAP(CExEdit, CEdit)
	//{{AFX_MSG_MAP(CExEdit)
	ON_CONTROL_REFLECT(EN_SETFOCUS, OnSetfocus)
	ON_CONTROL_REFLECT(EN_KILLFOCUS, OnKillfocus)
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CExEdit::OnSetfocus() 
{
	SetSel(0, -1);
	int id = GetDlgCtrlID();
	if (m_pParent)
		m_pParent->SendMessage(WM_LOCAL, MAKEWPARAM(EVT_SETFOCUS, 0), id);
	
}

void CExEdit::OnKillfocus() 
{
	if (m_pParent)
		m_pParent->SendMessage(WM_LOCAL, MAKEWPARAM(EVT_SETFOCUS, 0), 0);
}

void CExEdit::OnLButtonUp(UINT nFlags, CPoint point) 
{
	SetSel(0, -1);	
	CEdit::OnLButtonUp(nFlags, point);
}

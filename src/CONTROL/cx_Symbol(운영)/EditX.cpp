// EditX.cpp : implementation file
//

#include "stdafx.h"
#include "CX_Symbol.h"
#include "EditX.h"
#include "InfoPopup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditX

CEditX::CEditX()
{
	m_pParent = nullptr;
}

CEditX::~CEditX()
{
}


BEGIN_MESSAGE_MAP(CEditX, CEdit)
	//{{AFX_MSG_MAP(CEditX)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditX message handlers
BOOL CEditX::Create(CWnd* parent, UINT nID)
{
	m_brush.DeleteObject();
	m_brush.CreateSolidBrush(GetSysColor(COLOR_INFOBK));

	const	DWORD	dwStyle = WS_CHILD|ES_LEFT|ES_AUTOVSCROLL|ES_WANTRETURN|ES_MULTILINE;
	return CEdit::Create(dwStyle, CRect(0,0,0,0), parent, nID);
}

HBRUSH CEditX::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->SetBkColor(GetSysColor(COLOR_INFOBK));
	
	return m_brush;
	return NULL;
}

void CEditX::SetPopupParent(CWnd* pParent)
{
	m_pParent = pParent;
}

void CEditX::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);

	/*CInfoPopup *pTemp = (CInfoPopup*)m_pParent;
	
	if (pNewWnd == pTemp || pNewWnd == this)
		return;

	pTemp->ShowWindow(SW_HIDE);
	pTemp->m_sKey = CString(pTemp->m_jinfo.codx, 12);
	pTemp->m_sKey.Trim();
	if (!pTemp->m_sKey.IsEmpty()) 
		pTemp->saveServerMemo(pTemp->m_sKey);*/
}

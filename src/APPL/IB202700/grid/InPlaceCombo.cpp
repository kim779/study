// InPlaceCombo.cpp : implementation file
//

#include "stdafx.h"
#include "InPlaceCombo.h"
#include "griditem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInPlaceCombo

CInPlaceCombo::CInPlaceCombo(CWnd* parent, CRect& rect, DWORD dwStyle, UINT nID,
				int nRow, int nColumn, CString sInitText, 
				LOGFONT* logfont, int active)
{
	m_sInitText = sInitText;
	m_nRow      = nRow;
	m_nColumn   = nColumn;
	m_rect      = rect;

	rect.bottom += 200;
	if (!Create(WS_CHILD|WS_VISIBLE|dwStyle, rect, parent, nID))
		return;

	if (logfont != nullptr)
	{
		m_font.CreatePointFontIndirect(logfont);
		SetFont(&m_font);
	}
	else
	{
		SetFont(parent->GetFont());
	}

	SetFocus();

	int idx{}; CString string = sInitText, entry;
	while (!string.IsEmpty())
	{
		if ((idx = string.Find('|')) != -1)
		{
			entry = string.Left(idx++); string = string.Mid(idx);
		}
		else
		{
			entry = string; string = _T("");
		}
		AddString(entry);
	}

	if (GetCount() >= active) SetCurSel(active);
}

CInPlaceCombo::~CInPlaceCombo()
{
}


BEGIN_MESSAGE_MAP(CInPlaceCombo, CComboBox)
	//{{AFX_MSG_MAP(CInPlaceCombo)
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInPlaceCombo message handlers

void CInPlaceCombo::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_PRIOR || nChar == VK_NEXT || nChar == VK_RIGHT || nChar == VK_LEFT)
	{
		GetParent()->SetFocus();
		return;
	}
	
	CComboBox::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CInPlaceCombo::OnKillFocus(CWnd* pNewWnd) 
{
	CComboBox::OnKillFocus(pNewWnd);
	EndCombo();	
}

BOOL CInPlaceCombo::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_SYSCHAR)
		return TRUE;

	return CComboBox::PreTranslateMessage(pMsg);
}

void CInPlaceCombo::PostNcDestroy() 
{
	CComboBox::PostNcDestroy();
	delete this;
}

void CInPlaceCombo::EndCombo()
{
	MessageToParent(GVNM_ENDCOMBO);
	PostMessage(WM_CLOSE, 0, 0);
}

void CInPlaceCombo::OnSelchange() 
{
	MessageToParent(GVNM_SELCHANGECOMBO);
}

void CInPlaceCombo::MessageToParent(int nMessage)
{
	CString str; GetWindowText(str);

	NM_USER nmuser;

	nmuser.hdr.hwndFrom = GetSafeHwnd();
	nmuser.hdr.idFrom   = GetDlgCtrlID();
	nmuser.hdr.code     = nMessage;

	nmuser.item.mask    = LVIF_TEXT|LVIF_PARAM;
	nmuser.item.row     = m_nRow;
	nmuser.item.col     = m_nColumn;
	nmuser.item.text    = str;
	nmuser.item.param  = (LPARAM) GetCurSel(); 
    	CWnd* pOwner = GetOwner();
	if (pOwner) pOwner->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&nmuser);
}

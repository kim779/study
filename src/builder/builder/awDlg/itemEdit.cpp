// itemEdit.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "itemEdit.h"


// CItemEdit

IMPLEMENT_DYNAMIC(CItemEdit, CEdit)

CItemEdit::CItemEdit(int item, int subItem, CString& content)
{
	m_item    = item;
	m_subItem = subItem;
	m_content = content;
	m_escape  = FALSE; 
}

CItemEdit::~CItemEdit()
{
}


BEGIN_MESSAGE_MAP(CItemEdit, CEdit)
	ON_WM_CREATE()
	ON_WM_CHAR()
	ON_WM_NCDESTROY()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

// CItemEdit 메시지 처리기입니다.

BOOL CItemEdit::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{		
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_DELETE || pMsg->wParam == VK_ESCAPE || GetKeyState( VK_CONTROL))
		{			
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return 1;
		}	
	}
	return CEdit::PreTranslateMessage(pMsg);
}


int CItemEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;

	CFont*	font = GetParent()->GetFont();
	SetFont(font);
	SetWindowText(m_content);
	SetFocus();	
	SetSel(0, 0);
	return 0;
}


void CItemEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_ESCAPE:
		m_escape = TRUE;
	case VK_RETURN:
		GetParent()->SetFocus();
		return;	
	}

	CString	str;	
	CRect	rect, rc;
	CWindowDC dc(this);

	CEdit::OnChar(nChar, nRepCnt, nFlags);

	GetWindowText( str );	
	CFont*	pFont = GetParent()->GetFont();
	CFont*	pFontDC = dc.SelectObject(pFont);
	CSize	size = dc.GetTextExtent(str);	
	dc.SelectObject(pFontDC);
	size.cx += 5; // add some extra buffer

	GetClientRect(&rect);
	GetParent()->GetClientRect(&rc);
	ClientToScreen(&rect);
	GetParent()->ScreenToClient(&rect);

	if (size.cx > rect.Width())	
	{
		if (size.cx + rect.left < rc.right)
			rect.right = rect.left + size.cx;		
		else			
			rect.right = rc.right;
		MoveWindow(&rect);	
	}
}


void CItemEdit::OnNcDestroy()
{
	CEdit::OnNcDestroy();
	delete this;
}


void CItemEdit::OnKillFocus(CWnd* pNewWnd)
{
	CString str;
	LV_DISPINFO	lvDispInfo;

	CEdit::OnKillFocus(pNewWnd);

	GetWindowText(str);

	// Send Notification to parent of ListView ctrl	
	lvDispInfo.hdr.hwndFrom = GetParent()->m_hWnd;
	lvDispInfo.hdr.idFrom   = GetDlgCtrlID();	
	lvDispInfo.hdr.code     = LVN_ENDLABELEDIT;
	lvDispInfo.item.mask    = LVIF_TEXT;	
	lvDispInfo.item.iItem   = m_item;
	lvDispInfo.item.iSubItem = m_subItem;
	lvDispInfo.item.pszText  = m_escape ? NULL : LPTSTR((LPCTSTR)str);
	lvDispInfo.item.cchTextMax = str.GetLength();
	GetParent()->GetParent()->SendMessage(WM_NOTIFY, GetParent()->GetDlgCtrlID(), (LPARAM)&lvDispInfo);
	DestroyWindow();	
}


#include "stdafx.h"
#include "listEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CListEdit::CListEdit()
{
	m_pListCtrl = nullptr;
	m_Item      = 0;
	m_SubItem   = 0;
	m_bEscape   = FALSE;
}

CListEdit::CListEdit(CExListCtrl* pList, int iItem, int iSubItem, CString sInitText)
{
	m_pListCtrl = pList;
	m_Item      = iItem;
	m_SubItem   = iSubItem;
	m_InitText  = sInitText;
	m_bEscape   = FALSE;
}

CListEdit::~CListEdit()
{
}

BEGIN_MESSAGE_MAP(CListEdit, CEdit)
	//{{AFX_MSG_MAP(CListEdit)
	ON_WM_KILLFOCUS()
	ON_WM_NCDESTROY()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_CHAR()
	ON_WM_CREATE()
	ON_WM_GETDLGCODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CListEdit::SetListText()
{
	CString text;
	GetWindowText(text);

	LV_DISPINFO dispinfo;
	dispinfo.hdr.hwndFrom = GetParent()->m_hWnd;
	dispinfo.hdr.idFrom = GetDlgCtrlID();
	dispinfo.hdr.code = LVN_ENDLABELEDIT;

	dispinfo.item.mask = LVIF_TEXT;
	dispinfo.item.iItem = m_Item;
	dispinfo.item.iSubItem = m_SubItem;
	dispinfo.item.pszText = m_bEscape ? NULL : LPTSTR((LPCTSTR)text);
	dispinfo.item.cchTextMax = text.GetLength();

	GetParent()->GetParent()->SendMessage(WM_NOTIFY, GetParent()->GetDlgCtrlID(), (LPARAM) &dispinfo);
}

BOOL CListEdit::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN 
			|| pMsg->wParam == VK_DELETE 
			|| pMsg->wParam == VK_ESCAPE
			|| pMsg->wParam == VK_TAB
			|| pMsg->wParam == VK_UP
			|| pMsg->wParam == VK_DOWN
			|| GetKeyState(VK_CONTROL))
		{
			::TranslateMessage (pMsg);
			::DispatchMessage (pMsg);
			return TRUE;
		}
	}
	
	return CEdit::PreTranslateMessage(pMsg);
}

void CListEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	DestroyWindow();	
}

void CListEdit::OnNcDestroy() 
{
	SetListText();
	CEdit::OnNcDestroy();
	m_pListCtrl->SetEditState(false);
	m_pListCtrl->DeleteEdit();
}

void CListEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CListEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CListEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch (nChar)
	{
	case VK_ESCAPE :
		if (nChar == VK_ESCAPE)
			m_bEscape = TRUE;
		GetParent()->SetFocus();
		return;
	case VK_RETURN :
		m_bEscape = FALSE;
		SetListText();
		GetParent()->SetFocus();
		return;
	}

	CString text;
	GetWindowText(text);	

	bool	isHan = false;
	int	sel = -1;
	const	char*	test = (LPSTR)text.GetString();
	CString tmpS = _T("");

	for (int jj = 0; jj < text.GetLength(); jj++)
	{
		if (test[jj] & 0x80)
		{
			isHan = true;
			if (sel == -1)
				sel = jj;
		}
		else
		{
			tmpS += text.GetAt(jj);
		}
	}

	if ((m_SubItem == 3) && ((nChar >= '0' && nChar <= '9') || nChar == VK_BACK))
		;
	else if ((m_SubItem == 2)
		&& ((nChar >= '0' && nChar <= '9') || nChar == VK_BACK || nChar == '-' || nChar == '.'))
		;
	else if (m_SubItem == 1
		&& ((nChar >= '0' && nChar <= '9')
		|| (nChar >= 'a' && nChar <= 'z') || (nChar >= 'A' && nChar <= 'Z')
		|| nChar == VK_BACK))
		;
	else if (isHan)
	{
		text = tmpS;
		SetWindowText(text);
		SetSel(sel, sel);
		return;
	}
	else
	{
		return;	
	}

	CEdit::OnChar (nChar, nRepCnt, nFlags);

	CWindowDC DC(this);
	CFont	*pFont = GetParent()->GetFont();
	CFont	*pFontDC = DC.SelectObject(pFont);
	CSize Size = DC.GetTextExtent(text);

	DC.SelectObject(pFontDC);
	Size.cx += 5;

	CRect	rect, pRect;
	GetClientRect(&rect);
	GetParent()->GetClientRect(&pRect);

	ClientToScreen(&rect);
	GetParent()->ScreenToClient(&rect);

	if (Size.cx > rect.Width())
	{
		if (Size.cx + rect.left < pRect.right)
			rect.right = rect.left + Size.cx;
		else
			rect.right = pRect.right;
		MoveWindow(&rect);
	}
}

int CListEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CFont* Font = GetParent()->GetFont();
	SetFont (Font);

	SetWindowText(m_InitText);
	SetFocus();
	SetSel(0, -1);
	return 0;	
}

UINT CListEdit::OnGetDlgCode() 
{
	return CEdit::OnGetDlgCode() | DLGC_WANTARROWS | DLGC_WANTTAB;	
}



// itemCombo.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "itemCombo.h"


// CItemCombo

IMPLEMENT_DYNAMIC(CItemCombo, CComboBox)

CItemCombo::CItemCombo(int item, int subItem, CStringList* psList)
{
	m_item = item;
	m_subItem = subItem;
	m_strList.RemoveAll();

	if (psList)
		m_strList.AddTail(psList);

	m_escape = FALSE;
}

CItemCombo::~CItemCombo()
{
}


BEGIN_MESSAGE_MAP(CItemCombo, CComboBox)
	ON_WM_CREATE()
	ON_WM_CHAR()
	ON_WM_KILLFOCUS()
	ON_WM_NCDESTROY()
//	ON_WM_SIZE()
	ON_CONTROL_REFLECT(CBN_CLOSEUP, &CItemCombo::OnCbnCloseup)
	ON_CONTROL_REFLECT(CBN_DROPDOWN, &CItemCombo::OnCbnDropdown)
END_MESSAGE_MAP()


// CItemCombo 메시지 처리기입니다.

BOOL CItemCombo::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{		
		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)	
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);			
			return 1;
		}	
	}
	return CComboBox::PreTranslateMessage(pMsg);
}


int CItemCombo::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	CFont* font = GetParent()->GetFont();	
	SetFont(font);
	for (POSITION pos = m_strList.GetHeadPosition(); pos; )
		AddString((LPCTSTR)(m_strList.GetNext(pos)));	
	SetFocus();	
	return 0;
}


void CItemCombo::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_ESCAPE:
		m_escape = TRUE;
	case VK_RETURN:
		GetParent()->SetFocus();		
		return;	
	}	

	CComboBox::OnChar(nChar, nRepCnt, nFlags);
}


void CItemCombo::OnKillFocus(CWnd* pNewWnd)
{
	CComboBox::OnKillFocus(pNewWnd);

	int	nIndex = GetCurSel();

	// Send Notification to parent of ListView ctrl	
	if (nIndex != CB_ERR)
	{
		CString	str;	
		LV_DISPINFO lvDispinfo;

		GetWindowText(str);
		lvDispinfo.hdr.hwndFrom  = GetParent()->m_hWnd;
		lvDispinfo.hdr.idFrom    = GetDlgCtrlID();
		lvDispinfo.hdr.code      = LVN_ENDLABELEDIT;
		lvDispinfo.item.mask     = LVIF_TEXT | LVIF_PARAM;	
		lvDispinfo.item.iItem    = m_item;
		lvDispinfo.item.iSubItem = m_subItem;
		lvDispinfo.item.pszText  = m_escape ? NULL : LPTSTR((LPCTSTR)str);
		lvDispinfo.item.cchTextMax = str.GetLength();
		lvDispinfo.item.lParam     = GetItemData(GetCurSel());
		GetParent()->GetParent()->SendMessage(WM_NOTIFY, GetParent()->GetDlgCtrlID(), (LPARAM)&lvDispinfo);
	}
	PostMessage(WM_CLOSE);
}


void CItemCombo::OnNcDestroy()
{
	CComboBox::OnNcDestroy();
	delete this;	
}


void CItemCombo::OnCbnCloseup()
{
	GetParent()->SetFocus();	
}


void CItemCombo::OnCbnDropdown()
{
	CSize   sz;
	CString	str;
	int     dx = 0;
	CDC*    pDC = GetDC();

	for (int ii = 0; ii < GetCount(); ii++)
	{
		GetLBText(ii, str);
		sz = pDC->GetTextExtent(str);
		if (sz.cx > dx)
			dx = sz.cx;
	}
	ReleaseDC(pDC);
	SetDroppedWidth(dx);
}

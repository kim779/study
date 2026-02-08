// ListEdit.cpp : implementation file
//

#include "stdafx.h"
#include "IB200300.h"
#include "ListEdit.h"
#include "SListCtrl1.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListEdit

CListEdit::CListEdit()
{
}
CListEdit::CListEdit(CSListCtrl1* pList, int iItem, int iSubItem, CString sInitText)
{
	m_pListCtrl = pList;
	m_Item = iItem;
	m_SubItem = iSubItem;
	m_InitText = sInitText;
	int nReal =	0;
	nReal = ConvertToInt( m_InitText);
	m_bEscape = FALSE;
	char temp[108]= {0};
	itoa( nReal, temp,10);
	m_szNumber = temp;
	m_szNumber.TrimRight();
//	m_szNumber.Remove(0);
	
	m_nMaxDigit = 9;
	if( m_szNumber!= "0")
		m_nDigit =  m_szNumber.GetLength();
	else
		m_nDigit = 0;
	
	m_RealVal = nReal;
	m_nDigit = 0;
}


CListEdit::~CListEdit()
{
}


BEGIN_MESSAGE_MAP(CListEdit, CEdit)
	//{{AFX_MSG_MAP(CListEdit)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_WM_KILLFOCUS()
	ON_WM_NCDESTROY()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_CHAR()
	ON_WM_CREATE()
	ON_WM_GETDLGCODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListEdit message handlers

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

	if( m_SubItem == 11 && text.Right(1) != "%")
		text += "%";
	
	m_pListCtrl->SetItemText( m_Item,m_SubItem, text);
//	GetParent()->GetParent()->SendMessage(WM_NOTIFY, GetParent()->GetDlgCtrlID(), (LPARAM) &dispinfo);
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
	GetParent()->GetParent()->SendMessage(WM_DO_CALC,m_Item,m_SubItem);

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
	const BOOL Shift = GetKeyState(VK_SHIFT) < 0;
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

	bool isHan = false;
	int sel = -1;
	char* test = (LPSTR)text.operator LPCTSTR();
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

	if( (m_SubItem == 0)&&
		((nChar >= '0' && nChar <= '9') || nChar == VK_BACK|| nChar == VK_DELETE) )
	{
		CEdit::OnChar (nChar, nRepCnt, nFlags);
		
		CString str1;
		GetWindowText(str1);
		if( str1.GetLength() == 6 )
		{
			GetParent()->SetFocus();
			return;
		}
		
	}
	else if( ( (m_SubItem == 6)||(m_SubItem == 7 ) )
		&& ((nChar >= '0' && nChar <= '9') || nChar == VK_BACK|| nChar == VK_DELETE)){
		CEdit::OnChar (nChar, nRepCnt, nFlags);


		CString str1,str2;
		GetWindowText(str1);
		const int nReal = ConvertToInt( str1);
		char temp[108]={0};

		itoa( nReal, temp , 10);
		str1 = temp;
		str1.TrimRight();
		str2 = str1;
		for(int i = 0 ; i < (str2.GetLength()-1) / 3 ; i++){
			const int nIdx = str1.GetLength();
			str1.Insert( nIdx - ((i+1)*3+i) ,"," );
		}
		SetWindowText("");
		ReplaceSel(str1 );	
	}
	
	else if ((m_SubItem == 11)
		&& (
		(nChar >= '0' && nChar <= '9') 
		|| nChar == VK_BACK || nChar == '.' ||nChar == VK_DELETE)){
			CEdit::OnChar (nChar, nRepCnt, nFlags);
		}
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

	CWindowDC DC(this);

	CFont *pFont = GetParent()->GetFont();
	CFont *pFontDC = DC.SelectObject(pFont);
	CSize Size = DC.GetTextExtent(text);
	DC.SelectObject(pFontDC);
	Size.cx += 5;

	CRect rect, pRect;
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

void CListEdit::Clear()
{
	m_szNumber="0";
	m_nDigit = 0;
	m_RealVal = 0;
	SetNumText(m_szNumber);
}

void CListEdit::SetNumText(CString str){


	CString output ="";
	for( int i = 0 ; i < str.GetLength() ; i++ ){
		
		output+=str.GetAt( i );

		if( i != str.GetLength()-1 && m_nDigit > 3 && (m_nDigit-i)%3 == 1 ){
			output+=",";
		}

	}
	SetWindowText(output);

}
int CListEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CFont* Font = GetParent()->GetFont();
	SetFont (Font);

	SetWindowText(m_InitText);
	SetFocus();
	if( m_SubItem == 0)
		;//;SetSel(5, 5);
	else
		SetSel(0, -1);
	return 0;	
}

UINT CListEdit::OnGetDlgCode() 
{
	return CEdit::OnGetDlgCode() | DLGC_WANTARROWS | DLGC_WANTTAB;	
}



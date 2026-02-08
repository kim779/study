// TextEdit.cpp : implementation file
//

#include "stdafx.h"
#include "symbol.h"
#include "TextEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "SymbolTool.h"
#include "DrawingSymbol.h"

#include "Conversion.h"
/////////////////////////////////////////////////////////////////////////////
// CTextEdit

CTextEdit::CTextEdit(const COLORREF &color):
	m_pTextTool(NULL),
	m_nEnter(1),
	m_oneTextSize(-1,-1),
	m_strInputText("")
{
	m_Color = color;
}

CTextEdit::~CTextEdit()
{

}


BEGIN_MESSAGE_MAP(CTextEdit, CEdit)
	//{{AFX_MSG_MAP(CTextEdit)
	ON_WM_KILLFOCUS()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_NCDESTROY()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextEdit message handlers
void CTextEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
 
	m_pTextTool->SetEditText(m_strInputText);	
	m_strInputText.Empty();
	DestroyWindow();	
}

void CTextEdit::SetTextToolObject(CTextTool *pTextTool)
{
	m_pTextTool = pTextTool;
}

void CTextEdit::SetDrawingRegion(const CRect& drawingRegion)
{
	m_DrawingRegion = drawingRegion;
}

void CTextEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar == VK_RETURN)	{
		m_nEnter++;

		CRect rect = GetRect();	
		if(m_DrawingRegion.bottom <= rect.bottom + m_oneTextSize.cy)
			return;

		SetWindowPos(NULL, m_pTextTool->GetStartPoint().x, m_pTextTool->GetStartPoint().y, rect.Width(), m_oneTextSize.cy * m_nEnter + 4, SWP_NOMOVE|SWP_NOZORDER|SWP_SHOWWINDOW);
		return;
	}

	if(nChar == VK_ESCAPE)	
	{
		m_pTextTool->SetEditText("");
		if(m_pTextTool->Delete())
			DestroyWindow();
		return;
	}
/*
	if (GetKeyState(VK_CONTROL) & 0x8000)
	{
		if (nChar == 'V' || nChar == 'v')
			Paste();
		else if (nChar == 'C' || nChar == 'c')
			Copy();
	}
*/
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

CString CTextEdit::GetExtremeLongLengthofText(const CString &text)
{
	CString total = text;
	CString strEach = "";
	CString strLongWidth = "";
	int nCount = CDataConversion::GetStringDataCount(total, "\r\n");
	for(int i = 0 ; i< nCount  ; i++) {
		strEach = CDataConversion::GetStringData(total, "\r\n");
		if(strEach.GetLength() > strLongWidth.GetLength())
			strLongWidth = strEach;
	}
	if(total.GetLength() > strLongWidth.GetLength())
		strLongWidth = total;

	return strLongWidth;	
}

CSize CTextEdit::GetTextSize(const CString& text)
{
	//해당 font의 크기를 얻지 못하니까..logfont를 적용시킴.
	CClientDC dc(m_pTextTool->GetParent());
	CFont font;
	font.CreateFontIndirect(&m_pTextTool->GetLogFont());
	CFont* oldFont = dc.SelectObject(&font);

	CSize size(dc.GetTextExtent(text));

	dc.SelectObject(oldFont);
	font.DeleteObject();
	return size;
/*
	CString text;
	GetWindowText(text);
	int nCount = GetExtremeLongWidthofText(text).GetLength();

	return CSize(onetextsize.cx * nCount, onetextsize.cy);
*/
}
HBRUSH CTextEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	//sy 2005.04.06
	CBrush brush;
	brush.CreateSolidBrush(RGB(0, 0, 0));//m_Color);
	pDC->SetTextColor(RGB(0, 0, 0));//m_Color);
	return brush;
}

void CTextEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CEdit::OnChar(nChar, nRepCnt, nFlags);

	GetWindowText(m_strInputText);

	ResetWindowPos();
}

void CTextEdit::OnNcDestroy() 
{
	CEdit::OnNcDestroy();
	delete this;	
}

CRect CTextEdit::GetRect()
{
	CRect rect;	
	GetClientRect(&rect);
	// Transform rect to parent coordinates	
	ClientToScreen(&rect);
	GetParent()->ScreenToClient(&rect);
	return rect;
}

LRESULT CTextEdit::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_PASTE)
	{
		if (::OpenClipboard(GetSafeHwnd()))
		{
			LPTSTR lptstrPaste = NULL;
			HGLOBAL hglbPaste = ::GetClipboardData(CF_TEXT); 
			if (hglbPaste != NULL)
			{ 
				lptstrPaste = (TCHAR*)::GlobalLock(hglbPaste); 
				if (lptstrPaste != NULL)
					::GlobalUnlock(hglbPaste); 
			}
			::CloseClipboard();

			CString strText = CString(lptstrPaste);
			strText.Replace("\t", " ");

			CString strBefore;
			GetWindowText(strBefore);
			if (strBefore.GetLength() > 0)
			{
				CPoint pos = GetCaretPos();
				int n = CharFromPos(pos);
				int nCharIndex = LOWORD(n);

				strText = strBefore.Left(nCharIndex) + strText + strBefore.Mid(nCharIndex+1);
			}

			m_nEnter = CDataConversion::GetStringDataCount(strText, "\n") + 1;
			ResetWindowPos(strText, TRUE);
			SetWindowText(strText);

			return 0;
		}
	}

	LRESULT lResult = CWnd::WindowProc(message, wParam, lParam);

	return lResult;
}

void CTextEdit::ResetWindowPos(CString strText, BOOL bReset) 
{
	if (strText == "")
		strText = m_strInputText;
	else
		m_strInputText = strText;

	CSize size = GetTextSize(GetExtremeLongLengthofText(strText));

	CRect rect = GetRect();	
	if (m_oneTextSize.cx < 0 && m_oneTextSize.cy <0)
		m_oneTextSize = GetTextSize("글");

	if (!bReset)
	{
		if (m_DrawingRegion.right <= (rect.right + (m_oneTextSize.cx * 2)) || m_DrawingRegion.bottom <= rect.bottom) //(rect.bottom + size.cy)) 
			return;
	}

	if (size.cx + m_oneTextSize.cx + 5 > rect.Width())
		SetWindowPos(NULL, m_pTextTool->GetStartPoint().x, m_pTextTool->GetStartPoint().y, size.cx + m_oneTextSize.cx + 5, size.cy * m_nEnter + 4, SWP_NOMOVE|SWP_NOZORDER|SWP_SHOWWINDOW);
}


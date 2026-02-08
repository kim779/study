// InPlaceEdit.cpp : implementation file
//

#include "stdafx.h"
#include "InPlaceEdit.h"
#include "griditem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInPlaceEdit

CInPlaceEdit::CInPlaceEdit(CWnd* parent, CRect& rect, DWORD dwStyle, UINT nID, int nRow, int nColumn,
					CString sInitText, int limit, UINT nFirstChar, LOGFONT* logfont)
{
	m_sInitText     = sInitText;
	m_nRow          = nRow;
	m_nColumn       = nColumn;
	m_nLastChar     = 0; 
	m_bExitOnArrows = (nFirstChar != VK_LBUTTON);    // If mouse click brought us here,
	m_Rect		= rect;

	const DWORD dwEditStyle = WS_CHILD|WS_VISIBLE|dwStyle;
	if (!CreateEx(NULL, _T("EDIT"), "", dwEditStyle, rect, parent, nID))
		return;

	LimitText(limit);

	if (logfont != nullptr)
	{
		m_font.CreatePointFontIndirect(logfont);
		SetFont(&m_font);
	}
	else
	{
		SetFont(parent->GetFont());
	}

	SetWindowText(sInitText);
	SetFocus();

	switch (nFirstChar)
	{
	case VK_LBUTTON: 
	case VK_RETURN:
		SetSel((int)_tcslen(m_sInitText), -1);
		return;
	case VK_BACK:
		SetSel((int)_tcslen(m_sInitText), -1);
		break;
	case VK_TAB:
	case VK_DOWN: 
	case VK_UP:   
	case VK_RIGHT:
	case VK_LEFT:  
	case VK_NEXT:  
	case VK_PRIOR: 
	case VK_HOME:
	case VK_SPACE:
	case VK_END:
		SetSel(0, -1);
		return;
	default:
		SetSel(0, -1);
		break;
	}
    
	SendMessage(WM_CHAR, nFirstChar);
}

CInPlaceEdit::~CInPlaceEdit()
{
}

BEGIN_MESSAGE_MAP(CInPlaceEdit, CEdit)
	//{{AFX_MSG_MAP(CInPlaceEdit)
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_CHAR()
	ON_WM_GETDLGCODE()
	ON_WM_KEYUP()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_ACTIVATE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInPlaceEdit message handlers

void CInPlaceEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	LockEdit();
	if ((nChar == VK_PRIOR || nChar == VK_NEXT) && (m_bExitOnArrows || GetKeyState(VK_CONTROL) < 0))
	{
		m_nLastChar = nChar;
		GetParent()->SetFocus();
		return;
	}
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
	RedrawWindow();
	LockEdit();
}

void CInPlaceEdit::OnKillFocus(CWnd* pNewWnd) 
{
	m_bSetFocus = false;
	CEdit::OnKillFocus(pNewWnd);
	EndEdit();
}

void CInPlaceEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_TAB || nChar == VK_RETURN)
	{
		m_nLastChar = nChar;
		GetParent()->SetFocus();    // This will destroy this window
		return;
	}
	if (nChar == VK_ESCAPE) 
	{
		SetWindowText(m_sInitText);    // restore previous text
		m_nLastChar = nChar;
		GetParent()->SetFocus();
		return;
	}

	CEdit::OnChar(nChar, nRepCnt, nFlags);

	// Get text extent
	CString str; GetWindowText(str);
	str += _T("  ");
    
	CWindowDC dc(this);
	CFont *pFontDC = dc.SelectObject(GetFont());
	const CSize size = dc.GetTextExtent(str);
	dc.SelectObject(pFontDC);
       
	// Get client rect
	CRect parentRC;
	GetParent()->GetClientRect(&parentRC);
    
	// Check whether control needs to be resized
	// and whether there is space to grow
	if (size.cx > m_Rect.Width())
	{
		if (size.cx + m_Rect.left < parentRC.right)
			m_Rect.right = m_Rect.left + size.cx;
		else
			m_Rect.right = parentRC.right;
		MoveWindow(&m_Rect);
	}
}

UINT CInPlaceEdit::OnGetDlgCode() 
{
	return	DLGC_WANTALLKEYS;
}

////////////////////////////////////////////////////////////////////////////
// CInPlaceEdit overrides

BOOL CInPlaceEdit::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_SYSCHAR)
		return TRUE;

	return CEdit::PreTranslateMessage(pMsg);
}

void CInPlaceEdit::PostNcDestroy() 
{
	CEdit::PostNcDestroy();
	delete this;
}

////////////////////////////////////////////////////////////////////////////
// CInPlaceEdit implementation

void CInPlaceEdit::EndEdit()
{
	CString str; GetWindowText(str);

	// Send Notification to parent
	NM_USER nmuser;

	nmuser.hdr.hwndFrom = GetSafeHwnd();
	nmuser.hdr.idFrom   = GetDlgCtrlID();
	nmuser.hdr.code     = GVNM_ENDEDIT;

	nmuser.item.mask    = LVIF_TEXT|LVIF_PARAM;
	nmuser.item.row     = m_nRow;
	nmuser.item.col     = m_nColumn;
	nmuser.item.text    = str;
	nmuser.item.param  = (LPARAM) m_nLastChar; 
    
	const CWnd* pOwner = GetOwner();
	if (pOwner) pOwner->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&nmuser);
    
	PostMessage(WM_CLOSE, 0, 0);
}

void CInPlaceEdit::LockEdit()
{
	CRect rc;
	GetClientRect(&rc);
	ValidateRect(rc);
}

void CInPlaceEdit::UnlockEdit()
{
	CRect rc;
	GetClientRect(&rc);
	InvalidateRect(rc);
}

void CInPlaceEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	UnlockEdit();
	RedrawWindow();	
	CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CInPlaceEdit::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rc;
	CFont* pOldFont{};
	CString	sText = getText();
	GetClientRect(&rc);
	dc.IntersectClipRect(rc);
	rc.DeflateRect(LOWORD(GetMargins()), 0, HIWORD(GetMargins()), 0);

	if (IsWindowEnabled())
	{
		dc.FillSolidRect(&rc, GetSysColor(COLOR_WINDOW));
		dc.SetBkMode(TRANSPARENT);
		pOldFont = dc.SelectObject(GetFont());
		
		if (m_bSetFocus)
		{
			m_caretPos = GetCaretPos();
			m_caretPos.y = (rc.Height() - dc.GetTextExtent("A").cy) / 2;
			SetCaretPos(m_caretPos);
		}
	}
	else
	{
		dc.FillSolidRect(&rc, GetSysColor(COLOR_INACTIVEBORDER));
		dc.SetBkMode(TRANSPARENT);
		pOldFont = dc.SelectObject(GetFont());
	}

	rc.DeflateRect(0, 1, 0, 0);
	dc.DrawText(sText, rc, (GetStyle() & ES_CENTER?DT_CENTER:GetStyle()&ES_RIGHT?DT_RIGHT:DT_LEFT) | DT_VCENTER | DT_SINGLELINE);

	drawSelRect(&dc, rc, sText);
	dc.SelectObject(pOldFont);
	dc.SelectClipRgn(nullptr);
}

CString CInPlaceEdit::getText()
{
	CString sTmp;
	GetWindowText(sTmp);
	if (GetStyle() & ES_PASSWORD)
	{
		for (int ii = 0; ii < sTmp.GetLength(); ii++)
			sTmp.SetAt(ii, '*');
	}

	return sTmp;
}

void CInPlaceEdit::OnSetFocus(CWnd* pOldWnd) 
{
	m_bSetFocus = true;
	UnlockEdit();
	CEdit::OnSetFocus(pOldWnd);	
}

void CInPlaceEdit::drawSelRect(CDC *pDC, CRect rc, CString str)
{
	CString sTmp;
	
	GetSel(m_nStart, m_nEnd);
	
	if (GetStyle() & ES_CENTER)
	{
		sTmp = str.Left(m_nStart);
		rc.left = rc.left + (rc.Width() - pDC->GetTextExtent(str).cx) / 2 + pDC->GetTextExtent(sTmp).cx;
		sTmp = str.Mid(m_nStart, m_nEnd - m_nStart);
		rc.right = rc.left + pDC->GetTextExtent(sTmp).cx;
	}
	else if (GetStyle() & ES_RIGHT) 
	{
		sTmp = str.Right(str.GetLength() - m_nEnd);
		rc.right = rc.right - pDC->GetTextExtent(sTmp).cx;
		sTmp = str.Mid(m_nStart, m_nEnd - m_nStart);
		rc.left = rc.right - pDC->GetTextExtent(sTmp).cx;
	}
	else
	{
		sTmp = str.Left(m_nStart);
		rc.left = pDC->GetTextExtent(sTmp).cx;
		sTmp = str.Mid(m_nStart, m_nEnd - m_nStart);
		rc.right = rc.left + pDC->GetTextExtent(sTmp).cx;
	}

	rc.top = (rc.Height() - pDC->GetTextExtent("A").cy) / 2 + 1;
	rc.bottom = rc.top + pDC->GetTextExtent("A").cy;

	pDC->FillSolidRect(rc, GetSysColor(COLOR_HIGHLIGHT));
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
	pDC->DrawText(sTmp, rc, (GetStyle() & ES_CENTER?DT_CENTER:GetStyle()&ES_RIGHT?DT_RIGHT:DT_LEFT) | DT_VCENTER | DT_SINGLELINE);
}

void CInPlaceEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
	LockEdit();	
	CEdit::OnLButtonDown(nFlags, point);
	UnlockEdit();
	RedrawWindow();
}

void CInPlaceEdit::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	LockEdit();
	CEdit::OnActivate(nState, pWndOther, bMinimized);
	
	UnlockEdit();
	RedrawWindow();
	
}

void CInPlaceEdit::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (nFlags & MK_LBUTTON)
		LockEdit();	
	CEdit::OnMouseMove(nFlags, point);
	if (nFlags & MK_LBUTTON)
	{
		UnlockEdit();
		if (LOWORD(GetSel()) != m_nStart || HIWORD(GetSel()) != m_nEnd)
			RedrawWindow();
	}
}

void CInPlaceEdit::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	LockEdit();	
	CEdit::OnLButtonDblClk(nFlags, point);
	UnlockEdit();
	RedrawWindow();
}

LRESULT CInPlaceEdit::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_COPY || message == WM_PASTE || message == EM_SETSEL || message == WM_CLEAR)
		Invalidate();
	if (message == WM_SETFOCUS || message == WM_KILLFOCUS)
	{
		if (message == WM_KILLFOCUS)
		{
			m_bSetFocus = false;
			HideCaret();
			if ((HWND)wParam != GetParent()->m_hWnd)
				GetParent()->PostMessage(WM_USER + 1, VK_RETURN, 0);
		}
		else
		{
			m_bSetFocus = true;
			ShowCaret();
		}
		Invalidate();
	}	
	return CEdit::WindowProc(message, wParam, lParam);
}

void CInPlaceEdit::OnChange() 
{
	UnlockEdit();
}

// fxEdit.cpp : implementation file
//

#include "stdafx.h"
#include "fxEdit.h"
#include "misctype.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CfxEdit

CfxEdit::CfxEdit(CFont* pFont/*nullptr*/)
{
	m_pFont     = pFont;

	m_style     = esNORMAL;
	m_text      = _T("");
	m_alignment = alLEFT;
	m_clrBACK   = RGB(255, 255, 255);
	m_clrFORE   = RGB(  0,   0,   0);
	m_brBACK.CreateSolidBrush(m_clrBACK);

	m_attr      = 0;
	m_parent    = (CWnd *) 0;
}

CfxEdit::~CfxEdit()
{
	m_brBACK.DeleteObject();
}


BEGIN_MESSAGE_MAP(CfxEdit, CEdit)
	//{{AFX_MSG_MAP(CfxEdit)
	ON_WM_CTLCOLOR_REFLECT()
	ON_CONTROL_REFLECT(EN_KILLFOCUS, OnEditKillFocus)
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CfxEdit message handlers

BOOL CfxEdit::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	BOOL rc{};
	m_parent = pParentWnd;
	m_nID    = nID;
	
	rc = CreateEx(WS_EX_CLIENTEDGE, _T("EDIT"), "", dwStyle, rect, pParentWnd, nID);
	if (dwStyle & ES_PASSWORD)
		this->SetPasswordChar('*'); 

	if (m_pFont != nullptr) CWnd::SetFont(m_pFont);

	return rc;
}

HBRUSH CfxEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	pDC->SetTextColor(m_clrFORE);
	pDC->SetBkColor(m_clrBACK);

	if (m_pFont != nullptr) pDC->SelectObject(m_pFont);
	else pDC->SelectStockObject(SYSTEM_FONT);

	return m_brBACK;
}

void CfxEdit::OnEditKillFocus() 
{
	if (m_style == esPOPUP)
	{
		ShowWindow(SW_HIDE);
		m_parent->SendMessage(WM_USER+111, MAKEWPARAM(ENM_ENDEDIT, m_nID), 
			(LPARAM)(char *)GetText().operator LPCTSTR());
	}

}

void CfxEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
}

void CfxEdit::Init(DWORD style, DWORD attr, CString text, COLORREF clrBACK, COLORREF clrFORE, BYTE alignment)
{
	bool	redraw = false;

	m_attr  = attr;

	m_alignment = alignment;
	ModifyStyle(ES_LEFT|ES_CENTER|ES_RIGHT, 0);
	switch (alignment)
	{
	case alLEFT:
		ModifyStyle(0, ES_LEFT);	break;
	case alRIGHT:
		ModifyStyle(0, ES_RIGHT);	break;
	case alCENTER:
	default:
		ModifyStyle(0, ES_CENTER);	break;
	}
	if (m_style != style)
	{
		m_style = style;
		redraw  = true;
	}
	if (m_text.Compare(text))
	{
		m_text = text;
		SetWindowText(m_text);
		redraw = true;
	}
	if (m_clrFORE != clrFORE || m_clrBACK != clrBACK)
	{
		m_clrFORE = clrFORE;
		m_clrBACK = clrBACK;

		m_brBACK.DeleteObject(); m_brBACK.CreateSolidBrush(m_clrBACK);
		redraw = true;
	}
	
	if (redraw)
		Invalidate();
}

void CfxEdit::SetStyle(DWORD style)
{
	m_style = style;
}

void CfxEdit::SetText(CString text, bool redraw)
{
	if (m_attr & attrCOMMA)
		formatComma(text);

	m_text = text;
	SetWindowText(m_text);
	if (redraw) Invalidate();
}


CString CfxEdit::GetText()
{
	GetWindowText(m_text);
	if (m_attr & attrCOMMA)
		m_text.Remove(',');
	return m_text;
}

void CfxEdit::SetBkColor(COLORREF clrBACK, bool redraw)
{
	if (m_clrBACK == clrBACK)
		return;

	m_clrBACK = clrBACK;
	m_brBACK.DeleteObject();
	m_brBACK.CreateSolidBrush(m_clrBACK);

	if (redraw)
		Invalidate();
}

void CfxEdit::SetFgColor(COLORREF clrFORE, bool redraw)
{
	if (m_clrFORE == clrFORE)
		return;

	m_clrFORE = clrFORE;
	if (redraw)
		Invalidate();
}

void CfxEdit::SetFont(CFont* pFont, bool redraw)
{
	m_pFont = pFont;
	if (m_pFont != nullptr) CWnd::SetFont(m_pFont);
	if (redraw) Invalidate();
}

void CfxEdit::SetAlignment(BYTE alignment, bool redraw)
{
	m_alignment = alignment;
	ModifyStyle(ES_LEFT|ES_CENTER|ES_RIGHT, 0);
	switch (alignment)
	{
	case alCENTER:
		ModifyStyle(0, ES_CENTER);	break;
	case alRIGHT:
		ModifyStyle(0, ES_RIGHT);	break;
	case alLEFT:
	default:
		ModifyStyle(0, ES_LEFT);	break;
	}
}

void CfxEdit::ModifyAttribute(DWORD attrRM, DWORD attrADD)
{
	m_attr &= ~attrRM;
	m_attr |= attrADD;
}

void CfxEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	bool isHan = false, bDot = false, bMinus = false;
	CString tmpS = _T("");
	
	int jj = 0;
	int sel = -1;

	CString text;
	GetWindowText(text);
	const char* tmp = (LPSTR)text.operator LPCTSTR();

	switch (m_attr)
	{
	case attrCOMMA:
		for (jj = 0; jj < text.GetLength(); jj++)
		{
			if (tmp[jj] & 0x80)
			{
				if (sel == -1)
					sel = jj;
				isHan = true;
			}
			else if (tmp[jj] == '-')
			{
				sel = jj;
				bMinus = true;
			}
			else
			{
				tmpS += text.GetAt(jj);
			}
		}

		if ((nChar >= 0x30 && nChar <= 0x39) || nChar == VK_BACK)
			;
		else if (nChar == '-')
		{
			if (bMinus)
				return;
		}
		else if (isHan)
		{
			text = tmpS;
			SetWindowText(text);
			SetSel(sel, sel);
			return;
		}
		else
			return;	
		break;
	case attrFLOAT:
	case attrNUMERIC:
		for (jj = 0; jj < text.GetLength(); jj++)
		{
			if (tmp[jj] & 0x80)
			{
				if (sel == -1)
					sel = jj;
				isHan = true;
			}
			else if (tmp[jj] == '-')
			{
				sel = jj;
				bMinus = true;
			}
			else if (tmp[jj] == '.')
			{
				tmpS += text.GetAt(jj);
			 	bDot = true;
			}
			else
			{
				tmpS += text.GetAt(jj);
			}
		}

		if (bDot && nChar == '.')
			return;
		else if ((nChar >= 0x30 && nChar <= 0x39) 
			|| nChar == VK_BACK 
			|| (!bDot && nChar == '.'))
		;
		else if (nChar == '-')
		{
			if (bMinus)	return;
		}
		else if (isHan)
		{
			text = tmpS;
			SetWindowText(text);
			SetSel(sel, sel);
			return;
		}
		else
			return;	
		break;
	}

	CEdit::OnChar(nChar, nRepCnt, nFlags);
	CString realS;
	GetWindowText(realS);
	
	const int pos = realS.Find('-');
	if (pos > 0)
	{
		realS.Remove('-');
		SetWindowText(realS);
		SetSel(realS.GetLength(), realS.GetLength());
	}
		
	if (m_attr & attrCOMMA)
		commaText();

	const int nLimit = GetLimitText();

	if ((nLimit > 0 && realS.GetLength() >= nLimit))
	{
		NMHDR hdr;
		hdr.code = ENM_LIMITTEXT;
		hdr.hwndFrom = GetSafeHwnd();
		hdr.idFrom = GetDlgCtrlID();
		if (GetOwner())
			GetOwner()->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&hdr);
	}
}

void CfxEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
	if (m_style == esPOPUP && (nChar == VK_RETURN || nChar == VK_ESCAPE))
	{
		switch (nChar)
		{
		case VK_RETURN:
			ShowWindow(SW_HIDE);
			m_parent->SendMessage(WM_USER+111, MAKEWPARAM(ENM_ENDEDIT, m_nID), (LPARAM)(char *)GetText().operator LPCTSTR());
			break;

		case VK_ESCAPE:
			ShowWindow(SW_HIDE);

		default:
			break;
		}
	}
	else
	{
		if (nChar == VK_RETURN)
		{
			NMHDR hdr;
			hdr.code = ENM_VKRETURN;
			hdr.hwndFrom = GetSafeHwnd();
			hdr.idFrom = GetDlgCtrlID();
			if (GetOwner())
				GetOwner()->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&hdr);
			return;
		}
	}
	if (m_attr & attrCOMMA)
	{
		switch (nChar)
		{
		case VK_DELETE:
			commaText();
			break;
		}
	}
	
}

void CfxEdit::commaText()
{
	CString tmpS = GetText();
	SetText(tmpS);
	GetWindowText(tmpS);
	SetSel(tmpS.GetLength(), tmpS.GetLength());
}

void CfxEdit::formatComma(CString& text)
{
	CString tmps;
	if (m_attr & attrCOMMA && !text.IsEmpty())
	{
		bool bSign = false;
		CString tmpS = _T("");
		const int pos = text.Find('-');
		if (pos >= 0)
		{
			bSign = true;
			text.Remove('-');
		}
		text.Remove(',');
		text.MakeReverse();
		for (int ii = 0; ii < text.GetLength(); ii++)
		{
			if (ii && !(ii % 3))
				tmpS += ',';
			tmpS += text[ii];
		}
		tmpS.MakeReverse();
		text = tmpS;
		if (bSign)
			text = "-" + text;
	}
}

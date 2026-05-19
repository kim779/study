// memo.cpp : implementation file
//

#include "stdafx.h"
#include "memo.h"
#include "fmedit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Cmemo

Cmemo::Cmemo(CWnd* view, CfmEdit* edit)
{
	m_view     = view;
	m_edit     = edit;
	m_font     = (CFont *) NULL;
	m_readOnly = false;

	m_pRGB = PALETTERGB(255, 255, 255);
	m_tRGB = PALETTERGB(  0,   0,   0);
	m_brBACK.CreateSolidBrush(m_pRGB);
}

Cmemo::~Cmemo()
{
	if (m_font)
	{
		m_font->DeleteObject();
		delete m_font;
		m_font = (CFont *) NULL;
	}
	m_brBACK.DeleteObject();
}


BEGIN_MESSAGE_MAP(Cmemo, CEdit)
	//{{AFX_MSG_MAP(Cmemo)
	ON_WM_CHAR()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Cmemo message handlers

void Cmemo::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_TAB)
	{
		m_view->SetFocus();
		m_view->PostMessage(WM_KEYDOWN, (WPARAM)VK_TAB, 0);
		return;
	}

	m_edit->m_changed = true;
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

HBRUSH Cmemo::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	pDC->SetTextColor(m_tRGB);
	pDC->SetBkColor(m_pRGB);
	return m_brBACK;
}

void Cmemo::SetFont(const LOGFONT* logFont)
{
	if (!logFont)
		return;

	if (m_font)
	{
		m_font->DeleteObject();
		delete m_font;
	}

	m_font = new CFont();
	m_font->CreateFontIndirect(logFont);
	CEdit::SetFont(m_font);
}

void Cmemo::SetColor(COLORREF pRGB, COLORREF tRGB)
{
	if (m_pRGB == pRGB && m_tRGB == tRGB)
		return;

	m_tRGB = tRGB;
	m_pRGB = pRGB;
	m_brBACK.DeleteObject();
	m_brBACK.CreateSolidBrush(m_pRGB);
	Invalidate();
}

void Cmemo::SetReadOnly(bool readOnly)
{
	m_readOnly = readOnly;
	CEdit::SetReadOnly(m_readOnly ? TRUE : FALSE);
}


void Cmemo::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	if (pNewWnd && ::GetParent(::GetParent(pNewWnd->GetSafeHwnd())) == m_view->GetSafeHwnd())
		m_view->PostMessage(WM_KILLFOCUS, (WPARAM)pNewWnd->GetSafeHwnd());
}

void Cmemo::OnSetFocus(CWnd* pOldWnd) 
{
	bool	han = false;

	CEdit::OnSetFocus(pOldWnd);
	switch (m_edit->m_form->type)
	{
	case IO_HAN:
		han = true;
		break;
	case IO_MIXED:
	case IO_ANY:
		if (m_edit->m_form->attr & FA_HAN)
		{
			han = true;
			break;
		}
	default:
		break;
	}
	m_edit->DoListBox(han);
}

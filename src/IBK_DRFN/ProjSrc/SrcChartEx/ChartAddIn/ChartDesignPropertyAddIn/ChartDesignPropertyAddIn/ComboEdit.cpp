/* Standard Disclaimer: 
Copyright (C) 2000  Dennis Howard
This file is free software; you can redistribute it and/or
modify it without any conditions. There is no warranty,
implied or expressed, as to validity or fitness for a particular purpose.
*/

// ComboEdit.cpp : implementation file
//

#include "stdafx.h"
#include "ComboEdit.h"
#include "TreeCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CComboEdit

CComboEdit::CComboEdit()
{
}

CComboEdit::~CComboEdit()
{
}


BEGIN_MESSAGE_MAP(CComboEdit, CEdit)
	//{{AFX_MSG_MAP(CComboEdit)
	ON_WM_CHAR()
	ON_WM_CONTEXTMENU()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_MOUSEACTIVATE()
	ON_WM_ACTIVATE()
	ON_CONTROL_REFLECT(EN_SETFOCUS, OnSetfocus)
	ON_WM_MOUSEMOVE()
	ON_CONTROL_REFLECT(EN_KILLFOCUS, OnKillfocus)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
	ON_MESSAGE (WM_PASTE, OnIgnoreMessage )
	ON_MESSAGE (WM_CUT, OnIgnoreMessage)
	ON_MESSAGE (WM_CLEAR, OnIgnoreMessage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComboEdit message handlers

void CComboEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{

	UNREFERENCED_PARAMETER (nChar);
	UNREFERENCED_PARAMETER (nRepCnt);
	UNREFERENCED_PARAMETER (nFlags);

	//Customize this to do lookups, etc.
	return;	//make control read only
}

LRESULT CComboEdit::OnIgnoreMessage (WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER (wParam);
	UNREFERENCED_PARAMETER (lParam);
	// block cut, paste and clear messages
	return 0L;
}


void CComboEdit::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	UNREFERENCED_PARAMETER (pWnd);
	UNREFERENCED_PARAMETER (point);
	//no context menu	
}

void CComboEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
	
	UNREFERENCED_PARAMETER (point);
	UNREFERENCED_PARAMETER (nFlags);

//	CTreeCombo* pParent = static_cast<CTreeCombo*>(GetParent ());
//	_ASSERTE (pParent);
//	if (pParent)
//	{
//		//Drop down the tree control
//		CButton& DropDownButton = pParent->GetDropDownButton ();
//		pParent->PostMessage (WM_COMMAND, MAKEWPARAM(DropDownButton.GetDlgCtrlID(), BN_CLICKED), (LPARAM) DropDownButton.GetSafeHwnd ());
//	}
	
	
}

void CComboEdit::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	COLORREF TextColor;
	COLORREF BkColor;

	BOOL bDrawHighlight = FALSE;


	//Draw text matching the style of a droplist ComboBox
	CTreeCombo* pParent = static_cast<CTreeCombo*>(GetParent ());
	_ASSERTE (pParent);
	if (pParent && (pParent->IsEditHighlightOn () == TRUE))
	{
		bDrawHighlight = TRUE;
	}

	if (bDrawHighlight)
	{
		TextColor = GetSysColor (COLOR_HIGHLIGHTTEXT);
		BkColor = GetSysColor (COLOR_HIGHLIGHT);
	}
	else
	{
		TextColor = GetSysColor (COLOR_WINDOWTEXT);
		BkColor = GetSysColor (COLOR_WINDOW);
	}

	CString Text;
	GetWindowText (Text);

	CRect rect;
	GetClientRect (&rect);

	CBrush BkBrush (BkColor);

	dc.FillRect (&rect, &BkBrush);

	//dashed focus line if this or the dropdown button has the focus
	if (bDrawHighlight)
	{
		dc.DrawFocusRect (&rect);
	}

	int xBorder = GetSystemMetrics (SM_CXEDGE);
	rect.left += xBorder;

	CFont* pFont = GetFont ();
	CFont* pOldFont = dc.SelectObject (pFont);
	COLORREF oldTextColor = dc.SetTextColor (TextColor);
	COLORREF oldBkColor = dc.SetBkColor (BkColor);

	dc.DrawText (Text, &rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	dc.SelectObject (pOldFont);
	dc.SetTextColor (oldTextColor);
	dc.SetBkColor (oldBkColor);


}

int CComboEdit::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
{
	UNREFERENCED_PARAMETER (pDesktopWnd);
	UNREFERENCED_PARAMETER (nHitTest);
	UNREFERENCED_PARAMETER (message);


	return MA_ACTIVATE;   
}

void CComboEdit::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CEdit::OnActivate(nState, pWndOther, bMinimized);
	
	Invalidate ();	
}

void CComboEdit::OnSetfocus() 
{
	HideCaret ();  //don't show blinking caret
	Invalidate ();	
}

void CComboEdit::OnMouseMove(UINT nFlags, CPoint point) 
{
	UNREFERENCED_PARAMETER (nFlags);
	UNREFERENCED_PARAMETER (point);
	//disregard text selection using the mouse
}

void CComboEdit::OnKillfocus() 
{
	//restore caret
	ShowCaret ();
	Invalidate ();	
}

void CComboEdit::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	UNREFERENCED_PARAMETER (nFlags);
	UNREFERENCED_PARAMETER (point);
	//disregard text selection using a double click
}

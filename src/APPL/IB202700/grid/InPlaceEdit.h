//////////////////////////////////////////////////////////////////////
// InPlaceEdit.h : header file
//
// MFC Grid Control - inplace editing class
//
// Written by Chris Maunder <chris@codeproject.com>
// Copyright (c) 1998-2005. All Rights Reserved.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name and all copyright 
// notices remains intact. 
//
// An email letting me know how you are using it would be nice as well. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// For use with CGridCtrl v2.10+
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CInPlaceEdit : public CEdit
{
// Construction
public:
	CInPlaceEdit(CWnd* parent, CRect& rect, DWORD dwStyle, UINT nID,
		int nRow, int nColumn, CString sInitText, int limit, UINT nFirstChar, LOGFONT* logfont = nullptr);
// Attributes
public:
 
// Operations
public:
     void EndEdit();
 
// Overrides
     // ClassWizard generated virtual function overrides
     //{{AFX_VIRTUAL(CInPlaceEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PostNcDestroy();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
 
// Implementation
public:
	void drawSelRect(CDC *pDC, CRect rc, CString str);
	CString getText();
	void UnlockEdit();
	void LockEdit();
     virtual ~CInPlaceEdit();
 
// Generated message map functions
protected:
    //{{AFX_MSG(CInPlaceEdit)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnChange();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()

		// Attributes
private:
	int     m_nRow;
	int     m_nColumn;
	CString m_sInitText;
	UINT    m_nLastChar;
	BOOL    m_bExitOnArrows;
	CRect   m_Rect;
	
	CFont	m_font;
	
	bool	m_bSetFocus;
	CPoint	m_caretPos;
	int	m_nStart, m_nEnd;
	// Operations
};
 

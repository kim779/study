#pragma once
// InPlaceEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInPlaceEdit window

class CInPlaceEdit : public CEdit
{
// Construction
public:
	CInPlaceEdit(CWnd* parent, CRect& rect, DWORD dwStyle, UINT nID,
		 int nRow, int nColumn, CString sInitText, int limit, UINT nFirstChar, LOGFONT* logfont = NULL);
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
public:
	void	EndEdit();

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
};

/////////////////////////////////////////////////////////////////////////////


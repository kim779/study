#pragma once
// InPlaceCombo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInPlaceCombo window

class CInPlaceCombo : public CComboBox
{
// Construction
public:
	CInPlaceCombo(CWnd* parent, CRect& rect, DWORD dwStyle, UINT nID,
		 int nRow, int nColumn, CString sInitText, LOGFONT* logfont = nullptr, int active = 0);

// Attributes
private:
	int     m_nRow;
	int     m_nColumn;
	CString m_sInitText;
	UINT    m_nLastChar;
	BOOL    m_bExitOnArrows;
	CRect   m_rect;
	CFont	m_font;

// Operations
public:
	void	EndCombo();
private:
	void	MessageToParent(int nMessage);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInPlaceCombo)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CInPlaceCombo();

	// Generated message map functions
protected:
	//{{AFX_MSG(CInPlaceCombo)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSelchange();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

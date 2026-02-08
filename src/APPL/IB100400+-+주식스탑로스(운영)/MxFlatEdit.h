#pragma once
// MxFlatEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
class CMxEdit : public CEdit
{
// Construction
public:
	CMxEdit();
// Attributes
public:
// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMxEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMxEdit();
	// Generated message map functions
protected:
	//{{AFX_MSG(CMxEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
// CMxFlatEdit window

class CMxFlatEdit : public CWnd
{
// Construction
public:
	CMxFlatEdit();

// Attributes
public:
	COLORREF		m_linecolor{};
	CRect			m_ClientRect;
	std::unique_ptr<CMxEdit> m_pEidt{};
	CFont*			m_pFont{};
	int				m_iLimitLen{};
	UINT			m_nEditStyle{};

// Operations
public:
	BOOL			Create(UINT nID, CRect rect, CWnd* parent, CFont* pFont=NULL, UINT nEditStyle=0, COLORREF linecolor=0x8A8A8A );
	void			SetLimitText(int ilen);
	void			SetSelectFocus();
	CString			GetText();
	void			SetText(CString sText);
	void			SetBoxColor(COLORREF color){m_linecolor = color;};
	void			ChangeFont(CFont* pFont, int ifontpoint);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMxFlatEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMxFlatEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMxFlatEdit)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	LRESULT OnUser(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};


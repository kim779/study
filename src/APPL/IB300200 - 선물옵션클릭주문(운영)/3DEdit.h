#pragma once
// 3DEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// C3DEdit window

class C3DEdit : public CEdit
{
// Construction
public:
	C3DEdit();

public:
	BOOL	SetNumericFloat();
	BOOL	SetNumericPositiveFloat();
	BOOL	SetNumeric();

	void	SetFontFromParent(CFont* pfont);

protected:
	gsl::owner<CFont *>	m_pfont;
	BOOL		m_bCreateFont;
	
	BOOL		m_bSkipOnChange;
	UINT		m_uiChar;
	CString		m_strTextBefore;

	enum enumNumeric {	TEXT,				// 일반 Text
				NATURAL_NUMERIC,		// ES_NUMBER (자연수)
				FLOAT_NUMERIC,			// 999.99 (flaot)
				POSITIVE_FLOAT_NUMERIC,		// 양수 999.99 (flaot)
	} m_enumNumeric;

protected:
	int		Initialize();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(C3DEdit)
	public:
	virtual BOOL Create(DWORD dwExStyle, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CFont* pfont = NULL, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~C3DEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(C3DEdit)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnChange();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


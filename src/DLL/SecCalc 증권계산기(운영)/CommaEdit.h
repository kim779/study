#if !defined(AFX_COMMAEDIT_H__55F52A02_F1CD_485F_98ED_E04CBCA8525C__INCLUDED_)
#define AFX_COMMAEDIT_H__55F52A02_F1CD_485F_98ED_E04CBCA8525C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CCommaEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCommaEdit window

class CCommaEdit : public CEdit
{
// Construction
public:
	CCommaEdit();

public:
	BOOL	SetNumericFloat();
	BOOL	SetNumericPositiveFloat();
	BOOL	SetNumeric();

	void	SetFontFromParent(CFont* pfont);
	void	SetToParent(bool bToParent);

protected:
	bool		m_bToParent;
	CFont*		m_pfont;
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

	void		ToParent();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommaEdit)
	public:
	virtual BOOL Create(DWORD dwExStyle, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CFont* pfont = NULL, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCommaEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCommaEdit)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnChange();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMMAEDIT_H__55F52A02_F1CD_485F_98ED_E04CBCA8525C__INCLUDED_)

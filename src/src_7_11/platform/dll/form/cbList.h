#if !defined(AFX_CBLIST_H__ABC64AAA_8C4A_4498_8095_557D731E4851__INCLUDED_)
#define AFX_CBLIST_H__ABC64AAA_8C4A_4498_8095_557D731E4851__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// cbList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CcbList window

class CcbList : public CListBox
{
// Construction
public:
	CcbList(class CfmBase* form, CStringArray& items, CFont* font, int height, CRect oRc);

// Attributes
public:

protected:
	CfmBase*	m_form;
	CStringArray	m_items;
	CFont*		m_font;
	CRect		m_oRc;

	COLORREF	m_tRGB;
	COLORREF	m_pRGB;
	int		m_height;	// cell height

// Operations
public:
	void	MeasureItemX(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	void	DrawItemX(LPDRAWITEMSTRUCT lpDrawItemStruct);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CcbList)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CcbList();

	// Generated message map functions
protected:
	//{{AFX_MSG(CcbList)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CBLIST_H__ABC64AAA_8C4A_4498_8095_557D731E4851__INCLUDED_)

#if !defined(AFX_CBLIST_H__ABC64AAA_8C4A_4498_8095_557D731E4851__INCLUDED_)
#define AFX_CBLIST_H__ABC64AAA_8C4A_4498_8095_557D731E4851__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// cbList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CcbList window
typedef struct _LISTBOX_COLOR_
{
	CString strText;
	COLORREF fgColor;
	COLORREF bgColor;
	_LISTBOX_COLOR_()
	{
		strText.Empty();
		fgColor = RGB(0, 0, 0);
		bgColor = RGB(255, 255, 255);
	}
}LISTBOX_COLOR, *PLISTBOX_COLOR;


class CcbList : public CListBox
{
// Construction
public:
	CcbList(class CamBase* form, CStringArray& items, CFont* font);
	virtual ~CcbList();

// Attributes
public:

protected:
	CamBase*		m_form;
	CStringArray	m_items;
	CFont*			m_font;
	COLORREF		m_selbk;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CcbList)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetSelectColor(COLORREF col){ m_selbk = col; }
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
public:
	virtual void MeasureItem(LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CBLIST_H__ABC64AAA_8C4A_4498_8095_557D731E4851__INCLUDED_)

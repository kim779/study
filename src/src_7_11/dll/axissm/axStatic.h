#if !defined(AFX_AXSTATIC_H__3F31FC42_1B37_11D6_8AC9_00001CD7F9BE__INCLUDED_)
#define AFX_AXSTATIC_H__3F31FC42_1B37_11D6_8AC9_00001CD7F9BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// axStatic.h : header file
//

#include <afxtempl.h>
/////////////////////////////////////////////////////////////////////////////
// CAxStatic window

class CAxStatic : public CStatic
{
// Construction
public:
	CAxStatic();

// Attributes
public:
	CFont	m_font;

	COLORREF m_textColor;
	COLORREF m_redColor;
	COLORREF m_blueColor;
	COLORREF m_backColor;
	COLORREF m_flashColor;

	int	m_index;
	int	m_count;
	CString	m_text;
	CString	m_key;
	char	m_color;


// Operations
public:
	void	UpdateItem(CString str, char color = '=');
	void	ClearItem();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAxStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAxStatic)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXSTATIC_H__3F31FC42_1B37_11D6_8AC9_00001CD7F9BE__INCLUDED_)

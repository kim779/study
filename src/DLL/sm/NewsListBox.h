#if !defined(AFX_NEWSLISTBOX_H__3FD4A2F6_88D4_4FB8_89BE_75E86E6AC17B__INCLUDED_)
#define AFX_NEWSLISTBOX_H__3FD4A2F6_88D4_4FB8_89BE_75E86E6AC17B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewsListBox.h : header file
//
\
#include "usedefine.h"
/////////////////////////////////////////////////////////////////////////////
// NewsListBox window

typedef struct {
	CString date;
	CString source;
	CString title;
	CString item;
} NEWSITEM, *LPNEWSITEM;

class NewsListBox : public CListBox
{
// Construction
public:
	NewsListBox();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NewsListBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~NewsListBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(NewsListBox)
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	COLORREF m_backColor[2]{};
	COLORREF m_foreColor{};
	COLORREF m_gridColor{};
public:
	void DeleteAll();
	void Init();

	int AddItem(const char* date, const char* source, const char* title, const char* item);
	int AddEmptyItem();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWSLISTBOX_H__3FD4A2F6_88D4_4FB8_89BE_75E86E6AC17B__INCLUDED_)

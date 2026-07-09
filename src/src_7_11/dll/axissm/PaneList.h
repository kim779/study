#if !defined(AFX_PANELIST_H__1BEE8D5F_BB84_4634_B096_FFDCFE199DEB__INCLUDED_)
#define AFX_PANELIST_H__1BEE8D5F_BB84_4634_B096_FFDCFE199DEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PaneList.h : header file
//
#include <afxtempl.h>
#include "../ctrl/AxScrollBar.h"

#define		MAX_PANELIST	15
/////////////////////////////////////////////////////////////////////////////
// CPaneList window

class CPaneList : public CWnd
{
// Construction
public:
	CPaneList(class CPane* parent, int id, int kind, int count, COLORREF color);

protected:
	class CPane*	m_parent;
	CAxScrollBar	m_vbar;

	int		m_id;
	int		m_kind;
	int		m_count;

	COLORREF	m_bkColor;

	CStringArray*	m_arNews;
	CArray	< class _inter*, class _inter* > *m_arInter;

	CRect		m_pRc[MAX_PANELIST];

	void		DrawPaneList(CDC *pDC);
	void		DrawUpDown(CDC *pDC);
	void		DrawIndex(CDC *pDC);
	void		DrawInters(CDC *pDC);
	void		DrawNews(CDC *pDC);
	void		DrawConclusion(CDC *pDC);

	void		parseDat(CString dat, CMapStringToString& ary);
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPaneList)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPaneList();
	void	SetNews(CStringArray* ary);
	void	SetInters(CArray <class _inter*, class _inter*> *arInter);

	// Generated message map functions
protected:
	//{{AFX_MSG(CPaneList)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PANELIST_H__1BEE8D5F_BB84_4634_B096_FFDCFE199DEB__INCLUDED_)

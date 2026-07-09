#if !defined(AFX_XCOM_H__85D3E764_EEAA_4C60_8187_53176B3923B6__INCLUDED_)
#define AFX_XCOM_H__85D3E764_EEAA_4C60_8187_53176B3923B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Xcom.h : header file
//
#include "AxBitmap.h"

#define	XCOM_HEIGHT	19

#define	CTRL_SCREEN	1
#define	CTRL_CODE	2
/////////////////////////////////////////////////////////////////////////////
// CXcom window

class CXcom : public CWnd
{
// Construction
public:
	CXcom(UINT id);

// Attributes
protected:
	UINT		m_id;
	CRect		m_bRc;
	CPen		m_pen;
	CBrush		m_brush;
	BOOL		m_bDown;
	BOOL		m_bHover;
	CAxBitmap		m_arr;
	class CXedit*	m_edit;
// Operations
public:
	void	init(bool (*callback)(int, WPARAM, LPARAM), CString home);
	void	changepos();
	void	set_Home();
	void	set_Maps(CString mapname = _T(""));
	void	set_Code(CString code, CString home);
	void	set_List(CStringArray& array);
	void	add_History(CString dat);
	void	set_Owner();
	bool	IsInputFocus();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXcom)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CXcom();

	// Generated message map functions
protected:
	void	draw_Button(CDC* pDC);
	BOOL	ShowBmp(CDC* pDC, int x, int y, UINT nResID, int maskcolor);
	//{{AFX_MSG(CXcom)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg	LRESULT OnSelect(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XCOM_H__85D3E764_EEAA_4C60_8187_53176B3923B6__INCLUDED_)

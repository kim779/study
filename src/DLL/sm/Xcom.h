#if !defined(AFX_XCOM_H__85D3E764_EEAA_4C60_8187_53176B3923B6__INCLUDED_)
#define AFX_XCOM_H__85D3E764_EEAA_4C60_8187_53176B3923B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Xcom.h : header file
//

#define	XCOM_HEIGHT	18

#define	CTRL_SCREEN	1
#define	CTRL_CODE	2
/////////////////////////////////////////////////////////////////////////////
// CXcom window
#include "usedefine.h"
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
	CBitmap*		m_arr;
	CBitmap*		m_arr_dn;
	CBitmap*		m_bmpXCombBack;
	bool		m_push;
#ifdef DF_USESTL_XCOM
	std::unique_ptr<class CXedit> m_edit;
#else
	class CXedit* m_edit;
#endif
	
	CFont		m_font;
// Operations
public:
	void	init(bool (*callback)(int, WPARAM, LPARAM));
	void	changepos();
	void	set_Home();
	void	set_Maps(CString mapname = _T(""));
	void	set_Code(CString code);
	void	set_List(const CStringArray& array);
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
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	afx_msg	LONG OnSelect(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XCOM_H__85D3E764_EEAA_4C60_8187_53176B3923B6__INCLUDED_)

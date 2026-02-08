#if !defined(AFX_XEDIT_H__4CAF9A7F_BE48_4ACA_B944_F144F3C54469__INCLUDED_)
#define AFX_XEDIT_H__4CAF9A7F_BE48_4ACA_B944_F144F3C54469__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Xedit.h : header file
//
#define	L_SCRNO		4
#define	COLOR_XCOMBX		RGB(255, 255, 255)
#define	COLOR_BARTEXT		RGB(  0,   0,   0)
#define	COLOR_BARBACKGROUND	RGB(255, 255, 255)//RGB(238, 247, 255)

#define	WM_SELECT	WM_USER+963

#define	ID_TOOLTIP	1
/////////////////////////////////////////////////////////////////////////////
// CXedit window
#include "usedefine.h"
class CXedit : public CEdit
{
// Construction
public:
	CXedit();
	bool	(*m_axiscall)(int, WPARAM, LPARAM);

// Attributes
public:
	COLORREF	m_textColor;
	COLORREF	m_bkColor;
	CBrush		m_brush;
	bool		m_bSelect;
protected:
	UINT	m_len;
	CFont	m_font;
	CString	m_code;

	DWORD	m_Conversion, m_Sentence;

#ifdef DF_USESTL_XEDIT
	std::unique_ptr<class CScreenList> m_sdlg;
	std::unique_ptr<CToolTipCtrl> m_ToolTip;
#else
	class CScreenList* m_sdlg;
	CToolTipCtrl* m_ToolTip;
#endif

	CStringArray		m_list;
	int		m_ctrlKey;
	int		m_shiftKey;

	BOOL		m_bHover;
	BOOL		m_bTracking;
	

	

// Operations
public:
	void	set_Code(CString code);
	void	setlen(int len) { m_len = len; }
	void	setaxiscall(bool (*axiscall)(int, WPARAM, LPARAM));
	void	set_List(const CStringArray& array) { m_list.Copy(array); }
	int	get_ListSize();
	CString	get_mapN(int index);
	void	show_History();
	void	add_History(CString dat);
	void	set_ToolTipText(CString text, BOOL bActivate = TRUE);
	void	set_Owner();
protected:
	void	parse();
	void	show_List();
	bool	datCheck(int key);
	bool	checkHotkey(int key);
	BOOL	make_MapName(char* src, char* selc);
	void	InitToolTip();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXedit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	LRESULT OnMouseHover(WPARAM wparam, LPARAM lparam);
	LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	virtual ~CXedit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CXedit)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString getHotCode(int key);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XEDIT_H__4CAF9A7F_BE48_4ACA_B944_F144F3C54469__INCLUDED_)

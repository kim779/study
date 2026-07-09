#if !defined(AFX_XEDIT_H__4CAF9A7F_BE48_4ACA_B944_F144F3C54469__INCLUDED_)
#define AFX_XEDIT_H__4CAF9A7F_BE48_4ACA_B944_F144F3C54469__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Xedit.h : header file
//
#define	L_SCRNO		4
#define	COLOR_BARTEXT		RGB(  0,   0,   0)
#define	COLOR_BARBACKGROUND	RGB(255, 255, 255)//RGB(238, 247, 255)

#define	WM_SELECT	WM_USER+963
/////////////////////////////////////////////////////////////////////////////
// CXedit window

class CXedit : public CEdit
{
// Construction
public:
	CXedit(CString home);
	bool	(*m_axiscall)(int, WPARAM, LPARAM);

// Attributes
public:
	COLORREF	m_textColor;
	COLORREF	m_bkColor;
	CBrush		m_brush;
	bool		m_bSelect;

protected:
	UINT		m_len;
	CFont		m_font;
	CString		m_code;
	CString		m_home;
	CString		m_user;

	DWORD	m_Conversion, m_Sentence;
	class CScreenList*	m_sdlg;
	CStringArray		m_list;
	int		m_ctrlKey;
	int		m_shiftKey;

// Operations
public:
	void	set_Code(CString code, CString home);
	void	setlen(int len) { m_len = len; }
	void	setaxiscall(bool (*axiscall)(int, WPARAM, LPARAM));
	void	set_List(CStringArray& array) 
	{ 
		m_list.Copy(array);
	}
	int	get_ListSize();
	CString	get_mapN(int index);
	void	show_History();
	void	add_History(CString dat);
	void	set_Owner();

protected:
	void	parse();
	void	show_List();
	bool	datCheck(int key);
	bool	checkHotkey(int key);
	CString	getHotcode(int key);
	BOOL	make_MapName(char* src, char* selc);

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
	virtual ~CXedit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CXedit)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XEDIT_H__4CAF9A7F_BE48_4ACA_B944_F144F3C54469__INCLUDED_)

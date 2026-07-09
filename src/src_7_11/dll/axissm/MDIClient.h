#if !defined(AFX_MDICLIENT_H__C4DDEE95_168B_4012_A735_A1711686CF41__INCLUDED_)
#define AFX_MDICLIENT_H__C4DDEE95_168B_4012_A735_A1711686CF41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MDIClient.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMDIClient window
#define	COLOR_BKGROUND	GetSysColor(COLOR_APPWORKSPACE)
class AFX_EXT_CLASS CMDIClient : public CWnd
{
// Construction
public:
	CMDIClient(bool (*axiscall)(int, WPARAM, LPARAM), CString home, COLORREF color = COLOR_BKGROUND);
	virtual ~CMDIClient();

// Attributes
public:

protected:
	CString		m_home;
	CString		m_img;
	CString		m_userImg;
	COLORREF	m_bk;
	class CBb*	m_bb;

	BOOL		m_bUseImage;
	BOOL		m_bUseColor;
	int		m_pos;		// 0:°¡¿îµ¥, 1:¹ÙµÏÆÇ¾¿, 2:´ÃÀÌ±â

	int		m_ctrlKey;
	bool		m_shiftKey;
	bool	(*m_axiscall)(int, WPARAM, LPARAM);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMDIClient)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	void	draw_Img(CDC* pDC);
	void	draw_Button(CDC* pDC);
	void	change_Color(CString img, COLORREF color, CString user);
	void	make_board(COLORREF bk = GetSysColor(COLOR_APPWORKSPACE), bool visible = true);
	void	set_Key(int id, int key);
	int	get_Key(int id);
	CWnd*	get_Ctrl(int index);
	void	change_Board();
	void	Toggle();
	void	SetLastTime(CString time);

	BOOL	GetReload(int id);
	void	SetBZ();
	void	ShowBB();
	// Generated message map functions
protected:
	//{{AFX_MSG(CMDIClient)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MDICLIENT_H__C4DDEE95_168B_4012_A735_A1711686CF41__INCLUDED_)

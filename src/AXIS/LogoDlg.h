#if !defined(AFX_LOGODLG_H__2A150D63_9BF7_4A8B_85B8_3727305B2660__INCLUDED_)
#define AFX_LOGODLG_H__2A150D63_9BF7_4A8B_85B8_3727305B2660__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogoDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLogoDlg dialog

class CLogoDlg : public CDialog
{
// Construction
public:
	COLORREF ChangeSkinColor(int tp);
	COLORREF m_outline{};
	COLORREF m_outbg{};
	COLORREF m_selbg{};
	void draw_DataArea();
	CFont m_font;
	int m_activeItem{};
	int m_before_activeItem{};
	bool changeIndex{};
	CRect DataRect(int idx);
	int CalcDataRect(CPoint pt);
	int m_ListCount{};
	CStringArray slist;
	void parseAry(CString data, CStringArray& ary, CString separate);
	int LoadMenu();
	void Initialize(BOOL bSmall=FALSE);
	void MakeShape();
	void OnMainWndMoved();
	CLogoDlg(CWnd* pParent = NULL);   // standard constructor
	CBitmap* m_BG_top{};
	CBitmap* m_ico1{};
	CBitmap* m_ico2{};
	CWnd* m_parent{};
// Dialog Data
	//{{AFX_DATA(CLogoDlg)
	enum { IDD = IDD_LOGO_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogoDlg)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	COLORREF		m_clrItemBg{};
	COLORREF		m_clrItemSelect{};

	// Generated message map functions
	//{{AFX_MSG(CLogoDlg)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnMouseHover(WPARAM wparam, LPARAM lparam);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGODLG_H__2A150D63_9BF7_4A8B_85B8_3727305B2660__INCLUDED_)

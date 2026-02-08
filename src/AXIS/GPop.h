#if !defined(AFX_GPOP_H__E3ACB341_877C_475E_88B6_168776C0815B__INCLUDED_)
#define AFX_GPOP_H__E3ACB341_877C_475E_88B6_168776C0815B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GPop.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGPop frame

class CGPop : public CFrameWnd
{
//	DECLARE_DYNCREATE(CGPop)
public:
	CGPop(CWnd* pMain, bool stealth = true, bool depend = false);           // protected constructor used by dynamic creation
	virtual ~CGPop();

// Attributes
protected:
	int	m_width;
	int	m_height;
public:
	CSize GetSize();
	void  SetSize(CSize sz);
	void  SetSize(int width, int height, BOOL revise = TRUE);
public:
	CWnd*		m_pMain;
	CWnd*		m_viewparent;
	CView*		m_mdiView;

	int		m_cursor;
	int		m_key{};
	int		m_vwTYPE{};

	bool		m_bstealth;
	bool		m_active{};
	bool		m_closex;
	bool		m_depend;
	CString		m_mapN;
protected:
	CWnd		m_wndInvisible;
// Operations
public:
	CView*	AttachView(CCreateContext context);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGPop)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGPop)
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnClose();
	//}}AFX_MSG
	afx_msg LONG OnCHILDMSG(WPARAM wParam, LPARAM lParam);
	afx_msg	LONG OnAXIS(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GPOP_H__E3ACB341_877C_475E_88B6_168776C0815B__INCLUDED_)

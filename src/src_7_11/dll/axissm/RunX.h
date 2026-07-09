#if !defined(AFX_RUNX_H__89EED943_BB2A_4E68_8DE6_65B37B0B6712__INCLUDED_)
#define AFX_RUNX_H__89EED943_BB2A_4E68_8DE6_65B37B0B6712__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RunX.h : header file
//

#define	CTRL_PLAY	301
#define	CTRL_PAUSE	302
#define	CTRL_CONTINUE	303
#define	CTRL_STOP	304
#define	CTRL_PREV	305
#define	CTRL_NEXT	306

#define	ORDER_CODE	501
#define	ORDER_MAPN	502
#define KIND_MANUAL	503
#define KIND_AUTO	504
/////////////////////////////////////////////////////////////////////////////
// CRunX window

class CRunX : public CWnd
{
// Construction
public:
	CRunX(CWnd* parent, CString home, COLORREF bk);
	virtual ~CRunX();

// Attributes
protected:
	CWnd*	m_parent;
	int	m_interval;
	bool	m_code;
	bool	m_auto;
	CFont	m_font;
	CRect	m_rc[5];
	COLORREF	m_bk;
	CString		m_home;

	CButton	m_group1;
	CButton	m_group2;
	CEdit	m_edit;
	CSpinButtonCtrl m_spin;

	class CNButton*	m_play;
	class CNButton*	m_pause;
	class CNButton*	m_continue;
	class CNButton*	m_stop;
	class CNButton*	m_prev;
	class CNButton*	m_next;

// Operations
public:
	int	GetOrder();
	int	GetKind();
	void	SetKind();
	int	GetInterval();
	void	EnableButton(int nID, BOOL bEnable);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRunX)
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool	make_FB();
	void	draw_Form(CDC* pDC = NULL);
	void	show_Button();
	// Generated message map functions
protected:
	//{{AFX_MSG(CRunX)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg	LRESULT OnXMSG(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDeltaposSpincontrol(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RUNX_H__89EED943_BB2A_4E68_8DE6_65B37B0B6712__INCLUDED_)

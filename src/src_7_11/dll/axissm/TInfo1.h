#if !defined(AFX_TINFO1_H__6253E2D1_27BB_4928_A560_61A556CFFC55__INCLUDED_)
#define AFX_TINFO1_H__6253E2D1_27BB_4928_A560_61A556CFFC55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TInfo1.h : header file
//
#include <afxtempl.h>
/////////////////////////////////////////////////////////////////////////////
// CTInfo1 window

class AFX_EXT_CLASS CTInfo1 : public CDialogBar
{
// Construction
public:
	CTInfo1(bool (*callback)(int, WPARAM, LPARAM), CString home, CString userN);

// Attributes
protected:
	CString		m_home;
	CString		m_userN;
	int		m_height;

	CBitmap		m_bkIMG;
	CObList		m_arPane;

	class CNButton* m_hide;
	class CNButton*	m_deal;
	class CNButton*	m_screen;
	class CSimpleSplitter*	m_base;
	bool	(*m_axiscall)(int, WPARAM, LPARAM);

// Operations
public:
	void	drawBackground(CDC* pDC);
	void	DrawGripper(CDC* pDC);
	
	void	Config(int configN, COLORREF bkColor = RGB(255, 255, 255));
	void	ReConfig(int configN);
	void	ResizeBase();
	void	GetPaneSize(int configN, CArray <int, int> &ary);
	void	SetFmx(bool setting, int kind = 0, int key = 0, CString symbol = _T(""), CString dat = _T(""));
	void	SetInter(char* dat, bool bRotate = true);
	void	UpdatePaneData(int kind, CString dat);
	void	ProcessRTS(CString symbol, CString dat);
	CRect	GetGRect();
	void	createButton();
	void	ConclusionNotice(CString str);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTInfo1)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTInfo1();
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	// Generated message map functions
protected:
	//{{AFX_MSG(CTInfo1)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg	LRESULT OnXMSG(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TINFO1_H__6253E2D1_27BB_4928_A560_61A556CFFC55__INCLUDED_)

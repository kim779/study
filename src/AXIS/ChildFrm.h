// ChildFrm.h : interface of the CChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDFRM_H__E37F074E_A1EB_4AF5_A4C2_A7297C213DAA__INCLUDED_)
#define AFX_CHILDFRM_H__E37F074E_A1EB_4AF5_A4C2_A7297C213DAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define	POS_CHILDMOVE		5000

#include "XCaption.h"

class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// Attributes
private:
	bool	m_tView;
	int		m_fontsize;
protected:
	int	m_width;
	int	m_height;
public:
	CSize GetSize();
	void  SetSize(CSize sz);
	void  SetSize(int width, int height, BOOL revise = TRUE);
public:
	BOOL IsActive() { return m_xcaption.IsActive(); }		// cej 20070129
	bool IsIconic() { return m_bIconic; }
	CRect GetRestoreRC() { return m_restoreRc; }
	CXChildCaption		m_xcaption;

	int	m_key;
	int	m_vwTYPE;
	int	m_cursor;

	CString	m_mapN;
	CString m_usrData;

	bool	m_suspend;
	bool	m_bIconic;
	bool	m_bCaption;
	bool	m_bBackGround;
	bool	m_bSingle;
	bool	m_fixW;
	bool	m_fixH;
	BOOL	m_bActive;

	CPoint	m_iconpos;
	CPoint	m_sdiPt;			// sdi left top point
	CRect	m_restoreRc;			
	CRect	m_rectSDI;			// 단독실행시의 윈도우 위치 
	bool	(*m_axiscall)(int, WPARAM, LPARAM);

	void	SetTabView(bool isTabView);
	bool	IsTabView();

	CString m_slog{};
protected:
	CPoint	m_SP;
// Operations
public:
	CPoint	GetSP() { return m_SP; }
	void	SetMisc(UINT msg, CWnd* parent, bool (*axiscall)(int, WPARAM, LPARAM),
			 int fontsize, bool bCaption = true);
	void	HideButton(DWORD dwHide)	{ m_xcaption.HideButton(dwHide); }
	void	ChangeSkin(CString skinName);
	void	DrawFrame();
	void	actionCaption(int action, CPoint pt);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildFrame)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetMapName(bool (*axiscall)(int, WPARAM, LPARAM), CString sMapName);
	//void SetBkNotice();
	virtual ~CChildFrame();

	void	SetMinimize();
	void	SetRestore();
	void	SetMax();
	void	RedrawCaption() { m_xcaption.Invalidate(FALSE); }
	CString	GetTitle() { return m_xcaption.GetTitle(); }
	
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	bool	runCommand(int comm, LPARAM lParam);
	
	//{{AFX_MSG(CChildFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNcPaint();
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg LRESULT OnNcHitTest(CPoint point); //vc2019 UINT->LRESULT
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnClose();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	//}}AFX_MSG
	afx_msg void OnGroup(UINT uid);  //vc2019
	afx_msg	LONG OnAXIS(WPARAM wParam, LPARAM lParam);
	afx_msg	LONG OnCHILDMSG(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	MINMAXINFO	m_minMaxInfo;
	BOOL	m_revised;
public:
	void ShowBackGround(bool f_show);
	void	SetMinMaxInfo(MINMAXINFO* lpMinMaxInfo);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDFRM_H__E37F074E_A1EB_4AF5_A4C2_A7297C213DAA__INCLUDED_)

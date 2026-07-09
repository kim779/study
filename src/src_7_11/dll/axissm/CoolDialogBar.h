#if !defined(AFX_COOLDIALOGBAR_H__6EB5FA61_FFAD_11D1_98D4_444553540000__INCLUDED_)
#define AFX_COOLDIALOGBAR_H__6EB5FA61_FFAD_11D1_98D4_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CoolDialogBar.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CCoolDialogBar window

class AFX_EXT_CLASS CCoolDialogBar : public CControlBar
{
// Construction / destruction
public:
	CCoolDialogBar(CWnd* parent, bool (*callback)(int, WPARAM, LPARAM), CString home, class CTMenu* menu);

	const COLORREF	m_clrBtnHilight;
	const COLORREF	m_clrBtnShadow;
	
	COLORREF	m_color;
	
// Attributes
public:
	BOOL IsHorz() const;

// Operations
public:

// Overridables
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCoolDialogBar)
	public:
	virtual BOOL Create(CWnd* pParentWnd/*, CDialog *pDialog, CString &pTitle*/, UINT nID, DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_LEFT);
	virtual CSize CalcFixedLayout( BOOL bStretch, BOOL bHorz );
	virtual CSize CalcDynamicLayout( int nLength, DWORD dwMode );
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCoolDialogBar();
	void StartTracking();
	void StopTracking(BOOL bAccept);
	void OnInvertTracker(const CRect& rect);
	
// implementation helpers
	CPoint& ClientToWnd(CPoint& point);

	void	make_Ctrl();
	void	change_BandInfo();
	void	set_MenuInfo(class CTMenu* menu, CString user);
	void	reload_Code();
	CWnd*	GetCategoryWnd();
	CWnd*	GetMemoWnd();
	bool	GetFloat() { return m_bFloat; }

protected:
	bool (*m_axiscall)(int, WPARAM, LPARAM);
	void	DrawGripper(CDC &dc);
	void	ShowDialog();
	COLORREF	GetColor(int index);

	CWnd*		m_parent;
	CString		m_home;

	CSize		m_sizeMin;
	CSize		m_sizeHorz;
	CSize		m_sizeVert;
	CSize		m_sizeFloat;
	CRect		m_rectBorder;
	CRect		m_rectTracker;
	UINT		m_nDockBarID;
	CPoint		m_ptOld;
	BOOL		m_bTracking;
	BOOL		m_bInRecalcNC;
	int		m_cxEdge;
	
	CRect		m_rectUndock;
	CRect		m_rectClose;
	CRect		m_rectGripper;

	int		m_tab;
	int		m_cxGripper;
	int		m_cxBorder;
	
	bool		m_bFloat;

	CBrush		m_brushBkgd;
	CFont		m_font;

	class CTMenu*	m_menu;
	class CfxTab*	m_pTab;
	class CDbar0*		m_cDialog;	// 돌려보기
	class CMenuSearch*	m_mDialog;	// 메뉴찾기
	class CWnd*		m_sDialog;	// 메모/스케쥴
	
// Rob Wolpov 10/15/98 Added support for diagonal resizing
	int		m_cyBorder;
	int		m_cMinWidth;
	int		m_cMinHeight;
	int		m_cCaptionSize;
	
// Generated message map functions
protected:
	//{{AFX_MSG(CCoolDialogBar)
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnNcPaint();
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg	void OnTabChange(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COOLDIALOGBAR_H__6EB5FA61_FFAD_11D1_98D4_444553540000__INCLUDED_)

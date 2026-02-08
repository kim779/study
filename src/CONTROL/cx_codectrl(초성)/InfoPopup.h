
#pragma once
// InfoPopup.h : header file
//
#include "jinfo.h"
#include "editX.h"
#include "axwnd.h"
#include "NButton.h"

/////////////////////////////////////////////////////////////////////////////
// CInfoPopup window

class CInfoPopup : public CAxWnd
{
// Construction
public:
	CInfoPopup(CWnd *pWnd = NULL);

// Attributes
public:

protected:
	bool		m_fDone, m_bDrag;
	CPoint		m_firstPnt;
	CRect		m_rc;
	RECT		m_RECT;
	int		m_nSelected;
	std::unique_ptr<CEditX>	m_pEdit;
	Chjinfo*	m_hjinfo;
	Cejinfo*	m_ejinfo;
	CjHeader*	m_jHeader;
	
	CODE_KIND	m_CODEKIND;
	CString		m_sUserID, m_sKey, m_sRoot;

	bool		m_ujongsise;
	std::unique_ptr<CNButton> m_pCtlBtnClose;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInfoPopup)
	//}}AFX_VIRTUAL

// Implementation
public:
	CString LoadStringTb(UINT nIdx);
	CString getDiffSym(char chDiff);
	CString	SetComma(CString src);
	void	setPathInfo(CString sRoot, CString sID);
	void	loadMemo(CString sKey);
	void	saveMemo(CString sKey);
	void	drawData(CDC *pDC);
	void	drawCaption(CDC *pDC);
	void	drawIndicator(CDC* dc, CRect rect);
	void	drawIndicator1(CDC* dc, CRect rect, COLORREF clr);

	void	setJinfo(Chjinfo* hjinfo, Cejinfo* ejinfo, bool ujongsise);
	virtual ~CInfoPopup();

	// Generated message map functions
protected:
	//{{AFX_MSG(CInfoPopup)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
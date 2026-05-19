#pragma once

// ResultEditCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CResultEditCtrl window

class CResultEditCtrl : public CRichEditCtrl
{
// Construction
public:
	CResultEditCtrl();
	virtual ~CResultEditCtrl();

public:
	bool	m_bSNDRCV, m_bRTM, m_bMode;
	CString	m_sRtn;

	void Clear();

protected:
	CMenu*	m_pRMenu;
	HCURSOR	m_hcursor;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResultEditCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	LRESULT OnMessage(WPARAM wParam, LPARAM lParam);
	// Generated message map functions

protected:
	//{{AFX_MSG(CResultEditCtrl)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};


#pragma once
// TipDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTipDlg dialog
#include "resource.h"
class CTipDlg : public CDialog
{
// Construction
public:
	CTipDlg(CWnd* pView, CWnd* pWnd, bool bMap = false, CWnd* pParent = NULL);

// Attributes
public:
	CWnd*	m_pView;
	CWnd*	m_pWnd;
	CFont	m_font;
	CRect	m_rect;
	CRect	m_rect2;
	CString	m_tips;
	int	m_nHeight;
	std::unique_ptr<class CMapWnd>	m_pChild;
	bool	 m_bMap;
	
	CStringArray		m_arScreen;
	
// Operations
public:
	void	HideTips();
	void	ShowTips(CRect tRc, CRect wRc, CString tips);
	void	ShowTipsInItem(CRect tRc, CRect wRc, CString tips);
	void	ShowMap(CRect tRc, CRect wRc, CString mapname, CString option);
	CRect	FitSize(CRect rect, CPoint pt);
	CString GetTip() { return m_tips; }
// Dialog Data
	//{{AFX_DATA(CTipDlg)
	enum { IDD = IDD_TIP_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTipDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTipDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


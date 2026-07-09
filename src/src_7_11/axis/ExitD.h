#if !defined(AFX_EXITD_H__85031079_33CA_442C_BE78_899DCD325805__INCLUDED_)
#define AFX_EXITD_H__85031079_33CA_442C_BE78_899DCD325805__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExitD.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExitD dialog
#include "button.h"
class CExitD : public CDialog
{
// Construction
public:
	CExitD(CString home, CWnd* pParent = NULL);   // standard constructor
	~CExitD();

	void	SetTime(SYSTEMTIME time) { m_stime = time; };
// Dialog Data
	//{{AFX_DATA(CExitD)
	enum { IDD = IDD_DT_EXIT };
	CStatic	m_static;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExitD)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString	m_home;
	CFont	m_font;
	HBITMAP	m_bitmap;

	Cbutton	m_bnUSER;
	Cbutton	m_bnSTIME;
	Cbutton	m_bnETIME;
	Cbutton	m_bnTIME;
	Cbutton	m_bnOK;
	Cbutton	m_bnCANCEL;
	Cbutton	m_bnTEXT;

	SYSTEMTIME	m_stime;
	SYSTEMTIME	m_etime;

	SYSTEMTIME	GetUseTime();
	void		visibleButton();
	void		initButtons();

	HRGN DIBToRgn(HBITMAP hBmp,COLORREF BkColor,BOOL Direct);
	// Generated message map functions
	//{{AFX_MSG(CExitD)
	virtual BOOL OnInitDialog();
	afx_msg void OnOkx();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXITD_H__85031079_33CA_442C_BE78_899DCD325805__INCLUDED_)

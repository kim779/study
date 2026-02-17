#if !defined(AFX_BROWSER_H__CE7DD6E3_4A44_48F5_AD1B_CB354823D4EC__INCLUDED_)
#define AFX_BROWSER_H__CE7DD6E3_4A44_48F5_AD1B_CB354823D4EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// browser.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Cbrowser html view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include <afxhtml.h>

#define	TM_WAIT		100
#define	TMI_WAIT	50

class Cbrowser : public CHtmlView
{
public:
	Cbrowser();           // protected constructor used by dynamic creation
	virtual ~Cbrowser();

	DECLARE_DYNCREATE(Cbrowser)

// html Data
public:
	//{{AFX_DATA(Cbrowser)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	CString	m_url;

// Operations
public:
	void	xNavigate(CString url);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Cbrowser)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	navigate(CString url);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(Cbrowser)
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BROWSER_H__CE7DD6E3_4A44_48F5_AD1B_CB354823D4EC__INCLUDED_)

// DynamicHelpView.h : header file
//
#if !defined(__DYNAMICHELPVIEW_H__)
#define __DYNAMICHELPVIEW_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#if _MSC_VER >= 1200 // MFC 5.0
#include <afxhtml.h>
#else
#define CHtmlView CEditView
#endif

/////////////////////////////////////////////////////////////////////////////
// CDynamicHelpView html view

class CDynamicHelpView : public CHtmlView
{
protected:
	CDynamicHelpView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDynamicHelpView)

// html Data
public:
	//{{AFX_DATA(CDynamicHelpView)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	void	ShowHelp(LPCSTR lpszText);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDynamicHelpView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDynamicHelpView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CDynamicHelpView)
	afx_msg void OnNcPaint();
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	//}}AFX_MSG
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__DYNAMICHELPVIEW_H__)

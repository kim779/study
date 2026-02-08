#if !defined(AFX_NWEB_H__C5EE69AE_5A30_4F3F_8B66_CC52D40C97D3__INCLUDED_)
#define AFX_NWEB_H__C5EE69AE_5A30_4F3F_8B66_CC52D40C97D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NWeb.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNWeb html view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include <afxhtml.h>

class CNWeb : public CHtmlView
{
public:
	CNWeb();           // protected constructor used by dynamic creation
	virtual ~CNWeb();
//	DECLARE_DYNCREATE(CNWeb)

// html Data
public:
	//{{AFX_DATA(CNWeb)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNWeb)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CNWeb)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NWEB_H__C5EE69AE_5A30_4F3F_8B66_CC52D40C97D3__INCLUDED_)

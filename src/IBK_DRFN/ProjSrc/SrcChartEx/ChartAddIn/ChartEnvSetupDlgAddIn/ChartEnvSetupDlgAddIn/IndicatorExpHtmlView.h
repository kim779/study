// IndicatorExpHtmlView.h: interface for the CIndicatorExpHtmlView class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INDICATOREXPHTMLVIEW_H__A3DA74E7_20A6_4AC1_BA45_8FC8DF0B0CB6__INCLUDED_)
#define AFX_INDICATOREXPHTMLVIEW_H__A3DA74E7_20A6_4AC1_BA45_8FC8DF0B0CB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// CIndicatorExp html view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include <afxhtml.h>

class CIndicatorExpHtmlView : public CHtmlView  
{
protected:
	CIndicatorExpHtmlView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CIndicatorExpHtmlView)

// html Data
public:
	//{{AFX_DATA(CIndicatorExpHtmlView)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIndicatorExpHtmlView)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CIndicatorExpHtmlView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CIndicatorExpHtmlView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INDICATOREXPHTMLVIEW_H__A3DA74E7_20A6_4AC1_BA45_8FC8DF0B0CB6__INCLUDED_)

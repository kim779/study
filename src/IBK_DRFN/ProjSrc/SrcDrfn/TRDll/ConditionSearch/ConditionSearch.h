#if !defined(AFX_CONDITIONSEARCH_H__1C6584B4_AEAE_4C32_BC4A_BD016E60A6C7__INCLUDED_)
#define AFX_CONDITIONSEARCH_H__1C6584B4_AEAE_4C32_BC4A_BD016E60A6C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

// ConditionSearch.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConditionSearch window

class CConditionSearchApp : public CWinApp
{
// Construction
public:
	CConditionSearchApp();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConditionSearch)
public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CConditionSearchApp();

	// Generated message map functions
protected:
	//{{AFX_MSG(CConditionSearch)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONDITIONSEARCH_H__1C6584B4_AEAE_4C32_BC4A_BD016E60A6C7__INCLUDED_)

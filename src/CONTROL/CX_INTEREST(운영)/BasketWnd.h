#pragma once
// BasketWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBasketWnd window
#include "BaseWnd.h"
class CBasketWnd : public CBaseWnd
{
// Construction
public:
	CBasketWnd(CWnd* pMainWnd);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBasketWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBasketWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBasketWnd)

	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()

	void OperDraw(CDC* pDC);

};
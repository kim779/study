#pragma once
// HogaWnd.h : header file
//

#include "sharemsg.h"

/////////////////////////////////////////////////////////////////////////////
// CHogaWnd window

class CHogaWnd : public CWnd
{
// Construction
public:
	CHogaWnd(CWnd* parent, CWnd* mainWnd);

// Attributes
	CWnd*		m_pParent;
	CWnd*		m_pMainWnd;

	int			m_mapKey;
public:

// Operations
	CString		Variant(int comm, CString data = _T(""));


	COLORREF	GetColor(int color);
	LRESULT		SendAxisMessage(WPARAM wParam, LPARAM lPAram);
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHogaWnd)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHogaWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHogaWnd)
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CHogaWnd)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};


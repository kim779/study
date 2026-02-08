#pragma once

// OptionEx.h : header file
//

#include "Common.h"

/////////////////////////////////////////////////////////////////////////////
// COptionEx window

class COptionEx : public CWnd
{
// Construction
public:
	COptionEx(CWnd* pView, CWnd* parent, CString home);

// Attributes
public:
	CString		m_home;	
	CWnd*		m_pView;
	CWnd*		m_pParent;
	
	HINSTANCE	m_hOptionExDll;
	CWnd*		m_pOptionExCtrl;
// Operations
public:
	CString GetCode();
	void SetCode(CString strCode);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COptionEx();
	void createOptionExCtrl(CString ctrlName, int nkey, DWORD bColor);
	long OnMessage(WPARAM wParam, LPARAM lParam);
	void ChangeFont(int ipoint, LPCTSTR lszname);

	// Generated message map functions
protected:
	//{{AFX_MSG(COptionEx)
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


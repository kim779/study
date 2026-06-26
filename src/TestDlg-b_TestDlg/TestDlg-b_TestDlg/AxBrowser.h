#pragma once
// AxBrowser.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAxBrowser html view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include <afxhtml.h>

class CAxBrowser : public CHtmlView
{
public:
	CAxBrowser();           // protected constructor used by dynamic creation
	virtual ~CAxBrowser();
	DECLARE_DYNCREATE(CAxBrowser)

	int	m_scrGap;
// html Data
public:
	//{{AFX_DATA(CAxBrowser)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxBrowser)
	public:
	virtual void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);
	//virtual void OnNavigateComplete2(LPCTSTR strURL);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CAxBrowser)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CWnd* m_parentView;
public:
	void SetParentView(CWnd* parentView);
};
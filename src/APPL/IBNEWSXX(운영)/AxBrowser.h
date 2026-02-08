#pragma once

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include <afxhtml.h>
#include "../H/gsl/gsl"


class CAxBrowser : public CHtmlView
{
public:
	CAxBrowser();           // protected constructor used by dynamic creation
	virtual ~CAxBrowser();
	void NavigateFromMemory(LPCSTR dat, int len);
	void NavigateFromStream(IStream *pStream);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	DECLARE_DYNCREATE(CAxBrowser)

public:
	//{{AFX_DATA(CAxBrowser)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxBrowser)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	// Generated message map functions
	//{{AFX_MSG(CAxBrowser)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);
};


#if !defined(AFX_HELPOK_H__9471D758_6E00_4648_B7E5_9C219CAE4BAC__INCLUDED_)
#define AFX_HELPOK_H__9471D758_6E00_4648_B7E5_9C219CAE4BAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HelpOK.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHelpOK dialog
#include "useMaindefine.h"
class CHelpOK : public CDialog
{
// Construction
public:
	CHelpOK(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(CHelpOK)
	enum { IDD = IDD_REMOTEHELP };
	CStatic	m_static;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHelpOK)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HBITMAP	m_bitmap{};

	// Generated message map functions
	//{{AFX_MSG(CHelpOK)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
#ifdef DF_USE_CPLUS17
	std::unique_ptr<CControlChanger> m_changer{};
#else
	CControlChanger* m_changer{};
#endif
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HELPOK_H__9471D758_6E00_4648_B7E5_9C219CAE4BAC__INCLUDED_)

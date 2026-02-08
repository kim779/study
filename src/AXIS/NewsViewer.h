#if !defined(AFX_NEWSVIEWER_H__DDF26398_7A38_43C8_88BA_F68903E04C67__INCLUDED_)
#define AFX_NEWSVIEWER_H__DDF26398_7A38_43C8_88BA_F68903E04C67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewsViewer.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewsViewer dialog

class CNewsViewer : public CDialog
{
// Construction
public:
	CNewsViewer(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewsViewer)
	enum { IDD = IDD_NEWSVIEWER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewsViewer)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewsViewer)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWSVIEWER_H__DDF26398_7A38_43C8_88BA_F68903E04C67__INCLUDED_)

#if !defined(AFX_MESSAGEBAR_H__D13D5F9F_7CA0_4620_8D1C_2AAC9DC567F6__INCLUDED_)
#define AFX_MESSAGEBAR_H__D13D5F9F_7CA0_4620_8D1C_2AAC9DC567F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MessageBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMessageBar dialog
#include "RscDlg.h"
#include "resource.h"

class CMessageBar : public CRscDlg
{
// Construction
public:
	CMessageBar(CWnd* pParent = NULL);   // standard constructor
	void ShowMessage(LPCTSTR lpMessage,CWnd *pWnd = NULL);
// Dialog Data
	//{{AFX_DATA(CMessageBar)
	enum { IDD = IDD_DLGMSGBAR };
	CStatic	m_staticMessage;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMessageBar)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMessageBar)
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MESSAGEBAR_H__D13D5F9F_7CA0_4620_8D1C_2AAC9DC567F6__INCLUDED_)

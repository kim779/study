#if !defined(AFX_REQUESTMSG_H__D13D5F9F_7CA0_4620_8D1C_2AAC9DC567F6__INCLUDED_)
#define AFX_REQUESTMSG_H__D13D5F9F_7CA0_4620_8D1C_2AAC9DC567F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RequestMsg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRequestMsg dialog
#include "RscDlg.h"
#include "resource.h"

class CRequestMsg : public CRscDlg
{
// Construction
public:
	CRequestMsg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRequestMsg();
	void ShowMessage(LPCTSTR lpMessage,CWnd *pWnd = NULL);
	void HideMessage();
	CFont	m_font;
// Dialog Data
	//{{AFX_DATA(CRequestMsg)
	enum { IDD = IDD_DLGREQUEST };
	CStatic	m_staticMessage;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRequestMsg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRequestMsg)
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REQUESTMSG_H__D13D5F9F_7CA0_4620_8D1C_2AAC9DC567F6__INCLUDED_)

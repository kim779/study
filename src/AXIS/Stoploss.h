#if !defined(AFX_STOPLOSS_H__0C50124C_70BE_469D_A521_4705D797EDDF__INCLUDED_)
#define AFX_STOPLOSS_H__0C50124C_70BE_469D_A521_4705D797EDDF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Stoploss.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStoploss dialog

class CStoploss : public CDialog
{
public:
	class CDbar2 *m_pBar2;
// Construction
public:
	LRESULT OnStopMsg(WPARAM wParam, LPARAM lParam);
	CStoploss(class CDbar2 *pBar2 = NULL, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CStoploss)
	enum { IDD = IDD_STOPLOSS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStoploss)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStoploss)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STOPLOSS_H__0C50124C_70BE_469D_A521_4705D797EDDF__INCLUDED_)

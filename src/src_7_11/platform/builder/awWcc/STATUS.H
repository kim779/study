#if !defined(AFX_STATUS_H__47568300_6E09_11D4_A024_00001CD7F9BE__INCLUDED_)
#define AFX_STATUS_H__47568300_6E09_11D4_A024_00001CD7F9BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// status.h : header file
//

#include "resource.h"

#define	maxMSGs		4
/////////////////////////////////////////////////////////////////////////////
// CStatus dialog

class CStatus : public CDialog
{
// Construction
public:
	CStatus(CString mapN, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CStatus)
	enum { IDD = IDD_COMPILE };
	CProgressCtrl	m_step;
	CListCtrl	m_list;
	CString	m_error;
	CString	m_mapN;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatus)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Attributes
protected:
	CStatic*	m_bitmap[maxMSGs];
	CStatic*	m_msg[maxMSGs];
	bool		m_estate;
	CWnd*		m_parent;

// Implementation
public:
	void	SetRange(int lower, int upper);
	void	SetStep(int step);
	void	StepIt();
	void	SetMessage(int err, int kind, CString name, CString event,
				int message, CString param, int line);
	void	SetTotalError(int error);
	void	ShowBitmap(int idx = -1, bool show = false);

protected:
	void	showMessage(bool show = true);
	BOOL	addColumn(LPCTSTR str, int item, int width);
	BOOL	addItem(CString& str, int item, int subItem);
	CString	getFormKind(int kind = -1);
	CString	getMessage(int message, CString param);

	// Generated message map functions
	//{{AFX_MSG(CStatus)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATUS_H__47568300_6E09_11D4_A024_00001CD7F9BE__INCLUDED_)

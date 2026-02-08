#if !defined(AFX_MAINWND_H__7D0A7127_4E31_4DBC_BA37_B24F93F80C46__INCLUDED_)
#define AFX_MAINWND_H__7D0A7127_4E31_4DBC_BA37_B24F93F80C46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainWnd window
#include "PrintDlg.h"

class CParam
{
public:
	int		key;			// screen key
	CString		name;			// control CX_Symbol name
	CRect		rect;			// control rect
	CString		fonts;			// font name
	int		point;			// font point
	int		style;			// font style
	DWORD		tRGB;			// text color index
	DWORD		pRGB;			// paint color index
	CString		options;		// properties
};

class CMainWnd : public CWnd
{
// Construction
public:
	CMainWnd(CWnd* pParent);

// Attributes
public:

// Operations
public:
	CWnd*			m_pParent;
	CPrintDlg*		m_printDlg;
	CParam		m_Param;
	int			m_tokenSize;
	CString		m_printData[23];
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainWnd)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL
// Implementation
public:
	virtual ~CMainWnd();
	void	SetParam(struct _param *pParam);
	void GetSplit(CString sText, TCHAR sSplit);
private:
	CString	Variant(int comm, CString data);

	// Generated message map functions
protected:
	//{{AFX_MSG(CMainWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CMainWnd)
	afx_msg void SetProperties(LPCTSTR str);
	afx_msg BSTR GetProperties();
	afx_msg void SetData(LPCTSTR data);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINWND_H__7D0A7127_4E31_4DBC_BA37_B24F93F80C46__INCLUDED_)

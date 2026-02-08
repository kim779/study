#if !defined(AFX_CODEEDIT_H__3768CFDA_24F8_48F2_9B0B_E6602D2026DC__INCLUDED_)
#define AFX_CODEEDIT_H__3768CFDA_24F8_48F2_9B0B_E6602D2026DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CodeEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCodeEdit window

class CCodeEdit : public CEdit
{
// Construction
public:
	CCodeEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCodeEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCodeEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCodeEdit)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()


public:
	short	GetGubunFromCode(LPCTSTR szCode){return 0;};
	
	void	SetDataText(LPCTSTR szText);
	void	SetDataText2(LPCTSTR szCode);
	CString	GetDataText();
	
	void	SetEnabled(BOOL propVal);
	BOOL	GetEnabled();
	
	long	GetBase(LPCTSTR lpszKey);
	
	void	SetFocus();
	
	void	SetCurSel(short nIndex);
	
	OLE_HANDLE	GetHWnd();
	HWND	GetSafeHwnd();
	
	void	MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint);
	void	MoveWindow(LPCRECT lpRect, BOOL bRepaint);
	BOOL	SetWindowPos(const CWnd* pWndInsertAfter, int x, int y, int cx, int cy, UINT nFlags);
	BOOL	ShowWindow(int nCmdShow);
	void	GetWindowRect(LPRECT lpRect);
	void	GetClientRect(LPRECT lpRect);
private:
	HWND			m_hItemCode;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CODEEDIT_H__3768CFDA_24F8_48F2_9B0B_E6602D2026DC__INCLUDED_)

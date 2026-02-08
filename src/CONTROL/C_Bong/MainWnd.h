#if !defined(AFX_MAINWND_H__3A6B21CE_BE8B_4660_B4BC_62C4B98BFC73__INCLUDED_)
#define AFX_MAINWND_H__3A6B21CE_BE8B_4660_B4BC_62C4B98BFC73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainWnd window

class CCtrlWnd;

class CMainWnd : public CWnd
{
// Construction
public:
	CMainWnd(CWnd* pParent, struct _param* pInfo);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainWnd)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	inline CCtrlWnd* GetCtrlWnd() { return m_pCtrlWnd; };
	virtual ~CMainWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMainWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg LONG OnUser(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnCtrl(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnGrp(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CMainWnd)
	afx_msg void SetProperties(LPCTSTR str);
	afx_msg BSTR GetProperties();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	CWnd		*m_pParent;

private:
	class CGrpWnd	*m_pGrpWnd;
	class CCtrlWnd	*m_pCtrlWnd;
	
	struct _param	m_param;

	bool		m_bCtrl;

	void	Resize();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINWND_H__3A6B21CE_BE8B_4660_B4BC_62C4B98BFC73__INCLUDED_)

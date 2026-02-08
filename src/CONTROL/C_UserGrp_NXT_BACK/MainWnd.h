#pragma once
// MainWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainWnd window

class CMainWnd : public CWnd
{
// Construction
public:
	CMainWnd(CWnd *pParent, void *pInfo);

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
	afx_msg LONG OnPreview(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CMainWnd)
	BOOL m_show;
	afx_msg void OnShowChanged();
	afx_msg BOOL GetVisible();
	afx_msg void SetVisible(BOOL bNewValue);
	afx_msg BSTR GetProperties();
	afx_msg void SetProperties(LPCTSTR str);
	afx_msg void Clear();
	afx_msg void SetData(LPCTSTR str);
	afx_msg void SetShowLine(short nLIne, BOOL bShow);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	std::unique_ptr	<class CGrpWnd>		m_pGrpWnd;
	std::unique_ptr	<class CCtrlWnd>	m_pCtrlWnd;

	CWnd		*m_pParent;
	struct _param	m_param;
	bool		m_bCtrl;

	void	Resize();
};

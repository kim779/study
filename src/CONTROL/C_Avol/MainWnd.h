#pragma once
/////////////////////////////////////////////////////////////////////////////
// CMainWnd window

class CMainWnd : public CWnd
{
// Construction
public:
	CMainWnd(CWnd* pView, struct _param* pInfo);

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
	bool IsUseCtrl() { return m_bCtrl; }
	std::unique_ptr<class CCtrlWnd>	m_pCtrlWnd;

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
	afx_msg void Clear();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

private:
	std::unique_ptr<class CGrpWnd>	m_pGrpWnd;
	

	CWnd		*m_pView;
	struct _param	m_param;
	bool		m_bCtrl;

	void	Resize();
};

#pragma once
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
	virtual ~CMainWnd();
	inline CCtrlWnd* GetCtrlWnd() { return m_pCtrlWnd.get(); };
\
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
	afx_msg BSTR GetPropertise();
	afx_msg void SetProperties(LPCTSTR str);
	afx_msg BOOL Config();
	afx_msg void StartSignal(LPCTSTR price, LPCTSTR key);
	afx_msg void EndSignal(LPCTSTR key);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

public:
	CWnd		*m_pParent;

private:
	std::unique_ptr<class CGrpWnd>	m_pGrpWnd;
	std::unique_ptr<class CCtrlWnd> m_pCtrlWnd;
	
	struct _param	m_param;

	bool		m_bCtrl;

	void	Resize();
};

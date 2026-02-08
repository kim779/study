#pragma once
// ControlWnd.h : header file
//

class CControlWnd : public CWnd
{
// Construction
public:
	CControlWnd();
	void	CreateCtrl(CWnd* pParent, struct _param* pParam);

private:
	CWnd*	m_pParent;
	std::unique_ptr<class CInfoWnd>	m_pInfoWnd;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlWnd)
public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CControlWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CControlWnd)
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CControlWnd)
	afx_msg void AddData(LPCTSTR key, LPCTSTR val);
	afx_msg void RemoveData(LPCTSTR key);
	afx_msg void ShowData();
	afx_msg BSTR GetSelectKey();
	afx_msg BSTR GetRecentKey();
	afx_msg void RemoveAllData();
	afx_msg BSTR GetRecentVal();
	afx_msg BSTR GetSelectVal();
	afx_msg BSTR GetAllData();
	afx_msg long GetScreenMaxWidth();
	afx_msg long GetScreenMaxHeight();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};
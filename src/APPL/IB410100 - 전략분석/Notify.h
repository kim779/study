#pragma once
// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// ICxNotify wrapper class

class CNotify : public CWnd
{
public:
	CNotify();
public:

	CWnd*		m_parent{};
	CWnd*		m_pView{};
	CString		m_root;

	HINSTANCE	m_hInst{};
	CWnd*		m_Wcontrol{};
	//ICxNotify*	m_Icontrol;

	CWnd* (APIENTRY* axCreate)(CWnd*, void*) = nullptr;

public:
	BOOL	CreateNotify(CWnd* parent, CString root);

	CString GetSetAccount();
	void SetSetAccount(LPCTSTR accn);
	CString GetDataList();
	void SetDataList(LPCTSTR dat);
	CString GetFlag();
	void SetFlag(LPCTSTR flag);

	void Send(LPCTSTR sAccn, LPCTSTR sPswd);
	void AllAccnSend(LPCTSTR sUserID, LPCTSTR sPswd, LPCTSTR sCode);
	void Clear();
	void AddReminData(LPCTSTR sAccn, LPCTSTR sCode, LPCTSTR sRowData);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNotify)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNotify();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAccount)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	//}}AFX_MSG
	afx_msg LONG OnSend(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnRecv(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};
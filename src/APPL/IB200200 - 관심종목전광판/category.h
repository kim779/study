#pragma once
// category.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// IControlWnd wrapper class

class IControlWnd : public COleDispatchDriver
{
public:
	IControlWnd() {}		// Calls COleDispatchDriver default constructor
	IControlWnd(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IControlWnd(const IControlWnd& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	CString GetJCodes();
	void SetJCodes(LPCTSTR);

// Operations
public:
	CString GetCurCode();
	CString GetPreCode(LPCTSTR code);
	CString GetNextCode(LPCTSTR code);
};

/////////////////////////////////////////////////////////////////////////////
// CCategory window

class CCategory : public CWnd
{
// Construction
public:
	CCategory(CWnd* sendW, int sendK);

// Attributes
public:
	CWnd*	m_sendW = nullptr;
	int		m_sendK = 0;
	int		m_saveK =0;

	CWnd*		m_parent = nullptr;
	CString		m_root;

	HINSTANCE	m_hInst;
	CWnd*		m_Wcontrol = nullptr;
	std::unique_ptr<IControlWnd> m_Icontrol= nullptr;

	CWnd*		(APIENTRY* axCreate)(CWnd*, void*) = nullptr;

// Operations
public:
	BOOL	CreateCategory(CWnd* parent, CRect rect, CString root);

	CString category_GetJCodes();
	void	category_SetJCodes(CString codes);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCategory)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCategory();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCategory)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnClose();
	//}}AFX_MSG
	afx_msg LONG OnSend(WPARAM wParam, LPARAM lParam);
	afx_msg LONG OnRecv(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};


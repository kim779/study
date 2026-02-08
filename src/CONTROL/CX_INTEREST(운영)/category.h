#pragma once
// category.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// IControlWnd wrapper class

class ICategoryWnd : public COleDispatchDriver
{
public:
	ICategoryWnd() {}		// Calls COleDispatchDriver default constructor
	ICategoryWnd(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	ICategoryWnd(const ICategoryWnd& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	CString GetJCodes();
	void SetJCodes(LPCTSTR);

// Operations
public:
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
	CWnd*		m_sendW;
	int		m_sendK;
	int		m_saveK;

	CWnd*		m_parent;
	CString		m_root;

	HINSTANCE	m_hInst;
	CWnd*		m_Wcontrol;
	ICategoryWnd*	m_Icontrol;

	CWnd*		(APIENTRY* axCreate)(CWnd*, void*);

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
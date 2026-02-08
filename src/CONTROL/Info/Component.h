#pragma once
// Component.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CComponent command target

class CComponent : public CCmdTarget
{
	DECLARE_DYNCREATE(CComponent)

	CComponent();           // protected constructor used by dynamic creation
	virtual ~CComponent();

// Attributes
public:
	CString	m_sRoot;
	CString	m_sUser;

// Operations
public:
	CString Parser(CString &srcstr, CString substr);
	CString LoadFolder(CString path, CString sSection, CString sSearch = "");
	CString ReadFile(CString path, CString sSection, CString sKey);
	void	SaveFile(CString path, CString sSection, CString sKey, CString sData);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComponent)
public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CComponent)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CComponent)
	afx_msg BSTR GetInfoData(long nKind, LPCTSTR sFileName, LPCTSTR sSearchKey);
	afx_msg BSTR PopData(LPCTSTR sUserName, LPCTSTR sIniFileName, LPCTSTR sSectionName, LPCTSTR sKey);
	afx_msg void PushData(LPCTSTR sUserName, LPCTSTR sIniFileName, LPCTSTR sSectionName, LPCTSTR sKey, LPCTSTR sData);
	afx_msg BSTR Pop(LPCTSTR sSectionName, LPCTSTR sKey);
	afx_msg void Push(LPCTSTR sSectionName, LPCTSTR sKey, LPCTSTR sData);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};
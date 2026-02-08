#pragma once
// Filex.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CFilex command target

class CFilex : public CCmdTarget
{
	DECLARE_DYNCREATE(CFilex)

	CFilex();           // protected constructor used by dynamic creation

// Attributes
public:
protected:
	CStdioFile	m_File;
	CString		m_name;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFilex)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFilex();

	// Generated message map functions
	//{{AFX_MSG(CFilex)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CFilex)
	afx_msg BSTR GetName();
	afx_msg BOOL _Open(LPCTSTR mode, LPCTSTR FileN);
	afx_msg BSTR _Read(long size);
	afx_msg long _Seek(long to);
	afx_msg void _Close();
	afx_msg void _Write(LPCTSTR text);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};
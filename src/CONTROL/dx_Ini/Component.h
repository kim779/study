#pragma once
// Component.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CComponent command target

class CComponent : public CCmdTarget
{
	DECLARE_DYNCREATE(CComponent)

	CComponent();           // protected constructor used by dynamic creation

// Attributes
public:

private:
	CString m_file;


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComponent)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CComponent();

	// Generated message map functions
	//{{AFX_MSG(CComponent)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CComponent)
	afx_msg BOOL _Open(LPCTSTR FileName);
	afx_msg BOOL _SectionExists(LPCTSTR SectionName);
	afx_msg long _ReadInteger(LPCTSTR Section, LPCTSTR Ident, long Default);
	afx_msg BSTR _ReadString(LPCTSTR Section, LPCTSTR Ident, LPCTSTR Default);
	afx_msg double _ReadFloat(LPCTSTR Section, LPCTSTR Ident, double Default);
	afx_msg BOOL _ReadBool(LPCTSTR Section, LPCTSTR Ident, BOOL Default);
	afx_msg void _WriteFloat(LPCTSTR Section, LPCTSTR Ident, double Value);
	afx_msg void _WriteInteger(LPCTSTR Section, LPCTSTR Ident, long Value);
	afx_msg void _WriteString(LPCTSTR Section, LPCTSTR Ident, LPCTSTR Value);
	afx_msg void _WriteBoolean(LPCTSTR Section, LPCTSTR Ident, BOOL Value);
	afx_msg void _Close();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};
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
	CString		m_String;
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
	afx_msg BSTR GetText();
	afx_msg void SetText(LPCTSTR lpszNewValue);
	afx_msg long GetLength();
	afx_msg void Empty();
	afx_msg BOOL IsEmpty();
	afx_msg void MakeUpper();
	afx_msg void MakeLower();
	afx_msg long Replace(LPCTSTR oldStr, LPCTSTR newStr);
	afx_msg BSTR Mid(long nFirst, long nCount);
	afx_msg BSTR Right(long nCount);
	afx_msg BSTR Left(long nCount);
	afx_msg long Insert(long nIndex, LPCTSTR Str);
	afx_msg long Delete(long nIndex, long nCount);
	afx_msg long Compare(LPCTSTR Str);
	afx_msg long CompareNoCase(LPCTSTR Str);
	afx_msg short GetAt(long nIndex);
	afx_msg void SetAt(long nIndex, short nChar);
	afx_msg void TrimLeft();
	afx_msg void TrimRight();
	afx_msg long Find(LPCTSTR pszSub, long iStart);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

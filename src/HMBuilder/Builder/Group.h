// Group.h: interface for the CGroup class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GROUP_H__529F4ABA_58AD_4B3B_B3BF_1C4EC9AE5ADA__INCLUDED_)
#define AFX_GROUP_H__529F4ABA_58AD_4B3B_B3BF_1C4EC9AE5ADA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGroup : public CWnd
{
public:
	CGroup();
	virtual ~CGroup();
	CString		m_errMsg;
protected:
	CFile		m_file;
	CString		m_sGroup;
	CStringArray	m_groupArr;

public:
	CString getGroup(int idx);
	CString getGroup();
	void	SendData(bool req = true);

protected:
	void	loadFile();

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGroup)
	//}}AFX_VIRTUAL

// Implementation

	// Generated message map functions
protected:
	//{{AFX_MSG(CGroup)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_GROUP_H__529F4ABA_58AD_4B3B_B3BF_1C4EC9AE5ADA__INCLUDED_)

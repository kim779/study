#if !defined(AFX_OPTIONS_H__C8A10499_2425_4AB8_AE41_419EE91A0FEC__INCLUDED_)
#define AFX_OPTIONS_H__C8A10499_2425_4AB8_AE41_419EE91A0FEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Options.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptions dialog

class COptions : public CDialog
{
// Construction
public:
	COptions(CWnd* pParent = NULL);   // standard constructor

protected:
	CString		m_file;

	void		loadOptions();
	void		saveOptions();

public:
// Dialog Data
	//{{AFX_DATA(COptions)
	enum { IDD = IDD_OPTIONS };
	CListCtrl	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptions)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COptions)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnAdd();
	afx_msg void OnRemove();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONS_H__C8A10499_2425_4AB8_AE41_419EE91A0FEC__INCLUDED_)

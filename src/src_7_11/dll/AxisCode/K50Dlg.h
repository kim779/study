#if !defined(AFX_K50DLG_H__EFF36585_24BE_4C23_A1EA_95AC8A71D550__INCLUDED_)
#define AFX_K50DLG_H__EFF36585_24BE_4C23_A1EA_95AC8A71D550__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// K50Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CK50Dlg dialog

class CK50Dlg : public CDialog
{
// Construction
public:
	CK50Dlg(CPoint pt, CWnd* pParent = NULL);   // standard constructor
	CString	m_sRoot;

	CFont	m_CtrlFont;

	CPoint	m_Point;

	void	InitListCtrl();

	int	InsertListCtlCode(int nIdx, CString szCode, CString szName);
	void	ClearListCtlCode(int nItem = -1);
	void	AddListCtlCode(CString szCode, CString szName);

	CString GetCode() { return m_rtnData.code; }
	CString GetName() { return m_rtnData.name; }

	LOGFONT	SetFont(int Size);

	struct _exCode {
		CString code;
		CString name;
	};

	_exCode	m_rtnData;
	BOOL	loadJCode();

	void	SetDlgPos(int x, int y);

// Dialog Data
	//{{AFX_DATA(CK50Dlg)
	enum { IDD = IDD_DIALOG_KOSPI50 };
	CSkinListCtrl	m_ListCtlCode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CK50Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CK50Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnDblclkListCode(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_K50DLG_H__EFF36585_24BE_4C23_A1EA_95AC8A71D550__INCLUDED_)

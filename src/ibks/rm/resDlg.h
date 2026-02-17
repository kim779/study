#if !defined(AFX_RESDLG_H__D2AEA16B_DC53_4265_ADDC_3C4423C09F32__INCLUDED_)
#define AFX_RESDLG_H__D2AEA16B_DC53_4265_ADDC_3C4423C09F32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// resDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CResDlg dialog

class CResDlg : public CDialog
{
// Construction
public:
	CResDlg(int mode, CWnd* pParent = NULL);   // standard constructor
	~CResDlg();

// Dialog Data
	//{{AFX_DATA(CResDlg)
	enum { IDD = IDD_RESOURCE };
	CListCtrl	m_list;
	//}}AFX_DATA

protected:
	int	m_mode;
	int	m_kind;
	int	m_select;
	CStringArray	m_files;
	CMapStringToPtr	m_pfs;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	fillList(bool reset = false);
	BOOL	addColumn(CString columns, int col, int width);
	BOOL	addItem(CString items, int item, int subi);
	void	loadFiles();
	bool	checkFile(CString name);

	// Generated message map functions
	//{{AFX_MSG(CResDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnComboSelChange();
	afx_msg void OnDelete();
	afx_msg void OnTrans();
	afx_msg void OnListClick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg void OnKind(UINT cmdID);

	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESDLG_H__D2AEA16B_DC53_4265_ADDC_3C4423C09F32__INCLUDED_)

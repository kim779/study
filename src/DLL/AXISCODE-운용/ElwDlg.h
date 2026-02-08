
#include <afxtempl.h>

#pragma once


// ElwDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CElwDlg dialog

class CElwDlg : public CDialog
{
// Construction
public:
	CElwDlg(CPoint pt, CWnd* pParent = nullptr);   // standard constructor

public:
	BOOL	loadELWCode();
	void	SetDlgPos(int x, int y);
	BOOL	CountData();
	BOOL	CountBase();
	CString GetName() { return m_szName; }
	CString GetCode() { return m_szCode; }
	int	FindPBCode(CString code);
	void    listsort(int column);
protected:
	CPoint m_point;
	
	CString m_szName, m_szCode;
	CArray<ELWCODE, ELWCODE> m_arrayELWCode;
	CArray<PBCODE, PBCODE> m_arrayPBCode;
	CArray<BASECODE, BASECODE> m_arrayBASECode;

	std::vector<std::tuple<CString, CString, CString>> _vData;

	enum {cpAll, cpCall, cpPut}  m_CPType;
	enum {etAll, etNormal, etKoba} m_ElwType;

// Dialog Data
	//{{AFX_DATA(CElwDlg)
	enum { IDD = IDD_DIALOG_ELW };
	CButton	m_rdoType;
	CButton	m_rdoAll;
	CListCtrl	m_listCtrl;
	CListBox	m_listPB;
	CListBox	m_listBase;
	CComboBox	m_comMonth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CElwDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CElwDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnRadioShare();
	afx_msg void OnSelchangeListBase();
	afx_msg void OnSelchangeListCom();
	afx_msg void OnSelchangeComMonth();
	afx_msg void OnDblclkListElw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnElwTypeChange();
	afx_msg void OnGetdispinfoListElw(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


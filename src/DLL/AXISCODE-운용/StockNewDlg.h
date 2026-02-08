#pragma once
// StockNewDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStockNewDlg dialog

#include "SkinListCtrl.h"
#include "tool.h"
#include <afxtempl.h>

typedef struct _ncode
{
	CString		code;
	CString		name;
	CString		kind;
	CString		fill;

}NCODE;

class CStockNewDlg : public CDialog
{
// Construction
public:

	CStockNewDlg(CPoint pt = CPoint(-1, -1), int nType = 0, CWnd* pParent = nullptr);   // standard constructor
	BOOL	CountData();
	CString GetName() { return m_szName; }
	CString GetCode() { return m_szCode; }
	void	AddItem(int row, CString szCode, CString szName, CString szTcode, CString kind);
	BOOL	loadJCode();
	CString	GetName(char* code);
	void	SetDlgPos(int x, int y);
	void	SearchName(CString word);

// Dialog Data
	//{{AFX_DATA(CStockNewDlg)
	enum { IDD = IDD_DIALOG_STOCKNEW };
	CEdit	m_editSearch;
	CButton	m_btnSearch;
	CListCtrl	m_ListCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStockNewDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int		m_nType{};

	bool		m_bSortDirect{};
	CTool		m_Tool;

	CString		m_szCode;
	CString		m_szName;

	CPoint		m_point;

	HBRUSH		m_Br{};
	int		m_nSearchStart{};

	CArray<NCODE, NCODE>m_arrayNCode;

	// Generated message map functions
	//{{AFX_MSG(CStockNewDlg)
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkListNew(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCancelMode();
	afx_msg void OnColumnclickListNew(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonSearch();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChangeEditSearch();
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
};


#if !defined(AFX_ELWDLG_H__A2E08CDF_267D_4156_98BE_B294BC0FA8EC__INCLUDED_)
#define AFX_ELWDLG_H__A2E08CDF_267D_4156_98BE_B294BC0FA8EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#include "fxButton.h"
typedef struct _JCode
{
	CString Code;
	CString Name;
}JCODE;

static BOOL m_gListSort;

class CElwDlg : public CDialog
{
public:
	CElwDlg(CPoint pt, CWnd* pParent = NULL); 

	//{{AFX_DATA(CElwDlg)
	enum { IDD = IDD_DIALOG_ELW };
	CfxButton	m_ctlBtn9;
	CfxButton	m_ctlBtn8;
	CfxButton	m_ctlBtn7;
	CfxButton	m_ctlBtn6;
	CfxButton	m_ctlBtn5;
	CfxButton	m_ctlBtn4;
	CfxButton	m_ctlBtn3;
	CfxButton	m_ctlBtn22;
	CfxButton	m_ctlBtn21;
	CfxButton	m_ctlBtn20;
	CfxButton	m_ctlBtn2;
	CfxButton	m_ctlBtn19;
	CfxButton	m_ctlBtn18;
	CfxButton	m_ctlBtn17;
	CfxButton	m_ctlBtn16;
	CfxButton	m_ctlBtn15;
	CfxButton	m_ctlBtn14;
	CfxButton	m_ctlBtn13;
	CfxButton	m_ctlBtn12;
	CfxButton	m_ctlBtn11;
	CfxButton	m_ctlBtn10;
	CfxButton	m_ctlBtn1;
	CComboBox	m_ctlComboSearch;
	CEditX	m_edtSearch;
	CButton	m_rdoAll;
	CListBox	m_listPB;
	CListBox	m_listBase;
	CSkinListCtrl	m_listCtrl;
	CComboBox	m_comMonth;
	//}}AFX_DATA

// Overrides
	//{{AFX_VIRTUAL(CElwDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CElwDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnRadioAll();
	afx_msg void OnRadioCall();
	afx_msg void OnRadioPut();
	afx_msg void OnSelchangeListBase();
	afx_msg void OnSelchangeListCom();
	afx_msg void OnSelchangeComMonth();
	afx_msg void OnDblclkListElw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelendokComboSearch();
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	afx_msg void OnClickBtnSort(UINT nID);
	afx_msg LPARAM OnEditXMsg(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	BOOL	loadELWCode();
	void	SetDlgPos(int x, int y);
	CString GetName() { return m_szName; }
	CString GetCode() { return m_szCode; }

protected:
	void	initBtn();
	void	clearListCtrl();
	void	initListCtrl();
	void	FillMonth();
	void	fillListBox();
	void	fillListCtrl();
	void	addItem(int row, CString szCode, CString szName);
	bool	findJCode(CString code, CString& name);

	void	SearchString(CString szText);
	void	searchAction(int column);
	void	searchWord();
	int	findMatchIndex(CString text, int column);
	bool	SearchData(CString sName, bool bAddAll);
	bool	IsChosung(WORD wHangul, WORD &wStart, WORD &wEnd);
	WORD	MakeHangul(char cChar1, char cChar2);

protected:
	bool	m_bAddAll;	// ListCtrl의 중복 클리어를 방지하기 위해
	CPoint	m_point;
	CString m_szName, m_szCode;
	HBRUSH	m_hBrush;
	
	enum {cpAll, cpCall, cpPut}	m_cpType;

	

	CPtrArray	m_arSelCode;	// 현재 선택된 종목의 목록
};

//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_ELWDLG_H__A2E08CDF_267D_4156_98BE_B294BC0FA8EC__INCLUDED_)

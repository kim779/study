// 123456Dlg.h : header file
//

#pragma once

#include <AFXTEMPL.H>

/////////////////////////////////////////////////////////////////////////////
// CCommodityDlg dialog

typedef struct _ccode_in {
	int mode{};
	int year{};
	int month{};
	CString hname;
	CString ename;
	CString code;	
} ccode_in;

typedef struct _ccode_group {
	int mode{};
	CList<std::shared_ptr<ccode_in>, std::shared_ptr<ccode_in>> list;
} ccode_group;

typedef struct _cocode_in {
	int year{};
	int month{};
	CString code;
	float price{};
	BOOL bCall{};

	CString hname;
	CString ename;
}cocode_in;

#include "SkinListCtrl2.h"
#include "OptionListCtrl.h"

class CCommodityDlg : public CDialog
{
// Construction
public:
	CCommodityDlg(CWnd* pParent = NULL,CPoint pt = CPoint(-1, -1));	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCommodityDlg)
	enum { IDD = IDD_DIALOG_COMMODITY };
	CButton	m_btnOption;
	COptionListCtrl	m_Olist;
	CEdit	m_edit2;
	CEdit	m_edit1;
	CListBox	m_spdList;
	CButton	m_btnMoney;
	CButton	m_btnGumri;
	CButton	m_btnCommodity;
	CSkinListCtrl2	m_list;
	//}}AFX_DATA

	void SetSpreadMode( BOOL b );

	BOOL LoadCCode( void );	
	BOOL LoadCCodeFromFile( void );
	BOOL FindCode( CString str );
	BOOL FindOCode( CString strCode );

	BOOL LoadCOCodeFromFile( void );

	CString GetCode( void ) { return m_strCode; } 
	CString GetName( void ) { return m_strName; }
 
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommodityDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

protected:
	HICON m_hIcon{};

	
	void DeleteCCode( void );
	void DeleteCOCode( void );
	
	void AddGroup(std::shared_ptr<ccode_in> pin );
	void AddMonth(std::shared_ptr<ccode_in> pin, std::weak_ptr<ccode_group> pg );
	void AddList(std::weak_ptr<ccode_group> pg );

	int GetYear( CString str );
	int GetMonth( CString str );
	
	CArray<std::shared_ptr<ccode_group>, std::shared_ptr<ccode_group>> m_codeGroup;
	CList <std::shared_ptr<cocode_in>, std::shared_ptr<cocode_in>>	m_cocodeList;

	void DeleteAll( void );
	void DeleteSpread( void );
	enum { GUMRI = 0, MONEY };
		
	void SetList( int iMin, int iMax , int igubn = 2);
	void SetOption( CString strCode );
	void SetSpread(std::weak_ptr<ccode_in> pin );

	CString GetOptionName( CString strCode );
	
	BOOL CheckSelect( CString& code, CString& name );
	std::weak_ptr<ccode_in> GetSelect( void );

	CString m_strCode;
	CString m_strName;

	CBrush m_bk;
	CBrush m_1, m_2, m_3;

	CPoint m_pt;

	BOOL	m_bSpreadMode;

	void SetDlgPos(int x, int y);

	void SwapControl( int b );
	void ShowOption( BOOL bShow );

	// Generated message map functions
	//{{AFX_MSG(CCommodityDlg)
	virtual BOOL OnInitDialog();	
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnBtnGumri();
	afx_msg void OnBtnMoney();
	afx_msg void OnBtnCom();
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeList2();
	afx_msg void OnDblclkList2();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnButton1();
	afx_msg void OnBtnOption();
	//}}AFX_MSG
	afx_msg LRESULT OnListBDClick( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnListClick( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnSelectOption( WPARAM wParam, LPARAM lParam );
	DECLARE_MESSAGE_MAP()
};


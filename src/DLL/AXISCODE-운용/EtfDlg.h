#pragma once
// EtfDlg.h : header file
//
#include "resource.h"
#include "user.h"
#include "BtnST.h"
#include "sfont.h"
#include "skinlistctrl.h"
#include "EditX.h"
#include "../../h/jmcode.h"
#include "axiscode.h"
#include <afxtempl.h>

/////////////////////////////////////////////////////////////////////////////
// CEtfDlg dialog

enum all_code{ acETF, acETN, acETFETN}; //2014.10.28 KSJ 전체종목 

static BOOL m_eListsort;
/////////////////////////////////////////////////////////////////////////////
// CEtfDlg dialog

class CEtfDlg : public CDialog
{
// Construction
public:
	CEtfDlg(CPoint pt, CWnd* pParent = nullptr);   // standard constructor

	void	ListRemoveAll();
	CString GetCode() { return m_szCode; }
	CString GetName() { return m_szName; }
	void	SetCode(CString szCode){ m_szCode = szCode; }
	void	SetName(CString szName){ m_szName = szName; }

	
	std::vector<std::tuple<char, CString, CString>>
							&_vETFitem = ((CAxisCodeApp *)AfxGetApp())->_vETFitem;
	std::vector<std::tuple<char, char, CString>>		                
							&_vETFtema = ((CAxisCodeApp *)AfxGetApp())->_vETFtema;
	std::vector<std::tuple<char, char, CString>>
							&_vETFfore = ((CAxisCodeApp *)AfxGetApp())->_vETFfore;
	std::vector<std::tuple<char, CString, CString, CString, char, char>>
							&_vETFcode = ((CAxisCodeApp *)AfxGetApp())->_vETFcode;

	std::vector<std::pair<CString, CString>> 	_vData, _vSelect;
	std::vector<HCODEX*> &_vETNx = ((CAxisCodeApp *)AfxGetApp())->_vETNx;

	enum tree_class{
			tree_jjm = 0,
			tree_kspjjm,
			tree_ksdjjm,
			tree_kspup,
			tree_ksdup,
			tree_ksp200,
			tree_ksd50,
			tree_krx100,
			tree_frbd,
			tree_etf,
			tree_etf_item,
			tree_etf_tema,
			tree_etf_fore,
			tree_elw,
			tree_elw_base,
			tree_for,
			tree_ks,
			tree_tm,
			tree_group,
			tree_reits,
			tree_elw_gencode,
			tree_elw_jogicode,
			tree_konex		,//2013.06.05 KSJ 코넥스 추가
			tree_etn,		//2014.10.28 KSJ etn 추가
			tree_etfetn		//2014.10.28 KSJ etf/etn/리츠 전종목 추가
	};

// Dialog Data
	//{{AFX_DATA(CEtfDlg)
	enum { IDD = IDD_DIALOG_ETF };
	CEditX	m_EditSearch;
	CTreeCtrl	m_Tree;
	CSkinListCtrl	m_ListCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEtfDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	void	InitialTree();
	void	InitialList(int nETF);
	void	SearchList();
	void	CheckChildItems(HTREEITEM hItem);
	void	UnCheckChildItems(HTREEITEM hItem);
	void	DataGubn(HTREEITEM hItem);	
	void	DataReloadItem(CString data);
	void	DataReloadTema(CString data);
	void	DataReloadFore(CString data);
	void	CheckCompany(CString company, HTREEITEM hItem, HTREEITEM parentItem);
	void	UnCheckCompany(CString company, HTREEITEM hItem);
	bool	ExistCodeList(CString code);
	void	SetDlgPos(int x, int y);

	CString	m_szCode;
	CString	m_szName;
	CSFont	m_Font;
	CString m_root;
	CPoint  m_point;

	BOOL	m_search{};
	std::map<CString, struct hjcodex*> _mapCODE; 
	void InitReitList();

	// Generated message map functions
	//{{AFX_MSG(CEtfDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickListCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeStaticSearch();
	afx_msg void OnClickTreeBunryu(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg long OnEditXMsg(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	bool		m_bAddAll{};	// ListCtrl의 중복 클리어를 방지하기 위해
	void	searchAction(int column);
	int	findMatchIndex(CString text, int column);
	bool	SearchData(int kind, CString sName, bool bAddAll);
	bool	SearchJongmok(CString sName, bool bAddAll);
	bool	SearchWord(CString sName, bool bAddAll);
	bool	IsChosung(WORD wHangul, WORD &wStart, WORD &wEnd);
	WORD	MakeHangul(char cChar1, char cChar2);
	void    listsort(int column);
public:
	afx_msg void OnGetdispinfoListCtrl(NMHDR* pNMHDR, LRESULT* pResult);
};


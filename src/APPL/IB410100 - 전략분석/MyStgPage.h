
#include "StgPage.h"
#include "../../control/fx_misc/fxImgButton.h"

#pragma once
// MyStg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyStgPage dialog

class CMyStgPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CMyStgPage)

// Construction
public:
	CMyStgPage(CWnd* pView = NULL, CWnd* pParent = NULL);
	~CMyStgPage();

public:
	CIB410100App* m_pApp{};
	CWnd* m_pView{};
	CWnd* m_pParent{};
	CFont* m_pFont{};
	COLORREF m_bkColor{};
	CBrush	m_bkBr;

	CString m_sPath;
	std::unique_ptr<class CfxGrid> m_pPFGrid{};
	std::unique_ptr<class CGridWnd> m_pGridWnd{};
	std::unique_ptr<class CGrpWnd> m_pGrpWnd{};

	CArray <struct _ProjectCode, struct _ProjectCode>	m_ProjectCode;
	CString		m_sHval[4];	// 21,30,60,90
	CString		m_sIVVal[2], m_sVal;
	CArray<struct _mdat*, struct _mdat*>	m_mData;

	bool	m_bRts{};

public:
	// 나의 전략
	void	initGrid();
	void	initGridRow(int row);
	void	setList();
	BOOL	addColumn(CListCtrl *LC, LPCTSTR str, int item, int width);
	void	setGridItem(int row, CString dat);
	CString Parse(CString &dat, CString delimiter = ";");
	void	AddPCode(CString Code, int Num, int mmgb, double prc);

	// 만기손익
	void	CheckMinGam();
	void	InitData(struct _bdat *bdat);
	void	SetGraph();
	void	queryMingam();
	void	parseMingam(char *dat);
	void	sendTR(CString trCode, char* datB, int datL, int nKey, int nStat);
	int	findGrid(class CfxGrid* pGrid, CString code);
	int	findMData(CString code);
	int	findGData(CString code);
	void	deleteMData(CString code);
	void	generateMingam(int idx);
	void	RemoveProject();

// Dialog Data
	//{{AFX_DATA(CMyStgPage)
	enum { IDD = IDD_TAB_MYSTG };
	CfxImgButton	m_btnSetStg;
	CfxImgButton	m_btnDelStg;
	CfxImgButton	m_btnAddStg;
	CButton	m_ChkSonik;
	CListCtrl	m_ListStrategy;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CMyStgPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	afx_msg void OnClickListStrategy(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnInitData(WPARAM wParam, LPARAM lParam);
	afx_msg long OnMsgMouseValue(WPARAM wParam, LPARAM lParam);
	afx_msg long OnMsgGetHV(WPARAM wParam, LPARAM lParam);
	afx_msg long OnMsgGetFee(WPARAM wParam, LPARAM lParam);
	afx_msg long OnMsgRefreshList(WPARAM wParam, LPARAM lParam);
	//{{AFX_MSG(CMyStgPage)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnStg();
	afx_msg void OnBtnAddstg();
	afx_msg void OnBtnDel();
	afx_msg void OnMgchk1();
	afx_msg void OnMgchk2();
	afx_msg void OnMgchk3();
	afx_msg void OnMgchk4();
	afx_msg void OnMgchk5();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


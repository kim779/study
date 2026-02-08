#include <Afxtempl.h>
#include "../../control/fx_misc/fxImgButton.h"

#pragma once
// StgPage.h : header file
//

struct _ProjectCode{		// 서버에서 그리드 정보 받거나 리스트에서 전략 선택을 하거나 할때
	CString		Code;	// 코드
	int		Num=0;	// 수량
	int		mmgb=0;	// 매매구분 (-1. 매도, +1. 매수)
	double		jprc=0;	// 주문가
};

#define  GUIDEBAR_HEIGHT 20

/////////////////////////////////////////////////////////////////////////////
// CStgPage dialog

class CStgPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CStgPage)

// Construction
public:
	CStgPage(CWnd* pView = NULL, CWnd* pParent = NULL);
	~CStgPage();
public:
	CWnd*		m_pParent=nullptr;
	CWnd*		m_pView = nullptr;
	CIB410100App*	m_pApp = nullptr;
	CFont*		m_pFont = nullptr;
	CBrush		m_edtBr;
	COLORREF	m_memoColor{};
	COLORREF	m_bkColor{};
	CBrush		m_bkBr;

	int		m_nStgCode{};
	std::unique_ptr<class CStgBoard> m_pStgBoard = nullptr;
	std::unique_ptr<class CStgInfo>  m_pStgInfo = nullptr;
	std::unique_ptr<class CfxGrid>	 m_pPFGrid = nullptr;
	std::unique_ptr<class CMGrpWnd>	 m_pGrpWnd = nullptr;
	CArray <struct _ProjectCode, struct _ProjectCode>	m_projectCode;

	struct _MakeCodeData{
		CString	fcode;
		CString	year;
		CString	month;
		CString atmg;
		CArray <CString, CString> hsga;
	};
	_MakeCodeData m_MakeCodeData{};

public:
	COLORREF getIndexColor(int index);
	void	drawSelection(CDC &dc);
	void	initData();
	void	setCodeInfo(int Index);
	void	makeCode(int Index);
	void	addPCode(CString Code, int Num, int mmgb);
	//void	makeChartInfo(int Index);
	//void	addCCode(int ctype, int atmgap, int Num, int mmgb);
	CString getCode(int type, CString hsga /* = _T("") */);
	CString getHsga(int nItem);
	CString monthCode(int month);
	void	initGrid();
	void	initGridRow(int row);
	void	setGrid();
	void	setGridItem(int row, CString dat);
	CString Parse(CString &dat, CString delimiter = ";");

// Dialog Data
	//{{AFX_DATA(CStgPage)
	enum { IDD = IDD_TAB_STG };
	CfxImgButton	m_btnAddStg;
	CfxImgButton	m_btnSetStg;
	CEdit	m_edtStgInfo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CStgPage)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	afx_msg void OnClickStgBtn();
	//{{AFX_MSG(CStgPage)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnBtnAddstg();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg long OnUser(WPARAM wParam, LPARAM lParam);
	afx_msg long OnInitData(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

};


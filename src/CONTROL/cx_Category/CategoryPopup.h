#pragma once

// CategoryPopup.h : header file
//
#include <afxtempl.h>
#include "ExListCtrl.h"
#include "SStatic.h"
#include ".././../h/jmcode.h"

/////////////////////////////////////////////////////////////////////////////
// CCategoryPopup window
#define CHECKER_COUNT 10

struct	_mhjcode {
	CString	code;		// 코드	(12)
				// 코드(7) : A######
				// 코드(9) : 신주인수권, 증서
	CString	hnam;		// 한글명(18)
	CString	enam;		// 영문명

	CString	acdl;		// 업종대분류
	CString	acdm;		// 업종중분류(asis ucdm)
	CString	acds;		// 업종소분류(asis ucds)

	char	ecng=0;		// ECN 구분
				// 거래가능:1
	char	jsiz=0;		// 업종자본금 대중소
				// 장내 (대:38  중:39  소:40)
				// 장외 (1:KOSDAQ 100)
				// 장외 (2:KOSDAQ MID 300)
				// 장외 (3:KOSDAQ SMALL)
				// 배당지수(jsiz += 41)
				// 신주인수권종목 FULL코드용
	CString	symb;		// SYMBOL
	char	size=0;		// 자본금 규모
				// KOSPI  (대:2  중:3  소:4)
				// KOSDAQ (기타서비스:3  코스닥IT종합:4)
	char	jjug=0;		// 제조업 구분
				// KOSPI  (제조업:27)
				// KOSDAQ (벤처지수:2)
	char	kpgb=0;		// KOSPI, KOSDAQ 구분
				// KOSPI  (0:미포함 1:K200 2:K100 3:K50)
				// KOSDAQ (0:미포함 1:KQ50)
	char	kosd=0;		// 종목구분
				// 0:장내  10:장외  30:제3시장
	char	ssgb=0;		// 소속구분
				// 0:default
				// 4:뮤추얼펀드
				// 5:신주인수권
				// 6:리츠
				// 7:신주인수권증서
				// 8:ETF
				// 9:하이일드펀드
	char	ucmd=0;		// 우선주구분 (0:보통주  5:우선주)
	char	itgb=0;		// KOSPI-IT/KQ-IT50구분(1:채택)
	char	wsgb=0;		// KOSPI 지배구조지수:42 (0:보통주 1:우수)
	char	jqty=0;		// 기본주문단위
	char	star=0;		// KOSDAQ 스타지수
	char	unio=0;		// 통합지수
	char    jchk=0;		// 종목검색용 구분
				// 0x01 불성실공시종목
				// 0x02 신규상장종목
				// 0x04	권리락종목
				// 0x08	우선주종목
				// 0x10 관리종목
				// 0x20	감리종목
				// 0x40 거래정지종목
				// 0x80	투자유의종목
	char	fill[2]{};	//reservered;
};	
	
#define MAX_LIST_SIZE 1000

class CControlWnd;
#include "SComboBox.h"
#include "xSkinButton.h"

class CCategoryPopup : public CWnd
{
// Construction
public:
	CCategoryPopup(CControlWnd* pWnd, int Gap);
	virtual ~CCategoryPopup();

// Attributes
public:
	CControlWnd *m_pMainWnd;
	int	m_nKind;

protected:
	CBitmap	m_bmp1,m_bmp2;
	CBrush	m_brBk,m_brBk1,m_brSt;
	bool	m_fDone;
	CString	m_sRoot, m_sUser;

	CArray <std::shared_ptr<_mhjcode>, std::shared_ptr<_mhjcode>>	m_shjCodes;
	CArray <std::shared_ptr<_mhjcode>, std::shared_ptr<_mhjcode>>	m_elwbase;
	CStringArray m_history;

	CWnd*	m_pParent;

	std::unique_ptr<CTreeCtrl>	m_pTree;
	std::unique_ptr<CStatic>	m_pStatic;
	std::unique_ptr<CSComboBox>	m_pComboOrder;
	std::unique_ptr<CButton>	m_pCheckBtn[CHECKER_COUNT];
	std::unique_ptr<CExListCtrl>	m_pList;

	std::unique_ptr<CSStatic>	m_pQCStatic;
	std::unique_ptr<CStatic>	m_pNtStatic;
	std::unique_ptr<CComboBox>	m_pComboGap;
	std::unique_ptr<CxSkinButton>	m_pSearchBtn;
	std::unique_ptr<CxSkinButton>	m_pConfigBtn;
	std::unique_ptr<CxSkinButton>	m_pCloseBtn;
	std::unique_ptr<CButton>	m_chkPlay;

	CRect		m_rcCheck[CHECKER_COUNT];
	CString		m_szCheckName[CHECKER_COUNT];
	CFont*		m_pFont;
	
	int		m_Gap;
	bool		m_bInitState;

	HTREEITEM	m_iitem;	//관심종목 TREEITEM
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCategoryPopup)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	bool	IsFilteredHj(_mhjcode* hj);
	void	ListUpjongSort(int nidx, std::shared_ptr<_mhjcode> hjc,int ntype=0);
	void	InitControls();
	void	InitListCtrl();
	CString LoadStringTb(UINT nIdx);
	void	loadRank(HTREEITEM hItem);
	void	LoadGroup(char* datb, int datl);
	void	loadUCode(CString path, HTREEITEM hItem);
	void	SearchCode();
	CString	Parser(CString &srcstr, CString substr);
	void	setParent(CWnd *pWnd);
	void	setPathInfo(CString root, CString user);
	void	LoadFolder(CString path, HTREEITEM hItem, CString sSection, int nKey);
	void	LoadELWBase(CString path);
	std::shared_ptr<_mhjcode> FindCode(CString sCode);
	void	baseCX_Category();
	void	initBitmap();
	void	LoadInterestCode(const CStringArray& codes);

protected:
	void	fillList();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCategoryPopup)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnDestroy();
	afx_msg void OnConfigClk();
	afx_msg void OnCloseClk();
	afx_msg void OnSearchClk();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) ;	
	afx_msg void OnTreeDblClk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPaint();
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	afx_msg void OnChecked(UINT nID);
	afx_msg long OnEnd(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

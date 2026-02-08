#if !defined(AFX_CATEGORYPOPUP_H__3365E2BF_3265_4CDA_B4A9_1932F398A97C__INCLUDED_)
#define AFX_CATEGORYPOPUP_H__3365E2BF_3265_4CDA_B4A9_1932F398A97C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CategoryPopup.h : header file
//
#include "resource.h"
#include <afxtempl.h>
/////////////////////////////////////////////////////////////////////////////
// CCategoryPopup window
#define CHECKER_COUNT 10
//#include "XPButton.h"
#include "../ctrl/ExListCtrl.h"
#include "SStatic.h"
#include ".././../h/jmcode.h"
#include "usedefine.h"

struct	_mhjcode {
	CString	code;		// 코드	(12)
				// 코드(7) : A######
				// 코드(9) : 신주인수권, 증서
	CString	hnam;		// 한글명(18)
	CString	enam;		// 영문명

	CString	acdl;		// 업종대분류
	CString	acdm;		// 업종중분류(asis ucdm)
	CString	acds;		// 업종소분류(asis ucds)

	char	ecng{};		// ECN 구분
				// 거래가능:1
	char	jsiz{};		// 업종자본금 대중소
				// 장내 (대:38  중:39  소:40)
				// 장외 (1:KOSDAQ 100)
				// 장외 (2:KOSDAQ MID 300)
				// 장외 (3:KOSDAQ SMALL)
				// 배당지수(jsiz += 41)
				// 신주인수권종목 FULL코드용
	CString	symb;		// SYMBOL
	char	size{};		// 자본금 규모
				// KOSPI  (대:2  중:3  소:4)
				// KOSDAQ (기타서비스:3  코스닥IT종합:4)
			

	//char	ucdm{};		// 업종중분류
	//			// KOSDAQ:5~14  미분류:99
	//char	ucds{};		// 업종소분류
	//			// KOSDAQ:15~37  벤쳐:7  미분류:99


	char	jjug{};		// 제조업 구분
				// KOSPI  (제조업:27)
				// KOSDAQ (벤처지수:2)
	char	kpgb{};		// KOSPI, KOSDAQ 구분
				// KOSPI  (0:미포함 1:K200 2:K100 3:K50)
				// KOSDAQ (0:미포함 1:KQ50)
	char	kosd{};		// 종목구분
				// 0:장내  10:장외  30:제3시장
	char	ssgb{};		// 소속구분
				// 0:default
				// 4:뮤추얼펀드
				// 5:신주인수권
				// 6:리츠
				// 7:신주인수권증서
				// 8:ETF
				// 9:하이일드펀드
	char	ucmd{};		// 우선주구분 (0:보통주  5:우선주)
	char	itgb{};		// KOSPI-IT/KQ-IT50구분(1:채택)
	char	wsgb{};		// KOSPI 지배구조지수:42 (0:보통주 1:우수)
	char	jqty{};		// 기본주문단위
	char	star{};		// KOSDAQ 스타지수
	char	unio{};		// 통합지수
	char    jchk{};		// 종목검색용 구분
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

constexpr char* acd_KOSPI[] = {
	"",
	"KGG01P",	// 코스피		1
	"KGZ01P",	// 코스피 대형주	2	
	"KGZ02P",	// 코스피 중형주	3
	"KGZ03P",	// 코스피 소형주	4
	"KGS01P",	// 음식료품		5
	"KGS02P",	// 섬유, 의복		6
	"KGS03P",	// 종이, 목재		7
	"KGS04P",	// 화학			8
	"KGS05P",	// 의약품		9	
	"KGS06P",	// 비금속광물		10
	"KGS07P",	// 철강및금속		11
	"KGS08P",	// 기계			12
	"KGS09P",	// 전기, 전자		13
	"KGS19P",	// 의료정밀		14
	"KGS10P",	// 운수장비		15
	"KGS11P",	// 유통업		16
	"KGS20P",	// 전기가스업		17
	"KGS12P",	// 건설업		18	
	"KGS13P",	// 운수창고		19
	"KGS21P",	// 통신업		20
	"KGS14P",	// 금융업		21
	"KGS15P",	// 은행			22
	"",
	"KGS16P",	// 증권			24
	"KGS17P",	// 보험			25
	"KGS22P",	// 서비스업		26
	"KGS18P"	// 제조업		27
};

constexpr char* acd_KOSDAQ[] = {
	"",
	"",
	"",
	"",
	"",
	"",
	"QGS02P",	// 제조			6
	"QGS03P",	// 건설			7
	"QGS04P",	// 유통			8
	"",
	"QGS31P",	// 운송			10 
	"QGS05P",	// 금융			11
	"QGS06P",	// 통신방송		12
	"QGS07P",	// IT S / W & SVC	13
	"QGS08P",	// IT H / W		14      
	"QGS09P",	// 음식료·담배		15 
	"QGS10P",	// 섬유·의류		16
	"QGS11P",	// 종이 목재		17
	"QGS12P",	// 출판·매체복제	18
	"QGS13P",	// 화학			19
	"QGS33P",	// 제약			20
	"QGS14P",	// 비금속		21
	"QGS15P",	// 금 속		22
	"QGS16P",	// 기계·장비		23
	"QGS17P",	// 일반전기전자		24
	"QGS18P",	// 의료·정밀기기	25
	"QGS19P",	// 운송장비·부품	26
	"QGS20P",	// 기타 제조		27
	"QGS21P",	// 통신서비스		28
	"QGS22P",	// 방송서비스		29
	"QGS23P",	// 인터넷		30
	"QGS24P",	// 디지털컨텐츠		31
	"QGS25P",	// 소프트웨어		32
	"QGS26P",	// 컴퓨터서비스		33
	"QGS27P",	// 통신장비		34
	"QGS28P",	// 정보기기		35
	"QGS29P",	// 반도체		36
	"QGS30P",	// IT부품		37
	"QGS87P",	//         
	"",
	"",		//			39
	"",		//			40
	"QGS34P",	// 오락·문화		41
	"QGG03P",	// 코스닥 우량기업부	42
	"QGG04P",	// 코스닥 벤처기업부	43
	"QGG05P",	// 코스닥 중견기업부	44
	"QGG06P"	// 코스닥 기술성장기업부45 
};

class CControlWnd;
#include "SComboBox.h"
#include "xSkinButton.h"

class CCategoryPopup : public CWnd
{
// Construction
public:
	CCategoryPopup(CWnd* parent);

// Attributes
public:
	CWnd* m_parent;
#ifdef DF_USESTL_CATEGORY
	std::unique_ptr<CTreeCtrl>m_pTree;
	std::unique_ptr<CExListCtrl>m_pList;
#else
	CTreeCtrl	*m_pTree;
	CExListCtrl *m_pList;
#endif
	int			m_nKind;

protected:
	CBitmap*	m_bmp1;
	CBitmap*	m_bmp2;
	CBrush		m_brBk,m_brBk1,m_brSt;
	bool		m_fDone;
#ifdef DF_USESTL_CATEGORY
	CArray		<std::shared_ptr<_mhjcode>, std::shared_ptr<_mhjcode>>	m_shjCodes;
	CArray		<std::shared_ptr<_mhjcode>, std::shared_ptr<_mhjcode>>m_elwbase;
	std::unique_ptr<CStatic> m_pStatic;
	std::unique_ptr<CSStatic> m_pQCStatic;
	std::unique_ptr<CSComboBox> m_pComboOrder;
	std::unique_ptr<CButton>m_pCheckBtn[CHECKER_COUNT];
	std::unique_ptr<CxSkinButton>m_pSearchBtn;
	std::unique_ptr<CxSkinButton>m_pPlayBtn;
	std::unique_ptr<CxSkinButton>m_pCloseBtn;
#else
	CArray		<_mhjcode*, _mhjcode*>		m_shjCodes;
	CArray		<_mhjcode*, _mhjcode*>		m_elwbase;
	CStatic*	m_pStatic;
	CSStatic*	m_pQCStatic;
	CSComboBox*	m_pComboOrder;
	CButton*	m_pCheckBtn[CHECKER_COUNT];
	CxSkinButton*  m_pSearchBtn;
	CxSkinButton*  m_pPlayBtn;
	CxSkinButton*  m_pCloseBtn;
#endif
	CStringArray m_history;
	CWnd*		m_pParent;
	
	CRect		m_rcCheck[CHECKER_COUNT];
	CString		m_szCheckName[CHECKER_COUNT];
	bool		m_bCateChecked[CHECKER_COUNT];
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCategoryPopup)
	//}}AFX_VIRTUAL

// Implementation
public:
	
	bool IsFilteredHj(_mhjcode* hj);
#ifdef DF_USESTL_CATEGORY	
	void ListUpjongSort(int nidx, std::shared_ptr<_mhjcode>  hjc, int ntype = 0);
	std::shared_ptr<_mhjcode> FindCode(CString sCode);
#else
	void ListUpjongSort(int nidx,_mhjcode* hjc,int ntype=0);	
	_mhjcode* FindCode(CString sCode);
#endif
	void InitControls();
	void InitListCtrl();
	CString LoadStringTb(UINT nIdx);
	void	loadRank(HTREEITEM hItem);
	void	LoadGroup(CString path, HTREEITEM hItem);
	void	loadUCode(CString path, HTREEITEM	hItem);
	void	SearchCode();
	CString	Parser(CString &srcstr, CString substr);
	void	setParent(CWnd *pWnd);
	void	LoadFolder(CString path, HTREEITEM hItem, CString sSection, int nKey);
	void	LoadELWBase(CString path);
	void	baseCX_Category();
	virtual ~CCategoryPopup();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCategoryPopup)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnDestroy();
	afx_msg void OnChecked(UINT nID);
	afx_msg void OnPlayClk();
	afx_msg void OnCloseClk();
	afx_msg void OnSearchClk();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) ;	
	afx_msg void OnTreeDblClk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPaint();
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
#ifdef DF_USESTL_CATEGORY
	CArray <std::shared_ptr<_mhjcode>, std::shared_ptr<_mhjcode>> m_hjCodes;
	CArray <std::shared_ptr<CStringArray>, std::shared_ptr<CStringArray>> m_codeArr;
#else
	CArray		<_mhjcode*, _mhjcode*>		m_hjCodes;
	CArray		<CStringArray*, CStringArray*>	m_codeArr;
#endif
	CMapStringToString	m_codeMap;

	CString m_sCodeList;
	CString loadCodeList(int nGroup);
	void	loadHjCode();
	CFont   m_font;
public:
	void Initialize();
	HTREEITEM m_InterITEM;
	void InsertGroupList(CString sRet);
	void InsertCodeInGroup(CString sRet);
};

#define PT_CX 365
#define PT_CY 365

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CATEGORYPOPUP_H__3365E2BF_3265_4CDA_B4A9_1932F398A97C__INCLUDED_)

#pragma once
// MapWnd.h : header file
//

#include "../../h/axisfire.h"

#include <afxtempl.h>

#include "EditEx.h"
#include "InterMon.h"

class CfxStatic;
class CfxImgButton;
class CAccountCtrl;
class CfxEdit;
/////////////////////////////////////////////////////////////////////////////
// CMapWnd window

/*******************************************************************************
* 주식 잔고평가 조회 IO FORMAT
******************************************************************************/
struct  s_mid   {
    char    accn[11];
    char    pswd[8];
    char    cod2[12];
    char    allf[1];    /* 0.신용제외 1.All */
    char    fill[10];
};
#define L_smid	sizeof(struct s_mid)				// Length of stock input

struct  grid {
    char    rcod[7];        /* RTS CODE     */
    char    code[6];        /* 종목코드     */
    char    hnam[12];       /* 종목명       */
    char    curr[8];        /* 현재가       */
    char    diff[8];        /* 전일대비     */
    char    rate[7];        /* 대비         */
    char    gvol[10];       /* 거래량       */
    char    xrat[7];        /* 전일동시간 대비  */
};

struct mod {
    char    ssjm[7];        /* 상승 / 종목      */
    char    nrec[4];        /* # of records     */
    struct  grid    grid[200];
};

struct _code
{
	CString codeS;
	CString nameS;
	char	kosd{};			// 종목구분	// 0:장내  10:장외  30:제3시장
	char	ssgb{};			// 소속구분
	// 0:default
	// 4:뮤추얼펀드
	// 5:신주인수권
	// 6:리츠
	// 7:신주인수권증서
	// 8:ETF
	// 9:하이일드펀드
};

struct _fcode
{
	CString codeS;
	CString nameS;
};


#define		SJGOLIST				0
#define		SJGOROW					1
#define		GOOPKEY					2
#define		HOGALIST				3
#define		POOPKEY					4
#define		ORDERKEY				5
#define		LOGKEY					6

struct _trkey
{
	UINT	gubn:3;
	UINT	key:7;
};

struct	s_rec	{
	
	char	code[12];	// 종목코드
	char	hnam[40];	// 종목명
    char    jgyh[2];    /* 잔고유형         */
    char    jqty[10];   /* 잔고수량         */
    char    xqty[10];   /* 매도가능         */
    char    pamt[10];   /* 매입평균가       */
    char    mamt[15];   /* 매입금액         */
	
    char    curr[10];   /* 현재가     (*)   */
	char    diff[10];   /* 전일대비         */
	char    rate[10];   /* 등락률           */
    char    camt[15];   /* 평가금액         */
    char    tamt[15];   /* 평가손익   (*)   */
    char    srat[10];   /* 평가수익률 (*)   */
    char    sycd[2];    /* 신용코드         */
    char    sydt[8];    /* 대출일           */
    char    samt[15];   /* 신용금액         */
};

struct mid_aa {
    char    find[20];        /* 검색어 */
};

struct	s_mod	{
	char	accn[11];
	char	nrec[4];
	struct	s_rec	rec[1];
};


//struct _inters
//{
//	char	gubn[1];	// 종목구분	0:none, 1:현물, 2:선물, 3:옵션, 4:개별주식옵션, 5:지수
//	char	code[12];	// 종목코드
//	char	name[32];	// 종목명
//	char	xprc[10];	// 보유단가
//	char	xnum[10];	// 보유수량
//	char	xupnum[16];	// 상장 주식수
//	char	filler[24];	// Reserved
//};
#define	sz_inters	sizeof(struct _inters)

struct _listitem {		// tree1 선택시 보여지는 list1 item
	char	code[12];
	char	name[36];
	_inters* pInter;
};

#define	CHANGEACCOUNT			100
#define	tabCHAR					'/t'//0x09

#define CTRL_GAP 6

#define	BACKINDEXCOLOR	64
#define	TOOLINDEXCOLOR	66
#define	ROUND_PANEL		1
#define	ROUND_CONTENTS	2

#define	saveFILE	"portfolio.ini"
#define	tempFILE	"portfolio.ini.tmp"

#define STR_COMBODATA_ALL		"전체"
#define STR_COMBODATA_THEME		"섹터"
#define STR_COMBODATA_REMAINDER "잔고"
#define STR_COMBODATA_INTEREST  "관심종목"

////////////////////////// 
// 클래스명	    : CMapWnd
// 설 명	    : 전체,테마,잔고,관심종목에 대한 종명 및 엑셀관련 항목을 알아내고 
//				  엑셀 파일에 데이터를 보내주는 기능
// 작성일시	    : 2009/05/26
// 작성자       : kbkim(수정자)
// 관련요건	    : 000010(조원형:2009/05/12)
////////////////////////// 
class CMapWnd : public CWnd
{
// Construction
public:
	CMapWnd(CWnd* parent);

// Attributes
public:
	CWnd*	m_parent;

private:
	int sel_idx;
	CString	m_root;
	CFont*	m_font;
	COLORREF m_clrBG;
	CBrush	m_hbrush;

	std::unique_ptr<InterMon> inter;

	enum	ctype { cKospi=0, cKosdaq, cFutures, cOption, cIndex, cElw, cProgram, cTujaja } m_type;
	enum	Sel_type { SEL_ALL=0, SEL_THEME, SEL_REMAINDER, SEL_INTEREST, SEL_KRX} m_SearchType;
	enum	tId { xNONE = 0, xALLCODE, xKOSPI, xKOSDAQ, xINTEREST, xTHEMA, xGROUP, xFUTURE, xCALL, xPUT, xETF, xELW};
	enum	{ CG_NOT = 0, CG_KOSPI, CG_KOSDAQ, CG_FUTURE, CG_CALL, CG_PUT };

	std::unique_ptr < CComboBox>	m_pSelList;

	CArray<_code, _code> m_arCode;
	CStringList m_CodeList;
	
	// list control
	std::unique_ptr < CListCtrl>	m_code;		// all code
	std::unique_ptr < CListCtrl>	m_item;		// all item
	std::unique_ptr < CListCtrl>	m_scode;	// selected code
	std::unique_ptr < CListCtrl>	m_sitem;	// selected item

	// combobox
	std::unique_ptr < CComboBox>	m_rtype;	// right type
	std::unique_ptr < CComboBox>	m_bstock;	// base stock
	std::unique_ptr < CComboBox>	m_icorp;	// issue corp
	std::unique_ptr < CComboBox>	m_month;	// due month

	std::unique_ptr < CComboBox>	m_tujatype1;
	std::unique_ptr < CComboBox>	m_tujatype2;

	CString		m_strPal;
	HBRUSH      m_hRoundColor;
	COLORREF	m_clrRoundColor;
	
	// 전체
	std::unique_ptr < CButton>	m_pGrp1;
	std::unique_ptr < CButton>	m_pKospi;
	std::unique_ptr < CButton>	m_pKosdaq;
	std::unique_ptr < CButton>	m_pFuture;
	std::unique_ptr < CButton>	m_pOption;
	std::unique_ptr < CButton>	m_pIndex;
	std::unique_ptr < CButton>	m_pElw;
	std::unique_ptr<CButton>	m_pProgram;
	std::unique_ptr < CButton>	m_pTujaja;

	// 테마
	std::unique_ptr < CfxStatic>	m_pThemeLabel;
	std::unique_ptr < CEditEx>	m_pThemeSearch;
	std::unique_ptr < CComboBox>	m_pThemeList;

	// 잔고
	std::unique_ptr < CfxStatic>		m_pRemainderLabel;
	std::unique_ptr < CAccountCtrl>	m_pRemainderAcc;
	std::unique_ptr < CStatic>		m_pRemainderName;
	std::unique_ptr < CfxEdit>		m_pRemainderPassword;
	CWnd*			m_pWizard;
	CString			m_strAccount;
	CString			m_strPassword;

	// 관심종목
	std::unique_ptr < CfxStatic>	m_pInterestLabel;
	std::unique_ptr < CComboBox>	m_pInterestGroupList;
	CArray <_inters*,   _inters*>   m_inters;		// interest file_load

	// KRX
	std::unique_ptr < CfxStatic>	m_pKRXLabel;
	std::unique_ptr < CComboBox>	m_pKRXList;
	std::unique_ptr < CfxStatic>	m_pKRXName;

	// code add/del button
	std::unique_ptr < CfxImgButton>	m_code_addA;
	std::unique_ptr < CfxImgButton>	m_code_delA;
	std::unique_ptr < CfxImgButton>	m_code_add;
	std::unique_ptr < CfxImgButton>	m_code_del;

	// item add/del button
	std::unique_ptr < CfxImgButton>	m_item_addA;
	std::unique_ptr < CfxImgButton>	m_item_delA;
	std::unique_ptr < CfxImgButton>	m_item_add;
	std::unique_ptr < CfxImgButton>	m_item_del;

	// radio button for display
	std::unique_ptr < CButton>	m_grp2;
	std::unique_ptr < CButton>	m_horz;
	std::unique_ptr < CButton>	m_vert;
	std::unique_ptr < CButton>	m_number;	//2013.01.18 KSJ 셀서식 숫자변환 기능(기본 체크안됨)
	bool		m_bCheck;	//체크되었는지
	bool		m_bAddAll;		// 숫자입력시 매번 클리어 하는것 방지하기 위해

	// excel
	std::unique_ptr < CfxImgButton>	m_excel;

	// item string
	CString		m_itemKey;

	//코스피, 코스닥 종목검색
	std::unique_ptr < CfxStatic>	m_pNameLabel;
	std::unique_ptr < CEditEx>	m_pNameSearch;
	//KSJ

	CMapStringToPtr m_codedata;
//	CArray <_listitem*, _listitem*> m_listitem;		// m_list1 item
	//CArray <_listitem*, _listitem*> m_searchlist;		// m_list1 item
	std::vector<shared_ptr<_listitem>> m_listitem;
	std::vector<shared_ptr<_listitem>> m_searchlist;

// Operations
public:
	BOOL	CreateMap();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapWnd)
	public:
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMapWnd();

	// Generated message map functions
protected:
	COLORREF GetAxColor(int iColor);
	void	ChangeColors();
	CString GetPalette();
 	void	DrawRoundBitmap(CDC *pDC, CRect rc, int nKind);
 	void	DrawBitmap(CDC *pDC, CRect rc, CString strImage);

	void	CtrlAllHidden();
	void	CtrlAllShow();
	void	CtrlThemeShow();
	void	CtrlRemainderShow();
	void	CtrlInterestShow();
	void	CtrlKRXShow();

	void	CreateSearchCtrl();
	void	CreateSearchCtrl_All();
	void	CreateSearchCtrl_Theme();
	void	CreateSearchCtrl_Remainder();
	void	CreateSearchCtrl_Interest();
	void	CreateSearchCtrl_KRX();
	
	CString	m_sTabDir;
	BOOL	loadJCode();
	void	SelThemeList() ;
	CString ParserThemeList(CString &srcstr, CString substr);
	CString getCodeName(CString codeS, int& gubn);
	void	SetIndexCode();

	void	OnRemain();
	void	SetThemeComboList(CString strGetData);
	int		SplitString(CString strData, CString strToken, CStringArray &astrResult, int imaxcnt);

	CString Variant(int nComm, CString strData);
	CString Parser(CString &strSrc, char cDel);
	BOOL	SendTR(CString strTR, CString strData, int nKey, int nStat);
	void	SetThemeGroupName(mod *lsmod);
	void	SetRemainList(char* szReceive);
	BOOL	GetLedgerMessage(CString sLedger, CString& sMessage, BOOL& bNext);
	int		SetJangoFormat(CString sRows, CString& strCode, CString& strName);
	void	SetInterest();
	CString parseX(CString &srcstr, CString substr);
	int		loadingInterest(int gno);
	BOOL	ExistFile(int gno, bool temp);

	CBrush	m_br;
	void	create();
	CString	GetAxName(CString code);
	int		l_dudcode(int code);
	void	loadCode();
	void	loadItem();
	void	loadItemKey();

	void	addCode(CString code, CString name);
	BOOL	addColumn(CListCtrl* list, CString columns, int col, int width);
	BOOL	addItem(CListCtrl* list, CString items, CString subs, int item);
	void	removeAllItems();
	char* readCodeFile0(int* size, char* codefile);
	std::unique_ptr<char []> readCodeFile(int* size, char* codefile);

	void	addItem(CListCtrl* src, CListCtrl* dest, bool code, bool all = false);
	void	deleteItem(CListCtrl* dest, bool all = false);
	bool	checkDuplication(CListCtrl* dest, CString text);
	void	setElwCombo();
	void	settujaCombo();
	void	loadTUJA();
	void	search();
	void	runExcel();

	//종목 검색을위해 추가됨
	void	setSearchName();	//2013.02.28 KSJ 코스피, 코스닥 종목검색
	void	searchAction(int column);
	int		findMatchIndex(CString text);
	int		findMatchIndex(CString text, int column);
	void	AppendItemToList1(int ipos, CString code, CString name,  _listitem *pListItem);
	void	selcMark(CListCtrl* listctrl, int nItem, bool scroll = true);
	void	deselcMark(CListCtrl* listctrl);
	bool	SearchData(CString sName, bool bAddAll);
	WORD	MakeHangul(char cChar1, char cChar2);	// 2Byte문자로부터 한글을 맹글어주라~~
	bool	IsChosung(WORD wHangul, WORD &wStart, WORD &wEnd);	// 초성만 가진 한글인지~~
	//KSJ 

	BOOL	checkAddItem( CListCtrl* pSrc, CListCtrl* pDest, BOOL bAll );
	
	//CPtrArray m_arUPCODE;

	std::vector<shared_ptr<struct upcode>> m_arUPCODE;

	COLORREF getColor(int color);
	CFont*	getFont(int point, CString name = _T(""), int bold = FW_NORMAL, bool italic = false);
	HBITMAP	getBitmap(CString bmps);
	long	variant(int key, CString data = _T(""));
	int		getCodeType(CString strcode);
	CBrush* getAxBrush(COLORREF clr);
	COLORREF GetIndexColor(UINT nIndex)
	{
		if (!m_parent)
			return 0;
		
		if (nIndex & 0x02000000)
			return nIndex;

		return m_parent->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)nIndex);
	}
	//{{AFX_MSG(CMapWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSelChangeSearchCombo();
	afx_msg void OnSelChangeThemeList();
	afx_msg void OnSelChangeGroupList();
	afx_msg void OnSelChangeKRXTypeList();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	afx_msg LRESULT OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUser(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEditExMsg(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////


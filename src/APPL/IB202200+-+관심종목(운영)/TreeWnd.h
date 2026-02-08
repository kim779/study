#pragma once
// TreeWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTreeWnd window
#include "../IhUtils.h"
#include "../../h/jmcode.h"
#include "../../h/interst.h"
#include "Account.h"
#include "sharemsg.h"

struct _shjcode			// semi hjcode
{
	CString code;
	CString name;
	CString symb;		// symbol
	CString	ucdm;		// 업종중분류
	CString	ucds;		// 업종소분류
	CString acdl;		// 업종대분류
	char	ecng{};		// ECN 구분
	char	size{};		// 자본금규모	2:대, 3:중, 4:소
	char	jjug{};		// 제조업 구분(27: 제조업)
	char	kpgb{};		// KOSPI200 구분
	char	kosd{};		// 종목구분
	char	ssgb{};		// 소속구분
	char	ucmd{};		// 0:보통주, 5:우선주
	char	wsgb{};		// 0:일반 1:지배구조우수기업
	char	jsiz{};		// 업종자본금 대중소
	char	itgb{};		// KOSPI-IT/KQ-IT50구분(1:채택)
	char	star{};		// KOSDAQ 스타지수
	char	unio{};		// 통합지수 (KRX100)
	char	prmr{};		// 프리미어지수
	char    jchk{};		// 종목검색용 구분
				// 0x01 불성실공시종목
				// 0x02 신규상장종목
				// 0x04	권리락종목
				// 0x08	우선주종목
				// 0x10 관리종목
				// 0x20	감리종목
				// 0x40 거래정지종목
				// 0x80	투자유의종목
	char    nxtf{};		// nxt 거래소...
};

struct _efopcode
{
	char	type[1]{};			/*데이터 타입*/
	/*'1':운용사별*/
	/*'2':테마별*/
	/*'3':국내외 구분별*/
	/*'4':종목별*/
	char	code[6]{};			/*종목코드*/
	char	hnam[39]{};			/*종목명*/
	char	opcd[6]{};			/*운용사 코드*/
	char	etfm[1]{};			/*테마코드*/
	char	etfl[1]{};			/*국내외 구분*/
	char	lnfd[1]{};
};

struct _efopitem
{
	char	type[1]{};			/*데이터 타입*/
	/*'1':운용사별*/
	/*'2':테마별*/
	/*'3':국내외 구분별*/
	/*'4':종목별*/
	char	opcd[6]{};			/*운용사 코드*/
	char	hnam[30]{};			/*운용사 한글약명*/
	char	lnfd[1]{};			/*라인피드*/
	
	//	_efopcode *efopcode;
};


struct _efoptema
{
	char	type[1]{};			/*데이터 타입*/
	/*'1':운용사별*/
	/*'2':테마별*/
	/*'3':국내외 구분별*/
	/*'4':종목별*/
	char	tmcd[1]{};			/*테마코드*/
	char	tmnm[20]{};			/*테마명*/
	char	lnfd[1]{};			/*라인피드*/
};

struct _efopfore
{
	char	type[1]{};			/*데이터 타입*/
	/*'1':운용사별*/
	/*'2':테마별*/
	/*'3':국내외 구분별*/
	/*'4':종목별*/
	char	frcd[1]{};			/*국내외 구분('D':국내, 'F':해외)*/
	char	frnm[10]{};			/*국내외 구분명*/	//2015.11.18 KSJ 자릿수 4 -> 10
	char	lnfd[1]{};			/*라인피드*/
};

struct _elwdata
{
	CString		code;	// ELW 종목코드
	CString		name;	// 종목명
	CString		ktype;	// 권리유형 01:Call, 02:Put, 03:기타
	CString		pcode;	// 발행사 코드
	CString		pname;	// 발행사명
	CString		mjmt;	// 만기월 (YYYYMMDD)
	CString		bcode[5];
	BOOL		bkoba{};		// 조기종료ELW여부
};

struct _sfjcode			// semi fjcode
{
	CString code;
	CString name;
};

struct  _acInfo {
    char    accn[10]{};			// 계좌번호
    char    acnm[20]{};			// 계좌명
    char    aseq[2]{};			// 계좌순서
					// 00: 기본계좌
					// 01: 화면상에 보이지 않는계좌
    char    pwch[1]{};			// 은행연계계좌여부
};
#define	L_acInfo	sizeof(struct _acInfo)

struct  _acSet {
    char    func[1]{};			// FUNCTION CODE
					// 'Q' Query    'I' Insert
					// 'U' Update
    char    usid[12]{};			// 사용자아이디
    char    errc[1]{};			// Error Code
    char    emsg[80]{};			// Error message
    char    nrec[4]{};			// Account Count
    //struct  _acInfo acinfo[1];
};
#define	L_acSet		sizeof(struct _acSet)


class CAccn
{
public:
	bool    m_bFuture{};
	CString   m_accn;
	CString   m_pswd;
	CString   m_errmsg;
	bool    m_bCredit{};    // true:신용포함, false:신용제외
	int     m_dFee{}; //수수료제세금 적용 1 미적용 0
	double    m_dMass{}; //매체구분 0:hts 1:영업점/콜 2.ARS
	double    m_dSave{}; //매체구분 0:hts 1:영업점/콜 2.ARS
	
	CMapStringToPtr m_CodeMap;    // Ptr: F->class CRemainFuture
	//      C->class CRemain
	
public:
	CAccn()
	{
		m_bFuture = false;
		m_bCredit = false;
		m_accn = "";
		m_pswd = m_errmsg = "";
		m_dFee = 0; //수수료제세금 적용 1 미적용 0
		m_dMass = 0;
		m_dSave = 0;
		
		m_CodeMap.RemoveAll();
	}
};


class CTreeWnd : public CTreeCtrl
{
// Construction
public:
	CTreeWnd(CWnd* pMainWnd);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTreeWnd)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTreeWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTreeWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg LONG OnManage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	// szAt = _T("_") input at last szAt = _T("-") at first else ...
	BOOL IsFolder(HTREEITEM hItem);
	BOOL IsItem(HTREEITEM hItem);	
	void settingReminAcc();
	
	void clickRemain(UINT selItem);
	void clickRecommand(BOOL bRead);

//	void settingDataStock(char* buf, class CGridData& sdata);	// 주식잔고 데이터 빼내기
	void settingDataStock2(char* buf, class CGridData& sdata);	// 주식잔고 데이터 빼내기
	void settingDataFuture2(char* buf, class CGridData& sdata);	// 선물잔고 데이터 빼내기

	void sendUpjong();
	void ReSend();
	
private:
	int GetChildCount(HTREEITEM	hItem);
	CString CutCount(CString text);
	HTREEITEM FindChild(HTREEITEM hItem, CString text);

	void	loadingInterest(HTREEITEM hParent);
	void	selectinterest(HTREEITEM hParent, DWORD val);
	void	loadingHJcode();
	void	loadingFJcode();
	void	loadingOJcode();
	void	loadingPJcode();
	void	loadingUPcode();
	void	loadingELWBase();
	void	loadingCFcode(bool bCurrency);	//2012.10.04 KSJ 금리(6x), 통화(7x) 통화면 true
	void	loadingSFcode();	//2012.10.04 KSJ 주식선물

	void	queryRemain(CString strAccount);
	void	queryFutureRemain(CString strAccount);

	void	queryRemain2(CString strAccount, CString sPswd);
	void	queryFutureRemain2(CString strAccount, CString sPswd);
	BOOL    IsNumber(CString str);

	BOOL	sendTR(CString sTR, CString sData, int nKey, int nStat, CString Acnt);

	void	queryGroup();
	void	oubGroup(class CRecvData* data);
	void	oubAccUser(char* buf);
	void	oubAccList(char* buf, int len);
	void	oubAccListEx(char* buf, int len);
	void	oubRemain(char* buf, int len);
	void	oubRemainEx(char* buf, int len);
	void	init();
	void	initTree();
	BOOL	IsStockAcc(CString sAcc);
	
	void	saveGroupwhenClose(int index);

	BOOL	SendOper(UINT kind, int opt = 0);
	void	SetItemDataUJ(HTREEITEM hItem, UINT gubn);
	void	SetItemDataX(HTREEITEM hItem, UINT gubn);
	//bool	ExistFile(int gno);
	BOOL	CheckStock(UINT kind, _shjcode* hjcode); 
	BOOL	CheckSearch(_shjcode* hjcode, int opt);
	void	GetStock(class CGridData& sdata, int max, int opt);
	void	GetFuture(class CGridData& sdata, int max);
	void	GetOption(class CGridData& Sdata, int max);
	void	GetJisu(class CGridData& sdata, int max);
	void	GetPjcode(class CGridData& sdata, int max);
	void	GetInterest(class CGridData& sdata, int max);
	void	GetThema(class CGridData& sdata, int max);
	void	GetELWBase(class CGridData& sdata, int max);
	void	GetGroup(class CGridData& sdata, int max);
	void	GetRemain(class CGridData& sdata, int max);	

	void	GetCFuture(class CGridData& sdata, int max, bool bCurrency);	//2012.10.04 KSJ 금리/통화 통화일때는 true
	void	GetSFuture(class CGridData& sdata, int max);	//2012.10.04 KSJ 주식선물

	CString GetCodeName(CString code);


	double  Round(double data );		//반올림
	CString	MakePacket(CString& code, CString amount = _T(""), CString price = _T(""), CString name = _T(""), CString bookmark = _T(""), CString futureGubn=_T(""), CString creditPrc= _T(""), CString maeipPrc= _T(""), CString date = _T(""), CString bookrgb = _T(""), CString MemoYN = _T(""));
	// END MODIFY
	CString	makeAccn(CString accnnum, CString accnname);
	void	GetAccnTotal(CMapStringToString& mapACCN, CStringArray& arACCN, HTREEITEM hItem);
	void	GetAccnInGroup(CMapStringToString& mapACCN, CStringArray& arACCN, HTREEITEM hItem);
	CString	GetAccName(CString accn);
	CString	GetAccNum(CString accn);
	CString	GetAccGubn(CString accn);
	void	SendTreeData(const class CGridData& sdata);
	BOOL	SetItemData(HTREEITEM hItem, DWORD dwData);	
	BOOL	DeleteItem(HTREEITEM hItem);
	BOOL	DeleteAllItems();
	CString	GetDataTitle(DWORD dwData);
	bool	GetData(class CGridData& gdata, int max, int opt = 0);
	CString GetString(char *pChar, int nMinLen);
	CString Variant(int comm, CString param = _T(""));
	void	RecvOper(int kind, CRecvData* data);
	void	queryNews(int index = -1);
	void	queryAcc();							// 계좌조회
	void	queryStaffAcc(CString ledgerS);					// 직원계좌조회
	CString	getLedger(CString sTran, char cGubn, CString sSvcn, CString sLastKey = "");
	void	parsingNews(CRecvData* data);
	void	parsingRecommand(CRecvData* data);

	void	selectKospi();	
	void    SendInterest(HTREEITEM hItem);			//툴바에서 그룹 클릭시 작동
	void    saveGroupIndex(int index);				//선택 관심그룹 저장

	//ELW 발생회사별, 기초자산별 트리 추가로 인해 생성되는 모듈
	void	getELWBaseAsset(UINT selItem, HTREEITEM parent);	//ELW데이터 얻기 위해 Send날리는 함수//기초자산별
	void	getELWIssueSec(UINT selItem, HTREEITEM parent);		//ELW데이터 얻기 위해 Send날리는 함수//발행회사별
	void    setELWIssueSec(CRecvData* data);					//받은 데이터,종목만 짤라서 GridWnd에 보내기
	void    setELWBaseAsset(CRecvData* data);					//받은 데이터,종목만 짤라서 GridWnd에 보내기

	void	loadingETFcode();

	void	DataGubn(class CGridData& sdata,HTREEITEM hItem);	
	void	DataReloadItem(class CGridData& sdata,CString data);
	void	DataReloadTema(class CGridData& sdata,CString data);
	void	DataReloadFore(class CGridData& sdata,CString data);
	void	DataReloadELWAll(class CGridData& sdata,CString str, CString type);
private:

	CArray <_shjcode, _shjcode>	m_hjcode;		// 현물종목
	CArray <_sfjcode, _sfjcode>	m_fjcode;		// 선물종목
	CArray <ojcode, ojcode>		m_ojcode;		// 옵션종목
	CArray <pjcode, pjcode>		m_pjcode;		// 현물옵션
	CArray <upcode, upcode>		m_upcode;		// 업종코드	
	CArray <_sfjcode, _sfjcode>	m_elwbase;		// elw기초자산

	CArray <ccode, ccode>		m_cicode;		// 2012.10.04 KSJ 금리	
	CArray <ccode, ccode>		m_cccode;		// 2012.10.04 KSJ 통화	
	CArray <sfcode, sfcode>		m_sfcode;		// 2012.10.04 KSJ 주식선물	

	std::vector<std::unique_ptr<_efopitem>> _ETFitem;
	std::vector<std::unique_ptr<_efoptema>> _ETFtema;
	std::vector<std::unique_ptr<_efopfore>> _ETFfore;
	std::vector<std::unique_ptr<_efopcode>> _ETFcode;

	CIHStringMapArray		m_mapPJCODE;
	CMap<DWORD, DWORD, HTREEITEM, HTREEITEM>	m_mapParam;
	CWnd*		m_pMainWnd{};
	CString		m_root;
	CString		m_user;
	CString		m_id;
	CString		m_pass;
	char		m_gubn{};
	CString		m_sDeptCode;
	CString		m_szRET;
	
	bool		m_bCustomer{};
	bool		m_bOper{};
	UINT		m_kind{};
	UINT		m_selectTree{};
	UINT		m_selectTreetype{};
	HTREEITEM	m_selItem{};
	CArray <_elwdata, _elwdata> m_ELWdata;

	CStringArray	m_arJango;
	ACCOUNT		m_accn;			// 내 계좌목록 hold

	CImageList m_ilTree;
	HTREEITEM _treeInter{};
	std::vector<std::unique_ptr<_intersx>>	_vInters;
	UINT		m_ID{};


	std::array<CGridData, 120> _arrInter;

public:
	void makeInterGroup(std::vector<std::pair<CString, CString>>& vGroup);
	void receiveOub(CString& data, int key);
	CString m_slog;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
 

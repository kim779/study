#if !defined(AFX_CODEMONITOR_WND_H__80EE97FF_650D_41AE_B4E7_1474C0ECD953__INCLUDED_)
#define AFX_CODEMONITOR_WND_H__80EE97FF_650D_41AE_B4E7_1474C0ECD953__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CodeManagerWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCodeManagerWnd window

// 2012.2.1 박성경 >>
#include "../../../SrcSite/include/jmcode.h"
#include "../../../SrcSite/include/axisvar.h"
#include "../../../SrcSite/include/user.h"

// KRX 업종별 구분값
#define SEC1_AT       0x01  /* 자동차                   */
#define SEC1_SM       0x02  /* 반도체                   */
#define SEC1_HC       0x04  /* 건강                     */
#define SEC1_BK       0x08  /* 은행                     */
#define SEC2_IT       0x01  /* 정보통신                 */
#define SEC2_EC       0x02  /* 화학에너지               */
#define SEC2_ST       0x04  /* 철강                     */
#define SEC2_CS       0x08  /* 필수소비재               */
#define SEC3_MT       0x01  /* 미디어통신               */
#define SEC3_CO       0x02  /* 건설                     */
#define SEC3_FB       0x04  /* 금융서비스               */
#define SEC3_SI       0x08  /* 증권                     */
#define SEC4_SC       0x01  /* 조선                     */
#define SEC4_SR       0x02  /* 사회투자책임지수         */
#define SEC4_IS       0x04  /* 보험                     */
#define SEC4_TP       0x08  /* 운송                     */
#define SEC5_RT       0x01  /* 소비자유통               */
#define SEC5_LS       0x02  /* 레저                     */
#define SEC5_KX       0x04  /* KRX100                   */

//추가
template<class T> 
void QSortCArray( T& t, int (__cdecl *compare )(const void *elem1, const void *elem2 ) ) 
{ 	
    if( t.GetSize() <= 0 ) return; 
    qsort( t.GetData(), t.GetSize(), sizeof( t[0] ), compare ); 	
} 

// int 자료형비교함수 
int CompareChar( const void *elem1, const void *elem2 );

struct _shjcode			// semi hjcode
{
	CString code;
	CString name;
	CString symb;		// symbol
	char	ecng;		// ECN 구분
	char	size;		// 자본금규모	2:대, 3:중, 4:소
	char	ucdm[6];		// 업종중분류
	char	ucds[6];		// 업종소분류
	char	jjug;		// 제조업 구분(27: 제조업)
	char	kpgb;		// KOSPI200 구분
	char	kosd;		// 종목구분
	char	ssgb;		// 소속구분
	char	ucmd;		// 0:보통주, 5:우선주
	char	wsgb;		// 0:일반 1:지배구조우수기업
	char	jsiz;		// 업종자본금 대중소
	char	itgb;		// KOSPI-IT/KQ-IT50구분(1:채택)
	char	star;		// KOSDAQ 스타지수
	char	prmr;		// 프리미어지수
	char	unio;		// 통합지수 (KRX100)
	CHAR	uc10;		/* 건설기계		0x00			*/
						/* 조선운송		0x01			*/
						/* 철강소재		0x02			*/
						/* 에너지화학	0x03			*/
						/* 정보통신		0x04			*/
						/* 금융			0x05			*/
						/* 필수소비재	0x06			*/
						/* 자유소비재	0x07			*/
	char	uc20;		/* 우량기업		0x00			*/
						/* 벤처기업		0x01			*/
						/* 중견기업		0x02			*/
						/* 신성장기업	0x03			*/					
	/* 섹터지수                     */                                            
    char    sec1;       /*  자동차/반도체/건강/은행     */
    char    sec2;       /*  정보통신/화학E/철강/필수    */
    char    sec3;       /*  미디어/건설/금융/증권       */
    char    sec4;       /*  조선/사회투자/보험/운송     */
    char    sec5;       /*  소비자/레저/KRX100          */
};

struct _sjcode			// semi code
{
	CString code;
	CString name;
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
};

typedef struct _pbcode
{
	CString		code;	// 발행사 코드
	CString		name;	// 발행사명
}PBCODE;

typedef struct _sortStr
{
	CString code;
	CString name;
}SORTSTR;

// 해외 종목 정보
typedef struct _uscode
{
	char	symb[16];		/* 종목 SYMBOL			*/
	char	datg;			/* 대분류				*/
							/* 0: 지수	1: 종목		*/
							/* 2: 선물	3: 금리		*/
							/* 4: 국채	5: 환율		*/
							/* 7: others			*/
	char	jsgb;			/* 중분류(Combo Box)	*/
	char	knam[40];		/* 한글 SYMBOL명		*/
	char	enam[40];		/* 영문 SYMBOL명		*/
	char	icod[6];		/* 종목 업종코드		*/
	char	ncod[3];		/* 국가코드				*/
	char	exid[4];		/* 거래소코드			*/
}uscode;
/*****************************************************
 jsgb 구분값
 [지수]
 1. 미국	2. 아시아	3. 유럽		4. MSCI		5. 기타
 [종목]
 1. 미국	2. 일본		3. 홍콩		4. 대만		5. 영국		6. 프랑스		7. 독일		8. ADR		9. GDR
 [선물]
 1. CME지수선물			2.에너지	3. 상품선물
 [금리]
 1. 리보금리	2. 미국금리
 [국채]
 1. 미국	2. 일본		3. 영국		4. 독일
 [환율]
*****************************************************/
// 2012.2.1 박성경 <<

typedef struct _RealKey
{
	char    ucod[3];		/* 업종코드                     */
	char    rcod[5];		/* RealTime Code                */
	char    hnam[20];		/* 한글 업종명                  */
	char    fill[40];		/* filler                       */
	char    lnfd[1];		/* 라인피드                     */
} RealKey;

class CCodeManagerWnd : public CWnd
{
	// Construction
public:
	CCodeManagerWnd();

// Attributes
public:
	CString	m_szRootDir, m_szUserDir;
	CString m_strData;

// Operations
public:
	// 박성경 >>
	CArray <_shjcode, _shjcode>	m_hjcode;		// 현물종목
	CArray <_sjcode, _sjcode>	m_fjcode;		// 선물종목
	CArray <ojcode, ojcode>		m_ojcode;		// 옵션종목
	CArray <pjcode, pjcode>		m_pjcode;		// 현물옵션
	CArray <upcode, upcode>		m_upcode;		// 업종코드
	CArray <_sjcode, _sjcode>	m_elwbase;		// elwbase
	CArray <_elwdata, _elwdata> m_ELWdata;		// elw종목 정보
	CArray<PBCODE, PBCODE> m_arrayPBCode;		// elw 트리종목리스트
	CArray<SFCODEBASE, SFCODEBASE> m_sfbase;	// 주식선물 대표종목 정보
	CMap<CString,LPCSTR,SFCODEINFO,SFCODEINFO> m_mapsfinfo;	// 주식선물 트리종목리스트
	CMap<CString,LPCSTR,SFCODE,SFCODE> m_mapsfcode;	// 주식선물 종목정보
	CArray <uscode, uscode>		m_uscode;		// 해외종목 정보
	CArray <ccode, ccode>		m_ccode;		// 상품선물
	CMap<CString,LPCSTR,CString,CString> m_mapRealKey;		// 리얼키 정보
	CMap<CString,LPCSTR,hjcodex,hjcodex> m_maphjcode;	// ELW 존재하는 종목만 넣기위한 HJCode마스터 맵

	CArray <_sjcode, _sjcode>	m_mfcode;		// 미니선물종목
	CArray <ojcode, ojcode>		m_mocode;		// 미니옵션종목

	CArray <ojcode, ojcode>		m_wocode;		// 위클리옵션종목
	
	void	loadingHJcode();
	void	loadingFJcode();
	void	loadingOJcode();
	void	loadingPJcode();
	void	loadingUPcode();
	void	loadingELWcode();
	void	loadingSfCode();
	void	loadingUSCode();
	void	loadingCFcode();
	void	loadingMFcode();
	void	loadingMOcode();
	void	loadingWOcode();
	void	loadingRealKey();
	
	CString	GetCodeName(CString code);
	int FindPBCode(CString code);
	
	void GetAllCodeToList(CStringArray &arrCode, CStringArray &arrName);
	void GetKospiToList(CStringArray &arrCode, CStringArray &arrName);
	void GetKosdaqToList(CStringArray &arrCode, CStringArray &arrName);
	void GetKospi200ToList(CStringArray &arrCode, CStringArray &arrName);
	void GetKosdaq50ToList(CStringArray &arrCode, CStringArray &arrName);
	void GetKRX100ToList(CStringArray &arrCode, CStringArray &arrName);
	void GetFreeCodeToList(CStringArray &arrCode, CStringArray &arrName);
	void GetELWPBToList(CString strTitle, CStringArray &arrCode, CStringArray &arrName);
	void GetELWBaseToList(CString strTitle, CStringArray &arrCode, CStringArray &arrName);
	void GetETFCodeToList(CStringArray &arrCode, CStringArray &arrName);
	void GetSinjuCodeToList(CStringArray &arrCode, CStringArray &arrName);
	void GetFutureToList(CStringArray &arrCode, CStringArray &arrName);
	void GetCallCodeToList(CStringArray &arrCode, CStringArray &arrName);
	void GetPutCodeToList(CStringArray &arrCode, CStringArray &arrName);
	void GetKospiJisuToList(CStringArray &arrCode, CStringArray &arrName);
	void GetKosdaqJisuToList(CStringArray &arrCode, CStringArray &arrName);
	void GetKospiUpList(CStringArray &arrCode, CStringArray &arrName);
	void GetKosdaqUpList(CStringArray &arrCode, CStringArray &arrName);
	void GetKRXUpList(CStringArray &arrCode, CStringArray &arrName);
	void GetKNXToList(CStringArray &arrCode, CStringArray &arrName);
	void GetKRXJisuToList(CStringArray &arrCode, CStringArray &arrName);
	void GetKNXUpList(CStringArray &arrCode, CStringArray &arrName);
	void GetThemaList(CStringArray &arrCode, CStringArray &arrName);
	void GetPjcodeToList(CString code, char kind, CStringArray &arrCode, CStringArray &arrName);
	void GetKospiUpToList(int selItem, CStringArray &arrCode, CStringArray &arrName);
	void GetKosdaqUpToList(int selItem, CStringArray &arrCode, CStringArray &arrName);
	void GetKospiExToList(int selItem, CStringArray &arrCode, CStringArray &arrName);
	void GetKosdaqExToList(int selItem, CStringArray &arrCode, CStringArray &arrName);
	void GetThemaCodeToList(int selItem, CStringArray &arrCode, CStringArray &arrName);
	void GetGroupCodeToList(int selItem, CStringArray &arrCode, CStringArray &arrName);
	void GetInterestToList(int selItem, CStringArray &arrCode, CStringArray &arrName);
	void GetSFutCodeToList(CString strKey, CStringArray &arrCode, CStringArray &arrName);
	void GetELWBaseTreeList(CStringArray &arrCode, CStringArray &arrName);
	void GetELWPBTreeList(CStringArray &arrCode, CStringArray &arrName);
	void GetSFutTreeList(CStringArray &arrCode, CStringArray &arrName);
	void GetKRXToList(int selItem, CStringArray &arrCode, CStringArray &arrName);
	void GetKOSPI200Index(CStringArray &arrCode, CStringArray &arrName);
	void GetKOSPI100Index(CStringArray &arrCode, CStringArray &arrName);
	void GetForeignIndexTreeList(CStringArray &arrCode, CStringArray &arrName);
	void GetForeignItemTreeList(CStringArray &arrCode, CStringArray &arrName);
	void GetForeignFutureTreeList(CStringArray &arrCode, CStringArray &arrName);
	void GetForeignRateTreeList(CStringArray &arrCode, CStringArray &arrName);
	void GetForeignDebtTreeList(CStringArray &arrCode, CStringArray &arrName);
	void GetForeignExchangeTreeList(CStringArray &arrCode, CStringArray &arrName);
	void GetForeignToList(int selItem, CStringArray &arrCode, CStringArray &arrName);
	void GetKOFEXTreeList(CStringArray &arrCode, CStringArray &arrName, int nType=0);
	void GetKOFExCodeToList(int selItem, CStringArray &arrCode, CStringArray &arrName);
	void GetForeignMajorToList(CStringArray &arrCode, CStringArray &arrName);
	void GetFreeUpToList(CStringArray &arrCode, CStringArray &arrName);
	CString GetRecentChxUpCodeList(CString strCode);

	void GetEtnToList(CStringArray &arrCode, CStringArray &arrName);
	void GetVFToList(CStringArray &arrCode, CStringArray &arrName);
	void GetSF2UpList(CStringArray &arrCode, CStringArray &arrName);	
	void GetSF2CodeToList(CString strKey, CStringArray &arrCode, CStringArray &arrName);
	void GetSF2TreeList(CStringArray &arrCode, CStringArray &arrName);
	void GetETCUpToList(CStringArray &arrCode, CStringArray &arrName);
	
	void GetIBKMasterData(UINT selItem, CStringArray &arrCode, CStringArray &arrName);
	
	void AppendItemToList1(CString code, CString name, CStringArray &arrCode, CStringArray &arrName);
	
	CString	parseX(CString &srcstr, CString substr);
	bool	ExistFile(int gno, bool temp = false);
	// 박성경 <<

	void GetMiniFutureToList(CStringArray &arrCode, CStringArray &arrName);
	void GetMiniCallCodeToList(CStringArray &arrCode, CStringArray &arrName);
	void GetMiniPutCodeToList(CStringArray &arrCode, CStringArray &arrName);

	void GetWeeklyCallCodeToList(CStringArray &arrCode, CStringArray &arrName);
	void GetWeeklyPutCodeToList(CStringArray &arrCode, CStringArray &arrName);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCodeManagerWnd)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCodeManagerWnd();
	BOOL	OpenWindow();
	LPCSTR	GetUserDir();
	// Generated message map functions
protected:
	//{{AFX_MSG(CCodeManagerWnd)
	LRESULT OnUser(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	LRESULT _GetCode(LPARAM lParam);
	LRESULT	_GetCodeType(LPARAM lParam);
	LRESULT _GetCodeName(LPARAM lParam);
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CODEMONITOR_WND_H__80EE97FF_650D_41AE_B4E7_1474C0ECD953__INCLUDED_)

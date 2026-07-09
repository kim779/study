/*
#include <deque>
#include <stack>

#include <vector>
#include <set>

#include <algorithm>
#include <numeric>
#include <functional>

using namespace std;

typedef deque<int> INTDEQUE;  // 선언 예 .. int에 관한 deque를 쓸수 있다.
*/

// swap 사용 가능..

#define SAFE_DELETE(p) if(p) { delete p; p = NULL; }
#define COR_BLUE			RGB(0, 0, 255)
#define COR_RED				RGB(255, 0, 0)
#define COR_BLACK			RGB(0, 0, 0)
#define COR_WHITE			RGB(255, 255, 255)
#define USER_BACKCOLOR		RGB(222,219,222)
#define USER_BACKCOLOR2		RGB(199,206,223)
#define USER_BT_NORMALCOR	RGB(199,206,223)
#define USER_LINECOLOR		RGB(169,169,169)
#define USER_TEXT			RGB(45,54,139)
#define USER_FOCUS_COR		RGB(199, 203, 221)
#define COR_BACK			RGB(247,247,255)
#define COR_3DTOP			RGB(128,128,128)
#define COR_3DBOTTOM		RGB(221,221,209)


#define HJCODE_FILE		"hjcode.dat"
#define ELWCODE_FILE	"elwcod.dat"
#define CJCODE_FILE		"cjcode.dat"
#define SFCODE_FILE		"sfcode.dat"
#define PJCODE_FILE		"pjcode.dat"
#define	USCODE_FILE		"uscode.dat"
#define USLIST_FILE		"USLIST.INI"
#define FDCODE_FILE		"fdcode.dat"


//
//	user/userName폴더
//	file name : portfolio.i00 ~ portfolio.i99
//
// char gName[20];		// 그룹이름

struct _inters
{
	char	gubn;		// 종목구분
				// 0:none, 1:현물, 2:선물, 3:옵션, 4:개별주식옵션, 5:지수
	char	code[12];	// 종목코드
	char	name[30];	// 종목명
	int	possNum;	// 보유수량
	double	possPrice;	// 보유단가
	char	memo[30];	// 메모
	char	o_check;	// 주문 실행여부
				// 0:none, 1:checked
	char	o_kind;		// 주문구분
				// 1:매도, 2:매수
	char	o_mmgb;		// 매매구분
				// 1:보통, 2:시장가
	int	o_num;		// 주문수량
	double	o_price;	// 주문단가
	char	a_kind;		// 알람 설정
				// 1:그림만, 2:그림과 소리
	int	a_num;		// 조건 항목 갯수
	
	char	filler[100];	// Reserved
};

struct _alarms
{
	char	a_kind;		// 감시항목
				// 0:none, 1:현재가, 2:매도호가, 3:매수호가, 4:등락률, 5:손익평가, 6:수익률
	double	a_value;	// 감시 비교값
	char	a_condition;	// 감시 조건
				// 1:>=, 2:>, 3:=, 4:<, 5:<=
	char	filler[40];	// Reserved
};

typedef struct _listItem { 

	char*	Code[12];     //종목코드
	char*	idct[6];      //지표구분
	char*	mtry[6];      //만기구분
	char*	man[10];      //만기일
	char*	itrt[10];      //금리
	char*	intertype[6];      //이자유형
	char*	minfore[10];     //민평4
	char*	minthree[10];     //민평3
	char*	credit[6];     //신용등급
	char*	Name[100];    //종목명
	char*	valday[10];    //발행일
}LISTITEM;

typedef struct _listItem2 { 

	CString Code;
	CString	Name;
}LISTITEM2;

typedef struct PCode
{
	CString kind;
	CString name;
	CString code;
	CString hnam;
	CString month;
	CString price;
	CString	gcod;
	BOOL	atm;
	char	mchk;
}PCODE;

typedef struct FCode
{
	CString code;
	CString name;
	CString gnam;
	char	mchk;
}FCODE;

typedef struct  COCode  {                       /* 상품선물 종목코드 정보       */
        CString    codx;               /* 단축코드 (12)                */
        CString    hnam;               /* 한글명                       */
        CString    enam;               /* 영문명                       */
        char    filler;                 /*                              */
        CString    tjgb;                /* 기초자산 ID                  */
                                        /* B03 : 3년국채                */
                                        /* B05 : 5년국채                */
                                        /* B10 : 10년국채               */
                                        /* CDR : CD                     */
                                        /* MSB : 통안채                 */
                                        /* USD : USD                    */
                                        /* JPY : JPY                    */
                                        /* EUR : 유럽유로               */
                                        /* GLD : 금                     */
                                        /* PMT : 돈육                   */
                                        /* MGD : 미니금                 */
        CString    gcjs;               /* 기초자산 종목코드(7)         */
        CString    gnam;               /* 기초자산 종목명              */
        char    cmgb;                   /* 최근월물 구분                */
                                        /* 1 : 최근월 2 : 비해당        */
        CString    fill;               /* reserved                     */
}COCODE;


typedef struct _CJcode{
	CString code;	// 채권 코드... 
	CString name;	// 채권이름...
	CString		kind;  // 채권종류
	CString     idct;  //지표구분
	CString		valday;  //발행일
	CString		mtday; //만기일
	CString	    mtry; //만기구분	       
	CString		itrt;  //금리 
	CString		intertype; //이자유형
	CString		credit;  //신용등급
	CString		minthree;  //민평3
	CString		minfore;  //민평4
	CString		update;  //업데이트시각
}CJCODESTR;

typedef struct _cjallcode
{
	CString		code;
	char    gubn;	// 채권 구분... 
	CString		name;
	CString		idct;  // 지표  indicator
	CString	        mtry; //만기    maturity	       
	CString		mtday; //만기일
	CString		itrt;  //금리 interest rate
	CString		intertype; //이자유형
	CString		credit;  //신용등급
	CString		minthree;  //민평3
	CString		minfore;  //민평4
	CString		balday;   //발행일
}CJALLCODE;

typedef struct CUnBcode{
	CString code;	// 장외채권 코드... 
	char    gubn;	// 장외채권 구분... 
	CString name;	// 장외채권이름...
}CUNBCODE;

typedef struct Upcode
{
	int	 kind;
	CString	 ucod;
	CString  name;

}UPCODE;


struct  _FDCode {                       // 펀드코드 정보
	char	gubn[1];		// 구분
					// 1:채권형 2:채권혼합형
					// 3:주식형 4:주식혼합형
					// 5:MMF    6:해외펀드
	char	code[5];		// 펀드코드(5)
	char	hnam[30];		// 한글명
};

struct  _USCode {			/* 해외 SYMBOL 정보             */
        char    symb[16];               /* 종목 SYMBOL                  */
        char    datg;                   /* 데이타 구분                  */
                                        /* P: 미국지수  Q: 미국종목     */
                                        /* D: ADR       C: 선물         */
                                        /* F: CME 선물  G: GDR          */
                                        /* X: 환율      R: 금리         */
                                        /* L: 리보금리  B: 주요국정부채*/
                                        /* N: 국내금리  M: 반도체       */
                                        /* H: 세계주요종목 E: ECN       */
                                        /* W: 세계주가지수              */
        char    jsgb;                   /* 지수 편입 구분               */
                                        /* DOWJONES30 : 0x01            */
                                        /* NASDAQ100  : 0x02            */
                                        /* S&P500     : 0x04            */
        char    knam[48];               /* 한글 SYMBOL명                */
        char    enam[48];               /* 영문 SYMBOL명                */
        char    ncod[2];                /* 국가코드                     */
        char    excd[3];                /* 거래소코드                   */
};

struct JCode
{
		CString Code;
		CString Name;

		char	fill;
		char	stgb[2];	// 섹터지수 구분
		char	size;		// 자본금규모	2:대, 3:중, 4:소
		char	ucdm;		// 업종중분류
		char	ucds;		// 업종소분류
		char	jjug;		// 제조업 구분(27: 제조업)
		char	kpgb;		// KOSPI200 구분
							// 0:미포함 1:k200 2:k100 3:k50
		int	kosd;		// 종목구분
					// 0:장내  10:장외  30:제3시장
		char	ssgb;		// 소속구분
		char	itgb;		// IT...
		char	wsgb;		// 0:일반 1:지배구조우수기업
		char	ucmd;		// 0:보통주, 5:우선주
		char	star;		// KOSTAR
		char	unio;		// 통합지수
		char	jsiz;		// 업종자본금 대중소

		void*	pInf;		// elwInfo (struct _elwcode* pEcod)
};



typedef struct flist
{
	CString code;
	CString name;
}FLIST;

typedef struct favor
{
	CString fkey;
	CString name;
	int	count;

}FAVOR;

// ELW
typedef struct _elwcode
{
	CString		code;		// ELW 종목코드
	CString		name;		// 종목명
	CString		ktype;		// 권리유형 01:Call, 02:Put, 03:기타
	CString		pcode;		// 발행사 코드
	CString		pname;		// 발행사명
	CString		mjmt;		// 만기월 (YYYYMMDD)
	CString		bcode[5];	// 기초자산 코드들 "기초자산1;기초자산2;기초자산3;.."
}ELWCODE;

typedef struct _pbcode
{
	CString		code;		// 발행사 코드
	CString		name;		// 발행사명
}PBCODE;

typedef struct _basecode
{
	CString		code;		// 기초자산 코드
	CString		name;		// 기초자산명
}BASECODE;

typedef struct _ncode
{
	CString		code;
	CString		name;
	CString		kind;
	CString		fill;

}NCODE;

template <class T, class PT> class CTArray : public CArray <T, PT>
{
public:
	void QuickSort(BOOL bAscending = TRUE);
};

template <class T, class TP> void CTArray<T,TP>::QuickSort(BOOL bAscending/* = TRUE*/)
{
	if (GetSize() > 1)
	{
		::QuickS(GetData(), (int)GetSize(), bAscending);
	}
}

template <class T> BOOL QuickS(T *pArr, int iSize, BOOL bAscending = TRUE)
{
	BOOL rc = TRUE;

	if (iSize > 1) 
	{
		try 
		{
			int	low = 0,
				high = iSize - 1;

			QuickSortRe(pArr, low, high, bAscending);
		} 
		catch (...) 
		{
			::SetLastError(ERROR_INVALID_PARAMETER);
			rc = FALSE;
		}
	} 
	else 
	{
		::SetLastError(ERROR_INVALID_PARAMETER);
		rc = FALSE;
	}

	return rc;
}

template <class T> void QuickSortRe(T *pArr, int low, int high, BOOL bAscending)
{
	int ii, jj;
	T str;
	T TTemp;

	ii = high;
	jj = low;

	str = pArr[((int) ((low+high) / 2))];

	do {
		if (bAscending) 
		{
			//while (pArr[jj].Compare(str) < 0) jj++;
			//while (pArr[ii].Compare(str) > 0) ii--;

			while (strcmp(pArr[jj].price, str.price) < 0) jj++;
			while (strcmp(pArr[ii].price, str.price) > 0) ii--;
		}
		else
		{
//			while (pArr[jj].Compare(str) > 0) jj++;
//			while (pArr[ii].Compare(str) < 0) ii--;
			while (strcmp(pArr[jj].price, str.price) > 0) jj++;
			while (strcmp(pArr[ii].price, str.price) < 0) ii--;
		}

		if (ii >= jj) 
		{
			if (ii != jj) 
			{
				TTemp = pArr[ii];
				pArr[ii] = pArr[jj];
				pArr[jj] = TTemp;
			}
			ii--;
			jj++;
		}
	} while (jj <= ii);

	if (low < ii) QuickSortRe(pArr, low, ii, bAscending);
	if (jj < high) QuickSortRe(pArr, jj, high, bAscending);

}


template <class T, class PT> class CQArray : public CArray <T, PT>
{
public:
	void QuickSort(BOOL bAscending = TRUE);
};

template <class T, class TP> void CQArray<T,TP>::QuickSort(BOOL bAscending/* = TRUE*/)
{
	if (GetSize() > 1)
	{
		::QuickSort(GetData(), (int)GetSize(), bAscending);
	}
}

template <class T> BOOL QuickSort(T *pArr, int iSize, BOOL bAscending = TRUE)
{
	BOOL rc = TRUE;

	if (iSize > 1) 
	{
		try 
		{
			int	low = 0,
				high = iSize - 1;

			QuickSortRecursive(pArr, low, high, bAscending);
		} 
		catch (...) 
		{
			::SetLastError(ERROR_INVALID_PARAMETER);
			rc = FALSE;
		}
	} 
	else 
	{
		::SetLastError(ERROR_INVALID_PARAMETER);
		rc = FALSE;
	}

	return rc;
}

template <class T> void QuickSortRecursive(T *pArr, int low, int high, BOOL bAscending)
{
	int ii, jj;
	T str;
	T TTemp;

	ii = high;
	jj = low;

	str = pArr[((int) ((low+high) / 2))];

	do {
		if (bAscending) 
		{
			while (pArr[jj].Compare(str) < 0) jj++;
			while (pArr[ii].Compare(str) > 0) ii--;
		}
		else
		{
			while (pArr[jj].Compare(str) > 0) jj++;
			while (pArr[ii].Compare(str) < 0) ii--;
		}
		if (ii >= jj) 
		{
			if (ii != jj) 
			{
				TTemp = pArr[ii];
				pArr[ii] = pArr[jj];
				pArr[jj] = TTemp;
			}
			ii--;
			jj++;
		}
	} while (jj <= ii);

	if (low < ii) QuickSortRecursive(pArr, low, ii, bAscending);
	if (jj < high) QuickSortRecursive(pArr, jj, high, bAscending);

}

//-------------------------------------------------------------------------------------------

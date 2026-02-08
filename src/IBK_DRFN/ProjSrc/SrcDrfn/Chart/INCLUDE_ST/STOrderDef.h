#ifndef __STORDER_DEF__BYALZIO__060914__HEADER__
#define __STORDER_DEF__BYALZIO__060914__HEADER__

// #include "../Include_ST/STOrderDef.h"

#define		LANGUAGE_MAX			180000

typedef struct _ST_ORDERSIGN
{
	char In[5];
	char AcntNo[20];                      // 계좌번호
	char InptPwd[8];					  // 입력비밀번호
	char IsuNo[12];						  // 종목번호
	long OrdQty;						  // 주문수량
	double OrdPrc;						  // 주문가
	char BnsTp[1];						  // 매매구분
	char OrdprcPtnCode[2];				  // 호가유형코드
	char PrgmOrdprcPtnCode[2];			  // 프로그램호가유형코드
	char StslAbleYn[1];					  // 공매도가능여부
	char StslOrdprcTp[1];				  // 공매도호가구분
	char CommdaCode[2];					  // 통신매체코드
	char MgntrnCode[3];					  // 신용거래코드
	char LoanDt[8];						  // 대출일
	char MbrNo[3];						  // 회원번호
	char OrdCndiTp[1];					  // 주문조건구분
	char StrtgCode[6];					  // 전략코드
	char GrpId[20];						  // 그룹ID
	long OrdSeqNo;						  // 주문회차
	long PtflNo;						  // 포트폴리오번호
	long BskNo;							  // 바스켓번호
	long TrchNo;						  // 트렌치번호
	long ItemNo;						  // 아이템번호
	char OpDrtnNo[12];					  // 운용지시번호
	char Out[5];
//	long OrdNo;							  // 주문번호
} ST_ORDERSIGN;

typedef struct {
	int		nMarket;				// 0:주식, 1:선물/옵션
	int		nOrderType;				// VALENTRYSELL 매수진입, VALEXITLONG 매수청산, VALEXITSHORT 매도청산, VALENTRYBUY 매도진입
	char	aAcctNo[20+1];			// 계좌번호
	char	aAcctName[30+1];		// 계좌명
	char	aCode[12+1];			// 종목코드
	char	aCodeName[30+1];		// 종목코드명
	int		nOrderGubun;			// 주문구분, 0:지정가, MARKETENTRY : 진입시장가, MARKETCLEAR : 청산시장가.
	char	aOrderAmt[16+1];		// 주문수량
	// char	aOrderPrice[20+1];		// 주문가격
	short	nOrderPriceTypeEnt;		// 주문가격타입. 진입콤보의 인덱스
	short	nOrderPriceTypeClr;		// 주문가격타입. 청산콤보의 인덱스
	//-----------------------
	int		nSTType;				// 전략의 유형, 제공전략(0), 사용자 전략(1)
	char	szSTName[MAX_PATH];		// 전략명
	short	nAutoOrder;				// 자동주문? 0 : 주문없음 1/반자동 2/자동
} STORDER_INDATA;

#define		SZSTORDER_INDATA	sizeof(STORDER_INDATA)

typedef  int (WINAPI* FpSTOrder_DoModal)(long pSTORDER_INDATA);
typedef  int (WINAPI* FpSTOrder_Sign)(long, float);

class CStrategyOrderLoader
{
public:
	HINSTANCE m_hLib;
	CString m_szLibName;
	FpSTOrder_DoModal	m_fpSTOrder_DoModal;
	FpSTOrder_Sign	m_fpSTOrder_Sign;

public:
	CStrategyOrderLoader()
	{
		m_hLib = NULL;
		m_fpSTOrder_DoModal = NULL;
		m_szLibName = "StrategyOrder.dll";
	}

	~CStrategyOrderLoader()
	{
		UnLoadLib();
	}


	HINSTANCE LoadLib()
	{
		HINSTANCE hLib = NULL;
		hLib  = (HINSTANCE) ::LoadLibrary((LPCTSTR)m_szLibName);
		if( hLib == NULL)
		{
			CString szMsg;
			szMsg.Format("%s을 Load할 수 없습니다.", m_szLibName);
			TRACE("%s\n", szMsg);
			return NULL;
		}
		m_hLib = hLib;
		LoadProcess();

		return hLib;
	}

	BOOL UnLoadLib()
	{
		if(m_hLib == NULL)  return TRUE;

		BOOL bResult = ::FreeLibrary(m_hLib);
		m_hLib = NULL;

		return bResult;
	}


	void LoadProcess()
	{
		if(!m_hLib)	return ;

		m_fpSTOrder_DoModal = (FpSTOrder_DoModal)::GetProcAddress(m_hLib, "STOrder_DoModal");
		m_fpSTOrder_Sign = (FpSTOrder_Sign)::GetProcAddress(m_hLib, "STOrder_Sign");
	}

	int	STOrder_DoModal(long pSTORDER_INDATA)
	{
		if(m_hLib==NULL && LoadLib()==FALSE) return -1;	// DLL로드에러
		if(pSTORDER_INDATA==NULL) return -1;			// 입력값에러

		return m_fpSTOrder_DoModal(pSTORDER_INDATA);
	}

	int	STOrder_Sign(long pSTORDER_INDATA, float fHogaData)
	{
		if(m_hLib==NULL && LoadLib()==FALSE) return -1;	// DLL로드에러
		if(pSTORDER_INDATA==NULL) return -1;			// 입력값에러

		return m_fpSTOrder_Sign(pSTORDER_INDATA, fHogaData);
	}
};

//#define		STORDER_TITLE_0		"시스템트레이딩 매수진입"//VALENTRYBUY
//#define		STORDER_TITLE_1		"시스템트레이딩 매수청산"//VALEXITLONG
//#define		STORDER_TITLE_2		"시스템트레이딩 매도청산"//VALEXITSHORT
//#define		STORDER_TITLE_3		"시스템트레이딩 매도진입"//VALENTRYSELL
//#define		STORDER_TITLE_4		"시스템트레이딩 매도/매수청산" //VALSELL
//#define		STORDER_TITLE_5		"시스템트레이딩 매수/매도청산" //VALBUY
typedef struct _PROCEED_ORDER
{
	LPVOID pThis;
	int  nOrderIndex;
	int nOrderQty;				// 신규 주문 가능수량
	int nClearQty;				// 청산 주문 가능수량
	float fOrderPriceEnt;
	float fOrderPriceClr;
}PROCEED_ORDER;


//typedef struct	tagSIGNALORDERSINFO
//{
//	long    hDLLWnd;				//	 호출한 컨트롤 핸들
//	TCHAR	szIndexNo[2+1];		// - DLL Index번호 (예, 11(주식시세), 12(선옵시세), 13(지수시세), 14(주식스탑), 15(선옵스탑), 16(시스템트레이딩)
//	TCHAR	szWindName[8+1];		// - 신호이름 ("주식시세", "선옵시세", "선옵스탑", "조건검색" .....)
//	TCHAR	szSignalgb[2+1];		// - 신호구분 ("감", "스" ....., "시")
//	TCHAR	szPopgb[1+1];			// - 팝업Type(0:미실행, 1:알림창, 2:주문창,  4:자동주문)
//	TCHAR	szActionGb[2+1];		// - 버튼 처리 구분값 (1:주문, 2:취소, 3:조건해제) : x
//	TCHAR	szAcctno[11+1];			// - 계좌번호
//	TCHAR	szCode[12+1];			// - 종목코드
//	TCHAR	szLoangb[1+1];			// - 신용구분(0:현금, 1:신용)
//	TCHAR	szSygb[2+1];		  	// /*  신용구분                        */
//	TCHAR	szJanGogb[1+1];			// - 잔고구분(0:전체, 1:매도, 2:매수)
//	TCHAR	szMemegb[1+1];			// - 매매구분(1:매도, 2:매수, 3:정정, 4:취소, 5:신용매도, 6:신용매수 )
//	TCHAR	szOrderNo[5+1];			// - 주문번호
//	TCHAR	szOrgNo[5+1];			// - 원주문번호
//	TCHAR	szOrdgb[2+1];			// - 주문구분(보통가, 시장가,...)
//	TCHAR	szOrdqtygb[1+1];		// - 주문수량구분(0:수량, 1:금액, 2:매도가능수량%)
//	TCHAR	szOrdqty[15+1];			// - 주문수량
//	TCHAR	szOrdprcgb[1+1];		// - 주문단가구분(0:지정가...(인덱스))
//	TCHAR	szOrdprc[7+1];			// - 주문단가
//	TCHAR	szOrdRow[3+1];			// - 감시된 (스탑 걸린) ROW
//	TCHAR	szLoanDate[8+1];		// - 대출일자
//	TCHAR	szLoanType[1+1];		// - 대출구분	(0:자기, 1:유통)	
//	TCHAR	szbContinue[1+1];		// - 감시 진행여부
//	TCHAR	szMsg[512+1];			// - 감시MSG
//	TCHAR	szOrdMsg[512+1];		// - 감시MSG
//	
//// --> [Edit]  김정원 2008/07/07
//	// 스탑 로스 주문 로그 
//	TCHAR	szMenhceCancel[1+1];		// 미체결전송여부 (0:미체결전송안함, 1:미체결전송함)
//	TCHAR	szMenhceCancelSuccess[1+1];	// 미체결전송여부 (0:미체결 실패, 1:미체결 성공)
//// <-- [Edit]  김정원 2008/07/07
//
//// --> [Edit]  이용준 2008/05/23
//	TCHAR	szAccName[30+1];		// - 계좌명
//	TCHAR	szJmName[20+1];			// - 종목명
//	TCHAR	szRealOrdqty[15+1];		// - 실제주문수량
//	TCHAR	szRealOrdprc[7+1];		// - 실제주문단가
//// <-- [Edit]  이용준 2008/05/23
//} SIGNALORDERSINFO;


typedef struct	tagSTORDERINFO
{
	TCHAR	szPopgb[1+1];			// - 팝업Type(1 : 주문입력창만)
	TCHAR	szCode[12+1];			// - 종목코드
	TCHAR	szAcctno[11+1];			// - 계좌번호
	TCHAR	szMemegb[1+1];			// - 매매구분(0:매수, 1:매도)
	TCHAR	szOrdgb[2+1];			// - 주문구분(00:보통가, 03:시장가,...)
	TCHAR	szOrdqty[15+1];			// - 주문수량
	TCHAR	szOrdprc[10+1];			// - 주문단가
} STORDERINFO;

#define WMU_SIGNAL_CHART		(WM_USER + 659)	// 신호알리미 - 차트
typedef struct _stSTORDERMSGINFO
{
	char szTime[6];			// 발생시간
	char szItemCode[12];	// 종목코드
	char szItemName[30];	// 종목명
	char szSTName[40];		// 전략명
	char szOrderType[1];	// 매매구분(0:매도, 1:매수)
	char szAccount[11];		// 주문계좌번호
	char szOrdgb[2];		// 주문구분(1:시장가)
	char szOrdqty[15];		// 주문수량
	char szOrdprc[10];		// 주문단가
} stSTORDERMSGINFO;
#endif //__STORDER_DEF__BYALZIO__060914__HEADER__
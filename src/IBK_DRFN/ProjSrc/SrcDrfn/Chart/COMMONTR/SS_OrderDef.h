#if !defined(AFX_SAMSUNG_ORDERDEF_H_73649_BY_98B1A__INCLUDED_)
#define AFX_SAMSUNG_ORDERDEF_H_73649_BY_98B1A__INCLUDED_

// #include "../src/SSSTOrder/SS_OrderDef.h"
// 이 파일은 삼성의 주문관련 패킷을 정의한 파일임.
// 다른 사이트에는 맞지않음.
#define		STORDER_TITLE_0		"시스템트레이딩 매수진입"
#define		STORDER_TITLE_1		"시스템트레이딩 매수청산"
#define		STORDER_TITLE_2		"시스템트레이딩 매도청산"
#define		STORDER_TITLE_3		"시스템트레이딩 매도진입"
#define		STORDER_TITLE_4		"시스템트레이딩 강제청산"

////////////////////////////////////////////////////////////////////////////////////////
// Input structure                                                                   
//*표는 공백을 의미
////////////////////////////////////////////////////////////////////////////////////////

// -------------------------------------------------------------------------------------
//			선물고객용(HFIBOA0F2000)
// -------------------------------------------------------------------------------------
// HFIBOA0F2000InRec: 선물옵션정상주문                                               
/*
typedef struct
{
    char    InItem1[1];                           // #1(1)              1            
    char    InItem2[1];                           // 거래구분(1)        1            
    char    InItem3[10];                          // 계좌번호(10)       5001448503  
    char    InItem4[8];                           // 비밀번호(8)        aa1122**     
    char    InItem5[8];                           // 종목코드(8)        101A9***     
    char    InItem6[1];                           // 주문유형(1)        1    '1'지정가, '2'시장가
    char    InItem7[1];                           // #3(1)              3            
    char    InItem8[9];                           // 주문수량(9)        000000001    
    char    InItem9[13];                          // 주문가격(13)       173.00*******
    char    InItem10[2];                          // #10(2)             10           
}fn_HFIBOA0F2000_InRec1;


////////////////////////////////////////////////////////////////////////////////////////
// Output structure                                                                  
////////////////////////////////////////////////////////////////////////////////////////

// HFIBOA0F2000OutRec1: 선물옵션정상주문                                             
typedef struct
{
    char    OutItem1[1];                          // 처리구분OUT(1)        1         
    char    OutItem2[1];                          // 매매구분OUT(1)        2         
    char    OutItem3[10];                         // 계좌번호OUT(10)       5001448503          
    char    OutItem4[8];                          // 계좌비밀번호(8)       
    char    OutItem5[8];                          // 종목코드OUT(8)        101A9               
    char    OutItem6[1];                          // 주문유형OUT(1)        1         
    char    OutItem7[1];                          // 거래유형OUT_F200(1)   3         
    char    OutItem8[9];                          // 주문수량OUT(9)        000000001              
    char    OutItem9[13];                         // 주문가격OUT(13)       173.00          
    char    OutItem10[2];                         // 통신구분OUT(2)        10        
    char    OutItem11[7];                         // 주문번호(7)           0000001   
    char    OutItem12[20];                        // 계좌명(20)            테스트병    
}fn_HFIBOA0F2000_OutRec1;
*/

// -------------------------------------------------------------------------------------
//			선물 직원용 선물주문(HFIJOA0F1100)
// -------------------------------------------------------------------------------------
/*
typedef struct
{
	char	InItem1[11];							// 계좌번호
    char    InItem2[ 8];							// 비밀번호
    char    InItem3[ 1];							// 주문구분
    char    InItem4[12];							// 종목번호
    char    InItem5[ 7];							// 매매수량
    char    InItem6[ 6];							// 매매단가
    char    InItem7[ 1];							// 매매구분
	char    InItem8[ 2];							// 통신주문구분 ?
}fn_HFIJOAOF1100_InRec1;

typedef struct
{
    char    OutItem1[11];							// 계좌번호OUT
    char    OutItem2[10];							// 지점명OUT
	char    OutItem3[20];							// 계좌명OUT
	char    OutItem4[12];							// 종목번호OUT
	char    OutItem5[20];							// 종목명OUT
	char    OutItem6[ 7];							// 매매수량OUT
	char    OutItem7[ 6];							// 매매가격OUT
	char    OutItem8[ 5];							// 주문번호OUT
	char    OutItem9[ 1];							// 통신주문구분OUT
	char    OutItem10[ 8];							// 통신주문구분명OUT
	char    OutItem11[ 1];							// 매매구분OUT
	char    OutItem12[12];							// 매매구분명OUT
}fn_HFIJOAOF1100_OutRec1;
*/

// 직원용 선물옵션정상주문 Input structure
// #3001 화면참조.
/*
typedef struct
{
    char    InItem1[1];                           // 처리구분(1)  
    char    InItem2[1];                           // 거래구분(1)  
    char    InItem3[10];                          // 계좌번호(10) 
    char    InItem4[8];                           // 비밀번호(8)  
    char    InItem5[8];                           // 종목코드(8)  
    char    InItem6[1];                           // 주문유형(1)  
    char    InItem7[1];                           // #3(1)        
    char    InItem8[9];                           // 주문수량(9)  
    char    InItem9[13];                          // 주문가격(13) 
    char    InItem10[2];                          // #00(2)       
}fn_HFIJOA0F0011_InRec1;

// 직원용 선물옵션정상주문 Output structure
typedef struct
{
    char    OutItem1[7];                          // 주문번호(7)
    char    OutItem2[20];                         // 계좌명(20) 
}fn_HFIJOA0F0011_OutRec1;

// -------------------------------------------------------------------------------------
//			주식고객용 현금매수
// -------------------------------------------------------------------------------------
typedef struct
{
    char    InItem1[10];							// 계좌번호(10)
    char    InItem2[ 8];							// 비밀번호(08)
    char    InItem3[12];							// 종목코드(12) : 첫자리는 'A'
    char    InItem4[ 9];							// 매매수량(09)
    char    InItem5[ 9];							// 매매단가(09)
    char    InItem6[ 2];							// 매매구분(02)
    char    InItem7[ 2];							// 주문조건(02)
    char    InItem8[15];							// 전화번호(15)
    char    InItem9[ 2];							// 통신주문구분(02)
}fn_HFIBOA0Y3200_InRec1;

typedef struct
{
    char    OutItem1 [ 8];							// 카드번호(08)
    char    OutItem2 [ 2];							// 계좌구분(02)
    char    OutItem3 [20];							// 계좌명(20)
    char    OutItem4 [12];							// 종목번호(12)
    char    OutItem5 [20];							// 종목명_매(20)
    char    OutItem6 [ 9];							// 매매수량2(09)
    char    OutItem7 [ 9];							// 매매가격2(09)
    char    OutItem8 [ 5];							// 주문번호2(15)
    char    OutItem9 [ 2];							// 매매구분2(02)
	char    OutItem10[20];							// 매매구분명(20)
	char    OutItem11[ 2];							// 주문조건OUT(02)
	char    OutItem12[ 2];							// 통신주문구분2(2)
	char    OutItem13[10];							// 통신주문구분명(10)
}fn_HFIBOA0Y3200_OutRec1;

#define		fn_HFIBOA0Y3100_InRec1			fn_HFIBOA0Y3200_InRec1
#define		fn_HFIBOA0Y3100_OutRec1			fn_HFIBOA0Y3200_OutRec1

// -------------------------------------------------------------------------------------
//			직원용 현금매수
//			hfij20620 화면참조. 주식호가주문
// -------------------------------------------------------------------------------------
typedef struct
{
    char    InItem1[ 1];							// 프로세스타입, !2
    char    InItem2[10];							// 계좌번호(10)
    char    InItem3[ 8];							// 비밀번호(08)
    char    InItem4[12];							// 종목코드(12) : 첫자리는 'A'
    char    InItem5[ 9];							// 매매수량(09)
    char    InItem6[ 9];							// 매매단가(09)
    char    InItem7[ 2];							// 매매구분(02)
    char    InItem8[ 2];							// 주문조건(02)
    char    InItem9[15];							// 전화번호(15)
	char    InItem10[2];							// 통신주문구분(02), !00
}fn_HFIJOA0T1131_InRec1;

typedef struct
{
    char    OutItem1 [ 5];							// 주문번호(매수)
    char    OutItem2 [13];							// 전일재사용금액
    char    OutItem3 [13];							// 금일재사용금액
    char    OutItem4 [13];							// 증거현금(매수)
    char    OutItem5 [13];							// 증거대용(매수)
    char    OutItem6 [13];							// 재사용담보금액
    char    OutItem7 [13];							// 전일재사용가능금액
    char    OutItem8 [13];							// 금일재사용가능금액
    char    OutItem9 [13];							// 대용금액
	char    OutItem10[13];							// 전일증거금액(매수)
	char    OutItem11[13];							// 전일담보(매수)
	char    OutItem12[20];							// 계좌명(매수)
}fn_HFIJOA0T1131_OutRec1;

// -------------------------------------------------------------------------------------
//			직원용 현금매수(HFIJOA0Y6100)
// -------------------------------------------------------------------------------------
// 현금매수 In

typedef struct
{
	char    InItem1[ 1];							// #1(1)
    char    InItem2[10];							// 카드번호(10)
    char    InItem3[ 8];							// 비밀번호(08)
    char    InItem4[ 1];							// #A(1)
    char    InItem5[ 5];							// 종목코드(05)
    char    InItem6[ 6];							// #(06)
	char    InItem7[ 9];							// 매매수량(9)
	char    InItem8[ 9];							// 매매단가(9)
    char    InItem9[ 2];							// 매매구분(02)
	char    InItem10[15];							// 전화번호(15)
	char    InItem11[ 2];							// #00(2)
}fn_HFIJOA0Y6100_InRec1;

// 현금매수 Out
typedef struct
{
    char    OutItem1 [ 3];							// #3
    char    OutItem2 [ 2];							// 주문번호2
    char    OutItem3 [138];							// #138
    char    OutItem4 [20];							// 종목명
    char    OutItem5 [20];							// 지점명
    char    OutItem6 [ 1];							// #1
    char    OutItem7 [ 8];							// 카드번호2
    char    OutItem8 [ 2];							// 계좌구분2
    char    OutItem9 [ 8];							// #8
	char    OutItem10[12];							// 종목번호
	char    OutItem11[ 9];							// 매매수량2
	char    OutItem12[ 9];							// 매매가격2
	char    OutItem13[ 2];							// 매매구분2
	char    OutItem14[15];							// #15
	char    OutItem15[ 2];							// 통신주문구분2
}fn_HFIJOA0Y6100_OutRec1;
*/

// -------------------------------------------------------------------------------------
//			고객용 현금매수(HFIJOA0Y6100)
// -------------------------------------------------------------------------------------
// 현금매수 In
/*
typedef struct
{
	char    InItem1[ 1];							// #1(1)
    char    InItem2[10];							// 카드번호(10)
    char    InItem3[ 8];							// 비밀번호(08)
    char    InItem4[ 1];							// #A(1)
    char    InItem5[ 5];							// 종목코드(05)
    char    InItem6[ 6];							// #(06)
	char    InItem7[ 9];							// 매매수량(9)
	char    InItem8[ 9];							// 매매단가(9)
    char    InItem9[ 2];							// 매매구분(02)
	char    InItem10[15];							// 전화번호(15)
	char    InItem11[ 2];							// #00(2)
}fn_HFIJOA0Y6100_InRec1;

// 현금매수 Out
typedef struct
{
    char    OutItem1 [ 3];							// #3
    char    OutItem2 [ 2];							// 주문번호2
    char    OutItem3 [138];							// #138
    char    OutItem4 [20];							// 종목명
    char    OutItem5 [20];							// 지점명
    char    OutItem6 [ 1];							// #1
    char    OutItem7 [ 8];							// 카드번호2
    char    OutItem8 [ 2];							// 계좌구분2
    char    OutItem9 [ 8];							// #8
	char    OutItem10[12];							// 종목번호
	char    OutItem11[ 9];							// 매매수량2
	char    OutItem12[ 9];							// 매매가격2
	char    OutItem13[ 2];							// 매매구분2
	char    OutItem14[15];							// #15
	char    OutItem15[ 2];							// 통신주문구분2
}fn_HFIJOA0Y6100_OutRec1;

// -------------------------------------------------------------------------------------
//			직원용 현금매도(HFIJOA0T1111)
//			hfij80620주식매수 화면참조.
// -------------------------------------------------------------------------------------
typedef struct
{
    char    InItem1[ 1];							// 프로세스타입, !2
    char    InItem2[10];							// 계좌번호(10)
    char    InItem3[ 8];							// 비밀번호(08)
    char    InItem4[12];							// 종목코드(12) : 첫자리는 'A'
    char    InItem5[ 9];							// 매매수량(09)
    char    InItem6[ 9];							// 매매단가(09)
    char    InItem7[ 2];							// 매매구분(02)
    char    InItem8[ 2];							// 주문조건(02)
    char    InItem9[15];							// 전화번호(15)
	char    InItem10[2];							// 통신주문구분(02), !00
}fn_HFIJOA0T1111_InRec1;

typedef struct
{
    char    OutItem1 [ 5];							// 주문번호(매도)
    char    OutItem2 [ 9];							// 전일재사용수량(매도)
    char    OutItem3 [ 9];							// 금일재사용수량(매도)
    char    OutItem4 [ 9];							// 실물주문수량(매도)
    char    OutItem5 [ 9];							// 전일재사용가능수량(매도)
    char    OutItem6 [ 9];							// 금일재사용가능수량(매도)
    char    OutItem7 [ 9];							// 잔고수량(매도)
    char    OutItem8 [20];							// 계좌명(매도)
}fn_HFIJOA0T1111_OutRec1;

/*
#define COrderArray_FO CArray<COrder_FO, COrder_FO&> 
class COrder_FO
{
public:
	CString		clikey;		//   [ 8] 주문 key 
	char		gb;			//   [ 1] 1:매도, 2:매수, 3:정정, 4:취소
	CString		geja;		//   [11] 계좌번호
	CString		passwd;		//   [ 8] 비밀번호
	CString		code;		//   [15] 종목코드
	CString		codeName;	//	 종목코드명
	CString		wonno;		//   [ 7] 원주문번호
	char		jjgb;		//   [ 1] 일반 주문의 정정주문일 경우 : '1'가격정정 '2'주문유형정정
							//	 [ 1] 베이스 일괄주문 TR : 1:일부정정,2:전부정정,3:일부취소,4:전부취소
	char		jutype;		//   [ 1] 주문유형 1:지정가 2:시장가 3:조건부지정 4:최유리지정가
	int			juprc;		//   [10] 매매가격
	int			qty;		//   [ 7] 주문수량
	
	// [NOTE] : 주문확인창
	CString		gejaname;	//   계좌명

	// [NOTE] : 정정/취소 주문 확인창 사용시 원주문 정보를 채운다.
	char		org_gb;		//   원주문정보 - [ 1] 1:매도, 2:매수 (매도/수 구분만 사용한다.)
	char		org_jutype;	//   원주문정보 - [ 1] 주문유형 1:지정가 2:시장가 3:조건부지정 4:최유리지정가
	int			org_juprc;	//	 원주문가격 

	int			nMITPrice;	//   mit 예약주문가격셋팅

public:
	COrder_FO()
	{
		fnInitData();
	}

	void fnInitData()
	{
		clikey		= "";
		gb			= ' ';
		geja		= "";
		passwd		= "";
		code		= "";
		codeName	= "";
		wonno		= "";
		jjgb		= ' ';
		jutype		= ' ';
		juprc		= 0;
		qty			= 0;

		// [NOTE] : 주문확인창
		gejaname	= "";

		// [NOTE] : 주문 확인창 사용시 원주문 정보를 채운다.
		org_gb		= ' ';
		org_jutype	= ' ';
		org_juprc	= 0;
		nMITPrice	= 0;
	}
};


class COrderSave
{
public:
	COrderSave() { bIsMITOrder = FALSE; }
public:
	int		nIndex;			// 대표 index
	int		nEventType;		// 주문액션이 발생한 type(0:호가판, 1:popup, 2:MIT-현재종목, 3:MIT-back 4:슬라이더, 5:전체취소, 6:미체결리스트)
	int		nCount;			// 주문 건수
	int		nRowID;			// 장애가 나는 경우때문에
	int		nColID;			// 장애가 나는 경우때문에, 정정/취소 때문에
	BOOL	bIsMITOrder;	// MIT 주문 여부
	COrderArray_FO	m_arrOrdSave;
};
*/

class CSTOrderRData_SS
{
public:
	int			m_nMarket;				//0:주식, 1:선물/옵션, 2:주식선물, 3:주식옵션
	int			m_nOrderType;			//VALENTRYSELL 매수진입, VALEXITLONG 매수청산, VALEXITSHORT 매도청산, VALENTRYBUY 매도진입
	int			m_nRealOrderType;		//위의타입중 실제 주문내는 한 타입만. 주문전에 셋팅됨
	CString		m_szGeja;				//[11] 계좌번호
	CString		m_szGejaname;			//계좌명
	CString		m_szCode;				//[15] 종목코드
	CString		m_szCodeName;			//[12]종목코드명
	char		m_jutype[2];			//[02]매매구분 주식 : 00보통, 03시장가
										//			   선물 : 
//	int			m_nJuprc;				//[09]매매가격
	float		m_fJuprc;				//[09]매매가격
	int			m_nQty;					//[09]주문수량
	char		m_gb;					//[ 1] 1:매수, 2:매도, 3:정정, 4:취소
										// 1:매도, 2:매수
	CString		m_szPasswd;				//[08] 비밀번호
	int			m_nOrderPriceType;		//주문가격타입. 
										//0현재가,1-3매수1~3,4-6매도1~3,7시장가,8상한가,9하한가
	int			m_nSTType;				//전략의 유형, 제공전략(0), 사용자 전략(1)
	int			m_nAutoOrder;			//주문없음:0, 반자동:1
	CString		m_szSTPath;				//전략의 경로

	long		m_lPtrCSTConfigEnv;		// CSTConfigEnv 포인트

public:
	void Copy(CSTOrderRData_SS* pOrg)
	{
		m_nMarket		= pOrg->m_nMarket;
		m_nOrderType	= pOrg->m_nOrderType;
		m_nRealOrderType= pOrg->m_nRealOrderType;
		m_szGeja		= pOrg->m_szGeja;			
		m_szGejaname	= pOrg->m_szGejaname;		
		m_szCode		= pOrg->m_szCode;			
		m_szCodeName	= pOrg->m_szCodeName;		
		memcpy(m_jutype,  pOrg->m_jutype, 2);
		m_fJuprc		= pOrg->m_fJuprc;			
		m_nQty			= pOrg->m_nQty;			
		m_gb			= pOrg->m_gb;				
		m_szPasswd		= pOrg->m_szPasswd;			
		m_nOrderPriceType= pOrg->m_nOrderPriceType;
		m_nSTType		= pOrg->m_nSTType;		
		m_nAutoOrder	= pOrg->m_nAutoOrder;		
		m_szSTPath		= pOrg->m_szSTPath;	

		m_lPtrCSTConfigEnv = pOrg->m_lPtrCSTConfigEnv;
	}

	CSTOrderRData_SS()
	{
		m_lPtrCSTConfigEnv = NULL;
	}
};

class CSTOrderRData_EG : public CSTOrderRData_SS
{
public:
	int			m_nOrderMarketEntry;
	int			m_nOrderMarketClear;

	BOOL		m_bSound_Buy_Use;
	CString		m_szSound_Buy_File;
	BOOL		m_bSound_Sell_Use;
	CString		m_szSound_Sell_File;

	CSTOrderRData_EG()
	{
	}

	void Copy(CSTOrderRData_EG* pOrg)
	{
		CSTOrderRData_SS::Copy(pOrg);

		m_nOrderMarketEntry = pOrg->m_nOrderMarketEntry;
		m_nOrderMarketClear = pOrg->m_nOrderMarketClear;

		m_bSound_Buy_Use = pOrg->m_bSound_Buy_Use;
		m_szSound_Buy_File = pOrg->m_szSound_Buy_File;
		m_bSound_Sell_Use = pOrg->m_bSound_Sell_Use;
		m_szSound_Sell_File = pOrg->m_szSound_Sell_File;
	}
};


/*************************************************************************************/
/* define constant                                                                   */
/*************************************************************************************/
#define FN_TRAN                       "fn_HFIBOA0V8400"
#define LLONG                         long long


/*************************************************************************************/
/* Input structure                                                                   */
/*************************************************************************************/

/* HFIBOA0V8400InRec: 교육이수여부                                                   */
typedef struct
{
//    char    InItem1[10];                          /* $사용자ID(10)                   */
    char    InItem1[16];                          /* $사용자ID(16)                   */
}fn_HFIBOA0V8400_InRec1;


/*************************************************************************************/
/* Output structure                                                                  */
/*************************************************************************************/

/* HFIBOA0V8400OutRec1: 교육이수여부                                                 */
typedef struct
{
    char    OutItem1[1];                          /* Result(1)                       */
}fn_HFIBOA0V8400_OutRec1;

/*************************************************************************************/

#endif //AFX_SAMSUNG_ORDERDEF_H_73649_BY_98B1A__INCLUDED_

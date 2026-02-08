/* FX 체결,미체결 관리 구조체 */
typedef struct  
{
	char	szKeyType[5];				/*  [FX주문체결] 미체결 - OR01, 체결 CH01		*/
	char	szAccNo[11];				/*	[계좌번호]	 주문시 회원사+계좌번호			*/
	char	szMemberNo[3];				/*  [회원사ID]	 디폴트 000 (고정)				*/
	char	szOrdType[5];				/*  [주문유형]	 두번째 자리로 구분		  
														 OM - 시장가,
														 OE - 지정가,
														 OS - 역지정가					*/
	char	SHCODE[12];					/*  [종목명]	 형태 : CCY1/CCY2				*/
	char	szOrderID[20];				/*  [거래소 접수번호]							*/
	char	szRate[10];					/*  [약정지수]	 미체결시 주문가, 체결시 체결가	*/
	char	szQuote[10];				/*  [현재가]	 현재 체결된 포지션의 반대매매 현재가 */
	char	szStop[10];					/*  [Stop Price]								*/
	char	szLimit[10];				/*	[Limit Price								*/	
	char	szOldStop[10];				/*  [Old Stop]									*/
	char	szOldLimit[10];				/*  [Old Limit]									*/
	char	szOpenTime[30];				/*  [주문, 접수 시각]							*/
	char	szSLOpenTime[30];			/*  [SL 주문, 접수 시각]						*/
	char	szExeTime[30];				/*  [약정시각]	 체결시각						*/
	char	szOrgCustItem1[11];			/*  [원회원처리항목1]							*/
	char	szOrgCustItem2[14];			/*  [원회원처리항목2] 원주문번호				*/
	char	szOrgCustItem3[55];			/*  [원회원처리항목3]							*/
	char	szCustItem1[11];			/*  [회원처리항목 1]							*/
	char	szCustItem2[14];			/*  [회원처리항목 2]  주문번호					*/
	char	szCustItem3[55];			/*  [회원처리항목 3]							*/
	char	szStopItem[80];				/*  [Stop 회원처리항목]	Stop 주문번호 (미표시)	*/
	char	szLimitItem[80];			/*  [Limit 회원처리항목] Limit 주문번호	(미표시)*/
	char	szOldStopItem[80];			/*	[정취전 Stop 회원처리항목]					*/
	char	szOldLimitItem[80];			/*  [정취전 Limit 회원처리항목]					*/
	char	szTSCustItem[80];			/*  [TrailingStop 회원처리항목]					*/
	char	szSide[3];					/*  [매매구분] 
											079 - 매수, 081 - 매도, 
											080 - 환매, 082 - 전매						*/
	char	szAccName[20];				/*  [계좌명]									*/
	char	szUserID[20];				/*  [고객ID]									*/
	char	szFXCMPosID[20];			/*  [TicketNo]	체결번호						*/
	char	szStatus[1];				/*  [미체결 상태 구분 0:미접수, 1:접수]			*/
	char	szSign[1];					/*  [등락부호]	2 - 상승, 3 - 보합, 4 - 하락	*/
	long	nCommition;					/*  [수수료]									*/
	long	nTrailingStop;				/*  [Trailing Stop Pip]							*/
	long	nPipLowest;					/*	[소수점 유효자리수]							*/
	double	fAmount;					/*  [체결수량]									*/
	double	fOrderAmt;					/*  [미체결수량]								*/
	double	fTotalPL;					/*  [핍손익]									*/
	double	fGrossPL;					/*  [일일손익(가격환산)]						*/
	double	fRevRate;					/*  [체결시 반대지수]							*/
	double	fOpenMarkup;				/*  [OM (전환매시)]								*/
	double	fInterest;					/*  [이자]										*/
	double	fAddInterest;				/*  [누적된 이자]								*/
	double	fCloseMarkup;				/*  [CM (전환매시 사용)]						*/
	double	fOutBalance;				/*  [미결제금액(청산손익)]						*/
	double	fPipCost;					/*  [체결시PipCost]								*/
	double	fPredictMRate;				/*  예상 마진콜 환율							*/
} FX_CHE_STRUCT;

typedef struct tagRealTimeOrder
{
	char	szAccountNum[14];	/* 상태 : 0 - 신규 포지션 (추가)
								          1 - 청산 포지션 (삭제)
										  2 - 신규 주문   (추가)
										  3 - 정정 주문	  (이동)
										  4 - 취소 주문   (삭제)*/
	char	szCustItem[80];		/* 회원처리항목 (상태값 0, 2일때 KEY) */
	char	szCurrency[12];		/* 종목명 */
	char	szSide[4];			/* 매매구분 */
	char	szOrdType[5];		/* 주문유형 */
	double	dAmount;			/* 수향 */
	double	dPrice;				/* 가격 */
	char	szOrderNo[20];		/* 체결번호 혹은 주문번호 */
	char	szOrgCustItem[80];	/* 원회원처리항목 (상태값 1, 3, 4일 때 KEY) */
	char	szTranTime[30];		/* 처리시간 */
}REALTIME_ORDER;  //리얼 통보



#define CHAR_TO_STRING(xStr, oStr, nSize, VStr) 	memset(xStr, 0x00, sizeof(xStr));	memcpy(xStr, oStr, nSize);\
	VStr.Format("%s", xStr);\



// 체결
class COpenPosition
{
public:
	CString	m_strAccountNum;		/* 체결번호	*/
	CString	m_strTicketNo;			/* 종목명	*/
	CString	m_strRate;				// 체결가
	CString	m_strSide;				/* 매매구분	*/
	double	m_dAmount;				/* 수량 */		
	double	m_dOpenPrice;				/* 진입환율 */
	double	m_dClosePrice;			/* 현재환율 */
	double	m_dCloseStop;				/* 손절매 */
	double	m_dCloseLimit;			/* 이익실현 */
	double  m_dTotalPL;				/* Pip 손익 */
	double	m_dGrossPL;				/* 손익($) */
	double	m_dInterest;				/* 이자 */	
	double	m_dPipCost;				/* Pip Cost */
	CString	m_strExecutedTime;		/* 진입시각 */
	CString	m_strCustItem1;			/* 원회원처리항목1 */
	CString	m_strCustItem2;			/* 원회원처리항목2 원주문번호 */
	CString	m_strCustItem3;			/* 원회원처리항목3 */
	CString	m_strOrgCustItem1;		/* 회원처리항목 1 */
	CString	m_strOrgCustItem2;		/* 회원처리항목 2  주문번호 */
	CString	m_strOrgCustItem3;		/* 회원처리항목 3 */
	CString	m_strStopItemp;		/* 손절매 회원처리항목 */
	CString	m_strLimitItem;		/* 이익실형 회원처리항목 */

	void COpenPosition::Setdata(FX_CHE_STRUCT stOpenPosition)
	{
		char pBuff[128] = {0, };

		CHAR_TO_STRING(pBuff, stOpenPosition.szAccNo,		sizeof(stOpenPosition.szAccNo),	m_strAccountNum);
		CHAR_TO_STRING(pBuff, stOpenPosition.szFXCMPosID,	sizeof(stOpenPosition.szFXCMPosID),		m_strTicketNo);
		CHAR_TO_STRING(pBuff, stOpenPosition.szRate,		sizeof(stOpenPosition.szRate),			m_strRate);
		CHAR_TO_STRING(pBuff, stOpenPosition.szSide,		sizeof(stOpenPosition.szSide),			m_strSide);
		CHAR_TO_STRING(pBuff, stOpenPosition.szExeTime,		sizeof(stOpenPosition.szExeTime),  m_strExecutedTime);
		CHAR_TO_STRING(pBuff, stOpenPosition.szOrgCustItem1,	sizeof(stOpenPosition.szOrgCustItem1),		m_strCustItem1);
		CHAR_TO_STRING(pBuff, stOpenPosition.szOrgCustItem2,	sizeof(stOpenPosition.szOrgCustItem2),		m_strCustItem2);
		CHAR_TO_STRING(pBuff, stOpenPosition.szOrgCustItem3,	sizeof(stOpenPosition.szOrgCustItem3),		m_strCustItem3);
		CHAR_TO_STRING(pBuff, stOpenPosition.szOrgCustItem1, sizeof(stOpenPosition.szOrgCustItem1),   m_strOrgCustItem1);
		CHAR_TO_STRING(pBuff, stOpenPosition.szOrgCustItem2, sizeof(stOpenPosition.szOrgCustItem2),   m_strOrgCustItem2);
		CHAR_TO_STRING(pBuff, stOpenPosition.szOrgCustItem3, sizeof(stOpenPosition.szOrgCustItem3),   m_strOrgCustItem3);
		CHAR_TO_STRING(pBuff, stOpenPosition.szStopItem,	sizeof(stOpenPosition.szStopItem),		m_strStopItemp);
		CHAR_TO_STRING(pBuff, stOpenPosition.szLimitItem,	sizeof(stOpenPosition.szLimitItem),		m_strLimitItem);

		m_dAmount        = stOpenPosition.fAmount;        	
		m_dOpenPrice	 = stOpenPosition.fOpenMarkup;	 	
		m_dClosePrice	 = stOpenPosition.fCloseMarkup;	 
//		m_dCloseStop	 = stOpenPosition.dCloseStop;	 
//		m_dCloseLimit	 = stOpenPosition.dCloseLimit;	 	
		m_dTotalPL		 = stOpenPosition.fGrossPL;		 	
		m_dGrossPL		 = stOpenPosition.fTotalPL;		 		
		m_dInterest		 = stOpenPosition.fInterest;		 
		m_dPipCost		 = stOpenPosition.fPipCost;		
	}
}; //포지션 리스트


// 미체결
class CPendingOrder
{
public:
	CString	m_strAccountNum;	/* 주문번호 */
	CString	m_strOrderNo;		/* 주문 회원처리항목 */
	CString	m_strRate;			// 미체결가
	CString	m_strCustItem;		/* 종목명 */
	CString	m_strSide;			/* 매매구분 */
	double	m_dAmount;			/* 수량 */
	double	m_dOrderPrice;		/* 주문환율 */
	double	m_dCloseStop;		/* 손절매 */
	double	m_dCloseLimit;		/* 이익실현 */
	CString	m_strOrdType;		/* 주문유형 */
	CString	m_strStatus;		/* 주문상태 */
	CString	m_strOrderTime;		/* 주문시각 */

	void CPendingOrder::Setdata(FX_CHE_STRUCT stPendingOrder)
	{
		char pBuff[128] = {0, };

		CHAR_TO_STRING(pBuff, stPendingOrder.szAccNo,		sizeof(stPendingOrder.szAccNo),m_strAccountNum);
		CHAR_TO_STRING(pBuff, stPendingOrder.szOrderID,		sizeof(stPendingOrder.szOrderID),	m_strOrderNo);
		CHAR_TO_STRING(pBuff, stPendingOrder.szRate,		sizeof(stPendingOrder.szRate),			m_strRate);
		CHAR_TO_STRING(pBuff, stPendingOrder.SHCODE,		sizeof(stPendingOrder.SHCODE),	m_strCustItem);
		CHAR_TO_STRING(pBuff, stPendingOrder.szSide,		sizeof(stPendingOrder.szSide),		m_strSide);
		CHAR_TO_STRING(pBuff, stPendingOrder.szOrdType,		sizeof(stPendingOrder.szOrdType),	m_strOrdType);
		CHAR_TO_STRING(pBuff, stPendingOrder.szStatus,		sizeof(stPendingOrder.szStatus),	m_strStatus);
		CHAR_TO_STRING(pBuff, stPendingOrder.szOpenTime,	sizeof(stPendingOrder.szOpenTime), m_strOrderTime);
		
		m_dAmount	        = stPendingOrder.fOrderAmt;    
		m_dOrderPrice		= stPendingOrder.fOpenMarkup;   
//		m_dCloseStop		= stPendingOrder.dCloseStop;   
//		m_dCloseLimit		= stPendingOrder.dCloseLimit;
	}   //주문 리스트
};

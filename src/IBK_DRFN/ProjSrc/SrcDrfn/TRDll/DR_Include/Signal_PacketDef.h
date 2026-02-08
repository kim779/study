#ifndef SIGNAL_CLIENTPACKET
#define SIGNAL_CLIENTPACKET

/****************************************************************/
/*                                                              */
/*						 신호 Packet (Signal_PacketDef.h)		*/
/*																*/
/*						Client에서 사용하는 신호 Packet!		*/
/*						SVR -> SignalMng -> Client				*/
/*							(User)								*/
/*																*/
/*                   2006. 10.		 		Doori	co.    		*/
/*                      작성자 : 우동우							*/
/****************************************************************/

//# 신호 요청 TYPE!
#define TYPE_SIGNAL_SYS     145  //유진 170->142   //# 시스템 신호//
#define TYPE_SIGNAL_ALERT   142  //유진 171->145     //# 개별 시스템 or 사용자 조건 신호
#define TYPE_SIGNAL_NEWS    161  //유진 172->161    //# 요청 제공처 뉴스 신호  [주의] Only Client!
#define TYPE_REAL_NEWS      162		//(삼성)169    //# 요청 제공처 뉴스
//#define TYPE_REAL_ALERT		142

#define	WM_GET_BROD			WM_USER + 703

//# 입력 Values
//#  BOOL bAdvise : 요청 Flag -> TRUE :요청, FASE : 해지
//#  CWnd* pwndMain : 요청 데이터를 받을 윈도 
//#  CString strType : 데이터 요청 Type
//#  CString strKey  : 요청 데이터 Key -> "*" : ALL, [뉴스] : 제공처, [신호] : 조건번호(XXXX)
//extern "C" __declspec(dllexport) int _DLL_SetSiganl(BOOL bAdvise, CWnd* pwndMain, CString strType, CString strKey = "*");


//# 신호 Key Table 요청 Type!
#define TYPE_TABLE_SYSTEM   "시스템조건"
#define TYPE_TABLE_NEWS     "뉴스제공처"
#define TYPE_TABLE_PORT     "관심조건"
//# 신호 구분 테이블
//#  CString strType                Data 형태
//   - TYPE_TABLE_SYSTEM    ----->  (Key | 조건명)
//   - TYPE_TABLE_NEWS      ----->  (Key | 제공처명)
//   - TYPE_TABLE_PORT      ----->  (Key | 조건명)
//# CStringArray &saSignalTable
//   : Type에 따른 DATA! 
//# Return Values => Table Cnt!
//extern "C" __declspec(dllexport) BOOL _DLL_GetSiganlTable(CString strType, CStringArray &saSignalTable);


//# 뉴스 관련 종목 Type => SignalDegree
#define NEWS_CODETYPE_RPN       99    //# 대표종목
#define NEWS_CODETYPE_RLT       98    //# 연관종목
#define NEWS_CODETYPE_C_INS     97    //# 포함종목(본문)
#define NEWS_CODETYPE_T_INS     96    //# 포함종목(제목)


#define CODE_LEN						6
#define CODE_NAME_LEN					40	//@유진 20 -> 40
#define TIME_LEN						6
#define DATE_LEN						8
#define SIGNAL_NAME_LEN					40
#define SIGNAL_DEGREE_LEN				2
#define IMG_INDEX_LEN					2


#define STR_CONVERT(pBuff, cData, nSize, strData)	\
	pBuff = new char[nSize + 1];					\
	memset(pBuff, 0x00, nSize + 1);					\
	memcpy(pBuff, cData, nSize);					\
	strData = (CString)pBuff;						\
	strData.TrimRight();							\
delete [] pBuff;

#define INT_CONVERT(pBuff, cData, nSize, nData)		\
	pBuff = new char[nSize + 1];					\
	memset(pBuff, 0x00, nSize + 1);					\
	memcpy(pBuff, cData, nSize);					\
	nData = atoi(pBuff);							\
delete [] pBuff;


#define HAVE_SPACE_CODE     1


typedef struct tagSIGNAL_HEAD
{
	char		sSize[4];
	char    sType[20];
} SIGNAL_HEAD;
#define SZ_SIGNAL_HEAD  sizeof(SIGNAL_HEAD)

typedef struct tagSIGNAL_COMMON
{
	char    sMarket[1];
	char	sCode[CODE_LEN];		// 종목코드
	
	char    sPrice[8];          // 현재가 (단위:원)
	char    sSign;              // 전일대비부호          
	char    sChage[8];          // 전일대비          
	char    sChgRate[6];        // 등락율   Type(S999.99)          
	char    sVolume[9];         // 거래량 : Unit(1,000)          
	char    sVolumeRect[6];     // 거래량 대비율: Unit(1,000) 
	char	sResultField[1040];	// 한투 결과필드
} SIGNAL_COMMON;

typedef struct tagSIGNAL_CONDITION
{
//	SIGNAL_HEAD	stHEAD;
	
	char	sCode[CODE_LEN];					// 종목코드
	char	sName[20];//[CODE_NAME_LEN];			// 종목명
	char	sJobFlag[1];						  // 신규:'N', 재진입:'R', 탈퇴:'O'
	char	sEntryCnt[4];             // 진입 횟수
	char	sDate[DATE_LEN];					// 발생 일자 
	char	sTime[TIME_LEN];					// 발생 시간. HHMMSS
	
	char	sSignalImageIndex[IMG_INDEX_LEN];	// 신호강도에 해당하는 이미지 인덱스 셋팅 
	char	sSignalDegree[SIGNAL_DEGREE_LEN]; // 신호 강도. "-3" ~ "3"
	char	sOrderType[1];										// 주문 타입. '0': 주문을 내지 않음. '1': 반자동, '2': 자동
	
	char	sSignalKey[4];		                // System Alert 인덱스
	char	sSignalName[SIGNAL_NAME_LEN];		  // System Alert 인덱스에 해당하는 이름
	SIGNAL_COMMON   stCommon;
} SIGNAL_CONDITION;
#define SZ_SIGNAL_CONDITION  sizeof(SIGNAL_CONDITION)


class CSIGNAL_CONDITIONDATA
{
public:
//	CString m_strType;
	CString m_strSignalKey;
	
	int     m_nMarket;
	CString m_strPrice;
	CString	m_strCode;							// 종목코드
	CString	m_strName;							// 종목명
	
	CString	m_strDate;							// 발생 일자 
	CString	m_strTime;							// 발생 시간. HHMMS6S
	int			m_nSignalImageIndex;			// 신호강도에 해당하는 이미지 인덱스 셋팅 
	int			m_nSignalDegree;				// 신호 강도. "-3" ~ "3"
	CString	m_strOrderType;						// 주문 타입. '0': 주문을 내지 않음. '1': 반자동, '2': 자동
	
	CString	m_strSignalName;					// System Alert 인덱스에 해당하는 이름
	
	virtual void  Convert(SIGNAL_CONDITION* pInput)
	{
		char *pBuff;
//		STR_CONVERT(pBuff, pInput->stHEAD.sType, sizeof(pInput->stHEAD.sType), m_strType);
//		m_strType.TrimRight(" ");
		
		STR_CONVERT(pBuff, pInput->sSignalKey, sizeof(pInput->sSignalKey), m_strSignalKey);
		
		INT_CONVERT(pBuff, pInput->stCommon.sMarket, sizeof(pInput->stCommon.sMarket), m_nMarket);
		STR_CONVERT(pBuff, pInput->stCommon.sPrice, sizeof(pInput->stCommon.sPrice), m_strPrice);
		STR_CONVERT(pBuff, pInput->sCode, sizeof(pInput->sCode), m_strCode);
		STR_CONVERT(pBuff, pInput->sName, sizeof(pInput->sName), m_strName);
		m_strName.TrimRight(" ");
		
		STR_CONVERT(pBuff, pInput->sDate, sizeof(pInput->sDate), m_strDate);
		STR_CONVERT(pBuff, pInput->sTime, sizeof(pInput->sTime), m_strTime);
		INT_CONVERT(pBuff, pInput->sSignalImageIndex, sizeof(pInput->sSignalImageIndex), m_nSignalImageIndex);
		INT_CONVERT(pBuff, pInput->sSignalDegree, sizeof(pInput->sSignalDegree), m_nSignalDegree);
		STR_CONVERT(pBuff, pInput->sOrderType, sizeof(pInput->sOrderType), m_strOrderType);
		
		STR_CONVERT(pBuff, pInput->sSignalName, sizeof(pInput->sSignalName), m_strSignalName);
	}
};

typedef struct tagNEWS_KEY
{
	char    sDate       [ 8];
	char    sTime       [ 6];
	char    sSeqno      [ 12];
	char    sGbn_l      [ 2];
	char    sGbn_m      [ 2];
//	char    sGbn_s      [ 2];
//	char    sFiller		[ 2];
} DR_NEWS_KEY;
#define		SZ_DR_NEWS_KEY		sizeof(DR_NEWS_KEY)	

#define MAX_REAL_PACKET_SIZE 1024 * 16
typedef struct tagRealItem
{
	char	szPacketKey[4];
	char	szDataLength[4];
	char	szBcData[MAX_REAL_PACKET_SIZE + 1];

} ST_REALITEM;
#define  SZ_REALITEM sizeof(ST_REALITEM)

typedef struct tagSIGANL_NEWS
{
	SIGNAL_HEAD	stHEAD;
	
	DR_NEWS_KEY stNewsKey;
	char	sSignalImageIndex[IMG_INDEX_LEN];	// 신호강도에 해당하는 이미지 인덱스 셋팅 
	char	sSignalDegree[SIGNAL_DEGREE_LEN];	// 신호 강도. "-3" ~ "3"
	
	char	sGoodPoint[2];   		// 호재 뉴스 점수. + 상황문 발생
	char	sBadPoint[2];		 	// 악재 뉴스 점수. - 상황문 발생.
	
	char	sCode[CODE_LEN];		/* 종목 코드 				*/
	char	sName[CODE_NAME_LEN];	/* 종목명    				*/
	char	sProfferID[2];			/* 뉴스 제공처 ID		*/
	char	sProffer[20];			/* 뉴스 제공처 			*/
	char	sProfferTypeID[3];	//@유진변경sProfferTypeID[2];    /* 제공처 분류 ID		*/
	//char	sProfferTypeID[2];		/* 제공처 분류 ID		*/
	char	sProfferType[50];	    /* 제공처 분류 			*/
	char	sTitle[255];			/* 제 목 						*/
	
	char	sNsDt[8];					/* @유진추가:뉴스일자 */
	char	sNsTm[6];					/* @유진추가:뉴스시간 */

	SIGNAL_COMMON   stCommon;
} SIGANL_NEWS;
#define SZ_SIGANL_NEWS  sizeof(SIGANL_NEWS)

class CSIGANL_NEWSDATA
{
public:
	DR_NEWS_KEY stNewsKey;
	
	CString m_strDate;
	CString m_strTime;
	CString m_strSqNo;
	
	CString m_strType;
	CString m_strRealKey;
	
	int     m_nMarket;
	CString m_strCode;
	CString m_strName;
	
	int		m_nSignalImageIndex;		// 신호강도에 해당하는 이미지 인덱스 셋팅 
	int		m_nSignalDegree;			// 신호 강도. "-3" ~ "3"
	
	int			m_nProfferID;			// 제공처 아이디
	CString	m_strProffer;				// 제공처 명
	CString	m_strProfferTypeID;			// 제공처별 분류 아이디
	CString	m_strProfferType;			// 제공처별 분류명
	CString	m_strTitle;					// 제목
	
	int m_nGoodPoint;   				// 호재 뉴스 점수. + 상황문 발생
	int	m_nBadPoint;		 			// 악재 뉴스 점수. - 상황문 발생.
	
	virtual void  Convert(SIGANL_NEWS* pInput)
	{
		char *pBuff;
		STR_CONVERT(pBuff, pInput->stNewsKey.sDate, sizeof(pInput->stNewsKey.sDate), m_strDate);
		STR_CONVERT(pBuff, pInput->stNewsKey.sTime, sizeof(pInput->stNewsKey.sTime), m_strTime);
		STR_CONVERT(pBuff, pInput->stNewsKey.sSeqno, sizeof(pInput->stNewsKey.sSeqno), m_strSqNo);
		
		STR_CONVERT(pBuff, pInput->stHEAD.sType, sizeof(pInput->stHEAD.sType), m_strType);
		m_strType.TrimRight(" ");
		
		STR_CONVERT(pBuff, pInput->sProfferID, sizeof(pInput->sProfferID), m_strRealKey);
		
		INT_CONVERT(pBuff, pInput->stCommon.sMarket, sizeof(pInput->stCommon.sMarket), m_nMarket);
		STR_CONVERT(pBuff, pInput->sCode, sizeof(pInput->sCode), m_strCode);
		STR_CONVERT(pBuff, pInput->sName, sizeof(pInput->sName), m_strName);
		m_strName.TrimRight(" ");
		
		INT_CONVERT(pBuff, pInput->sSignalImageIndex, sizeof(pInput->sSignalImageIndex), m_nSignalImageIndex);
		INT_CONVERT(pBuff, pInput->sSignalDegree, sizeof(pInput->sSignalDegree), m_nSignalDegree);
		//STR_CONVERT(pBuff, pInput->sOrderType, sizeof(pInput->sOrderType), m_strOrderType);
		
		INT_CONVERT(pBuff, pInput->sProfferID, sizeof(pInput->sProfferID), m_nProfferID);
		STR_CONVERT(pBuff, pInput->sProffer, sizeof(pInput->sProffer), m_strProffer);
		STR_CONVERT(pBuff, pInput->sProfferTypeID, sizeof(pInput->sProfferTypeID), m_strProfferTypeID);
		STR_CONVERT(pBuff, pInput->sProfferType, sizeof(pInput->sProfferType), m_strProffer);
		STR_CONVERT(pBuff, pInput->sTitle, sizeof(pInput->sTitle), m_strTitle);
		
		INT_CONVERT(pBuff, pInput->sGoodPoint, sizeof(pInput->sGoodPoint), m_nGoodPoint);
		INT_CONVERT(pBuff, pInput->sBadPoint, sizeof(pInput->sBadPoint), m_nBadPoint);
	}
};



#endif
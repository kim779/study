#ifndef SIGNAL_CLIENTPACKET
#define SIGNAL_CLIENTPACKET

/****************************************************************/
/*                                                              */
/*						 신호 Packet (Signal_ClientPacket.h)							*/
/*																															*/
/*						Client에서 사용하는 신호 Packet!									*/
/*						SVR -> DRDS -> SignalMng -> Client								*/
/*																				(User)								*/
/*																															*/
/*                   2005. 12.		 		Doori											*/
/*                      작성자 : 우동우													*/
/****************************************************************/

#define TYPE_SIGNAL_SYS     "SYS_SIGNAL"
#define TYPE_SIGNAL_USER    "USER_SIGNAL"
#define TYPE_SIGNAL_NEWS    "NEWS_SIGNAL"
#define TYPE_REAL_NEWS      "NEWS_REAL"



//# 뉴스 관련 종목 Type => SignalDegree
#define NEWS_CODETYPE_RPN       99    //# 대표종목
#define NEWS_CODETYPE_RLT       98    //# 연관종목
#define NEWS_CODETYPE_C_INS     97    //# 포함종목(본문)
#define NEWS_CODETYPE_T_INS     96    //# 포함종목(제목)


#define CODE_LEN						6
#define CODE_NAME_LEN				20
#define TIME_LEN						6
#define DATE_LEN						8
#define SIGNAL_NAME_LEN			40
#define SIGNAL_DEGREE_LEN		2
#define IMG_INDEX_LEN       2


#define STR_CONVERT(pBuff, cData, nSize, strData)	\
  pBuff = new char[nSize + 1];							\
  memset(pBuff, 0x00, nSize + 1);						\
  memcpy(pBuff, cData, nSize);							\
  strData = (CString)pBuff;									\
  strData.TrimRight();											\
delete [] pBuff;

#define INT_CONVERT(pBuff, cData, nSize, nData)	\
  pBuff = new char[nSize + 1];						\
  memset(pBuff, 0x00, nSize + 1);					\
  memcpy(pBuff, cData, nSize);						\
  nData = atoi(pBuff);										\
delete [] pBuff;


#define HAVE_SPACE_CODE     1


typedef struct tagSIGNAL_HEAD
{
  char		sSize[4];
  char    sType[4];
} SIGNAL_HEAD;
#define SZ_SIGNAL_HEAD  sizeof(SIGNAL_HEAD)


typedef struct tagSIGNAL_CONDITION
{
  SIGNAL_HEAD	stHEAD;
  
  char	sCode[CODE_LEN];								  // 종목코드
  char	sName[CODE_NAME_LEN];						  // 종목명
  char	sDate[DATE_LEN];								  // 발생 일자 
  char	sTime[TIME_LEN];								  // 발생 시간. HHMMSS
  char	sSignalImageIndex[IMG_INDEX_LEN];	// 신호강도에 해당하는 이미지 인덱스 셋팅 
  char	sSignalDegree[SIGNAL_DEGREE_LEN]; // 신호 강도. "-3" ~ "3"
  char	sOrderType[1];										// 주문 타입. '0': 주문을 내지 않음. '1': 반자동, '2': 자동
  
  char	sSignalName[SIGNAL_NAME_LEN];		  // System Alert 인덱스에 해당하는 이름
} SIGNAL_CONDITION;
#define SZ_SIGNAL_CONDITION  sizeof(SIGNAL_CONDITION)


class CSIGNAL_CONDITIONDATA
{
public:
  CString	m_strCode;							// 종목코드
  CString	m_strName;							// 종목명
  CString	m_strDate;							// 발생 일자 
  CString	m_strTime;							// 발생 시간. HHMMSS
  int			m_nSignalImageIndex;				// 신호강도에 해당하는 이미지 인덱스 셋팅 
  int			m_nSignalDegree;				// 신호 강도. "-3" ~ "3"
  CString	m_strOrderType;					// 주문 타입. '0': 주문을 내지 않음. '1': 반자동, '2': 자동
  
  CString	m_strSignalName;				// System Alert 인덱스에 해당하는 이름
  
  virtual void  Convert(SIGNAL_CONDITION* pInput)
  {
    char *pBuff;
    STR_CONVERT(pBuff, pInput->sCode, sizeof(pInput->sCode), m_strCode);

    if ((int)HAVE_SPACE_CODE == 1)
      m_strCode += " ";
    
    STR_CONVERT(pBuff, pInput->sName, sizeof(pInput->sName), m_strName);
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
    char    sSeqno      [ 6];
    char    sGbn_l      [ 2];
    char    sGbn_m      [ 2];
    char    sGbn_s      [ 2];
    char    sFiller[2];
} DR_NEWS_KEY;
#define		SZ_DR_NEWS_KEY		sizeof(DR_NEWS_KEY)	

typedef struct tagSIGANL_NEWS
{
  SIGNAL_HEAD	stHEAD;
  
  DR_NEWS_KEY stNewsKey;
  char	sSignalImageIndex[IMG_INDEX_LEN];	// 신호강도에 해당하는 이미지 인덱스 셋팅 
  char	sSignalDegree[SIGNAL_DEGREE_LEN];	// 신호 강도. "-3" ~ "3"
  char	sOrderType[1];										// 주문 타입. '0': 주문을 내지 않음. '1': 반자동, '2': 자동

  char	sGoodPoint[2];   	// 호재 뉴스 점수. + 상황문 발생
  char	sBadPoint[2];		 	// 악재 뉴스 점수. - 상황문 발생.

  char	sCode[CODE_LEN];			/* 종목 코드 				*/
  char	sName[CODE_NAME_LEN]; /* 종목명    				*/
  char	sProfferID[2];			  /* 뉴스 제공처 ID		*/
	char	sProffer[20];				  /* 뉴스 제공처 			*/
  char	sProfferTypeID[2];    /* 제공처 분류 ID		*/
	char	sProfferType[50];	    /* 제공처 분류 			*/
	char	sTitle[255];				  /* 제 목 						*/
} SIGANL_NEWS;
#define SZ_SIGANL_NEWS  sizeof(SIGANL_NEWS)

class CSIGANL_NEWSDATA
{
public:
  DR_NEWS_KEY stNewsKey;

  CString m_strCode;
  CString m_strName;

  int			m_nSignalImageIndex;		// 신호강도에 해당하는 이미지 인덱스 셋팅 
  int			m_nSignalDegree;				// 신호 강도. "-3" ~ "3"
  CString	m_strOrderType;					// 주문 타입. '0': 주문을 내지 않음. '1': 반자동, '2': 자동
    
  int			m_nProfferID;						// 제공처 아이디
  CString	m_strProffer;						// 제공처 명
  CString	m_strProfferTypeID;				// 제공처별 분류 아이디
  CString	m_strProfferType;					// 제공처별 분류명
  CString	m_strTitle;							// 제목
  
  int m_nGoodPoint;   						// 호재 뉴스 점수. + 상황문 발생
  int	m_nBadPoint;		 						// 악재 뉴스 점수. - 상황문 발생.
    
  virtual void  Convert(SIGANL_NEWS* pInput)
  {
    char *pBuff;
    STR_CONVERT(pBuff, pInput->sCode, sizeof(pInput->sCode), m_strCode);
    STR_CONVERT(pBuff, pInput->sName, sizeof(pInput->sName), m_strName);
    if ((int)HAVE_SPACE_CODE == 1)
      m_strCode += " ";
    
    INT_CONVERT(pBuff, pInput->sSignalImageIndex, sizeof(pInput->sSignalImageIndex), m_nSignalImageIndex);
    INT_CONVERT(pBuff, pInput->sSignalDegree, sizeof(pInput->sSignalDegree), m_nSignalDegree);
    STR_CONVERT(pBuff, pInput->sOrderType, sizeof(pInput->sOrderType), m_strOrderType);
    
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
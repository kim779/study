
typedef struct
{
	char    gsAlertNum[4];      /* 신호 키정보(앞의 4자리 채움, 이후 space) */
	char    sKey[4];
	char    gsAlertGubun[1];    /* System('S'), User('U')                   */
	char    gsDate[8];          /* 신호 발생일자                            */
	char    gsTime[6];          /* 신호 발생시간                            */
	char    gsTrueCnt[4];       /* 현재만족 종목수(기존+신규)               */
	char    gsDataCnt[4];       /* 전송종목수                               */
	char    gsStatus[1];        /* 신호발생:'S',조건이상:'E',조건 점검:'W'  */
	/* 조건 초기화: 'I', 조건 강제종료: 'K'     */
} ALERT_OUT_HEAD;
#define SZ_ALERT_OUT_HEAD  sizeof(ALERT_OUT_HEAD)

typedef struct
{
	char    gsJobFlag;                      /* 신규:'N',재진입:'R',탈퇴:'O' */
	char    gsCode[6];                      /* 종목코드                     */
	char    gsHname[20];                    /* 종목명                       */
	char    gsEntryCnt[4];                  /* 진입 횟수                    */

	char    gsManagement[1];                /* 관리종목                     */
	char    gsControl[1];                   /* 이상급등 종목                */
	char    gsTradeStop[1];                 /* 거래정지 종목                */
	char    gsInvestAttention[1];           /* 투자유의 종목                */
	char    gsInsincerityNotice[1];         /* 불성실공시 종목              */
	char    gsConfidenceLimit[1];           /* 신용제한 종목                */
	char    gsCodeNotice[1];                /* 종목공시                     */
	char    gsJungriMeme[1];                /* 정리매매 종목                */
	char    gsMarket[1];                    /* 시장구분                     */
	char    gsDepositMoney[3];              /* 증거금 비율                  */

	char    gsPrice[8];                     /* 현재가                       */
	char    gsSign[1];                      /* 전일대비등락부호             */
	char    gsChange[8];                    /* 전일대비                     */
	char    gsChgRate[6];                   /* 전일대비등락율               */
	char    gsVolume[9];                    /* 거래량                       */
	char    gsVolumeRate[6];                /* 거래량 전일대비율            */
	char    gsAmount[9];                    /* 거래대금                     */
} ALERT_OUT_LIST;
#define SZ_ALERT_OUT_LIST  sizeof(ALERT_OUT_LIST)
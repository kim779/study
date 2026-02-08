///////////////////////////////////////간편인증
//공통헤더
struct st_comheader
{
	char msgid[4];   //msgid
	char uvalue[16];   //unique value
	char filler[8];    
};

#define SZ_COMHEADER sizeof(struct st_comheader)


//서명원문등록
struct st_tagREG_PRE
{
	char rqst_CHNL_DCD[21];				/* 요청매체구분코드          (필수)    API를 호출한 매체구분코드               */
	char tran_LGN_YN[2];				/* 로그인거래여부            (필수)    Y:로그인, N:금융거래                    */
	char tran_DTL_DCD[4];				/* 거래상세구분코드          (    )    업무에서 정의한 현재 화면의 거래상세구분코드*/
	char cprs_pubkey[1025];				/* 축약서명검증용공개키      (    )    축약서명검증용공개키                    */
	char tran_elsg_ontt_con[1001];		/* 거래전자서명원문내용      (    )    거래전자서명원문내용                    */
	char custno[21];					/* 고객번호                  (    )    고객번호                                */
};

struct st_OUT_tagREG_PRE
{
	char ERR_CD[7];								/* 에러코드             (필수       )  오류코드                                */
	char REL_TRAN_NO[21];						/* 연계거래번호         (성공시 필수)  연계거래번호                            */
    char REL_TRAN_EASY_NO[11];					/* 연계간편인증번호     (성공시 필수)  연계간편인증번호                        */
};

#define SZ_TAGREGPRE sizeof(struct st_tagREG_PRE)


//서명여부조회
typedef struct tagINQUIRY_REG_TRAN_BY_RELAY_REQ  //서명여부조회
{
    char        RQST_CHNL_DCD[20 + 1];			/* 요청매체구분코드          (필수)    API를 호출한 매체구분코드               */
    char        REL_TRAN_NO[20 + 1];			/* 연계거래번호              (필수)    연계거래번호                            */
    char        REL_TRAN_EASY_NO[10 + 1];		/* 연계간편인증번호          (필수)    연계간편인증번호                        */
} INQUIRY_REG_TRAN_BY_RELAY_REQ;
#define SZ_INQUIRY_REG_TRAN_BY_RELAY_REQ    sizeof(INQUIRY_REG_TRAN_BY_RELAY_REQ)  //서명여부조회

typedef struct tagINQUIRY_REG_TRAN_BY_RELAY_RES  //서명여부조회
{
    char        ERR_CD[6 + 1];					/* 에러코드             (필수       )  오류코드                                */
    char        REG_TRAN_YN[1 + 1];				/* 서명메시지등록여부   (성공시 필수)  서명메시지등록여부(Y:등록, N:미등록)    */
    char        CRTC_PTRN_DCD[2 + 1];			/* 인증유형구분코드     (REG_TRAN_YN='Y'인 경우 필수)  인증유형구분코드(10:PIN, 20:BIO, 30:PTN)*/
    char        BIO_CRTC_DCD[2 + 1];			/* BIO인증구분코드      (REG_TRAN_YN='Y'인 경우 필수)  BIO인증구분코드(00:사용안함, 01:지문, 02:안면) */
    char        BIO_CRTC_SUCC_YN[1 + 1];		/* 바이오인증성공여부   (REG_TRAN_YN='Y'인 경우 필수)  바이오인증성공여부(Y:성공, N:실패)      */
    char        LAST_ERR_CD[6 + 1];				/* 가장최근에러코드     (           )  가장최근에러코드                        */
    char        LAST_ERR_TS[17 + 1];			/* 가장최근에러일시     (           )  가장최근에러일시                        */
} INQUIRY_REG_TRAN_BY_RELAY_RES;
#define SZ_INQUIRY_REG_TRAN_BY_RELAY_RES    sizeof(INQUIRY_REG_TRAN_BY_RELAY_RES)  //서명여부조회


//직전 인증 성공한 인증거래번호를 조회한다.
typedef struct tagINQUIRY_AUTH_TRAN_NO_BY_RELAY_REQ  //직전 인증 성공한 인증거래번호를 조회한다.
{  
    char        RQST_CHNL_DCD             [20 + 1];    /* 요청매체구분코드          (필수)    API를 호출한 매체구분코드               */
    char        REL_TRAN_NO               [20 + 1];    /* 연계거래번호              (필수)    연계거래번호                            */
    char        REL_TRAN_EASY_NO          [10 + 1];    /* 연계간편인증번호          (필수)    연계간편인증번호                        */
} INQUIRY_AUTH_TRAN_NO_BY_RELAY_REQ;  //직전 인증 성공한 인증거래번호를 조회한다.
#define SZ_INQUIRY_AUTH_TRAN_NO_BY_RELAY_REQ    sizeof(INQUIRY_AUTH_TRAN_NO_BY_RELAY_REQ)  //직전 인증 성공한 인증거래번호를 조회한다.


typedef struct tagINQUIRY_AUTH_TRAN_NO_BY_RELAY_RES
{  
    char        ERR_CD                    [6 + 1];    /* 에러코드             (필수       )  오류코드                                */
    char        AUTH_TRAN_NO              [20 + 1];   /* 인증거래번호         (성공시 필수)  인증거래번호                            */
} INQUIRY_AUTH_TRAN_NO_BY_RELAY_RES;   //직전 인증 성공한 인증거래번호를 조회한다.
#define SZ_INQUIRY_AUTH_TRAN_NO_BY_RELAY_RES    sizeof(INQUIRY_AUTH_TRAN_NO_BY_RELAY_RES)  //직전 인증 성공한 인증거래번호를 조회한다.




//로그인 거래 검증 구조체 -> 서명메시지를 분석하여, 서명원문 일치여부를 판단한다. 
typedef struct tagVERIFY_LOGIN_TRAN_BY_RELAY_REQ
{   
    char        RQST_CHNL_DCD             [20 + 1];    /* 요청매체구분코드          (필수)    API를 호출한 매체구분코드               */
    char        REL_TRAN_NO               [20 + 1];    /* 연계거래번호              (필수)    연계거래번호                            */
    char        REL_TRAN_EASY_NO          [10 + 1];    /* 연계간편인증번호          (필수)    연계간편인증번호                        */
    char        AUTH_TRAN_NO              [20 + 1];    /* 인증거래번호              (필수)    인증거래번호                            */
} VERIFY_LOGIN_TRAN_BY_RELAY_REQ;
#define SZ_VERIFY_LOGIN_TRAN_BY_RELAY_REQ   sizeof(VERIFY_LOGIN_TRAN_BY_RELAY_REQ)


typedef struct tagVERIFY_LOGIN_TRAN_BY_RELAY_RES
{      
    char        ERR_CD                    [6 + 1];    /* 에러코드             (필수       )  오류코드                                */
    char        CUST_NO                   [20 + 1];    /* 고객번호             (성공시 필수)  고객번호                                */
    char        VRFC_SUCC_YN              [1 + 1];    /* 거래검증성공여부     (성공시 필수)  거래검증성공여부                        */
    char        CPRS_PUB_KEY              [1024 + 1];    /* 축약서명검증용공개키 (성공시 필수)  축약서명검증용공개키                    */
} VERIFY_LOGIN_TRAN_BY_RELAY_RES;
#define SZ_VERIFY_LOGIN_TRAN_BY_RELAY_RES   sizeof(VERIFY_LOGIN_TRAN_BY_RELAY_RES)



//금융 거래 검증 -> 금융거래 서명메시지를 검증한다.
typedef struct tagVERIFY_TRAN_BY_RELAY_REQ
{  
    char        RQST_CHNL_DCD             [20 + 1];    /* 요청매체구분코드          (필수)    API를 호출한 매체구분코드               */
    char        REL_TRAN_NO               [20 + 1];    /* 연계거래번호              (필수)    연계거래번호                            */
    char        REL_TRAN_EASY_NO          [10 + 1];    /* 연계간편인증번호          (필수)    연계간편인증번호                        */
    char        AUTH_TRAN_NO              [20 + 1];    /* 인증거래번호              (필수)    인증거래번호                            */
} VERIFY_TRAN_BY_RELAY_REQ;
#define SZ_VERIFY_TRAN_BY_RELAY_REQ sizeof(VERIFY_TRAN_BY_RELAY_REQ)


typedef struct tagVERIFY_TRAN_BY_RELAY_RES
{      
    char        ERR_CD                    [6 + 1];    /* 에러코드            (필수       )  오류코드                                 */
    char        CUST_NO                   [20 + 1];    /* 고객번호            (성공시 필수)  고객번호                                 */
    char        VRFC_SUCC_YN              [1 + 1];    /* 거래검증성공여부    (성공시 필수)  거래검증성공여부                         */
} VERIFY_TRAN_BY_RELAY_RES;
#define SZ_VERIFY_TRAN_BY_RELAY_RES sizeof(VERIFY_TRAN_BY_RELAY_RES)

//OTP조회
typedef struct tagOTP_SEARCH
{  
    char        RQST_CHNL_DCD             [20 + 1];    /* 요청매체구분코드          (필수)    API를 호출한 매체구분코드               */
    char        CUST_NO                   [20 + 1];    /* 고객번호					(성공시 필수)  고객번호 - 로그인 검증시 받아오 데이타*/
    char        OTP_SEQ_NO                [12 + 1];    /* OTP일련번호                             */
} OTP_SEARCH;
#define SZ_tagOTP_SEARCH sizeof(tagOTP_SEARCH)

typedef struct tagOTP_RES_SEARCH
{  
	char        ERR_CD                    [6 + 1];     /* 에러코드					 (필수       )  오류코드                         */
	char        OTP_SEQ_NO		          [12 + 1];    /* OTP일련번호                (필수       )                                   */
	char        CUST_NO					  [20 + 1];    /* 고객번호					 (필수       )                                   */
	char        OTP_STTS_DCD	          [2 + 1];     /* 10 : 발급,  30 : 폐기      (필수       )  (yyyyMMddHHmmsssss)              */
	char        OTP_ISNC_TS		          [17 + 1];     /* OTP발급일시               (필수       )  (yyyyMMddHHmmsssss)              */
	char        OTP_LOCK_TS		          [17 + 1];     /* OTP잠김일시               (필수       )  (yyyyMMddHHmmsssss)              */
	char        OTP_DCRD_TS		          [17 + 1];     /* OTP폐기일시               (필수       )  (yyyyMMddHHmmsssss)              */
	char        VLTE_STTG_YMD	          [8  + 1];     /* 유효기간시작년월일        (필수       )                */
	char        VLTE_EXPY_YMD	          [8  + 1];     /* 유효기간만료년월일        (필수       )                */
	char        OTP_EXPY_GDNC_YN          [1  + 1];     /* OTP만료공지여부           (필수       )                */
	char        USR_CCTN_MCTL_ID          [200  + 1];   /* 이용자접속기기ID          (필수       )                */
	char        OTP_PIN_MAX_EROR_ALWN_CNT [3  + 1];     /* OTP PIN최대오류허용횟수   (필수       )                */
	char        OTP_PIN_EROR_CNT          [3  + 1];     /* OTP PIN오류횟수           (필수       )                */
	char        OTP_LOCK_YN               [1  + 1];     /* OTP잠김여부               (필수       )                */
	char        OTP_LAST_VRFC_TS          [17  + 1];     /* OTP최종검증일시          (필수       )                */
	char        VRFC_EROR_CNT             [3  + 1];     /* 검증오류횟수              (필수       )                */
} OTP_RES_SEARCH;
#define SZ_tagOTP_RES_SEARCH sizeof(tagOTP_RES_SEARCH)


//OTP검증
typedef struct tagOTP_CERTREG
{  
    char        RQST_CHNL_DCD             [20 + 1];    /* 요청매체구분코드          (필수)    API를 호출한 매체구분코드               */
	char        MSB_VRSN_VL               [32 + 1];    /* 라이브러리 버전정보 (타매체 사용시  *****)					              */
    char        CUST_NO                   [20 + 1];    /* 고객번호            (성공시 필수)  고객번호                                 */
    char        OTP_SEQ_NO                [12 + 1];    /* OTP일련번호                             */
	char        OTP_TRAN_NO               [20 + 1];    /* OTP거래번호                             */
    char        PIN_DATA_CON			  [64 + 1];    /* 핀데이터내용 (HexString, 64Byte)                                 */
	char        PRE_TRAN_USE_YN			  [1 + 1];    /* 사전거래사용여부                              */
	char        OTP_CLI_VL				  [6 + 1];    /* 고객입력OTP값                              */
} OTP_CERTREG;
#define SZ_tagOTP_CERTREG sizeof(tagOTP_CERTREG)  //OTP검증

typedef struct tagOTP_RES_CERTREG
{  
	char        ERR_CD                    [6 + 1];    /* 에러코드					(필수       )  오류코드                                 */
	char        OTP_VRFC_SUCC_YN          [1 + 1];    /* OTP검증성공여부            (필수       )  오류코드                                 */
	char        OTP_VRFC_MAX_EROR_ALWN_CNT[3 + 1];    /* OTP검증최대오류허용횟수    (필수       )  오류코드                                 */
	char        OTP_VRFC_EROR_CNT         [3 + 1];    /* OTP검증오류횟수            (필수       )  오류코드                                 */
} OTP_RES_CERTREG;
#define SZ_tagOTP_RES_CERTREG sizeof(tagOTP_RES_CERTREG)

//인증서 조회 (IBK MBGID 0104)
//(1) 현재 단말에 발급된 인증서를 조회한다. (기기ID와 APP ID를 이용하여 조회한다.) 
//(2) 일련번호 및 고객번호는 필요에 따라 설정하여 준다. 
//ㄱ. 인증서일련번호(CERT_SEQ_NO)만 설정한 경우 해당 인증서일련번호로 조회
//   - 조회 결과 미존재 시 030101(등록되지 않은 인증서 일련번호) 에러코드 리턴
//   - 조회 결과 존재 시 000000(정상) 에러코드 및 조회결과 리턴
//ㄴ. 고객번호(CUST_NO)만 설정한 경우 해당 고객번호+이용자접속기기ID+인증서상태(정상)으로 조회 <------------------------------ 이걸쓰게 될듯 하다
//	 - 조회 결과 미존재 시 030102(등록되지 않은 고객번호) 에러코드 리턴
//	 - 조회 결과 존재 시 000000(정상) 에러코드 및 조회결과 리턴 
//ㄷ. 인증서일련번호(CERT_SEQ_NO)+고객번호(CUST_NO) 설정한 경우 해당 인증서일련번호로 먼저 조회 후 고객번호 일치 여부 확인
//	 - 조회 결과 미존재 시 030101(등록되지 않은 인증서 일련번호) 에러코드 리턴
//	 - 조회 결과 존재 시 입력으로 들어온 고객번호(CUST_NO)로 일치 여부 체크
//		. 불일치하는 경우 030103(인증서 일련번호와 고객번호 불일치) 에러코드 리턴
//		. 일치하는 경우 000000(정상) 에러코드 및 조회결과 리턴
//(3) 인증서 조회는 이용자접속기기ID와 매체구분코드를 이용하여 현재기기에 유효한 ID만 조회한다.

//인증서 조회
typedef struct tagCERT_INQUIRY
{  
    char        RQST_CHNL_DCD             [20 + 1];    /* 요청매체구분코드          (필수)    API를 호출한 매체구분코드               */
	char        CERT_SEQ_NO               [12 + 1];    /* 인증서 일련번호												              */
    char        CUST_NO                   [20 + 1];    /* 고객번호					                                */
    char        USR_CCTN_MCTL_ID          [200 + 1];    /* 이용자접속기기ID          (필수)라고는 하는데 고객번호로만 조회하면 내려준다고 함        */
	char        NCFR_UNIQ_NO              [32 + 1];    /* 비대면고유번호                           */
} CERT_INQUIRY;
#define SZ_tagCERT_INQUIRY sizeof(tagCERT_INQUIRY)

typedef struct tagCERT_RES_INQUIRY
{  
    char        ERR_CD                    [6 + 1];     /* 에러코드					 (필수       )                                   1*/
	char        CERT_SEQ_NO               [12 + 1];    /* 인증서일련번호             (필수       )                                   2*/
	char        MSB_CERT_SEQ_NO           [15 + 1];    /* MSB인증서일련번호          (필수       )                                   3*/
	char        CUST_NO                   [20 + 1];    /* 고객번호                   (필수       )                                   4*/
	char        RQST_CHNL_DCD             [20 + 1];    /* 매체구분코드               (필수       )                                   5*/
	char        CERT_STTS_DCD             [2  + 1];    /* 인증서상태구분코드         (필수       )                                   6*/
	char        CERT_ISNC_TS              [17 + 1];    /* 인증서발급일시             (필수       )                                   7*/
	char        CERT_DCRD_TS              [17 + 1];    /* 인증서폐기일시             (필수       )                                   8*/
	char        VLTE_STTG_YMD             [8 + 1];     /* 유효기간시작년월일         (필수       )                                   9*/
	char        VLTE_EXPY_YMD             [8 + 1];     /* 유효기간만료년월일         (필수       )                                   10*/
	char        USR_CCTN_MCTL_ID          [200 + 1];   /* 이용자접속기기ID           (필수       )                                   11*/
	char        USR_CCTN_MCTL_OPSS_DCD    [2 + 1];     /* 아용자접속기기OS구분코드   (필수       )                                   12*/
	char        USR_CCTN_MCTL_OPSS_VRSN_VL[50 + 1];    /* 이용자접속기기OS버전값     (필수       )                                   13*/
	char        USR_CCTN_MCTL_MDL_NM	  [100 + 1];   /* 이용자접속기기모델명       (필수       )                                   14*/
	char        PIN_LOCK_TS				  [17 + 1];    /* PIN잠김일시                (필수       )                                   15*/
	char        PIN_LOCK_YN				  [1 + 1];     /* PIN잠김여부                (필수       )                                   16*/
	char        PIN_EROR_CNT			  [3 + 1];     /* PIN오류횟수                (필수       )                                   17*/
	char        BIO_LOCK_TS			      [17 + 1];    /* BIO잠김일시                (필수       )                                   18*/
	char        BIO_LOCK_YN			      [1 + 1];     /* BIO잠김여부                (필수       )                                   19*/
	char        BIO_USE_YN			      [1 + 1];     /* BIO사용여부                (필수       )                                   20*/
	char        BIO_EROR_CNT     	      [3 + 1];     /* BIO오류횟수                (필수       )                                   21*/
	char        BIO_DISUSE_TS     	      [17 + 1];    /* BIO사용중지일시            (필수       )                                   22*/
	char        BIO_CRTC_DCD     	      [2 + 1];     /* BIO인증구분코드            (필수       )                                   23*/
	char        NCFR_UNIQ_NO     	      [32 + 1];    /* 비대면고유번호             (필수       )                                   24*/
	char        PRIV_CTFC_RE_ERLM_OBJT_YN [1 + 1];     /* 육안확인완료여부           (필수       )                                   25*/
	char        PRIV_CTFC_ERLM_MTHD_CD    [2 + 1];     /* 사설인증등록방법코드       (필수       )                                   26*/
	char        PRSF_CFRM_MTHD_CD         [2 + 1];     /* 본인확인방법코드           (필수       )                                   27*/
	char        NCFR_UNIQ_NO_REG_TS       [17 + 1];    /* 본인확인방법코드등록일시   (필수       )                                   28*/
	char        NCFR_STATUS_MDFY_TS       [17 + 1];    /* 비대면육안확인상태변경일시 (필수       )                                   29*/
} CERT_RES_INQUIRY;
#define SZ_tagCERT_RES_INQUIRY sizeof(tagCERT_RES_INQUIRY)


//. 인증서 로그인 사전거래 등록 (IBK MBGID 0105)
//(1) 모바일인증서를 통해 로그인을 처리하기 위한 사전거래를 처리한다.
//(2) 로그인 프로세스는 임의의 난수를 생성하고, 추후 검증거래에서 해당 난수를 서명한 내용을 검증하는 프로세스
//(3) 사전거래는 매체구분코드와 일련번호, 이용자접속기기ID를 체크하여, 유효한 접속기기 여부가 판단이 되면 생성된 난수를 거래번호와 함께 클라이언트로 전달한다.
//(5) 생체인식정보가 수정되었다는 메시지를 받은 경우, 생체인증을 중지시킨다.
//(6) 환경설정에 등록되어 있는 만료 공지여부 값을 이용하여, 인증서 갱신이 가능할 경우 만료 공지여부를 'Y'로 설정하여 응답해 준다.

//인증서 로그인 사전거래 등록
typedef struct tagRegLogin_PRETRAN
{  
    char        RQST_CHNL_DCD             [20 + 1];    /* 요청매체구분코드          (필수)    API를 호출한 매체구분코드               */
	char        CERT_SEQ_NO               [12 + 1];    /* 인증서 일련번호			(필수) 									              */
    char        MSB_VRSN_VL               [32 + 1];    /* 라이브러리 버전정보       (필수) 					              */                              
    char        USR_CCTN_MCTL_ID          [200 + 1];    /* 이용자접속기기ID         (필수)라고는 하는데 고객번호로만 조회하면 내려준다고 함        */
	char        BIO_INFO_MDFC_YN          [1 + 1];    /* BIO정보변경여부            (필수)                 */
} RegLogin_PRETRAN;
#define SZ_tagRegLogin_PRETRAN sizeof(tagRegLogin_PRETRAN)


typedef struct tagRegLogin_RES_PRETRAN
{  
    char        ERR_CD                    [6 + 1];     /* 에러코드					 (필수       )                                   1*/
	char        CERT_TRAN_NO              [20 + 1];    /* 인증서거래번호             (필수       )                                   2*/
	char        LGN_ELSG_ONTT_CON         [1000 + 1];  /* 로그인전자서명원문내용     (필수       )                                   3*/
	char        EXPY_GDNC_YN              [1 + 1];     /* 만료공지여부               (필수       )                                   4*/
	char        BIO_INFO_MDFC_YN          [1 + 1];     /* BIO정보변경여부            (필수       )                                   5*/
	char        VLTE_STTG_YMD             [8  + 1];    /* 유효기간시작년월일         (필수       )                                   6*/
	char        VLTE_EXPY_YMD             [8 + 1];     /* 유효기간만료년월일         (필수       )                                   7*/
	char        PIN_EROR_CNT              [3 + 1];     /* PIN오류횟수                (필수       )                                   8*/
	char        BIO_USE_YN                [1 + 1];     /* BIO사용여부                (필수       )                                   9*/
	char        BIO_CRTC_DCD              [2 + 1];     /* BIO인증구분코드            (필수       )                                   10*/
} RegLogin_RES_PRETRAN;
#define SZ_tagRegLogin_RES_PRETRAN sizeof(tagRegLogin_RES_PRETRAN)

/////////////////////////////////////////////////
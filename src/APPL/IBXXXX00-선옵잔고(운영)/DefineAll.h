//Map Size 
#define	MAP_WIDTH	300
#define	MAP_HEIGHT	200

#define WM_REMAIN		WM_USER + 2
#define WM_ALLACCNREMAIN	WM_USER + 3
#define WM_REMAIN_CLOSE		WM_USER + 4
#define WM_LAWCHEG		WM_USER + 5
// 체결 symbol
#define RT_ORDNO	"904"	// 주문번호
#define RT_JCODE	"907"	// 종목코드
#define RT_JNAME	"908"	// 종목명
#define RT_GUBN		"912"	// 구분
#define RT_JPRICE	"910"	// 평가금액
#define RT_CPRICE	"916"	// 체결가
#define RT_CAMT		"931"	// 체결량
#define RT_STATE	"988"	// 주문상태
#define RT_CREDIT	"911"	// 신용구분
#define RT_CTDATE	"994"	// 신용대출일(8)
#define RT_CTSYMT	"933"	// 신용금/대출상환금
#define RT_JFUNIT	"995"	// 주식선물거래단위(10)
#define RT_MOD		"999"	// Mod Value
#define RT_JGTYPE	"993"	// 잔고코드

#define RT_ANAME	"906"	// 계좌명
#define RT_MCAMT	"921"	// 미체결량
#define RT_ORDGB	"922"	// 주문구분
#define RT_ORDQTY	"909"	// 주문수량
#define RT_JOTYPE	"996"	// 접수,체결
#define RT_JTYPE	"926"	// 종목시장

#define	RT_CCURR	"023"	// 체결:현재가
#define	RT_CDIFF	"024"	// 체결:대비
#define RT_CRATE	"033"	// 체결:등락률
#define	RT_CYYDT	"370"	// 체결:영업일수
#define	RT_CDDDT	"314"	// 체결:달력일수

// 체결 구분	  
enum { 
	CHEG_NOT = 0,			// 0 not
	CHEG_UPDATE,			// 1 update
	CHEG_DELETE,			// 2 delete
	CHEG_INSERT			// 3 insert(oop send)
};

#define	LEN_HCODE	7		// 주식 종목코드 길이
#define	LEN_FCODE	8		// 선물 종목코드 길이

#define	LEN_ACODE	12		// 주식 전체코드 길이
#define	LEN_SYGB	2		// 신용구분 길이
#define	LEN_DATE	8		// 날짜
#define	LEN_SYMT	15		// 신용대출금액 길이
#define LEN_JGGB	2

#define sv_SJGO     "SONAQ052"
#define sv_FJGO     "SONBQ021"
#define sv_LAW      "SONAQ056"

#define TAX_RATE 0.003
#define TAX_RATE_K_OTC 0.005	//2015.04.22 KSJ K-OTC 수수료율
/*******************************************************************************
 * 주식 잔고평가 조회 IO FORMAT
 ******************************************************************************/
/*
struct	s_mid	{		// 주식 Input
	char	accn[8];
	char	cogb[2];
	char	pswd[8];
	char	cod2[12];
	char    sygb[1];	// 0.신용제외 1.All
	char    fees[1];	// 매입단가 수수료 0.포함 1.미포함 2006.10.30 매입단가 수수료 미포함 추가
};

#define	CREDIT	// 신용사용

struct	s_rec	{
	char	cod2[12];	// 종목코드
	char	hnam[40];	// 종목명
#ifdef	CREDIT
	char	sygb[2];	// 신용구분
				// 01:유통융자, 02:단기융자, 03:일반신용, 05:유통대주, 06:매입자금, 07:자기대주, 08:예탁담보,
				// 10:KP, 20:KQ, 30:OTC, 40:채권, 50:대용
#endif
	char	xqty[15];	// 결제잔고
	char	bqty[15];	// 주문잔량
	char	pprc[15];	// 매입평균가
	char	cqty[15];	// 매도가능잔고
	char	curr[15];	// 현재가
	char	pamt[15];	// 평가금액
	char	psam[15];	// 평가손익
	char	srat[15];	// 평가수익률
	char	mamt[15];	// 매입금액
#ifdef	CREDIT
	char	symt[15];	// 신용대출금액
	char	sydt[8];	// 대출일
#endif
};


struct	s_mod	{
	char	accn[10];
#ifdef	CREDIT

#else
	char	accm[40];	// 계좌명			
#endif
	char	nrec[4];
	struct	s_rec	rec[1];
};
*/

/*******************************************************************************
 * 주식 잔고평가 조회 IO FORMAT
 ******************************************************************************/
struct  s_mid   {
    char    accn[11];
    char    pswd[8];
    char    cod2[12];
    char    allf[1];    /* 0.신용제외 1.All */
    char    fill[10];
};

struct  s_rec   {
    char    cod2[12];   /* 종목코드         */
    char    hnam[40];   /* 종목명           */
    char    jggb[2];    /* 잔고코드         */
    char    jqty[10];   /* 잔고수량         */
    char    xqty[10];   /* 매도가능         */
    char    pamt[10];   /* 매입평균가       */
    char    mamt[15];   /* 매입금액         */

    char    curr[10];   /* 현재가     (*)   */
#ifdef SISE_EXTEND
	char    diff[10];	/* 전일대비         */
	char    rate[10];   /* 등락률           */
#endif
    char    camt[15];   /* 평가금액         */
    char    tamt[15];   /* 평가손익   (*)   */
    char    srat[10];   /* 평가수익률 (*)   */
    char    sycd[2];    /* 신용코드         */
    char    sydt[8];    /* 대출일           */
    char    samt[15];   /* 신용금액         */
}; 
struct  s_mod   {
    char    accn[11];
    char    nrec[4];
    struct  s_rec   rec[1];
}; 

/*******************************************************************************
 * 법인 체결내역조회 IO FORMAT
 ******************************************************************************/
struct  law_mid   {
	char	rec_in[5];
    char    acc_gb[1];
    char    mng_no[3];
    char    mk_cod[2];
    char    con_gb[2];    
    char    grp_no[20];
};

struct  law_rec   {
    char    mng_no[3];     /* 관리지점번호     */
    char    acc_no[20];    /* 계좌번호         */
    char    acc_nm[40];    /* 계좌명           */
    char    cod_no[12];    /* 종목번호         */
    char    cod_nm[40];    /* 종목명           */
    char    trd_gb[1];     /* 매매구분         */
    char    ord_cd[2];     /* 주문유형코드     */
    char    ord_nm[40];    /* 주문유형명       */
    char    ord_qty[16];   /* 주문수량         */
    char    chg_qty[16];   /* 체결수량         */
    char    chg_avg[13];   /* 체결평균가       */
    char    chg_amt[16];   /* 체결금액         */
}; 
struct  law_mod   {
    char    rec_out[5];
	char	serial_no[10];
    char    nrec[5];
    struct  law_rec   rec[1];
}; 

/*******************************************************************************
 * 선물/옵션 잔고평가 조회 IO FORMAT
 ******************************************************************************/
struct	f_mid	{		// 선물 Input
	char	accn[11];
	char	pswd[8];
	char	date[8];
	char	cod2[8];
};

struct	f_rec	{
	char	cod2[8];	/* 종목코드		*/
	char	hnam[30];	/* 종목명		*/
	char	dsgb[6];	/* 구분			*/
	
	char	bqty[10];	/* 수량			*/
	char	cqty[10];	/* 매도가능수량		*/
	char	pprc[10];	/* 평균가/정산가	*/
	char	curr[10];	/* 현재가		*/
	char	diff[10];	/* 평균가 대비		*/
	char	pamt[15];	/* 평가금액		*/
	char	psam[15];	/* 평가손익		*/
	char	prat[10];	/* 수익률		*/
	char	tamt[15];	/* 매입금액		*/
};

struct	f_mod	{
	char	accn[11];
	char	nrec[4];
	struct	f_rec	rec[1];
};

#define L_smid	sizeof(struct s_mid)				// Length of stock input
#define L_smod	sizeof(struct s_mod) - sizeof(struct s_rec)	// Length of stock output
#define L_srec	sizeof(struct s_rec)				// Length of stock Grid

#define L_fmid	sizeof(struct f_mid)				// Length of Future input
#define L_fmod	sizeof(struct f_mod) - sizeof(struct f_rec)	// Length of Future output
#define L_frec	sizeof(struct f_rec)				// Length of Future Grid

#define L_lawmid	sizeof(struct law_mid)				// Length of stock input
#define L_lawmod	sizeof(struct law_mod) - sizeof(struct law_rec)	// Length of stock output
#define L_lawrec	sizeof(struct law_rec)				// Length of stock Grid

enum {JK_FUTURE, JK_SPREAD, JK_CALLOPT, JK_PUTOPT, JK_JCALLOPT, JK_JPUTOPT, JK_STAR, JK_SFUTURE, JK_SFSPREAD, JK_KTBFUTURE, JK_USDFUTURE, JK_GOLDFUTURE, JK_PIGFUTURE, JK_MINIGOLDFUTURE, JK_MINI_FUTURE, JK_MINI_SPREAD, JK_MINI_CALLOPT, JK_MINI_PUTOPT};

/*******************************************************************************
 * 선물/옵션 미체결 조회 IO FORMAT
 ******************************************************************************/
struct f_cmid {
	char	accn[11];
	char	pswd[8];
	char	gubn[1];	/* 0.All 1.체결 2.미체결    */
	char	juno[6];
	char	sort[1];	/* 0.X   1.역순 2.정순      */
	char	date[8];	
	char	cod2[8];	/* 종목코드                 */
    char    mkgb[1];    /* 0.All F.선물 C.콜옵션 P.풋옵션 S.스프레드 X.특정종목 */
    char    dsgb[1];    /* 0.All 1.매도 2.매수      */
};

struct f_crec {
	char	mono[6];	/* 원주문번호		*/
	char	juno[6];	/* 주문  번호		*/
	char	ojno[6];	/* 원주문 번호		*/
	char	cod2[8];	/* 종목코드			*/
	char	hnam[30];	/* 종목명			*/
	char	odgb[8];	/* 주문구분			*/
	char	hogb[20];	/* 주문유형			*/
	char	jprc[11];	/* 주문가격			*/
	char	jqty[7];	/* 주문수량			*/
	char	dlgb[4];	/* 체결구분			*/
	char	dprc[11];	/* 체결가격			*/
	char	dqty[7];	/* 체결수량			*/
	char    dtim[6];    /* 체결시간         */
	char	wqty[7];	/* 미체결수량		*/
    char    hqty[7];    /* 정정/취소수량    */
	char	stat[8];	/* 접수상태			*/
	char	time[6];	/* 접수시간			*/
    char    jseq[6];    /* 접수번호         */
    char    yseq[7];    /* 약정번호         */
    char    ecod[4];    /* 거부코드         */
};

struct	f_cmod {
	char	accn[11];
	char	nrec[4];
	struct	f_crec rec[1];
};

#define L_fcmid	sizeof(struct f_cmid)				// Length of Future input
#define L_fcmod	sizeof(struct f_cmod) - sizeof(struct f_crec)	// Length of Future output
#define L_fcrec	sizeof(struct f_crec)				// Length of Future Grid




// 주문설정 업로드
struct _pidouini_item {
	char	usid[8  ];				// 사용자 ID
	char	innm[100];				// INI 파일명
	char	senm[100];				// 섹션명
	char	skey[100];				// KEY
	char	valu[2000];				// VALUE
	char	date[8  ];				// 업데이트 일자
};

struct _pidouini_mid {
	char	gubn[1 ];				//구분 'Q': 조회, 'I': 입력
	struct	_pidouini_item item; 
};

struct  _pidouini_aid{
	char    gubn        [ 1];           /*  구분('A')       */
	char    nrec        [ 2];           /*  COUNT           */
	struct  _pidouini_mid     mid [16];  /*  MID             */
};

#define	sz_pidouini	sizeof(struct _pidouini_mid)

struct	usrinfo {
	char    gubn[2];                /* 업무 구분                    */
	/* MY: 관심 종목                */
	/* UL: 사용자 데이타 목록       */
	/* UD: 사용자 데이타            */
	/* UX: Delet Chart		*/
	char    dirt[1];                /* 전송 방향                    */
	/* U: PC=>HOST  D: HOST=>PC     */
	char    cont[1];                /* 연속 구분                    */
	/* F: First     M: Middle       */
	/* L: Last      C: Cancel       */
	/* D: delete file               */	
	char    name[80];               /* file name (필요시만)         */
	char    nblc[5];                /* 블럭 갯수 or                 */
	/* 화일 SIZE (max. 64K)         */
	char    retc[1];                /* O: 정상      E: 에러         */
	char    emsg[40];               /* error message                */
};

struct  d_list {                        /* 사용자 데이타 LIST           */
	char    name[80];               /* file name                    */
	char    size[5];                /* file size                    */
};

#define L_usr	sizeof(struct usrinfo)
#define L_lst	sizeof(struct d_list)

#define SAVEFILENAME	"SaveRemainLog.dat"
















































































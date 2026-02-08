#ifndef __CONFIGBAR_TR_H__
#define __CONFIGBAR_TR_H__

#define INTER_UP	1
#define INTER_DOWN	2
#define INTER_RANK	3
#define INTER_GUBUN	"MY"
#define GUBUN_LEN	2
#define INTER_ACC       4
#define INTER_RECENT	5


////// 등락순위
#define RANK_TRCODE	"chbojrnk"

struct  rank_mid     {
	char    gubn[2];                /* 조회구분                     */
	char    mark[1];                /* 시장구분                     */
	/* 1 : KOSPI,   2 : KOSDAQ      */
	/* 3 : 제3시장                  */
};

struct  rank_grid    {
	char    code[6];                /* 종목코드                     */
	char    hnam[16];               /* 종목명                       */
};

struct  rank_mod     {
	char    nrec[4];                /* # of records                 */
	struct  rank_grid    grid[1];
};

#define ACC_TRCODE	"chbohold"

struct  acc_mid {			/* Message Input Data           */
	char    acno[11];		/* 계좌번호                     */
	char    pswd[8];		/* 비밀번호                     */
	char    gubn[1];		/* 0: 현물     1: 선물          */
};


struct  acc_grid {
	char    code[8];        /* A¾¸nAUμa                     */
	char    hnam[20];       /* A¾¸n¸i                       */
};

struct  acc_mod {                   /* Message Output Data             */
	char    recn[4];        /* # of Records                    */
	struct  acc_grid    grid[1];
	/* sndl = ((# of recn) * sizeof(struct grid)) + sizeof(recn)    */
};


#define RECENT_TRCODE	"chhoinqu"

struct	recent_mid {                     /* Message Input Data   */
	char    usid[12];               /* USER ID              */
};

struct recent_grid {
	char    code[6];        /* A¾¸nAUμa                    */
	char    hnam[20];       /* A¾¸n¸i                      */
};

struct  recent_mod {                   /* Message Output Data            */
	char    recn[4];        /* # of Records                   */
	struct  recent_grid    grid[1];
	/* sndl = ((# of recn) * sizeof(struct grid)) + sizeof(recn)    */
};


// 특징주

#define FEATURE_TRCODE	"chbospcl"

#define MAX_R		100
#define TIT_LEN		132
typedef struct {
	char sjgb;				// 시장구분 0:전체 1:거래소 2:코스닥
	char flag;				// 0: 제목+본문 1: 제목 2: 본문 
	char gubn[2];			// 뉴스구분 00:전체 01:시황 02:공시 03:머니투데이 04:이데일리 05:해외뉴스 
							// 06:한국경제 07:연합뉴스 08:인포스탁 09:FNGuide
	char fday[8];			// 조회일자
} FEATUREMID;

typedef struct {
	char code[6];				// 종목코드
	char titl[TIT_LEN];			// 제목
} FEATUREGRID, *LPFEATUREGRID;

typedef struct {
	char nrec[4];
	FEATUREGRID grid[MAX_R];
} FEATUREMOD, *LPFEATUREMOD;


/*******************************************************************************
 * 주식 잔고평가 조회 IO FORMAT
 ******************************************************************************/
#define mkty_STOCK  '1'
#define mkty_FUTURE '2'
#define mkty_ETC    '3'
#define odty_QUERY  '1'
#define odty_ORDER  '2'
#define odty_TRANS  '3' 

#define	ACC_SJANGO	"PIBOSJGO"
struct  s_mid   {
	char	accn[11];
	char	pswd[8];
	char	cod2[12];
	char	sygb[1];	// 0.신용제외 1.All
	char	fees[10];
};

struct  s_rec   {
	char	cod2[12];	// 종목코드
	char	hnam[40];	// 종목명
	char	jgyh[2];	// 잔고유형('00'보통, '01'신용, '02'~'05'대용, '06'기타)
	char	jqty[10];	// 잔고수량
	char	xqty[10];	// 매도가능
	char	pamt[10];	// 매입평균가
	char	mamt[15];	// 매입금액

	char	curr[10];	// 현재가
	char	diff[10];	// 전일대비
	char	rate[10];	// 등락률
	char	camt[15];	// 평가금액
	char	tamt[15];	// 평가손익
	char	srat[10];	// 평가손익률
	char	sycd[2];	// 신용코드
	char	sydt[8];	// 대출일
	char	samt[15];	// 신용금액
};

struct  s_mod   {
	char	accn[11];
	char    nrec[4];
	struct  s_rec   rec[1];
}; 

/*******************************************************************************
 * 선물/옵션 잔고평가 조회 IO FORMAT
 ******************************************************************************/
#define	ACC_FJANGO	"PIBOFJGO"
struct f_mid {  // 선물 Input
	char accn[11];
	char pswd[8];
	char date[8];
	char cod2[8];
};

struct f_rec {
	char cod2[8];	// 종목코드
	char hnam[30];	// 종목명
	char dsgb[6];	// 매매구분 - "+매수", "-매도"

	char bqty[10];	// 잔고수량
	char cqty[10];	// 청산가능수량
	char pprc[10];	// 평균가
	char curr[10];	// 현재가
	char diff[10];	// 평균가 대비
	char pamt[15];	// 평가금액
	char psam[15];	// 평가손익
	char prat[10];	// 수익률
	char tamt[15];	// 매입금액
};

struct f_mod {
	char accn[11];
	char nrec[4];
	struct f_rec rec[1];
};

#define L_smid	sizeof(struct s_mid)				// Length of stock input
#define L_smod	sizeof(struct s_mod) - sizeof(struct s_rec)	// Length of stock output
#define L_srec	sizeof(struct s_rec)				// Length of stock Grid

#define L_fmid	sizeof(struct f_mid)				// Length of Future input
#define L_fmod	sizeof(struct f_mod) - sizeof(struct f_rec)	// Length of Future output
#define L_frec	sizeof(struct f_rec)				// Length of Future Grid


#endif // __CONFIGBAR_TR_H__


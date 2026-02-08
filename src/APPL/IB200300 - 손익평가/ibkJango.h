#pragma once

/*******************************************************************************
 * 주식 잔고평가 조회 IO FORMAT
 ******************************************************************************/
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
	char    diff[10];   /* 전일대비         */
	char    rate[10];   /* 등락률           */
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

#define mkty_STOCK  '1'
#define mkty_FUTURE '2'
#define mkty_ETC    '3'
#define odty_QUERY  '1'
#define odty_ORDER  '2'
#define odty_TRANS  '3' 

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



#define L_smid sizeof(struct s_mid)    // Length of stock input
#define L_smod sizeof(struct s_mod) - sizeof(struct s_rec) // Length of stock output
#define L_srec sizeof(struct s_rec)    // Length of stock Grid

#define L_fmid sizeof(struct f_mid)    // Length of Future input
#define L_fmod sizeof(struct f_mod) - sizeof(struct f_rec) // Length of Future output
#define L_frec sizeof(struct f_rec)    // Length of Future Grid



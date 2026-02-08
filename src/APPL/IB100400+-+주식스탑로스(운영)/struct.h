
#pragma once
#pragma pack(1)
/*******************************************************************************
 * 주식 잔고평가 조회 IO FORMAT _______ PIBOSJGO
 ******************************************************************************/
#define	CREDIT	// 신용사용

struct	s_mid	{		// 주식 Input

    char    accn[11];
    char    pswd[8];
    char    cod2[12];
    char    allf[1];    /* 0.신용제외 1.All */
    char    fill[10];
};

struct	s_rec	{

	char	code[12];	// 종목코드
	char	hnam[40];	// 종목명
    char    jgyh[2];    /* 잔고유형         */
    char    jqty[10];   /* 잔고수량         */
    char    xqty[10];   /* 매도가능         */
    char    pamt[10];   /* 매입평균가       */
    char    mamt[15];   /* 매입금액         */

    char    curr[10];   /* 현재가     (*)   */
	char    diff[10];   /* 전일대비         */
    char    rate[10];   /* 등락률           */
    char    camt[15];   /* 평가금액         */
    char    tamt[15];   /* 평가손익   (*)   */
    char    srat[10];   /* 평가수익률 (*)   */
    char    sycd[2];    /* 신용코드         */
    char    sydt[8];    /* 대출일           */
    char    samt[15];   /* 신용금액         */
};

struct	s_mod	{
	char	accn[11];
	char	nrec[4];
	struct	s_rec	rec[1];
};

struct	_recR	{
	char	ask1[9];		// 1차매도호가
	char	ask2[9];		// 2차매도호가
	char	bid1[9];		// 1차매수호가
	char	bid2[9];		// 2차매수호가
};
#define	L_recR		sizeof(struct _recR)

//설정정보
struct _config
{
	int istopexe[4]{}; //0-손실목표, 1-이익, 2-보존, 3-고점대비
	int iunit[4]{};//0-손실목표, 1-이익, 2-보존, 3-고점대비
	int ival[4]{};//0-손실목표, 1-이익, 2-보존, 3-고점대비
	
	int igubn{};//구분:0-지정가, 1-시장가, 2-조건부, 3-최유리
	int ivoltype{};//0-%, 1-입력
	int ivolval{};//수량비율
	int ivol{};//수량
	int iprctype{};//가격타입
	CString sprctype;//가격타입 이름
	int iprcval{};//가격 

	int iauto[2]{};
	int	inow{};//구분:0-아니요,1-예
};

#define	nREC		4
/*******************************************************************************
//	order format
/*******************************************************************************/
//
//	order format
//
struct	_orderR	{
	char	nrec[nREC];//처리개수
	char	odgb;//주문유형 1: 일반(위탁,저축)  2: 선물옵션 3. 단주매매 4: 신용매매 5: 채권매매
	char	mkgb;//시장구분 1: 거래소 2: 코스닥 3: 프리보드  5: 장외
	char    mmgb;//매매구분 1: 매도 2: 매수 3: 정정 4: 취소
 	char	accn[11];//
	char	pswd[8];
	char	orgn[12];//주문번호
	char	code[12];//종목코드
	char	jmsr[8];//주문수량
	char	jprc[10];//주문단가
	char	hogb[2];/*호가구분 ** 일반매매 
						00: 지정가 03: 시장가 05: 조건부지정가 06: 최유리지정가 07: 최우선지정가
						09: 자사주 10: 자사주스톡옵션 12: 자사주신탁 
						61: 장개시전시간외 81: 시간외종가 91:시간외단일가 
						99: 원주문호가(정정주문시 사용)*/
	char	jmgb[2];//주문조건 01: IOC 02: FOK
//	char	jjgb[1];//정정취소 1: 일부 2: 전부
	char	crgb[1];/* 처리구분 A, B, C, D (아래의 30 Bytes 는 주문유형에 따라 유동적으로 사용)
					** 이하의 값이 셋팅되어지지 않을 경우 ‘X’
					** 이하의 30bytes 는 필요시 추가 정의 가능*/
	char	fill[30];
};
#define	L_orderR	sizeof(struct _orderR)

struct	_respR	{
	char	nrec[nREC];		// only "0001"
	char	jmno[6];
	char	jjcs[6];
	char	omsg[80];
};


struct _trkey
{
	UINT	gubn:3;
	UINT	key:7;
};

#pragma pack()
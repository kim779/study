#pragma once

#include "inc/axisfire.h"
#include "TR_IO.h"

#pragma	pack(1)

/************************************************************************/
/* AXLOGON                                                              */
/************************************************************************/
struct axlogon_mid
{
	char user[12];	/* 아이디				*/
	char pass[10];	/* 접속비밀번호			*/
	char forc[1];	/* 최초[0], 강제[2]		*/
	char atcp[1];	/* 수동[0], 자동[1]		*/
	char norm[1];	/* 주문[0], 시세[1]		*/
	char cust[1];	/* 직원[0], 고객[1]		*/
	char type[1];	/* OPEN API[Y] 대문자	*/	//2012.06.01 추가 KSJ
	char rsvd[5];	/* Reserved				*/	//2012.04.17 6 -> 5로 수정
	char cpas[30];	/* 공인인증비밀번호		*/
	char uips[15];	/* IP-Address			*/
	char madr[16];	/* MAC-Address			*/
};

struct axlogon_mod
{
	char rcod;
	char gbip[32];
};

#define L_axlogon_mid sizeof(struct axlogon_mid)
#define L_axlogon_mod sizeof(struct axlogon_mod)


/************************************************************************/
/* USER_TH                                                              */
/************************************************************************/
struct user_th
{
	char trxc[8];
	char key;
	char stat;
};
#define L_user_th sizeof(struct user_th)

/************************************************************************/
/* 계좌 비밀번호 검증                                                   */
/************************************************************************/
struct SACMT207_mid {
	char in [5];
	char acno[20];
	char acpw[8];
	char zTrxTp[1];
};
#define L_SACMT207_mid sizeof(struct SACMT207_mid)

struct SACMT207_mod {
	struct SACMT207_mid mid;
	char out[5];
	char lPwdContErrCnt[9];
	char lAtmPwdContErrCnt[9];
	char lMtrsfPwdContErrCnt[9];
	char zAcntNm[40];
};
#define L_SACMT207_mod sizeof(struct SACMT207_mod)

/************************************************************************/
/* LEDGER                                                               */
/************************************************************************/
struct ledger {
	char	size[6];	// 0	전문길이			I	S	X	전문전체(Size-6)
	char	type[1];	// 6	전문구분			I	S		B:계정계, I:정보계, F:파일송수신, N:Notice 자동갱신(R:정보계, D:계정계)
	char	encf[2];	// 7	압축구분			I/O	S	X	전문압축/암호화구분(00:압축,암호화 안함)
	char	dptr[3];	// 9	데이터 옵셋			I/O	S		입/출력 데이타가 시작되는 위치(공통헤더를 포함)
	char	svcd[8];	// 12	서비스코드			I	Cf	●	서비스코드               ♥ TxHead(3) of Map
	char	cono[3];	// 20	회사번호			I	S		회사번호 "991"
	char	usid[16];	// 23	사용자ID			I	C		트랜잭션을 발생시킨 사용자 고유정보 (예:사용자 접속ID)
	char	cogb[2];	// 39	접속분류코드		I	S		접속 접속매체 (HT : 자체HTS)
	char	fep1[12];	// 41	접속채널번호		I		X	접속 순차번호(3)+일련번호(9)
	char	pbid[16];	// 53	트랜잭션추적ID		I	Cf	X	전문 추적ID ('@'로시작)  ♥ 로그인 성공시 받은값 유지
	char	puip[12];	// 69	공인IP				I	S		공인IP (168.126.63.1의 경우 "168126063001" 형태)
	char	pcip[12];	// 81	사설IP				I	C		사설IP (168.126.63.1의 경우 "168126063001" 형태)
	char	dept[3];	// 93	처리지점 번호		I			처리PC가 위치한 지점
	char	brno[3];	// 96	지점번호			I			계좌가 속한 지점
	char	term[8];	// 99	단말번호			I	Cm	●	매체코드 (직원 : 001 고객 : 002)
	char	lang[1];	// 107	언어구분			I	S		K:한글, E:영문, C:중국, J:일본
	char	time[9];	// 108	AP처리시각			O			요청 처리시각 (HHMMSSmmm)
	char	ecod[4];	// 117	메시지코드			O			오류코드 :(정상코드: 0000 ~ 0999, 비정상코드 : 1000 ~ 9999, 요청시 Space로 채움)
	char	eknd[1];	// 121	출력메시지구분		O			
	char	fep2[1];	// 122	압축요청구분		I		X	
	char	fkey[4];	// 123	기능키목록			I/O	Cm	●	입력: 기능키(1바이트) ♥ TxHead(2) of Map (5:확인 7:연속 C:전송)
	char	rcnt[4];	// 127	요청 레코드 개수	I	Cm	●	일반적으로 "0000"을 입력
	char	fil1[6];	// 131	Filler				-		X	
	char	next[1];	// 137	연속여부			I/O	C		연속 데이터 유무 (Y:연속있음, 이외의 값:연속없음)
	char	nkey[18];	// 138	연속키				I/O	C	●	연속처리를 위한 키값
	char	usiz[2];	// 156	가변시스템정보길이	I		X	IBK 고정	
	char	hsiz[2];	// 158	가변헤더정보길이	I/O		X	미사용
	char	msiz[2];	// 160	가변메시지길이		O		X	성공 및 오류메시지 (요청시 "00"입력)
	char	fep3[10];	// 162	Reserved for  FEP		S	X	
	char	mkty[1];	// 172	시장구분				Cfm	●	1:현물 2:선옵 3:기타    ♥TxHead(0) of MAP
	char	odrf[1];	// 173	주문구분				Cfm	●	1:주문 2:조회 3:이체    ♥TxHead(1) of MAP
	char	evid[4];	// 174	Reserved for  I/F			X	
	char	svrm[12];	// 178	서버IP					S		서버IP (168.126.63.1의 경우 "168126063001" 형태)
	char	skip[1];	// 190	Flag for I/F
	char	sjmf[1];	// 191	Flag for I/F
	char	csym[8];	// 192	사용자정의 ID KSJ 2012.06.04
	char	fil2[40];	// 200	Filler						X	
	char	eact[1];	// 240	에러구분						에러구분(0:상태바 1:메세지박스)
	char	emsg[99];	// 241	오류메세지						코드(4)+메시지(94)(9999 오류입니다 !)
	char	epwd[44];	// 340	sha256암호화 된 계좌비밀번호
};
#define	L_ledger	sizeof(struct ledger)		// 340 bytes	//2014.04.15 암호화비번 추가해서 384 bytes

struct pidouini_item
{
	char usid[ 8];   /* 사용자 ID                */
	char innm[100];  /* INI 파일명               */
	char senm[100];  /* 섹션명                   */
	char skey[100];  /* Key                      */
	char valu[500];  /* Value                    */
	char date[ 8];   /* 업데이트일자             */
};

struct pidouini_mid
{
	char gubn[1];
	struct pidouini_item item;
};
#define L_pidouini_mid		sizeof(struct pidouini_mid)

struct pidouini_mod
{
	struct pidouini_item item;
};
#define L_pidouini_mod		sizeof(struct pidouini_mod)

struct  pihoitgy_grid
{	
    char    gubn[3];            /* 구분                 */	
	/* ex) exe, dev, map    */	
    char    mnam[47];           /* 모듈명               */	
};

struct  pihoitgy_grid2
{
    char    gubn[3];            /* 구분                     */
	/* ex) exe, dev, map        */
    char    mnam[47];           /* 모듈명                   */	
    char    skey[44];           /* 클라이언트 SHA256 결과   */
};

struct  pihoitgy_mid 
{	
    char    gubn[1];            /*  1: LIST 요청    2: 무결성 검증  */	
    char    nrec[4];            /* 무결성 검증시 요청 갯수          */	
    struct  pihoitgy_grid2   grid2[1];	
};
#define L_pihoitgy_mid		sizeof(struct pihoitgy_mid)

struct  pihoitgy_mod 
{	
    char    nrec[4];            /*  갯수            */	
    struct  pihoitgy_grid    grid[1];    	
};
#define L_pihoitgy_mod		sizeof(struct pihoitgy_mod)

struct pibotran_mid {
    char seqn[6];
    char start[8];
    char file[64];  
};
#define L_pibotran_mid		sizeof(struct pibotran_mid)

struct pibotran_mod {
    char seqn[6];
    char max_seqn[6];
    char start[8];
    char size[8];
    char data[1024*32];
};
#define L_pibotran_mod		sizeof(struct pibotran_mod)

// 시스템매매 교육이수 등록 서비스 코드 : 3056
#define SYSTRADING_SVCCODE		"3056"
struct saceq320_mid
{
	char In[5];
	char fnUserId[16];
	char servCode[4];
};
#define L_saceq320_mid		sizeof(struct saceq320_mid)

struct saceq320_grid
{
	char Acnt[20];
};

struct saceq320_mod
{
	struct saceq320_mid mid;
	char In[5];
	struct saceq320_grid grid[1];
};

struct sacaq504_mid
{
	char In[5];
	char QryTp[1];           /* 조회구분         */
	char SvcKindCode[4];     /* 서비스종류코드   */
	char PdptnCode[2];       /* 상품유형코드     */
	char AcntNo[20];         /* 계좌번호         */
	char BrnNo[3];           /* 지점번호         */
	char QrySrtDt[8];        /* 조회시작일       */
	char QryEndDt[8];        /* 조회종료일       */
	char RegTp[1];           /* 등록구분         */
	char AcntStatCode[2];    /* 계좌상태코드     */
};
#define L_sacaq504_mid		sizeof(struct sacaq504_mid)

struct sacaq504_grid
{
	char AcntNo[20];          /* 계좌번호               */
	char AcntNm[40];          /* 계좌명                 */
	char PdptnCode[2];        /* 상품유형코드           */
	char PdptnNm[40];         /* 상품유형명             */
	char RegTp[1];            /* 등록구분               */
	char RegTpNm[20];         /* 등록구분               */
	char SvcKindCode[4];      /* 서비스종류코드         */
	char SvcKindNm[40];       /* 서비스종류명           */
	char TrdIttNm[40];        /* 거래기관명             */
	char OppIttAcntNo[20];    /* 상대기관계좌번호       */
	char OppTrdAcntNm[40];    /* 상대거래계좌명         */
	char CtrctRegDt[8];       /* 약정등록일             */
	char CtrctSrtDt[8];       /* 약정시작일             */
	char CtrctXpirDt[8];      /* 약정만료일             */
	char IntaccNo[20];        /* 종합계좌번호           */
	char BnkDlgConnAcntNo[20];/* 은행대표연결계좌번호   */
	char BnkCpcrdNo[20];      /* 은행제휴카드번호       */
	char BbkpgPtnCode[2];     /* 일괄대체유형코드       */
	char BbkpgPtnNm[40];      /* 일괄대체유형           */
	char OthbkpTp[1];         /* 타사대체구분           */
	char OthbkpTpNm[10];      /* 타사대체구분명         */
	char AbkpDt[2];           /* 자동대체일             */
	char AmtrBkeepAmt[16];    /* 자동이체대체금액       */
	char UndDsgnYn[1];        /* 인수도지정여부         */
	char UndDsgnNm[40];       /* 인수도지정             */
	char BuyFundNo[12];       /* 매수펀드번호           */
	char SellFundNo[12];      /* 매도펀드번호           */
	char Rmnno[13];           /* 실명확인번호           */
	char Imreq[16];           /* 신용설정보증금         */
	char RatVal[19];          /* 비율값                 */
	char MgempNm[40];         /* 관리사원명             */
	char AbndDt[8];           /* 해지일                 */
	char AbndEmpNo[9];        /* 해지사원번호           */
	char AbndPtnNm[40];       /* 해지유형               */
	char ChgUserId[16];       /* 변경사용자ID           */
	char AcntStatNm[40];      /* 계좌상태명             */
	char TrxIserId[16];		  /* 처리자ID               */	//2014.03.31 추가됨
	char TrxBrn[3];			  /* 처리지점               */	//2014.03.31 추가됨
	char AcntOpnDt[8];		  /* 계좌개설일             */	//2014.03.31 추가됨
};

struct sacaq504_mod
{
	struct sacaq504_mid mid;
	char Out[5];              
	char AcntNm[40];          /* 계좌명                 */
	char BrnNm[40];           /* 지점명                 */
	char Out2[5];             
	struct sacaq504_grid grid[1];	
};


#pragma	pack()
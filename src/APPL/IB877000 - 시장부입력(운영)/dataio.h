#pragma once

#define MAX_R			20      /* max grid row     */
#define TIT_LEN			132     /* length of title  */
#define DAT_LEN         1536
#define MAX_CODES       10	
#define SIHWANG_GUBN    1
#define M_LINE			15      /* max. # of lines  */
#define M_COL			132     /* 공시             */
#define X_COL			80      /* 뉴스, 시황       */
#define MAX_PIDOTUJA	64

#define TK_PIBO7710		0
#define TK_PIBONEWS		1
#define TK_PIDOSIHW		2
#define TK_PIDOTUJA		4

/* GRID_T.gdir */
#define GD_TOP      1       /* top ending           */
#define GD_BOTTOM   2       /* bottom ending        */

/* GRID_T.aflg */
#define AD_NOT      0       /* reset                */
#define AD_TOP      1       /* add to top           */
#define AD_BOTTOM   2       /* add to bottom        */

/* GRID_T.xpos */
#define PS_END      0x40               /* no continuoue          */
#define PS_PREV     0x01               /* enable previous        */
#define PS_NEXT     0x02               /* enable next            */
#define PS_PNXT     (PS_PREV|PS_NEXT)  /* enable next & previous */
#define PS_SORT     0x04               /* no local sorting       */

/* GRID_T.ikey */
#define KY_ENTER    0       /* enter : default          */
#define KY_PGUP     1       /* PageUp                   */
#define KY_PGDN     2       /* PageDn                   */
#define KY_SCUP     3       /* ScrollUp                 */
#define KY_SCDN     4       /* ScrollDn                 */
#define KY_FIRST    5       /* First or End             */
#define KY_SORT     6       /* click sorting column     */

typedef struct {            /* GRID form input format       */
    char    vrow[2];        /* visible rows                 */
    char    nrow[4];        /* # of rows                    */
    char    vflg[1];        /* if '1' variable row          */
    char    gdir[1];        /* add to top or bottom         */
							/* '1':top, '2':bottom          */
    char    sdir[1];        /* sort direction               */
							/* '1':asc, '2':desc            */
    char    scol[16];       /* sorting column symbol        */
    char    ikey[1];        /* key action                   */
							/* '0':Enter or Other           */
							/* '1':PgUp '2':PgDn            */
							/* '3':ScrollUp '4':ScrollDn    */
							/* '5':First or End             */
							/* '6':Please sort              */
    char    page[4];        /* current page number          */
    char    save[80];       /* grid inout save field        */
} GRID_I;

typedef struct {            /* GRID form output format         */
    char    aflg[1];        /* add action to top or bottom     */
							/* '0':replace '1':top, '2':bottom */
    char    sdir[1];        /* sort direction                  */
							/* '1':asc, '2':desc               */
    char    scol[16];       /* sorting column symbol           */
    char    xpos[1];        /* continuous status               */
							/* 0x40: default(normal)           */
							/* 0x01: PgUp, ScrUp enable        */
							/* 0x02: PgDn, ScrDn enable        */
							/* 0x04: no local sorting          */
    char    page[4];        /* # of page                       */
    char    save[80];       /* grid inout save field           */
} GRID_O;

typedef struct {      /* NEXT KEY       */
    char    cod2[6];        /* 종목코드       */
    char    fday[8];        /* 조회일자       */
//  char    gubn[13];       /* 구분           */
	char    gubn[20];       /* 구분           */  //test  13->20
    char    skey[20];       /* 검색어         */
    char    isrn[10];       /* ISAM KEY       */
} pibo7710_nkey;

typedef struct {
    char    cod2[6];        /* 종목코드                         */
    char    fday[8];        /* 조회일자                         */
//	char    gubn[13];       /* 구분                             */
	char    gubn[20];       /* 구분                             */ //test  13->20
							/* gubn[0 ] : 시황                  */
							/* gubn[1 ] : 공시                  */
							/* gubn[2 ] : 이투데이              */
							/* gubn[3 ] : 시황속보  -인포스탁   */
							/* gubn[4 ] : 전자신문              */
							/* gubn[5 ] : 이데일리              */
							/* gubn[6 ] : 월드인덱스-인포스탁   */
							/* gubn[7 ] : 연합뉴스              */
							/* gubn[8 ] : 머니투데이            */
							/* gubn[9 ] : 한경뉴스              */
							/* gubn[10] : DUMMY                 */
							/* gubn[11] : DUMMY                 */
    char    skey[20];       /* 검색어                           */
    char    subg[3];        /* 분류코드(전체:'999')             */
    GRID_I  grid_i;
    char    type[ 1];       /* 타입('1':종목, 그외:전체);       */
} pibo7710_mid;

typedef struct {
    char    date[8];        /* 날짜                             */
    char    time[4];        /* 시간                             */
    char    titl[TIT_LEN];  /* 제목                             */
    char    gubn[12];       /* 정보구분명                       */
    char    subg[20];       /* 분류코드명                       */
    char    cod2[12];       /* 종목코드                         */
    char    hnam[20];       /* 종목명                           */
    char    gisn[10];       /* CISAM RECORD NO.(GRID)           */
    char    sisn[10];       /* CISAM RECORD NO.(본문조회)       */
    char    keyv[24];       /* 본문조회용 KEY VALUE             */
							/* 일자(8)+정보구분(2)+분류코드(4)+일련번호(10) */
    char    flag[1];        /* TEXT: 0, HTML: 1                 */
} pibo7710_grid;

typedef struct {
    GRID_O  grid_o;
    char    nrec[4];
    pibo7710_grid grid[MAX_R];
} pibo7710_mod;

typedef struct {
    char    date[8];        /* 자료일자     */
    char    gubn[2];        /* 정보구분     */
    char    subg[4];        /* 분류코드     */
    char    seqn[10];       /* 일련번호     */
    char    titl[TIT_LEN];  /* 뉴스 제목    */
} pibonews_mid;

typedef struct {
    char    titl[TIT_LEN];  /* 뉴스 제목        */
    char    subcod[10][12]; /* 서브 코드        */
    char    size[5];        /* size of data     */
    char    data[1];
} pibonews_mod;


struct  pnewsrc_mid {
    char    date[8];        // 자료일자     
    char    gubn[2];        // 정보구분     
    char    subg[4];        // 분류코드     
    char    seqn[10];       // 일련번호     
    char    titl[TIT_LEN];     // 뉴스 제목        
};

struct  pnewsrc_mod {
	char	cflag[1];				//2017.02.10 KSJ 데이터 맞춤
	char    titl[TIT_LEN];     // 뉴스 제목        
    char    subcod[10][12]; // 서브 코드 
    char    size[7];        // size of data     	//2015.11.20 KSJ size 5 -> 7
    char    data[1];
};


typedef struct {
    char code[6];
} pidosihw_jdat;

typedef struct {
    char flag[1];                   /* I,U,D 입력구분    */
	char step[1];                   /* F, M, L           */
    char usid[8];                   /* 입력직원 ID(사번) */
    char date[8];                   /* 입력일            */
    char time[6];                   /* 입력시간          */
    char seqn[8];                   /* 일련번호          */
    char gubn[8];                   /* 정보구분          */
    char subg[8];                   /* 분류코드          */
    char titl[TIT_LEN];             /* 제목              */
    pidosihw_jdat cods[MAX_CODES];  /* 종목코드          */
    char dlen[8];                   /* 본문길이          */
    char data[1];                   /* 본문              */
} pidosihw_mid;

typedef struct {
    char seqn[8];   /* 입력Sequence      */
    char emsg[128]; /* 결과 메세지       */
} pidosihw_mod;

typedef struct {
    char tjcd[3];           /* 투자자코드      */
    char cod2[12];          /* 종목코드        */
    char msgb[1];           /* 1(매수),2(매도) */
    char xvol[8];           /* 수량(주)        */
} pidotuja_grid;

typedef struct {
    char flag[1];           /* I(Insert)       */
    char seqn[2];           /* 회차            */
    char nrec[8];           /* record #        */
    pidotuja_grid grid[1];    
} pidotuja_mid;

typedef struct mod {
    char emsg[128];         /* 결과메세지      */
} pidotuja_mod;      


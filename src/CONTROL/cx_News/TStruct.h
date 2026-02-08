#ifndef	_REPORTSTRUCT_H
#define	_REPORTSTRUCT_H

#if _MSC_VER > 1000
#pragma once
#endif

// Basic Color...
#define COR_BLUE	RGB(0, 0, 255)
#define COR_RED		RGB(255, 0, 0)
#define COR_BLACK	RGB(0, 0, 0)
#define COR_WHITE	RGB(255, 255, 255)

#define	cTAB	'\t'
#define MAX_R	20

#define L_title 132

typedef struct  {                       /* GRID form input format       */
        char    vrow[2];                /* visible rows                 */
        char    nrow[4];                /* # of rows                    */
        char    vflg[1];                /* if '1' variable row          */
        char    gdir[1];                /* add to top or bottom         */
                                        /* '1':top,     '2':bottom      */
        char    sdir[1];                /* sort direction               */
                                        /* '1':asc,     '2':desc        */
        char    scol[16];               /* sorting column symbol        */
        char    ikey[1];                /* key action                   */
                                        /* '0':Enter or Other           */
                                        /* '1':PgUp     '2':PgDn        */
                                        /* '3':ScrollUp '4':ScrollDn    */
                                        /* '5':First or End             */
                                        /* '6':Please sort              */
        char    page[4];                /* current page number          */
        char    save[80];               /* grid inout save field        */
} GRID_I;                               /* grid header                  */

typedef struct  {                       /* GRID form output format      */
        char    aflg[1];                /* add action to top or bottom  */
                                        /* '0':replace '1':top, '2':bottom */
        char    sdir[1];                /* sort direction               */
                                        /* '1':asc,     '2':desc        */
        char    scol[16];               /* sorting column symbol        */
        char    xpos[1];                /* continuous status            */
                                        /* 0x40: default(normal)        */
                                        /* 0x01: PgUp, ScrUp enable     */
                                        /* 0x02: PgDn, ScrDn enable     */
                                        /* 0x04: no local sorting       */
        char    page[4];                /* # of page                    */
        char    save[80];               /* grid inout save field        */
} GRID_O;    


struct  grid    {
        char    date[8];        /* 날짜                         */
        char    time[4];        /* 시간                         */
        char    titl[L_title];       /* 제목                         */
        char    gubn[12];       /* 정보구분                     */
        char    subg[20];       /* 분류코드                     */
        char    hnam[20];       /* 종목명                       */
        char    gisn[10];       /* CISAM RECORD NO.(GRID)       */
        char    sisn[10];       /* CISAM RECORD NO.(본문조회)   */
	char    keyv[22];       /* 본문조회용 KEY VALUE         */
        char    flag[1];        /* TEXT: 0, HTML: 1             */
};

struct  grid3   {
        char    date[8];        /* 날짜                         */
        char    time[4];        /* 시간                         */
        char    titl[L_title];	/* 제목                         */
        char    subg[20];       /* 분류코드                     */
        char    hnam[20];       /* 종목명                       */
        char    gisn[10];       /* CISAM RECORD NO.(GRID)       */
        char    sisn[10];       /* CISAM RECORD NO.(본문조회)   */
	char    keyv[22];       /* 본문조회용 KEY VALUE         */
        char    flag[1];        /* TEXT: 0, HTML: 1             */
};

struct  mid {
        char    cod2[6];        /* 종목코드                     */
        char    fday[8];        /* 조회일자                     */
        char    gubn[2];        /* 구분                         */
        char    skey[20];       /* 검색어                       */
        char    isrn[10];       /* CISAM RECORD NO.             */
        GRID_I  grid_i;
};

struct  mod {
        char    isrn[10];       /* CISAM RECORD NO.             */
        GRID_O  grid_o;
        char    nrec[4];
        struct  grid    grid[MAX_R];
};

struct  mod3 {
        char    isrn[10];       /* CISAM RECORD NO.             */
        GRID_O  grid_o;
        char    nrec[4];
        struct  grid3    grid[MAX_R];
};
//////////////////////////////////////////////////////////////////////////////////////]
// 조회용..
				 
struct  mid2 {
        //char    isrn[10];               /* CISAM RECORD NO.     */
	char	keyv[22];		/* 본문조회용 keyv	*/
        char    titl[L_title];          /* 뉴스 제목            */
};

struct  mod2 {
        char    titl[L_title];          /* 뉴스 제목            */
        char    size[5];                /* size of data         */
        char    data[1];
};

struct  mid_M {
        char    cod2[6];        /* 종목코드                     */
        char    fday[8];        /* 조회일자                     */
        char    subg[3];        /* 분류코드                     */
        char    skey[20];       /* 검색어                       */
        char    isrn[10];       /* CISAM RECORD NO.             */
        GRID_I  grid_i;
};

typedef struct _alert
{
	CString	keyv;	// 본문조회용 key
	CString date;	// 날짜..
	CString titl;
	CString key;	// key..
	CString name;	// 종목명..
	CString gubn;	// 정보구분..
	CString sort;	// 분류
	CString time;	// 시간
	CString snam;	// 분류명
	CString gnam;	// 정보구분 이름..
}ALERT;

typedef struct _text
{
	CString date;	// 날짜..
	CString titl;	// 제목..
	CString key;	// key..
	CString ikey;	// second key..
	CString name;	// 종목명..
	CString gubn;	// 정보구분..
	CString sort;   // 분류
	CString time;   // 시간 
	CString snam;	// 분류명 
	CString gnam;	// 정보구분 이름..
}NEWS;

#endif	// _REPORTSTRUCT_H
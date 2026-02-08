#pragma once

#define MAX_R		20

#define title_len	132

#define MAX_NEWS 8			
#define MAX_NEWS_SUB 20
#define MAX_NEWS_COLOR 8

#define WM_COMBO_ENTERED WM_USER + 1007
//#define NEWS_SIZEGUBN 13				// 구분자 추가로 뉴스종목 개수  khs
#define NEWS_SIZEGUBN 20				// 구분자 추가로 뉴스종목 개수  khs

#pragma warning (disable : 26485)
#pragma warning (disable : 26429)
#pragma warning (disable : 26481)

typedef struct {
	char fday[8];			// 조회일자
	char vgbn[2];			// 조회기준  "00"
	char gubn[12];			// 정보구분 
	char subg[3];			// 분류코드 "999"
	char sdir[1];			// 정렬
	char cpag[4];			// 요청 페이지 
} req_news_count;

typedef struct {
	char emsg[50];
	char cpag[4];
	char tpag[4];
	char nrec[4];
} res_news_count_header;

typedef struct {
	char titl[80];
	char flag[1];
	char time[6];
	char vcnt[6];
	char date[8];
	char gubn[2];
	char subg[4];
	char seqn[10];
} res_news_count_content;

typedef struct {
	int iCurPage;
	int iMaxPage;
	int iRecCnt;
} news_count_page_header;

// 분류		  TR		  
//-----------------------------------
//종합시황	 PIBO5501
//공시		 BO550110
//머니투데이	BO550120
//한경뉴스	 BO550140
//이데일리	 BO550150

//#pragma pack(push,1)
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
} GRID_I;

typedef struct  {           	/* GRID form output format  */
    char    aflg[1];        	/* add action to top or bottom  */
                    		/* '0':replace '1':top, '2':bottom */
    char    sdir[1];        	/* sort direction       */
                    		/* '1':asc, '2':desc    */
    char    scol[16];       	/* sorting column symbol    */
    char    xpos[1];        	/* continuous status        */
                    		/* 0x40: default(normal)    */
                    		/* 0x01: PgUp, ScrUp enable */
                    		/* 0x02: PgDn, ScrDn enable */
                    		/* 0x04: no local sorting   */
    char    page[4];        	/* # of page            */
    char    save[80];       	/* grid inout save field    */
} GRID_O;               	/* grid header          */


enum sendtr_type{
	type_gongsi = 0,			// 공시
	type_etoday,				// 이투데이 1
	type_infostock,				// 인포스탁 2
	type_eletcnews,				// 전자신문 3
	type_edaily,				// 이데일리 4
	type_worldindex,			// 월드인덱스  5
	type_ibk,				//IBK투자증권      6
	type_yunhab = 7,			// 연합뉴스    7
	type_moneytoday,			// 머니투데이  8 
	type_hk,					// 한경뉴스    9
	type_mk,					// 매경뉴스    10    //test
	type_asia,                  // 11
	type_IRGO,                  // 12
	type_CONSUMER,              // 13
	type_total,					// 전체  12
	type_content,				// 뉴스본문  13
	type_category,              // 14
	type_subtotal,			// 카테고리별 조회
	type_foreigner,			//외국인/기관 동향
	type_PBOpinion,				//촉앤토크
	type_symbol = 20,
	type_news_count_day,		// 일별
	type_news_count_week,		// 주별 
	type_news_count_month,		// 월별 
	type_scrap,
	type_Edge_Env
};

//한경 추가 수정 부분
struct mid_title{
	char    cod2[6];	/* 종목코드			*/
	char    fday[8];	/* 조회일자			*/
	char	gubn[NEWS_SIZEGUBN];	/* 구분			*/
				/* gubn[0] : 시황			*/
				/* gubn[1] : 공시			*/
				/* gubn[2] : 이투데이		*/
				/* gubn[3] : 시황속보		*/
				/* gubn[4] : 전자신문		*/
				/* gubn[5] : 이데일리		*/
				/* gubn[6] : 월드인덱스		*/
				/* gubn[7] : 연합뉴스		*/
				/* gubn[8] : 머니투데이		*/
				/* gubn[9] : 한경닷컴		*/
				/* gubn[10] : 매경			*/				
				/* gubn[12] : 아시아경제	*/
	char    skey[20];       /* 검색어			*/
	
	char	sugb[3];	// 분류 영역
	GRID_I  grid_i;
	char    typ[1];
	char    sNewGubn[20];
};

struct  grid    {
        char    date[8];        /* 날짜                         */
        char    time[4];        /* 시간                         */
        char    titl[title_len];/* 제목                         */
        char    gubn[12];       /* 정보구분                     */
        char    subg[20];       /* 분류코드								 */
	char	code[12];			/* 종목코드			*/
        char    hnam[20];       /* 종목명                       */
        char    gisn[10];       /* CISAM RECORD NO.(GRID)       */
        char    sisn[10];       /* CISAM RECORD NO.(본문조회)   */
	char    keyv[24];			/* 본문조회용 KEY VALUE     정보구분 추가로 인해 +2   */		
        char    flag[1];        /* TEXT: 0, HTML: 1             */
};



//-------------------
//output
//-------------------
// 종합

struct  mod_title{
//        char    isrn[10];       /* CISAM RECORD NO.             */
        GRID_O  grid_o;
        char    nrec[4];
        struct  grid    grid[MAX_R];
};



//--------------	
//본문조회
//--------------
//PIBONEWS
struct  mid_content
{
        //char    isrn[10];             /* CISAM RECORD NO.     */
/*	char	date[8];
	char	gubn;
	char	subgubn;
	char	seqn[10];*/
	char	keyv[24];		/* Key Value */
    char    titl[title_len];        /* 뉴스 제목            */
};

struct  mod_content
{
		char	cflag[1];				//2017.02.10 KSJ 데이터 맞춤
        char    titl[title_len];        /* 뉴스 제목            */
		char	subcod[10][12];
        char    size[7];                /* size of data         */	//2015.11.20 KSJ  size 5 -> 7
        char    data[1];
};

struct  news_info_ex {
        char    gisn[10];       /* CISAM RECORD NO.(GRID)       */
        char    sisn[10];       /* CISAM RECORD NO.(본문조회)   */
		char    keyv[24];		/* 본문조회용 KEY VALUE         */
		char	code[12];		/* 코드							*/
        char    flag[1];        /* TEXT: 0, HTML: 1             */
};


//#pragma pack(pop)
struct news_info{
	CString    date;        /* 날짜                         */
	CString    time;        /* 시간                         */
	char       titl[title_len];		/* 제목                 */
	CString	   gubn;		// 정보구분 
	CString    subg;       /* 분류코드                     */
	CString    hnam;       /* 종목명                       */
	CString    gisn;       /* CISAM RECORD NO.(GRID)       */
	CString    sisn;       /* CISAM RECORD NO.(본문조회)   */
	CString    keyv;       /* 본문조회용 KEY VALUE         */
	CString    flag;        /* TEXT: 0, HTML: 1             */

	CString    snam;		//분류명
	CString    gnam;		//구분명
	CString	   key;			//key
	CString	   code;		//종목코드
	int		   rCnt;		// 조회수 
		
		news_info()
		{
			date = "";        /* 날짜                         */
			time = "";        /* 시간                         */
			memset(titl,0x00,title_len);
			//   titl = "";		/* 제목                         */
			gubn = "";		// 정보구분 
			subg = "";       /* 분류코드                     */
			hnam = "";       /* 종목명                       */
			gisn = "";       /* CISAM RECORD NO.(GRID)       */
			sisn = "";       /* CISAM RECORD NO.(본문조회)   */
			keyv = "";       /* 본문조회용 KEY VALUE         */
			flag = "";        /* TEXT: 0, HTML: 1             */
			snam = "";		//분류명
			gnam = "";		//구분명
			key = "";			//key
			code = "";		//종목코드
			rCnt = 0;
		}
};

struct group {
	int gubn;
	int subg;
};

struct  subg_group {         /* ?분류드별 gubn, subg 목록 */
	int subg;
	struct group sgroup[100];
};

#include <afxtempl.h>
#define		MAXCNT			130
struct NewsList
{
	CString		sGubnName;
	CString		sGubn;
	CString		sKey;

	BOOL		bCategory;
	int			ibulruCnt;
	CString		sBulruGubnList[MAXCNT];
	CString		sBulruNameList[MAXCNT];
	int			iCheck[MAXCNT];

	int			ikey;
	int			iGubn;
	HTREEITEM	hTreeItem;
	int			iAllCheck;
	int			iFirst;
	subg_group  stype;

	NewsList(CString sData, CString sData2, int iCnt, CString* sList, CString* sList2 )
	{
		sGubnName = sData;
		sGubn = sData2;
		iGubn = atoi(sGubn);
		ibulruCnt = iCnt;
		for(int i=0; i<iCnt; i++)
		{
			sBulruGubnList[i] = sList[i];
			sBulruNameList[i] = sList2[i];
			iCheck[i] = 1;
		}

		iAllCheck = 1;
		iFirst = 1;
		bCategory = FALSE;
	};

	NewsList(CString sData, CString sData2, int idata, CString key, BOOL bFlag, subg_group type, HTREEITEM hitem)
	{
		sGubnName = sData;
		sGubn = sData2;
		iGubn = atoi(sGubn);
		ibulruCnt = 0;	//2013.06.07 KSJ 초기화하지 않으면 가끔씩 에러발생함(스크랩, 조회수상위)
		sKey = key;
		bCategory = bFlag;
		ikey = idata;
		hTreeItem = hitem;
		stype = type;

		iAllCheck = 1;
		iFirst = 1;
	};

	NewsList(CString sData, CString sData2, int iCnt, CString* sList, CString* sList2, int idata,  HTREEITEM hitem)
	{
		sGubnName = sData;
		sGubn = sData2;
		iGubn = atoi(sGubn);
		ibulruCnt = iCnt;
		for(int i=0; i<iCnt; i++)
		{
			sBulruGubnList[i] = sList[i];
			sBulruNameList[i] = sList2[i];
			iCheck[i] = 1;
		}

		ikey = idata;
		hTreeItem = hitem;
		iAllCheck = 1;
		iFirst = 1;
		bCategory = FALSE;
	};

	NewsList()
	{
		iGubn = ibulruCnt = 0;
		sGubnName = _T("");
		iFirst = 1;
	};
};
typedef CArray<NewsList, NewsList> CNewsListArray;

struct AList
{
	CString		sList[MAXCNT];
	int			iDataCnt;
	int			iList[MAXCNT]{};


	CString		sTxt;
	int			iVal{};
	AList(int icnt, CString* sData)
	{
		iDataCnt = icnt;
		for(int i=0; i<icnt; i++)
			sList[i] = sData[i];
	};

	AList(int icnt, int* iData)
	{
		iDataCnt = icnt;
		for(int i=0; i<icnt; i++)
			iList[i] = iData[i];
	};

	AList(CString sData, int iData)
	{
		sTxt = sData;
		iVal = iData;
	};

	AList()
	{
		iDataCnt = 0;
	};

};
typedef CArray<AList, AList> CAListArray;

#pragma warning (default : 26485)
#pragma warning (default : 26429)
#pragma warning (default : 26481)
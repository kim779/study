#ifndef __EKKKEELFJ__EKL090917_FOR_WINIX__GRIDIO__HEADER__
#define	__EKKKEELFJ__EKL090917_FOR_WINIX__GRIDIO__HEADER__

//#include "../../../SrcSite/include/axisGrid.h"

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
} GRID_O;                               /* grid header                  */

//TR : pooppoop

typedef struct  {           /* GRID form input format   */
    char    nrec[6];        /* # of nrec            */
    char    dumy[6];        /* dummy data number        */
    char    kind[1];        /* data patten kind     */
    char    dkey[1];        /* data key         */
    char    gday[8];        /* 기준일자         */
    char    unit[1];        /* data Unit            */
    char    indx[1];        /* data Index           */
    char    igap[4];        /* Tick gap, 분 gap     */ 
    char    ltic[4];        /* Last Tick count      */ 
    char    optn1[1];       /* extra option         */
    char    optn2[1];       /* extra option         */
    char    rcod[16];       /* Real time code       */
    char    ikey[1];        /* key action           */
	/* '0':Enter or Other       */
	/* '1':PgUp '2':PgDn    */
	/* '3':ScrollUp '4':ScrollDn    */
	/* '5':First or End     */
	/* '6':Please sort      */
    char    xpos[1];        /* continuous status        */
	/* 0x40: default(normal)    */
	/* 0x01: PgUp, ScrUp enable */
	/* 0x02: PgDn, ScrDn enable */
	/* 0x04: no local sorting   */
    char    page[4];        /* current page number      */
    char    save[80];       /* grid inout save field    */
} GRAPH_IO;
//Graph_IO
#endif //__EKKKEELFJ__EKL090917_FOR_WINIX__GRIDIO__HEADER__

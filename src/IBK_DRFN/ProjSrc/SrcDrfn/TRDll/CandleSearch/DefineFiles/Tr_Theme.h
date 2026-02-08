#if !defined(AFX_TR_THEME_DLDLDEE_H__AF_AD91_7EE99FC62AAF__INCLUDED_)
#define AFX_TR_THEME_DLDLDEE_H__AF_AD91_7EE99FC62AAF__INCLUDED_

//#include "TreeList_Thema.h"


//[[삼성테마관련 
/*************************************************************************************/
/* Input structure                                                                   */
/*************************************************************************************/

/* InRec: 테마조회                                                                   */
typedef struct
{
    char    Sect2[1];                             /* 구분2(1)                        */
    char    WMonthSect[1];                        /* 주월간구분(1)                   */
    char    RefrWord2[50];                        /* 조회단어2(50)                   */
}fn_HFIBOA076220_InRec1;


/*************************************************************************************/
/* Output structure                                                                  */
/*************************************************************************************/

/* OutRec1: 테마조회                                                                 */
typedef struct
{
    char    PrntCnt3[4];                          /* 출력건수3(4)                    */
    char    filler;                               /*                                 */
}fn_HFIBOA076220_OutRec1;

/* OutRec2: 테마조회                                                                 */
// fn_HFIBOA076220_OutRec2의 배열 정보 = 'fn_HFIBOA076220_OutRec1.PrntCnt3'
typedef struct
{
    char    ThemaGrpCd[5];                        /* 테마그룹코드(5)                 */
    char    ThemaGrpNm[50];                       /* 테마그룹명(50)                  */
}fn_HFIBOA076220_OutRec2;

/*************************************************************************************/



/*************************************************************************************/
/* Input structure                                                                   */
/*************************************************************************************/

/* InRec: 종목조회                                                                   */
typedef struct
{
    char    InItem1[4];                           /* #30(4)                          */
    char    InItem2[1];                           /* #(1)                            */
    char    ContinueKey4[6];                      /* 연속키4(6)                      */
    char    ThemaCd[5];                           /* 테마코드(5)                     */
}fn_HFIBOA076210_InRec1;


/*************************************************************************************/
/* Output structure                                                                  */
/*************************************************************************************/

/* OutRec1: 종목조회                                                                 */
typedef struct
{
    char    PrntCnt3[4];                          /* 출력건수3(4)                    */
    char    EndingYN[1];                          /* 끝여부(1)                       */
    byte    OutItem3[3];                             /* #3(3)                           */
    byte    OutItem4;                             /* #1(1)                           */
    char    ContinueKey4[6];                      /* 연속키4(6)                      */
    byte    OutItem6;                             /* #1(1)                           */
}fn_HFIBOA076210_OutRec1;

/* OutRec2: 종목조회                                                                 */
// fn_HFIBOA076210_OutRec2의 배열 정보 = 'fn_HFIBOA076210_OutRec1.PrntCnt3'
typedef struct
{
    byte    OutItem1;                             /* #1(1)                           */
    char    ItemCd13[6];                          /* 종목코드13(6)                   */
    byte    OutItem3;                             /* #1(1)                           */
    char    ItemNm1[20];                          /* 종목명1(20)                     */
    long    CurPrc;                               /* 현재가(4)                       */
    char    Symbl[1];                             /* 부호(1)                         */
    byte    OutItem7[3];                          /* #3(3)                           */
    long    UpNDn1;                               /* 등락1(4)                        */
    long    UpNDnRt3;                             /* 등락율(4)                       */
    long    TrdVol1;                              /* 거래량1(4)                      */
}fn_HFIBOA076210_OutRec2;
/*************************************************************************************/

#endif // !defined(AFX_TR_THEME_DLDLDEE_H__AF_AD91_7EE99FC62AAF__INCLUDED_)
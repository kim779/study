/*************************************************************************************/
/*  1. 프로그램ID : EU_p0621                                                         */
/*  2. 프로그램명 : 조건검색_1                                                       */
/*  3. 서비스명   : EU_p0621                                                         */
/*  4. 사용테이블 :                                                                  */
/*  5. 작성자     : drfn                                                             */
/*  6. 입력항목   :                                                                  */
/*  7. 출력항목   :                                                                  */
/*************************************************************************************/
#ifndef __EU_TRAN_EU_p0621_H__
#define __EU_TRAN_EU_p0621_H__

/*************************************************************************************/
/* include file                                                                      */
/*************************************************************************************/

/*************************************************************************************/
/* define constant                                                                   */
/*************************************************************************************/
#define EU_TRAN0621                       "EU_p0621"
#define LLONG                         long long

#define RESULT_FIELD_MAX_EUGENE     240
/*************************************************************************************/
/* Input structure                                                                   */
/*************************************************************************************/

/* InRec1:                                                                           */
typedef struct
{
	char    sJongFillterChk[1];					  /* 대상지정 체크 여부  '0' : No, '1' : Yes */       
	char    sJongFillterIndex[RESULT_MAX];		  /* 대상 종목코드Bitset          */

    char    giCallGubun[4];                       /* 콜구분(4)                       */
    char    giTargetGubun[2];                     /* 타겟구분(2)                     */
    char    giTargetIdxCnt[2];                    /* 대상지표갯수(2)                 */
    char    giTargetResultViewCnt[2];             /* 대상값보기필드갯수(2)           */
    char    gsTargetExp[80];                      /* 대상조건조합표현식(80)          */
    char    giComplexIdxCnt[2];                   /* 복합지표갯수(2)                 */
    char    giComplexResultViewCnt[2];            /* 복합값보기필드개수(2)           */
    char    gsComplexExp[80];                     /* 복합조건조합표현식(80)          */
    char    giFieldEditerCnt[2];                  /* 고정필드편집개수(2)             */
    char    giFieldList[RESULT_FIELD_MAX_EUGENE];                      /* 필드편집번호리스트(80)          */
    char    giResultIdxCnt[4];                    /* 결과값비교 개수         NEW(4)  */
    char    gsResultExp[80];                      /* 결과값 비교조합 표현 식 NEW(80) */
    char    gsTargetBitChk[1];                    /* TargetBit(1)                    */
    char    gsJongmokIndex[4000];           		/* 종목코드Bitset(4000)            */	//3000 -> 4000
    char    gsSort_flag[1];                       /* 정렬 방향(0:오름, 1:내림)(1)    */
    char    giSort_Pos[4];                        /* 정렬 번호(값보기필드pos)(4)     */
    char    giBackDate[8];                        /* backtest 일자(미사용:-1)(4)     */
    char    giBackTime[6];                        /* backtest 시간(미사용:-1)(4)     */
}EU_p0621_InRec1;

/* InRec2:                                                                           */
char EU_p0621_InRec2_count[4];
typedef struct
{
    char    giIndexNo[2];                         /* 지표번호(2)                     */
    char    giGroupNo[2];                         /* 그룹번호(2)                     */
    char    giGroupIdxCnt[2];                     /* 그룹내의지표개수(2)             */
    char    giTargetBase[2];                      /* 타겟데이타(2)                   */
    char    giType[4];                            /* 지표의종류(4)                   */
    char    giType1[2];                           /* Type1조건형태(2)                */
    char    giType2[2];                           /* 상세형태(2)                     */
    char    giSTerm[4];                           /* 그룹검색시작위치(4)             */
    char    giETerm[4];                           /* 그룹검색끝위치(4)               */
    char    giRank[4];                            /* 순위검색종목수(4)               */
    char    giTrend[4];                           /* 추세횟수(4)                     */
    char    giSignalCnt1[4];                      /* 신호발생횟수1(4)                */
    char    giSignalCnt2[4];                      /* 신호발생횟수2(4)                */
    char    giResultViewNum[2];                   /* 결과값보기필드갯수(2)           */
    char    gsIndexType[1];                       /* 검색구분0:일반1:순위2:복합(1)   */
    char    gsSearchType[1];                      /* 검색형태 0:일반 1:사용자정의(1) */
    char    gsOperator[1];                        /* 연산자0:OR1:AND2:NEXT(1)        */
    char    gsReverse[1];                         /* 결과제외0:A1:NotA(1)            */
    char    gsIdxContiGubun[1];                   /* 0:단일발생1:연속발생지표(1)     */
    char    gsElement[1];                         /* 조건식사용문자(A)(1)            */
    char    gsScode[6];                           /* shortcode(6)                    */
//	char    gdParam[48];                          /* 지표관련변수값(48)              */
//	char    gdCondition[96];                      /* 기타설정변수값(96)              */
    char    gdParam[96];                          /* 지표관련변수값(48)              */
    char    gdCondition[192];                      /* 기타설정변수값(96)              */
}EU_p0621_InRec2;


/*************************************************************************************/
/* Output structure                                                                  */
/*************************************************************************************/

/* OutRec1:                                                                          */
typedef struct
{
    char    giCallGubun[4];                       /* 콜구분(4)                       */
    char    giTotalJongCnt[4];                    /* 검색된전제종목수(4)             */
    char    giOutFieldCnt[2];                     /* OutFieldCount(2)                */
    char    giOutListPacketSize[4];               /* 결과List사이즈(한종목)(4)       */
    char    giFindTime[8];                        /* 조건검색현재시간(8)             */
}EU_p0621_OutRec1;

/* OutRec2:                                                                          */
char EU_p0621_OutRec2_count[4];
typedef struct
{
    char    gsDaebi[1];                           /* 대비부호(1)                     */
    char    gsCode[6];                            /* 종목코드(6)                     */
    char    gsHname[20];                          /* 종목명(20)                      */
    char    gsMarket[1];                          /* 마켓구분(1)                     */
    char    giOrderRank[4];                       /* 순위검색순위(4)                 */
    char    gdResultField[320];                   /* 결과필드(320)                   */
}EU_p0621_OutRec2;

/*************************************************************************************/

#endif


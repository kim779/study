/*************************************************************************************/
/*  1. 프로그램ID : KB_d1022                                                         */
/*  2. 프로그램명 : 조건검색_원클릭종목조회                                          */
/*  3. 서비스명   : KB_d1022                                                         */
/*  4. 사용테이블 :                                                                  */
/*  5. 작성자     : drfn                                                             */
/*  6. 입력항목   :                                                                  */
/*  7. 출력항목   :                                                                  */
/*************************************************************************************/
#ifndef __EU_TREELIST_TRAN_KB_d1022_H__
#define __EU_TREELIST_TRAN_KB_d1022_H__

/*************************************************************************************/
/* include file                                                                      */
/*************************************************************************************/
//#include "../../../TRDll/Dr_Include/Condition_MngDef.h"
#include "./Condition_MngDef.h"
#include "./client_typedef.h"
#include "./server_typedef.h"

#define INT2STR_BUF(xStr, nSize, nStr) sprintf(xStr, "%#0*.*d", nSize, nSize, nStr)
#define Safe_CheckCpy(xSource, xTarget) if(::lstrlen(xTarget) > 0) memcpy(xSource, (LPCSTR)xTarget, ::lstrlen(xTarget))
#define Safe_CheckCopy2(xSource, xTarget, xLen, xStr) \
	xStr.Format("%-*.*s", xLen, xLen, xTarget); \
	xStr.TrimRight();	\
	if(xStr.GetLength()) memcpy(xSource, (LPCSTR)xStr, xStr.GetLength())


/*************************************************************************************/
/* define constant                                                                   */
/*************************************************************************************/
#define NAME_d1022     "d1022"
#define LLONG          long long


/*************************************************************************************/
/* Input structure                                                                   */
/*************************************************************************************/

/* InRec1:                                                                           */
typedef struct
{
    char    giCallGubun[4];                       /* giCallGubun(4)                  */
	char	giType[1];
    char    gsFilterGubun[1];                     /* 필터구분(1)                     */
} KB_d1022_InRec1;

/* InRec2:                                                                           */
typedef struct
{
	char    giCallGubun[4];                       /* giCallGubun(4)                  */
	char    giTargetGubun[2];                     /* 타겟구분(2)                     */
	char    giTargetIdxCnt[2];                    /* 대상지표갯수(2)                 */
	char    giTargetResultViewCnt[2];             /* 값보기필드갯수(2)               */
	char    gsTargetExp[80];                      /* 대상조건조합표현식(80)          */
	char    giComplexIdxCnt[2];                   /* 복합지표갯수(2)                 */
	char    giComplexResultViewCnt[2];            /* 복합값보기갯수(2)               */
	char    gsComplexExp[80];                     /* 복합조건조합표현식(80)          */
	char    giFieldEditerCnt[2];                  /* 고정필드편집개수(2)             */
	char    giFieldList[240];                      /* 필드편집번호리스트(240)          */
	char    giResultIdxCnt[4];                    /* 결과값비교갯수(4)               */
	char    gsResultExp[80];                      /* 결과값비교조합(80)              */
	char    gsTargetBitChk[1];                    /* TargetBit(1)                    */
	char    gsJongmokIndex[4000];           		/* 종목코드Bitset(4000)            */	//3000 -> 4000
	char    gsSort_flag[1];                       /* 정렬방향(1)                     */
	char    giSort_Pos[4];                        /* 정렬번호(4)                     */
	char    giBackDate[4];                        /* backtest일자(4)                 */
	char    giBackTime[4];                        /* backtest시간(4)                 */
}KB_d1022_InRec2;

/* InRec3:                                                                           */
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
    char    gsSearchType[1];                      /* 검색형태(1)                     */
    char    gsOperator[1];                        /* 연산자0:OR1:AND2:NEXT(1)        */
    char    gsReverse[1];                         /* 결과제외0:A1:NotA(1)            */
    char    gsIdxContiGubun[1];                   /* 0:단일발생1:연속발생지표(1)     */
    char    gsElement[1];                         /* 조건식사용문자(A)(1)            */
    char    gsScode[6];                           /* shortcode(6)                    */
//	char    gdParam[48];                          /* 지표관련변수값(48)              */
//	char    gdCondition[96];                      /* 기타설정변수값(96)              */
    char    gdParam[96];                          /* 지표관련변수값(48)              */
    char    gdCondition[192];                      /* 기타설정변수값(96)              */
}KB_d1022_InRec3;


/*************************************************************************************/
/* Output structure                                                                  */
/*************************************************************************************/

/* OutRec1:                                                                          */
typedef struct
{
    int     giCallIndex;                       /* 클라이언트조회구분(1)           */
    char    gsJongCnt[4];                         /* 검색종목수(4)                   */
    char    gsHigh[4];                            /* 상한종목수(4)                   */
    char    gsUp[4];                              /* 상승종목수(4)                   */
    char    gsNoch[4];                            /* 보합종목수(4)                   */
    char    gsDown[4];                            /* 하락종목수(4)                   */
    char    gsLow[4];                             /* 하한종목수(4)                   */
    char    gsUpRate[6];                          /* 상승비중(6)                     */
    char    gsChgrateAvg[6];                      /* 평균등락율(6)                   */
}KB_d1022_OutRec1;

/* OutRec2:                                                                          */
typedef struct
{
    char    gsMflag[1];                           /* 시장구분(1)                     */
    char    gsCode[6];                            /* 종목코드(6)                     */
    char    gsHname[20];                          /* 종목명(20)                      */
    char    gsSignCnt[3];                         /* 등락연속봉수(3)                 */
    char    gsClose[8];                           /* 현재가(8)                       */
    char    gsSign[1];                            /* 등락부호(1)                     */
    char    gsChange[8];                          /* 전일대비(8)                     */
    char    gsChgRate[6];                         /* 전일대비등락율(6)               */
    char    gsVolume[9];                          /* 거래량(9)                       */
    char    gsVolumeRate[6];                      /* 거래량전일대비율(6)             */
	char    gsFiller[4];    //   4 Filler
	char    gdResultField[960];       /*  결과필드 (16 * 60 = 960)    */
}KB_d1022_OutRec2;

/*************************************************************************************/

class CTranHelperd1022
{
public:
	// --------------------------------------------------------------------------------------
	// Input처리
	// --------------------------------------------------------------------------------------
//	void TR_RESULT_IN_To_KB_d1022_InRec1(TR_RESULT_IN* pIn, KB_d1022_InRec1* pOut)
//	{
//		memset(pOut, 0x20, sizeof(KB_d1022_InRec1));
//
//		char tmpVal[100]={0,};
//
//		INT2STR_BUF(tmpVal, 4, pIn->nTrIndex);
//		memcpy(pOut->giCallGubun, tmpVal, 4);
//
//		pOut->gsFilterGubun[0] = pIn->sType + '0';
//	}
	void TR_RESULT_IN_To_KB_d1022_InRec1(TR_RESULT_IN* pIn, KB_d1022_InRec1* pOut)
	{
		memset(pOut, 0x20, sizeof(KB_d1022_InRec1));

		char tmpVal[100]={0,};
		CString strOp="";
		CString strVal="";

		INT2STR_BUF(tmpVal, 4, pIn->nTrIndex);
		if(tmpVal[0] == '-') 
		{
			
			strVal.Format("%s", tmpVal);
			strOp.Format("%s", strVal.Left(1));
			if(strVal.Mid(1, 1) == '0')
			{
				if(strVal.Mid(2, 1) == '0')
				{
					if(strVal.Mid(3, 1) == '0')
					{
						strVal.Format("%s", strVal.Right(1) + "  ");
						strVal.Format("%s", strOp+strVal);
					}
					else
					{
						strVal.Format("%s", strVal.Right(2) + " ");
						strVal.Format("%s", strOp+strVal);
					}
				}
				else
				{
					strVal.Format("%s", strVal.Right(3));
					strVal.Format("%s", strOp+strVal);
				}				
			}
			memcpy(pOut->giCallGubun, strVal, strVal.GetLength());
		}
		else
			memcpy(pOut->giCallGubun, tmpVal, 4);

		pOut->giType[0] = pIn->sType ;

		pOut->gsFilterGubun[0] = '1';
	}

//	void User_Finder_To_KB_d1022_InRec2(User_Finder* pUserFinder, char* pOut, int &nPos)
//	{
//		KB_d1022_InRec2* pInRec2 = (KB_d1022_InRec2*)&pOut[nPos];
//		memset(pInRec2, 0x20, sizeof(KB_d1022_InRec2));
//
//		CString szVal;
//		char tmpVal[100]={0,};
//		int  nTargetIdxCnt=0, nComplexIdxCnt=0;
//
//		INT2STR_BUF(tmpVal, 4, pUserFinder->TrIndex);				// giCallGubun(4)
//		memcpy(pInRec2->giCallGubun, tmpVal, 4);
//
//		INT2STR_BUF(tmpVal, 2, pUserFinder->nTargetType);			// 0 :종목 , 10 : 업종	동부 [NEW]
//		memcpy(pInRec2->giTargetGubun, tmpVal, 2);
//
//		INT2STR_BUF(tmpVal, 2, pUserFinder->TargetIdxCnt);          // 대상 지표 갯수       
//		memcpy(pInRec2->giTargetIdxCnt, tmpVal, 2);
//		tmpVal[2] = 0;
//		nTargetIdxCnt = atoi(tmpVal);
//
//		INT2STR_BUF(tmpVal, 2, pUserFinder->TargetResultViewCnt);	// 대상 값보기필드 개수 
//		memcpy(pInRec2->giTargetResultViewCnt, tmpVal, 2);
//
//		//INT2STR_BUF(tmpVal, 80, pUserFinder->TargetExp[EXPLEN]);	// 대상조건 조합 표현 식	
//		//memcpy(pInRec2->gsTargetExp, pUserFinder->TargetExp, 80);
//		Safe_CheckCopy2(pInRec2->gsTargetExp, pUserFinder->TargetExp, 80, szVal);
//
//		INT2STR_BUF(tmpVal, 2, pUserFinder->ComplexIdxCnt);			// 복합 지표 갯수       
//		memcpy(pInRec2->giComplexIdxCnt, tmpVal, 2);
//		tmpVal[2] = 0;
//		nComplexIdxCnt = atoi(tmpVal);
//
//		INT2STR_BUF(tmpVal, 2, pUserFinder->ComplexResultViewCnt);	// 복합 값보기필드 개수 
//		memcpy(pInRec2->giComplexResultViewCnt, tmpVal, 2);
//
//		//INT2STR_BUF(tmpVal, 80, pUserFinder->ComplexExp[EXPLEN]);	// 복합조건 조합 표현 식	
//		//memcpy(pInRec2->gsComplexExp, pUserFinder->ComplexExp, 80);
//		Safe_CheckCopy2(pInRec2->gsComplexExp, pUserFinder->ComplexExp, 80, szVal);
//
//		INT2STR_BUF(tmpVal, 4, pUserFinder->ResultIdxCnt);		// 필드편집 개수   [Fix + Move] 
//		memcpy(pInRec2->giResultIdxCnt, tmpVal, 4);
//
//		//INT2STR_BUF(tmpVal, 80, pUserFinder->FieldList[REAL_IDXCNT]);// 필드편집 번호리스트 [Just Fix + Move] 
//		//memcpy(pInRec2->gsResultExp, pUserFinder->FieldList, 80);
//		Safe_CheckCopy2(pInRec2->gsResultExp, pUserFinder->ResultExp, 80, szVal);
//
//		INT2STR_BUF(tmpVal, 2, pUserFinder->FieldEditerCnt);			// 결과값비교 개수          [NEW]
//		memcpy(pInRec2->giFieldEditerCnt, tmpVal, 2);
//
//		INT2STR_BUF(tmpVal, 80, pUserFinder->FieldList);	// 결과값 비교조합 표현 식  [NEW]		
//		//memcpy(pInRec2->giFieldList, pUserFinder->ResultExp, 80);
//		Safe_CheckCopy2(pInRec2->giFieldList, pUserFinder->ResultExp, 80, szVal);
//
//		//INT2STR_BUF(tmpVal, 1, pUserFinder->TargetBitChk);				// TargetBit 사용 여부 체크
//		pInRec2->gsTargetBitChk[0] = pUserFinder->TargetBitChk;
//
//		//INT2STR_BUF(tmpVal, 2500, &pUserFinder->TargetBit[0]);	// 검색 대상 종목			
//		//memcpy(pInRec2->gsJongmokIndex, pUserFinder->TargetBit, RESULT_MAX);
//		Safe_CheckCopy2(pInRec2->gsJongmokIndex, pUserFinder->TargetBit, 2500, szVal);
//
//		INT2STR_BUF(tmpVal, 1, pUserFinder->Sort_flag);					// 정렬 방향(0:오름, 1:내림)
//		pInRec2->gsSort_flag[0] = pUserFinder->Sort_flag;
//
//		INT2STR_BUF(tmpVal, 4, pUserFinder->Sort_Num);					// 정렬 번호(값보기필드pos) 	    
//		memcpy(pInRec2->giSort_Pos, tmpVal, 4);
//
//		if(pUserFinder->BackDay==-1)	sprintf(tmpVal,"-001");
//		else		INT2STR_BUF(tmpVal, 4, pUserFinder->BackDay);					// backtest 일자(미사용:-1) 	
//		memcpy(pInRec2->giBackDate, tmpVal, 4);
//
//		if(pUserFinder->BackTime==-1)	sprintf(tmpVal,"-001");
//		else	INT2STR_BUF(tmpVal, 4, pUserFinder->BackTime);					// backtest 시간(미사용:-1) 
//		memcpy(pInRec2->giBackTime, tmpVal, 4);
//
//		nPos += sizeof(KB_d1022_InRec2);
//
//		KB_d1022_InRec3* pInRec3;
//		for(int i=0; i<nTargetIdxCnt+nComplexIdxCnt; i++)
//		{
//			pInRec3 = (KB_d1022_InRec3*)&pOut[nPos];
//			memset(pInRec3, 0x20, sizeof(KB_d1022_InRec3));
//			nPos += sizeof(KB_d1022_InRec3);
//
//			INT2STR_BUF(tmpVal, 2, pUserFinder->stUser_Define[i].IndexNo);	// 지표번호(2)
//			memcpy(pInRec3->giIndexNo, tmpVal, 2);
//
//			INT2STR_BUF(tmpVal, 2, pUserFinder->stUser_Define[i].GroupNo);	// 그룹번호(2)
//			memcpy(pInRec3->giGroupNo, tmpVal, 2);
//
//			INT2STR_BUF(tmpVal, 2, pUserFinder->stUser_Define[i].GroupIdxCnt);	// 그룹내의지표개수(2)
//			memcpy(pInRec3->giGroupIdxCnt, tmpVal, 2);
//
//			INT2STR_BUF(tmpVal, 2, pUserFinder->stUser_Define[i].TargetBase);	// 타겟데이타(2)
//			memcpy(pInRec3->giTargetBase, tmpVal, 2);
//
//			INT2STR_BUF(tmpVal, 4, pUserFinder->stUser_Define[i].Type);	// 지표의종류(4)
//			memcpy(pInRec3->giType, tmpVal, 4);
//
//			INT2STR_BUF(tmpVal, 2, pUserFinder->stUser_Define[i].Type1);	// Type1조건형태(2)
//			memcpy(pInRec3->giType1, tmpVal, 2);
//
//			INT2STR_BUF(tmpVal, 2, pUserFinder->stUser_Define[i].Type2);	// 상세형태(2)
//			memcpy(pInRec3->giType2, tmpVal, 2);
//
//			INT2STR_BUF(tmpVal, 4, pUserFinder->stUser_Define[i].STerm);	// 그룹검색시작위치(4)
//			memcpy(pInRec3->giSTerm, tmpVal, 4);
//
//			INT2STR_BUF(tmpVal, 4, pUserFinder->stUser_Define[i].ETerm);	// 그룹검색끝위치(4)
//			memcpy(pInRec3->giETerm, tmpVal, 4);
//
//			INT2STR_BUF(tmpVal, 4, pUserFinder->stUser_Define[i].Rank);	// 순위검색종목수(4)
//			memcpy(pInRec3->giRank, tmpVal, 4);
//
//			INT2STR_BUF(tmpVal, 4, pUserFinder->stUser_Define[i].Trend);	// 추세횟수(4)
//			memcpy(pInRec3->giTrend, tmpVal, 4);
//
//			INT2STR_BUF(tmpVal, 4, pUserFinder->stUser_Define[i].SignalCnt1);	// 신호발생횟수1(4)
//			memcpy(pInRec3->giSignalCnt1, tmpVal, 4);
//
//			INT2STR_BUF(tmpVal, 4, pUserFinder->stUser_Define[i].SignalCnt2);	// 신호발생횟수2(4)
//			memcpy(pInRec3->giSignalCnt2, tmpVal, 4);
//
//			INT2STR_BUF(tmpVal, 2, pUserFinder->stUser_Define[i].ResultViewNum);	// 결과값보기필드갯수(2)
//			memcpy(pInRec3->giResultViewNum, tmpVal, 2);
//
//			pInRec3->gsIndexType[0] = pUserFinder->stUser_Define[i].IndexType;          // 검색구분0:일반1:순위2:복합(1) 
//			pInRec3->gsSearchType[0] = pUserFinder->stUser_Define[i].SearchType;        // 검색형태(1)                   
//			pInRec3->gsOperator[0] = pUserFinder->stUser_Define[i].Operator;            // 연산자0:OR1:AND2:NEXT(1)      
//			pInRec3->gsReverse[0] = pUserFinder->stUser_Define[i].Reverse;              // 결과제외0:A1:NotA(1)          
//			pInRec3->gsIdxContiGubun[0] = pUserFinder->stUser_Define[i].IdxContiGubun;	// 0:단일발생1:연속발생지표(1)   
//			pInRec3->gsElement[0] = pUserFinder->stUser_Define[i].Element;              // 조건식사용문자(A)(1)
//
//			memcpy(pInRec3->gsScode, pUserFinder->stUser_Define[i].Code, 6);	// shortcode(6)  
//
//			for(int j=0; j<MAX_PARAM; j++)
//			{
//				sprintf(tmpVal, "%016f", pUserFinder->stUser_Define[i].Param[j]);
//				memcpy(&pInRec3->gdParam[16*j], tmpVal, 16);	// 지표관련변수값(48)
//			}
//
//			for(j=0; j<MAX_CONDI; j++)
//			{
//				sprintf(tmpVal, "%016f", pUserFinder->stUser_Define[i].Condition[j]);
//				memcpy(&pInRec3->gdCondition[16*j], tmpVal, 16);	// 기타설정변수값(96) 
//			}
//		}
//	}
	void User_Finder_To_KB_d1022_InRec2(User_Finder* pUserFinder, char* pOut, int &nPos)
	{
		KB_d1022_InRec2* pInRec2 = (KB_d1022_InRec2*)&pOut[nPos];
		memset(pInRec2, 0x20, sizeof(KB_d1022_InRec2));

		CString szVal;
		char tmpVal[300]={0,};
		int  nTargetIdxCnt=0, nComplexIdxCnt=0;
		int i = 0;

		INT2STR_BUF(tmpVal, 4, pUserFinder->TrIndex);				// giCallGubun(4)
		memcpy(pInRec2->giCallGubun, tmpVal, 4);

		INT2STR_BUF(tmpVal, 2, pUserFinder->nTargetType);			// 0 :종목 , 10 : 업종	동부 [NEW]
		memcpy(pInRec2->giTargetGubun, tmpVal, 2);

		INT2STR_BUF(tmpVal, 2, pUserFinder->TargetIdxCnt);          // 대상 지표 갯수       
		memcpy(pInRec2->giTargetIdxCnt, tmpVal, 2);
		tmpVal[2] = 0;
		nTargetIdxCnt = atoi(tmpVal);

		INT2STR_BUF(tmpVal, 2, pUserFinder->TargetResultViewCnt);	// 대상 값보기필드 개수 
		memcpy(pInRec2->giTargetResultViewCnt, tmpVal, 2);

		//INT2STR_BUF(tmpVal, 80, pUserFinder->TargetExp[EXPLEN]);	// 대상조건 조합 표현 식	
		//memcpy(pInRec2->gsTargetExp, pUserFinder->TargetExp, 80);
		Safe_CheckCopy2(pInRec2->gsTargetExp, pUserFinder->TargetExp, EXPLEN, szVal);

		INT2STR_BUF(tmpVal, 2, pUserFinder->ComplexIdxCnt);			// 복합 지표 갯수       
		memcpy(pInRec2->giComplexIdxCnt, tmpVal, 2);
		tmpVal[2] = 0;
		nComplexIdxCnt = atoi(tmpVal);

		INT2STR_BUF(tmpVal, 2, pUserFinder->ComplexResultViewCnt);	// 복합 값보기필드 개수 
		memcpy(pInRec2->giComplexResultViewCnt, tmpVal, 2);

		//INT2STR_BUF(tmpVal, 80, pUserFinder->ComplexExp[EXPLEN]);	// 복합조건 조합 표현 식	
		//memcpy(pInRec2->gsComplexExp, pUserFinder->ComplexExp, 80);
		Safe_CheckCopy2(pInRec2->gsComplexExp, pUserFinder->ComplexExp, EXPLEN, szVal);

		INT2STR_BUF(tmpVal, 4, pUserFinder->ResultIdxCnt);		// 필드편집 개수   [Fix + Move] 
		memcpy(pInRec2->giResultIdxCnt, tmpVal, 4);

		//INT2STR_BUF(tmpVal, 80, pUserFinder->FieldList[REAL_IDXCNT]);// 필드편집 번호리스트 [Just Fix + Move] 
		//memcpy(pInRec2->gsResultExp, pUserFinder->FieldList, 80);
		Safe_CheckCopy2(pInRec2->gsResultExp, pUserFinder->ResultExp, EXPLEN, szVal);

		INT2STR_BUF(tmpVal, 2, pUserFinder->FieldEditerCnt);			// 결과값비교 개수          [NEW]
		memcpy(pInRec2->giFieldEditerCnt, tmpVal, 2);

// 		INT2STR_BUF(tmpVal, 240, pUserFinder->FieldList);	// 결과값 비교조합 표현 식  [NEW]		
// 		memcpy(pInRec2->giFieldList, pUserFinder->ResultExp, 240);
// 		Safe_CheckCopy2(pInRec2->giFieldList, pUserFinder->FieldList, 240, szVal);
// 		CString strNULL; strNULL.Format("%240s", " ");
// 		memcpy(pInRec2->giFieldList,strNULL, 240);
		for( i = 0; i < (int)pUserFinder->FieldEditerCnt; i++)
		{
			INT2STR_BUF(tmpVal, 4, pUserFinder->FieldList[i]);
			memcpy(&pInRec2->giFieldList[i * 4], tmpVal, 4);
		}
	
		//INT2STR_BUF(tmpVal, 1, pUserFinder->TargetBitChk);				// TargetBit 사용 여부 체크
		pInRec2->gsTargetBitChk[0] = pUserFinder->TargetBitChk;

		//INT2STR_BUF(tmpVal, 2500, &pUserFinder->TargetBit[0]);	// 검색 대상 종목			
		//memcpy(pInRec2->gsJongmokIndex, pUserFinder->TargetBit, RESULT_MAX);
		Safe_CheckCopy2(pInRec2->gsJongmokIndex, pUserFinder->TargetBit, 4000, szVal);	//3000 -> 4000

		INT2STR_BUF(tmpVal, 1, pUserFinder->Sort_flag);					// 정렬 방향(0:오름, 1:내림)
		pInRec2->gsSort_flag[0] = pUserFinder->Sort_flag;

		INT2STR_BUF(tmpVal, 4, pUserFinder->Sort_Num);					// 정렬 번호(값보기필드pos) 	    
		memcpy(pInRec2->giSort_Pos, tmpVal, 4);

		if(pUserFinder->BackDay==-1)	sprintf(tmpVal,"-001");
		else		INT2STR_BUF(tmpVal, 4, pUserFinder->BackDay);					// backtest 일자(미사용:-1) 	
		memcpy(pInRec2->giBackDate, tmpVal, 4);

		if(pUserFinder->BackTime==-1)	sprintf(tmpVal,"-001");
		else	INT2STR_BUF(tmpVal, 4, pUserFinder->BackTime);					// backtest 시간(미사용:-1) 
		memcpy(pInRec2->giBackTime, tmpVal, 4);

		nPos += sizeof(KB_d1022_InRec2);

		KB_d1022_InRec3* pInRec3;
		for(i=0; i<nTargetIdxCnt+nComplexIdxCnt; i++)
		{
			pInRec3 = (KB_d1022_InRec3*)&pOut[nPos];
			memset(pInRec3, 0x20, sizeof(KB_d1022_InRec3));
			nPos += sizeof(KB_d1022_InRec3);

			INT2STR_BUF(tmpVal, 2, pUserFinder->stUser_Define[i].IndexNo);	// 지표번호(2)
			memcpy(pInRec3->giIndexNo, tmpVal, 2);

			INT2STR_BUF(tmpVal, 2, pUserFinder->stUser_Define[i].GroupNo);	// 그룹번호(2)
			memcpy(pInRec3->giGroupNo, tmpVal, 2);

			INT2STR_BUF(tmpVal, 2, pUserFinder->stUser_Define[i].GroupIdxCnt);	// 그룹내의지표개수(2)
			memcpy(pInRec3->giGroupIdxCnt, tmpVal, 2);

			INT2STR_BUF(tmpVal, 2, pUserFinder->stUser_Define[i].TargetBase);	// 타겟데이타(2)
			memcpy(pInRec3->giTargetBase, tmpVal, 2);

			INT2STR_BUF(tmpVal, 4, pUserFinder->stUser_Define[i].Type);	// 지표의종류(4)
			memcpy(pInRec3->giType, tmpVal, 4);

			INT2STR_BUF(tmpVal, 2, pUserFinder->stUser_Define[i].Type1);	// Type1조건형태(2)
			memcpy(pInRec3->giType1, tmpVal, 2);

			INT2STR_BUF(tmpVal, 2, pUserFinder->stUser_Define[i].Type2);	// 상세형태(2)
			memcpy(pInRec3->giType2, tmpVal, 2);

			INT2STR_BUF(tmpVal, 4, pUserFinder->stUser_Define[i].STerm);	// 그룹검색시작위치(4)
			memcpy(pInRec3->giSTerm, tmpVal, 4);

			INT2STR_BUF(tmpVal, 4, pUserFinder->stUser_Define[i].ETerm);	// 그룹검색끝위치(4)
			memcpy(pInRec3->giETerm, tmpVal, 4);

			INT2STR_BUF(tmpVal, 4, pUserFinder->stUser_Define[i].Rank);	// 순위검색종목수(4)
			memcpy(pInRec3->giRank, tmpVal, 4);

			INT2STR_BUF(tmpVal, 4, pUserFinder->stUser_Define[i].Trend);	// 추세횟수(4)
			memcpy(pInRec3->giTrend, tmpVal, 4);

			INT2STR_BUF(tmpVal, 4, pUserFinder->stUser_Define[i].SignalCnt1);	// 신호발생횟수1(4)
			memcpy(pInRec3->giSignalCnt1, tmpVal, 4);

			INT2STR_BUF(tmpVal, 4, pUserFinder->stUser_Define[i].SignalCnt2);	// 신호발생횟수2(4)
			memcpy(pInRec3->giSignalCnt2, tmpVal, 4);

			INT2STR_BUF(tmpVal, 2, pUserFinder->stUser_Define[i].ResultViewNum);	// 결과값보기필드갯수(2)
			memcpy(pInRec3->giResultViewNum, tmpVal, 2);

			pInRec3->gsIndexType[0] = pUserFinder->stUser_Define[i].IndexType;          // 검색구분0:일반1:순위2:복합(1) 
			pInRec3->gsSearchType[0] = pUserFinder->stUser_Define[i].SearchType;        // 검색형태(1)                   
			pInRec3->gsOperator[0] = pUserFinder->stUser_Define[i].Operator;            // 연산자0:OR1:AND2:NEXT(1)      
			pInRec3->gsReverse[0] = pUserFinder->stUser_Define[i].Reverse;              // 결과제외0:A1:NotA(1)          
			pInRec3->gsIdxContiGubun[0] = pUserFinder->stUser_Define[i].IdxContiGubun;	// 0:단일발생1:연속발생지표(1)   
			pInRec3->gsElement[0] = pUserFinder->stUser_Define[i].Element;              // 조건식사용문자(A)(1)

			memcpy(pInRec3->gsScode, pUserFinder->stUser_Define[i].Code, 6);	// shortcode(6)  

			int j = 0;
			for(j=0; j<MAX_PARAM; j++)
			{
				sprintf(tmpVal, "%016f", pUserFinder->stUser_Define[i].Param[j]);
				memcpy(&pInRec3->gdParam[16*j], tmpVal, 16);	// 지표관련변수값(96)
			}

			for(j=0; j<MAX_CONDI; j++)
			{
				sprintf(tmpVal, "%016f", pUserFinder->stUser_Define[i].Condition[j]);
				memcpy(&pInRec3->gdCondition[16*j], tmpVal, 16);	// 기타설정변수값(192) 
			}
		}
	}

	// --------------------------------------------------------------------------------------
	// Output처리
	// --------------------------------------------------------------------------------------
	int  D1022OutRec1_GetDataCount(KB_d1022_OutRec1* pOutRec)
	{
		char szTmp[5] = {NULL, };
		::memcpy(szTmp, pOutRec->gsJongCnt, sizeof(pOutRec->gsJongCnt)); // 검색된 전제 종목 수
		if(::lstrlen(szTmp) == 0) return 0;

		int nRet = atoi(szTmp);
		{
//			if(szTmp.CompareNoCase("-900")==0) AfxMessageBox("결과 0");
//			else
			if(!::strcmp(szTmp, "-901"))
				AfxMessageBox("Input에러");
		}
		// 조회된 종목이 없을 경우 (-900)이 리턴된다고 함.
		return (nRet<0)? 0: nRet;
	}

	void D1022OutRec1_SetData(KB_d1022_OutRec1* pOutRec, TR_RESULT_HEADER* pTrResult)
	{
		//TR_RESULT_HEADER* pTrResult = (TR_RESULT_HEADER*)pOutBuf;
		//ZeroMemory(pTrResult, sizeof(TR_RESULT_HEADER));
		memset(pTrResult, 0x20, sizeof(TR_RESULT_HEADER));

		char* szTemp = new char[7];
		
		::memset(szTemp, 0x00, 7);
		::memcpy(szTemp, pOutRec, 4);

		pTrResult->giCallIndex = atoi(szTemp);				// 클라이언트조회구분(1)

		::memset(szTemp, 0x00, 7);
		::memcpy(szTemp, pOutRec->gsJongCnt, sizeof(pOutRec->gsJongCnt));
		Safe_CheckCpy(pTrResult->sJongCnt, szTemp);		// 검색종목수(4)
		int nRet = atoi(szTemp);
		if(nRet<=0)
		{
			//memset(pTrResult->sJongCnt, 0x20, )
			return;
		}

		::memset(szTemp, 0x00, 7);
		::memcpy(szTemp, pOutRec->gsHigh, sizeof(pOutRec->gsHigh));
		Safe_CheckCpy(pTrResult->sHigh, szTemp);		// 상한종목수(4)        

		::memset(szTemp, 0x00, 7);
		::memcpy(szTemp, pOutRec->gsUp, sizeof(pOutRec->gsUp));
		Safe_CheckCpy(pTrResult->sUp, szTemp);			// 상승종목수(4)        

		::memset(szTemp, 0x00, 7);
		::memcpy(szTemp, pOutRec->gsNoch, sizeof(pOutRec->gsNoch));
		Safe_CheckCpy(pTrResult->sNoch, szTemp);		// 보합종목수(4)        

		::memset(szTemp, 0x00, 7);
		::memcpy(szTemp, pOutRec->gsDown, sizeof(pOutRec->gsDown));
		Safe_CheckCpy(pTrResult->sDown, szTemp);		// 하락종목수(4)        

		::memset(szTemp, 0x00, 7);
		::memcpy(szTemp, pOutRec->gsLow, sizeof(pOutRec->gsLow));
		Safe_CheckCpy(pTrResult->sLow, szTemp);			// 하한종목수(4)        

		::memset(szTemp, 0x00, 7);
		::memcpy(szTemp, pOutRec->gsUpRate, sizeof(pOutRec->gsUpRate));
		Safe_CheckCpy(pTrResult->sUpRate, szTemp);		// 상승비중(6)

		::memset(szTemp, 0x00, 7);
		::memcpy(szTemp, pOutRec->gsChgrateAvg, sizeof(pOutRec->gsChgrateAvg));
		Safe_CheckCpy(pTrResult->sChgrateAvg, szTemp);	// 평균등락율(6)

		delete [] szTemp;
	};

	void D1022OutRec2_SetData(KB_d1022_OutRec2* pOutRec2, char* pDataBuf, int nCnt)
		//TR_RESULT_LIST* pResultList
	{
		//TR_RESULT_LIST* pResultList = (TR_RESULT_LIST*)pOutBuf;

		int nPos=0;
		char szTemp[21];
		for(int i=0; i<nCnt; i++)
		{
			TR_RESULT_LIST* pResultList = (TR_RESULT_LIST*)&pDataBuf[nPos];
			nPos += sizeof(TR_RESULT_LIST);
			ZeroMemory(pResultList, sizeof(TR_RESULT_LIST));

			int nSize = sizeof(KB_d1022_OutRec2);
			KB_d1022_OutRec2* pOutRec = (KB_d1022_OutRec2*)(pOutRec2 + i);

			::memset(szTemp, 0x00, sizeof(szTemp));
			::memcpy(szTemp, pOutRec->gsMflag, sizeof(pOutRec->gsMflag));
			pResultList->sMarket[0] = szTemp[0];			// 시장구분(1)

			::memset(szTemp, 0x00, sizeof(szTemp));
			::memcpy(szTemp, pOutRec->gsCode, sizeof(pOutRec->gsCode));
			Safe_CheckCpy(pResultList->sCode, szTemp);	// 종목코드(6)    

			::memset(szTemp, 0x00, sizeof(szTemp));
			::memcpy(szTemp, pOutRec->gsHname, sizeof(pOutRec->gsHname));
			Safe_CheckCpy(pResultList->sName, szTemp);       // 종목명(20)         

			::memset(szTemp, 0x00, sizeof(szTemp));
			::memcpy(szTemp, pOutRec->gsSignCnt, sizeof(pOutRec->gsSignCnt));
			Safe_CheckCpy(pResultList->sSignCnt, szTemp);        // 등락연속봉수(3)       

			::memset(szTemp, 0x00, sizeof(szTemp));
			::memcpy(szTemp, pOutRec->gsClose, sizeof(pOutRec->gsClose));
			Safe_CheckCpy(pResultList->sPrice, szTemp);        // 현재가(8)          

			::memset(szTemp, 0x00, sizeof(szTemp));
			::memcpy(szTemp, pOutRec->gsSign, sizeof(pOutRec->gsSign));
			//Safe_CheckCpy(pResultList->sSign , szTemp);        // 등락부호(1)        
			pResultList->sSign[0] = szTemp[0];

			::memset(szTemp, 0x00, sizeof(szTemp));
			::memcpy(szTemp, pOutRec->gsChange, sizeof(pOutRec->gsChange));
			Safe_CheckCpy(pResultList->sChange , szTemp);        // 전일대비(8)        

			::memset(szTemp, 0x00, sizeof(szTemp));
			::memcpy(szTemp, pOutRec->gsChgRate, sizeof(pOutRec->gsChgRate));
			Safe_CheckCpy(pResultList->sChgRate, szTemp);       // 전일대비등락율(6)  

			::memset(szTemp, 0x00, sizeof(szTemp));
			::memcpy(szTemp, pOutRec->gsVolume, sizeof(pOutRec->gsVolume));
			Safe_CheckCpy(pResultList->sVolume, szTemp);       // 거래량(9)          

			::memset(szTemp, 0x00, sizeof(szTemp));
			::memcpy(szTemp, pOutRec->gsVolumeRate, sizeof(pOutRec->gsVolumeRate));
			Safe_CheckCpy(pResultList->sVolumeRate, szTemp);        // 거래량전일대비율(6)
		}
	}
};

#endif	// __EU_TREELIST_TRAN_KB_d1022_H__


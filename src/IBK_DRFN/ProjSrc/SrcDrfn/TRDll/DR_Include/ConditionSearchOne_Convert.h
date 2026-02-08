#ifndef AFX_CONDITIONSEARCH_ONE_CONVERT__HEADER
#define AFX_CONDITIONSEARCH_ONE_CONVERT__HEADER

#include "./EU_p0623.h"
#include "../ConditionSearch_One/DefineFiles/Server_typedef.h"

#define INT2STR_BUF(xStr, nSize, nStr) sprintf(xStr, "%#0*.*d", nSize, nSize, nStr)
#define Safe_CheckCopy(xSource, xTarget) if(xTarget.GetLength()) memcpy(xSource, (LPCSTR)xTarget, xTarget.GetLength())
#define Safe_CheckCopy2(xSource, xTarget, xLen, xStr) \
	xStr.Format("%-*.*s", xLen, xLen, xTarget); \
	xStr.TrimRight();	\
	if(xStr.GetLength()) memcpy(xSource, (LPCSTR)xStr, xStr.GetLength())

class C0622TR
{
public:
	// --------------------------------------------------------------------------------------
	// Input처리
	// --------------------------------------------------------------------------------------
	void TR_RESULT_IN_To_EU_p0622_InRec1(TR_RESULT_IN* pIn, EU_p0622_InRec1* pOut)
	{
		memset(pOut, 0x20, sizeof(EU_p0622_InRec1));

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

	void User_Finder_To_EU_p0622_InRec2(User_Finder* pUserFinder, char* pOut, int &nPos)
	{
		EU_p0622_InRec2* pInRec2 = (EU_p0622_InRec2*)&pOut[nPos];
		memset(pInRec2, 0x20, sizeof(EU_p0622_InRec2));

		CString szVal;
		char tmpVal[300]={0,};
		int  nTargetIdxCnt=0, nComplexIdxCnt=0;

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
		int i = 0;
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

		nPos += sizeof(EU_p0622_InRec2);

		EU_p0622_InRec3* pInRec3;
		for(i=0; i<nTargetIdxCnt+nComplexIdxCnt; i++)
		{
			pInRec3 = (EU_p0622_InRec3*)&pOut[nPos];
			memset(pInRec3, 0x20, sizeof(EU_p0622_InRec3));
			nPos += sizeof(EU_p0622_InRec3);

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

// 	int  P0622OutRec1_GetDataCount(CHARVECTOR2 &vvRpValueList)
// 	{
// 		CString szTmp;
// 		szTmp	 = vvRpValueList[1][1];	// 검색된 전제 종목 수
// 		if(szTmp.GetLength()==0) return 0;
// 
// 		int nRet = atoi(szTmp);
// 		{
// //			if(szTmp.CompareNoCase("-900")==0) AfxMessageBox("결과 0");
// //			else
// 				if(szTmp.CompareNoCase("-901")==0) AfxMessageBox("Input에러");
// 		}
// 		// 조회된 종목이 없을 경우 (-900)이 리턴된다고 함.
// 		return (nRet<0)? 0: nRet;
// 	}

// 	void P0622OutRec1_SetData(CHARVECTOR2 &vvRpValueList, TR_RESULT_HEADER* pTrResult)
// 	{
// 		//TR_RESULT_HEADER* pTrResult = (TR_RESULT_HEADER*)pOutBuf;
// 		//ZeroMemory(pTrResult, sizeof(TR_RESULT_HEADER));
// 		memset(pTrResult, 0x20, sizeof(TR_RESULT_HEADER));
// 
// 		CString szTmp;
// 		szTmp	 = vvRpValueList[1][0];
// 		pTrResult->nTrIndex = atoi(szTmp);				// 클라이언트조회구분(1)
// 
// 		szTmp	 = vvRpValueList[1][1];
// 		Safe_CheckCopy(pTrResult->sJongCnt, szTmp);		// 검색종목수(4)
// 		int nRet = atoi(szTmp);
// 		if(nRet<=0)
// 		{
// 			//memset(pTrResult->sJongCnt, 0x20, )
// 			return;
// 		}
// 
// 		szTmp	 = vvRpValueList[1][2];
// 		Safe_CheckCopy(pTrResult->sHigh, szTmp);		// 상한종목수(4)        
// 
// 		szTmp	 = vvRpValueList[1][3];
// 		Safe_CheckCopy(pTrResult->sUp, szTmp);			// 상승종목수(4)        
// 
// 		szTmp	 = vvRpValueList[1][4];
// 		Safe_CheckCopy(pTrResult->sNoch, szTmp);		// 보합종목수(4)        
// 
// 		szTmp	 = vvRpValueList[1][5];
// 		Safe_CheckCopy(pTrResult->sDown, szTmp);		// 하락종목수(4)        
// 
// 		szTmp	 = vvRpValueList[1][6];
// 		Safe_CheckCopy(pTrResult->sLow, szTmp);			// 하한종목수(4)        
// 
// 		szTmp	 = vvRpValueList[1][7];
// 		Safe_CheckCopy(pTrResult->sUpRate, szTmp);		// 상승비중(6)
// 
// 		szTmp	 = vvRpValueList[1][8];
// 		Safe_CheckCopy(pTrResult->sChgrateAvg, szTmp);	// 평균등락율(6)
// 	};

// 	void P0622OutRec2_SetData(CHARVECTOR2 &vvRpValueList, char* pDataBuf, int nCnt)
// 		//TR_RESULT_LIST* pResultList
// 	{
// 		//TR_RESULT_LIST* pResultList = (TR_RESULT_LIST*)pOutBuf;
// 
// 		int nPos=0;
// 		CString szTmp;
// 		for(int i=0; i<nCnt; i++)
// 		{
// 			TR_RESULT_LIST* pResultList = (TR_RESULT_LIST*)&pDataBuf[nPos];
// 			nPos += sizeof(TR_RESULT_LIST);
// 			ZeroMemory(pResultList, sizeof(TR_RESULT_LIST));
// 
// 			szTmp	 = vvRpValueList[i+1][0];
// 			pResultList->sMarket[0] = szTmp[0];			// 시장구분(1)
// 
// 			szTmp	 = vvRpValueList[i+1][1];
// 			Safe_CheckCopy(pResultList->sCode, szTmp);	// 종목코드(6)    
// 
// 			szTmp	 = vvRpValueList[i+1][2];
// 			Safe_CheckCopy(pResultList->sName, szTmp);       // 종목명(20)         
// 
// 			szTmp	 = vvRpValueList[i+1][3];
// 			Safe_CheckCopy(pResultList->sSignCnt, szTmp);        // 등락연속봉수(3)       
// 
// 			szTmp	 = vvRpValueList[i+1][4];
// 			Safe_CheckCopy(pResultList->sPrice, szTmp);        // 현재가(8)          
// 
// 			szTmp	 = vvRpValueList[i+1][5];
// 			//Safe_CheckCopy(pResultList->sSign , szTmp);        // 등락부호(1)        
// 			pResultList->sSign[0] = szTmp[0];
// 
// 			szTmp	 = vvRpValueList[i+1][6];
// 			Safe_CheckCopy(pResultList->sChange , szTmp);        // 전일대비(8)        
// 
// 			szTmp	 = vvRpValueList[i+1][7];
// 			Safe_CheckCopy(pResultList->sChgRate, szTmp);       // 전일대비등락율(6)  
// 
// 			szTmp	 = vvRpValueList[i+1][8];
// 			Safe_CheckCopy(pResultList->sVolume, szTmp);       // 거래량(9)          
// 
// 			szTmp	 = vvRpValueList[i+1][9];
// 			Safe_CheckCopy(pResultList->sVolumeRate, szTmp);        // 거래량전일대비율(6)
// 		}
// 	}

};

class CUser_Finder_To_0623InRec2
{
public:
	void ConvertData(User_Finder* pUserFinder, EU_p0623_InRec2* pInRec2)
	{
		//EU_p0623_InRec2* pInRec2;
		memset(pInRec2, 0x20, sizeof(EU_p0623_InRec2));

		char tmpVal[100]={0,};

		INT2STR_BUF(tmpVal, 4, pUserFinder->TrIndex);				// giCallGubun(4)
		memcpy(pInRec2->giCallGubun, tmpVal, 4);

		INT2STR_BUF(tmpVal, 2, pUserFinder->nTargetType);			// 0 :종목 , 10 : 업종	동부 [NEW]
		memcpy(pInRec2->giTargetGubun, tmpVal, 2);

		INT2STR_BUF(tmpVal, 2, pUserFinder->TargetIdxCnt);          // 대상 지표 갯수       
		memcpy(pInRec2->giTargetIdxCnt, tmpVal, 2);

		INT2STR_BUF(tmpVal, 2, pUserFinder->TargetResultViewCnt);	// 대상 값보기필드 개수 
		memcpy(pInRec2->giTargetResultViewCnt, tmpVal, 2);

		//INT2STR_BUF(tmpVal, 80, pUserFinder->TargetExp[EXPLEN]);	// 대상조건 조합 표현 식	
		memcpy(pInRec2->gsTargetExp, pUserFinder->TargetExp, EXPLEN);

		INT2STR_BUF(tmpVal, 2, pUserFinder->ComplexIdxCnt);			// 복합 지표 갯수       
		memcpy(pInRec2->giComplexIdxCnt, tmpVal, 2);

		INT2STR_BUF(tmpVal, 2, pUserFinder->ComplexResultViewCnt);	// 복합 값보기필드 개수 
		memcpy(pInRec2->giComplexResultViewCnt, tmpVal, 2);

		//INT2STR_BUF(tmpVal, 80, pUserFinder->ComplexExp[EXPLEN]);	// 복합조건 조합 표현 식	
		memcpy(pInRec2->gsComplexExp, pUserFinder->ComplexExp, EXPLEN);

		INT2STR_BUF(tmpVal, 4, pUserFinder->FieldEditerCnt);		// 필드편집 개수   [Fix + Move] 
		memcpy(pInRec2->giResultIdxCnt, tmpVal, 4);

		//INT2STR_BUF(tmpVal, 80, pUserFinder->FieldList[REAL_IDXCNT]);// 필드편집 번호리스트 [Just Fix + Move] 
		memcpy(pInRec2->gsResultExp, pUserFinder->FieldList, EXPLEN);

		INT2STR_BUF(tmpVal, 2, pUserFinder->ResultIdxCnt);			// 결과값비교 개수          [NEW]
		memcpy(pInRec2->giFieldEditerCnt, tmpVal, 2);

		INT2STR_BUF(tmpVal, 80, pUserFinder->ResultExp[EXPLEN]);	// 결과값 비교조합 표현 식  [NEW]		
		memcpy(pInRec2->giFieldList, pUserFinder->ResultExp, EXPLEN);

		//INT2STR_BUF(tmpVal, 1, pUserFinder->TargetBitChk);				// TargetBit 사용 여부 체크
		pInRec2->gsTargetBitChk[0] = pUserFinder->TargetBitChk;

		//INT2STR_BUF(tmpVal, 2500, &pUserFinder->TargetBit[0]);	// 검색 대상 종목			
		memcpy(pInRec2->gsJongmokIndex, pUserFinder->TargetBit, RESULT_MAX);

		INT2STR_BUF(tmpVal, 1, pUserFinder->Sort_flag);					// 정렬 방향(0:오름, 1:내림)
		pInRec2->gsSort_flag[0] = pUserFinder->Sort_flag;

		INT2STR_BUF(tmpVal, 4, pUserFinder->Sort_Num);					// 정렬 번호(값보기필드pos) 	    
		memcpy(pInRec2->giSort_Pos, tmpVal, 4);

		INT2STR_BUF(tmpVal, 4, pUserFinder->BackDay);					// backtest 일자(미사용:-1) 	
		memcpy(pInRec2->giBackDate, tmpVal, 4);

		INT2STR_BUF(tmpVal, 4, pUserFinder->BackTime);					// backtest 시간(미사용:-1) 
		memcpy(pInRec2->giBackTime, tmpVal, 4);
	};
};




#include "./EU_p0825.h"

class CUSERALERT_TR0825
{
public:
	// --------------------------------------------------------------------------------------
	// Input처리
	// --------------------------------------------------------------------------------------
	void REAL_USER_IN_To_EU_USERALERT_IN1(REAL_USER_IN *pIn, EU_USERALERT_IN1 *pOut)
	{
		memset(pOut, 0x20, sizeof(EU_USERALERT_IN1));

		CString szVal;
		char tmpVal[100]={0,};

		pOut->gsSysUserFlag[0] = pIn->sType;
		pOut->gsFlag[0] = pIn->sFlag;

		Safe_CheckCopy2(pOut->gsAlertNum, pIn->sAlertNum, 4, szVal);
		Safe_CheckCopy2(pOut->gsSendCnt, pIn->sSendCnt, 2, szVal);
		Safe_CheckCopy2(pOut->gsUserID, pIn->sUserID, 20, szVal);
	}



	void USER_FINDER_To_EU_USERALERT_IN2(User_Finder *pIn, EU_USERALERT_IN2 *pOut)
	{
		memset(pOut, 0x20, sizeof(EU_USERALERT_IN2));

		char tmpVal[100]={0,};
		CString szVal;
		CString strOp="";
		CString strVal="";

		INT2STR_BUF(tmpVal, 4, pIn->TrIndex);
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

		CString strData;

		INT2STR_BUF(tmpVal, 2, pIn->nTargetType);
		memcpy(pOut->giTargetGubun, tmpVal, 2);
		INT2STR_BUF(tmpVal, 2, pIn->TargetIdxCnt);
		memcpy(pOut->giTargetIdxCnt, tmpVal, 2);
		INT2STR_BUF(tmpVal, 2, pIn->TargetResultViewCnt);
		memcpy(pOut->giTargetResultViewCnt, tmpVal, 2);		
//		memcpy(pOut->gsTargetExp, pIn->TargetExp, sizeof(pIn->TargetExp));
		Safe_CheckCopy2(pOut->gsTargetExp, pIn->TargetExp, 80, szVal);

		INT2STR_BUF(tmpVal, 2, pIn->ComplexIdxCnt);
		memcpy(pOut->giComplexIdxCnt, tmpVal, 2);
		INT2STR_BUF(tmpVal, 2, pIn->ComplexResultViewCnt);
		memcpy(pOut->giComplexResultViewCnt, tmpVal, 2);		
	//	memcpy(pOut->gsComplexExp, pIn->ComplexExp, sizeof(pIn->ComplexExp));
		Safe_CheckCopy2(pOut->gsComplexExp, pIn->ComplexExp, 80, szVal);

		INT2STR_BUF(tmpVal, 2, pIn->FieldEditerCnt);
		memcpy(pOut->giFieldEditerCnt, tmpVal, 2);	

		int nSize = 0;
		for(int nIndex = 0; nIndex < pIn->FieldEditerCnt; nIndex++)
		{			
			INT2STR_BUF(tmpVal, 4, pIn->FieldList[nIndex]);
			memcpy(pOut->giFieldList + nSize , tmpVal, 4);
			nSize += 4;
		}

		INT2STR_BUF(tmpVal, 4, pIn->ResultIdxCnt);
		memcpy(pOut->giResultIdxCnt, tmpVal, 4);
	//	memcpy(pOut->gsResultExp, pIn->ResultExp, sizeof(pIn->ResultExp));
		Safe_CheckCopy2(pOut->gsResultExp, pIn->ResultExp, 80, szVal);

		memcpy(pOut->gsTargetBitChk, (void*)&pIn->TargetBitChk, sizeof(pIn->TargetBitChk));
		memcpy(pOut->gsJongmokIndex, pIn->TargetBit, sizeof(pIn->TargetBit));

		memcpy(pOut->gsSort_falg, (void*)&pIn->Sort_flag, sizeof(pIn->Sort_flag));
		INT2STR_BUF(tmpVal, 4, pIn->Sort_Num);
		memcpy(pOut->giSort_Pos, tmpVal, 4);
		

		INT2STR_BUF(tmpVal, 4, pIn->BackDay);
		memcpy(pOut->giBackDate, tmpVal, 4);
		INT2STR_BUF(tmpVal, 4, pIn->BackTime);
		memcpy(pOut->giBackTime, tmpVal, 4);	

	}

	void USER_DEFINE_To_EU_USERALERT_IN3(User_Define *pIn, EU_USERALERT_IN3 *pOut)
	{
		memset(pOut, 0x20, sizeof(EU_USERALERT_IN3));
		char tmpVal[100]={0,};
		CString strOp="";
		CString strVal="";

		INT2STR_BUF(tmpVal, 2, pIn->IndexNo);
		memcpy(pOut->giIndexNo, tmpVal, 4);
		
		INT2STR_BUF(tmpVal, 2, pIn->GroupNo);
		memcpy(pOut->giGroupNo, tmpVal, 4);

		INT2STR_BUF(tmpVal, 2, pIn->GroupIdxCnt);
		memcpy(pOut->giGroupIdxCnt, tmpVal, 4);

		INT2STR_BUF(tmpVal, 2, pIn->TargetBase);
		memcpy(pOut->giTargetBase, tmpVal, 4);

		INT2STR_BUF(tmpVal, 4, pIn->Type);
		memcpy(pOut->giType, tmpVal, 4);

		INT2STR_BUF(tmpVal, 2, pIn->Type1);
		memcpy(pOut->giType1, tmpVal, 4);

		INT2STR_BUF(tmpVal, 2, pIn->Type2);
		memcpy(pOut->giType2, tmpVal, 4);

		INT2STR_BUF(tmpVal, 4, pIn->STerm);
		memcpy(pOut->giSTerm, tmpVal, 4);

		INT2STR_BUF(tmpVal, 4, pIn->ETerm);
		memcpy(pOut->giETerm, tmpVal, 4);

		INT2STR_BUF(tmpVal, 4, pIn->Rank);
		memcpy(pOut->giRank, tmpVal, 4);

		INT2STR_BUF(tmpVal, 4, pIn->Trend);
		memcpy(pOut->giTrend, tmpVal, 4);
		
		INT2STR_BUF(tmpVal, 4, pIn->SignalCnt1);
		memcpy(pOut->giSignalCnt1, tmpVal, 4);	

		INT2STR_BUF(tmpVal, 4, pIn->SignalCnt2);
		memcpy(pOut->giSignalCnt2, tmpVal, 4);	

		INT2STR_BUF(tmpVal, 2, pIn->ResultViewNum);
		memcpy(pOut->giResultViewNum, tmpVal, 4);	

		pOut->gsIndexType[0]	 = pIn->IndexType;
		pOut->gsSearchType[0]	 = pIn->SearchType;
		pOut->gsOperator[0]		 = pIn->Operator;
		pOut->gsReverse[0]		 = pIn->Reverse;
		pOut->gsIdxContiGubun[0] = pIn->IdxContiGubun;
		pOut->gsElement[0]		 = pIn->Element;

		memcpy(pOut->gsScode, pIn->Code, sizeof(pIn->Code));

		int nIndex = 0;
		for(nIndex=0; nIndex<MAX_PARAM; nIndex++)
		{
			sprintf(tmpVal, "%016f", pIn->Param[nIndex]);
			memcpy(&pOut->gdParam[16*nIndex], tmpVal, 16);	// 지표관련변수값(48)
		}

		for(nIndex=0; nIndex<MAX_CONDI; nIndex++)
		{
			sprintf(tmpVal, "%016f", pIn->Condition[nIndex]);
			memcpy(&pOut->gdCondition[16*nIndex], tmpVal, 16);	// 기타설정변수값(96) 
		}
	}
	

// 	void EU_USERALERT_OUT_SetData(CHARVECTOR2	&vvRpValueList, char *pBuff	)
// 	{
// 		CString strData;
// 
// 		REAL_USER_OUT	stREAL_USER_OUT;
// 		memset(&stREAL_USER_OUT, 0x00, sizeof(REAL_USER_OUT));
// 
// 		strData = vvRpValueList[1][0];
// 		stREAL_USER_OUT.sType = strData[0];	
// 
// 		strData = vvRpValueList[1][1];
// 		stREAL_USER_OUT.sFlag = strData[0];
// 		
// 		strData = vvRpValueList[1][2];
// 		stREAL_USER_OUT.sResultFlag = strData[0];
// 
// 
// 		strData = vvRpValueList[1][3];
// 		memcpy(stREAL_USER_OUT.sTime, strData, strData.GetLength());
// 		
// 		strData = vvRpValueList[1][4];
// 		memcpy(stREAL_USER_OUT.sAlertNum, strData, strData.GetLength());
// 
// 		strData = vvRpValueList[1][5];
// 		strData.TrimRight();
// 		memcpy(stREAL_USER_OUT.sErrMsg, strData, strData.GetLength());
// 
// 		memcpy(pBuff, &stREAL_USER_OUT, sizeof(REAL_USER_OUT));
// 	}

};


#endif //AFX_CONDITIONSEARCH_ONE_CONVERT__HEADER

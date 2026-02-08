#ifndef SERVER_TYPEDEF
#define SERVER_TYPEDEF

/************************************************************/
/*																													*/
/*	One Clk Condition Server =>  STRUCT/DEFINE/TYPEDEF 정의	*/
/*																													*/
/*					2005. 1.  			Doori														*/
/*					작성자 : 우동우																	*/
/************************************************************/
#define QUERY_STRJONGMOK		    	"p0602"     //차트 종목
//#define TRNO_MSTCODE				"p0827"	//"p0627"		//종목리스트 조회
#define TRNO_THEMEGROUP				"p0828"	//"p0628"		//테마 그룹 조회
#define TRNO_THEMEITEM				"p0830"	//"p0630"		//테마 종목 조회
#define		S_RQ_JAN_CODEINFO		"a7001"		// 주식	    계좌 잔고 데이터 요청 TR

/*
typedef struct 
{
	char	accno			[11];		// 계좌번호 
} JAN_RQ_INPUT;
*/
/*
#define CONDITION_MAXCNT		50
#define TRNO_MSTCODE        "50008"
#define TRNO_CONDIONECLK_LIST			"50005"
#define TRNO_CONDIONECLK_RESULT		"50004"
#define TRNO_TEMACLK_LIST					"50006"
#define TRNO_TEMACLK_RESULT				"50007"
#define TRNO_CONDITION_RESULT			"50009"
#define TRNO_USER_REALREG				  "50002"
*/

// typedef struct tagMSTCode
// {
//   char    sMarket[1];
//   char    sCode[7];
//   char    sName[20];
//   char    sSetPos[4];
// } MSTCode;

typedef struct tagMSTCode
{
	char    sMarket[1];
	char    sCode[7];
	char    sName[20];
	char    sSetPos[4];
	char    sUsjg[1];               /* 우선주   (0:해당없음,1:해당) */
	char    sUnder[1];              /* 관리종목                     */
	char    sStop[1];               /* 거래정지종목                 */
	char    sDishonest[1];          /* 불성실공시종목               */
	char    sConfidencelimit[1];    /* 신용제한종목?               */
	char    sJrmm[1];          	    /* 정리매매종목                 */
	char    sDrat[3];               /* 증거금비율                   */
	char    sMrktalrm[1];           /* 시장경보
										1:투자주의, 2:투자경고
										3:투자위험, 4:투자위험예고*/
	char	sMonth[2];				/* 결산월 */
	char	sETF[1];				/* ETF							*/
} MSTCode;


#define COND_MST_VER	1


typedef struct tagTrConVol
{
	char		sDataSection[4];		// 데이터구분
	char		sPacketCode[4];			// 패킷구분코드
	char		sAlertBody[1];			// System, User
	char		sSignaldate[8];			// 신호 발생일자
	char		sSignaltime[6];			// 신호 발생시간
	char		sNowItemCnt[4];			// 현재만족 종목수
	char		sSendItemCnt[4];		// 전송종목수
	char		sSignalBirth[1];		// 신호발생
	char		sJobflag[1];			// 신규,재진입,탈퇴
	char		sItemCode[6];			// 종목코드
	char		sItemName[20];			// 종목명
	char		sEntryCnt[4];			// 진입 횟수
	char		sCategorybranch_l[4];	// 업종대분류
	char		sCategorybranch_m[4];	// 업종중분류
	char		sCategorybranch_s[4];	// 업종소분류
	char		sPreferShare[1];		// 우선주
	char		sManagement[1];			// 관리종목
	char		sControl[1];			// 이상급등 종목
	char		sTradestop[1];			// 거래정지 종목
	char		sInsinceritynotice[1];	// 불성실공시 종목
	char		sConfidencelimit[1];	// 신용제한 종목
	char		sCodenotice[1];			// 종목공시
	char		sJungrimeme[1];			// 정리매매 종목
	char		sMarket[1];				// 시장구분
	char		sDepositmoney[3];		// 증거금 비율
	char		sInvestrisk[1];			// 투자위험 종목
	char		sInvestwarn[1];			// 투자경고 종목
	char		sInvestattention[1];	// 투자주의 종목
	char		sInvestrisknotice[1];	// 투자위험예고 종목
	char		sPrice[8];				// 현재가
	char		sSign[1];				// 전일대비등락부호
	char		sChange[8];				// 전일대비
	char		sChgrate[6];			// 전일대비등락율
	char		sVolume[9];				// 거래량
	char		sVolumerate[6];			// 거래량 전일대비율
	char		sAmount[9];				// 거래대금](누적순간체결대금)   단위:천원
	char		sCvolume[9];			// 순간체결량
	char		sCvolumerate[6];		// 누적거래량대비체결량
	char		sCamount[9];			// 순간체결대금]  단위:원
	char		sCGubun[1];				// 체결구분 ('+':매수 '-':매도)
	char		sVolumePower[9];        // 체결강도 (20090408 추가)
	char		sMflag[1];				// 마켓 구분 '1' = 거래소 '2' = 코스닥
} StTrConVol;   //Eugene 순간 체결량 Packet




/////////////////////



// class C0627OutBlock1
// {
// public:
// 	int  GetDataCount(CHARVECTOR2 &vvRpValueList)
// 	{
// 		CString szTmp;
// 		szTmp	 = vvRpValueList[1][0];	// 검색된 전제 종목 수
// 		if(szTmp.GetLength()==0) return 0;
// 
// 		int nRet = atoi(szTmp);
// 		return (nRet<0)? 0: nRet;
// 	}
// };
// 
// class C0627OutBlock2
// {
// public:
// 	void SetData(CHARVECTOR2 &vvRpValueList, char* pDataBuf, int nDataCnt)
// 	{
// 		CString sMarket,		//마켓구분
// 				sCode,		//종목코드
// 				sName,		//종목명
// 				sSetPos;	//인덱스
// 		int nPos = 0;
// 
// 		for(int i=0; i<nDataCnt; i++)
// 		{
// 			MSTCode* pOutData = (MSTCode*)&pDataBuf[nPos];
// 			nPos += sizeof(MSTCode);
// 
// 			ZeroMemory(pOutData, sizeof(MSTCode));
// 
// 			sMarket	 = vvRpValueList[i+1][0];
// 			pOutData->sMarket[0] = sMarket[0];
// 
// 			sCode = vvRpValueList[i+1][1];
// 			Safe_CheckCopy(pOutData->sCode ,sCode);
// 			pOutData->sCode[6] = 0;
// 
// 			sName = vvRpValueList[i+1][2];
// 			sName.TrimRight();
// 			//Safe_CheckCopy(pOutData->sName ,sName);
// 			memcpy(pOutData->sName, (LPCSTR)sName, 20);
// 			pOutData->sName[sName.GetLength()]=0;
// 
// 			sSetPos = vvRpValueList[i+1][3];
// 			Safe_CheckCopy(pOutData->sSetPos ,sSetPos);
// 			//memcpy(pOutData->sSetPos, (LPCSTR)sSetPos, 4);
// 		}
// 	};
// };

///////////테마그룹
///////////////////
typedef struct
{
	char		giCallIndex[4];
	char		gsCode[6];
	char		gsFilterGubun[1];
}ST_THEME_IN;					//p0628InBlock;
#define SZ_THEME_IN sizeof(ST_THEME_IN)

typedef struct
{
	char		sTrIndex[4];
	char		sCnt[4];
}ST_THEME_OUT1;						//p0628OutBlock1;
#define SZ_THEME_OUT1 sizeof(ST_THEME_OUT1)

typedef struct
{
	char		gsThemeCode[4];
	char		gsThemeName[36];
}ST_THEME_OUT2;						//p0628OutBlock2;
#define SZ_THEME_OUT2 sizeof(ST_THEME_OUT2)


///테마 종목
typedef struct
{
	char		sTrIndex[4];
	char		sCode[4];
}ST_THEMEITEM_IN;					//p0630InBlock;
#define SZ_THEMEITEM_IN sizeof(ST_THEMEITEM_IN)

typedef struct
{
	char		sTrIndex[4];
	char		sCnt[4];
}ST_THEMEITEM_OUT1;					//p0630OutBlock1;
#define SZ_THEMEITEM_OUT1 sizeof(ST_THEMEITEM_OUT1)

typedef struct
{
	char		sItemCode[6];
	char		sMarketFlag[1];
}ST_THEMEITEM_OUT2;					//p0630OutBlock2;
#define SZ_THEMEITEM_OUT2 sizeof(ST_THEMEITEM_OUT2)
#endif // SERVER_TYPEDEF


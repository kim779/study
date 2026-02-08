// ConfigSet.cpp: implementation of the CConfigSet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ConfigSet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConfigSet::CConfigSet()
{
	InitMember();
}

CConfigSet::~CConfigSet()
{

}

//---------------------------------------------------------
// Author       : Sang-Yun, Nam			Date : 2005/01/28
// Modifier     : 
// Comments		: 외부에서 값을 한꺼번에 바꿀 경우 호출된다.
//---------------------------------------------------------
void CConfigSet::SetConfigValue(const CConfigSet* p_pConfigSet)
{
	if(p_pConfigSet == NULL) return;

	m_bChkYSprice 		= p_pConfigSet->m_bChkYSprice 		;//전일가격 시가			체크 여부
	m_bChkYHprice 		= p_pConfigSet->m_bChkYHprice 		;//전일가격 고가			체크 여부
	m_bChkYLprice 		= p_pConfigSet->m_bChkYLprice 		;//전일가격 저가			체크 여부
	m_bChkYEprice 		= p_pConfigSet->m_bChkYEprice 		;//전일가격 종가			체크 여부
	m_bChkYSHL 			= p_pConfigSet->m_bChkYSHL 			;//전일가격 (시+고+저)/3	체크 여부
	
	m_bChkTSprice 		= p_pConfigSet->m_bChkTSprice 		;//당일가격 시가			체크 여부
	m_bChkTHprice 		= p_pConfigSet->m_bChkTHprice 		;//당일가격 고가			체크 여부
	m_bChkTLprice 		= p_pConfigSet->m_bChkTLprice 		;//당일가격 저가			체크 여부
	m_bChkTEprice 		= p_pConfigSet->m_bChkTEprice 		;//당일가격 종가			체크 여부
	m_bChkTHL 			= p_pConfigSet->m_bChkTHL 			;//당일가격 (고+저)/2		체크 여부
	m_bChkTSHL 			= p_pConfigSet->m_bChkTSHL 			;//당일가격 (시+고+저)/3	체크 여부
	m_bChkTHLE 			= p_pConfigSet->m_bChkTHLE 			;//당일가격 (고+저+(종*2))/4체크 여부
	m_bChkHighLimit		= p_pConfigSet->m_bChkHighLimit		;//상한가					체크 여부
	m_bChkLowLimit		= p_pConfigSet->m_bChkLowLimit		;//하한가				 체크 여부
	
	m_bChkPivot2Rst 	= p_pConfigSet->m_bChkPivot2Rst		;// 피봇 2차 저항 			체크 여부
	m_bChkPivot1Rst 	= p_pConfigSet->m_bChkPivot1Rst		;// 피봇 1차 저항 			체크 여부
	m_bChkPivotGLine 	= p_pConfigSet->m_bChkPivotGLine	;// 피봇 기준선  			체크 여부
	m_bChkPivot1GG 		= p_pConfigSet->m_bChkPivot1GG 		;// 피봇 1차 지지 			체크 여부
	m_bChkPivot2GG 		= p_pConfigSet->m_bChkPivot2GG 		;// 피봇 2차 지지 			체크 여부
	
	m_bChkDMKPreHPrice 	= p_pConfigSet->m_bChkDMKPreHPrice	;// Demark 예상고가  		체크 여부
	m_bChkDMKGLine 		= p_pConfigSet->m_bChkDMKGLine 		;// Demark 기준선			체크 여부
	m_bChkDMKPreLPrice 	= p_pConfigSet->m_bChkDMKPreLPrice	;// Demark 예상저가  		체크 여부
	
	m_bChk2Pyung1 		= p_pConfigSet->m_bChk2Pyung1		;// 이동평균 첫번째 입력값	체크 여부
	m_bChk2Pyung2 		= p_pConfigSet->m_bChk2Pyung2		;// 이동평균 두번째 입력값	체크 여부
	m_bChk2Pyung3 		= p_pConfigSet->m_bChk2Pyung3		;// 이동평균 세번째 입력값	체크 여부
	m_bChk2Pyung4 		= p_pConfigSet->m_bChk2Pyung4		;// 이동평균 네번째 입력값	체크 여부
	
	m_bChkPmmDanga 		= p_pConfigSet->m_bChkPmmDanga		;// 평균매입 단가			체크 여부
	m_bChkPmmPDanga 	= p_pConfigSet->m_bChkPmmPDanga		;// 평균매입 단가 +n%		체크 여부
	m_bChkPmmMDanga 	= p_pConfigSet->m_bChkPmmMDanga		;// 평균매입 단가 -n%		체크 여부
	
	
	
	m_clrYSprice 		= p_pConfigSet->m_clrYSprice		;//전일가격 시가			색 지정값
	m_clrYHprice 		= p_pConfigSet->m_clrYHprice		;//전일가격 고가			색 지정값
	m_clrYLprice 		= p_pConfigSet->m_clrYLprice		;//전일가격 저가			색 지정값
	m_clrYEprice 		= p_pConfigSet->m_clrYEprice		;//전일가격 종가			색 지정값
	m_clrYSHL 			= p_pConfigSet->m_clrYSHL 			;//전일가격 (시+고+저)/3	색 지정값
	
	m_clrTSprice 		= p_pConfigSet->m_clrTSprice		;//당일가격 시가			색 지정값
	m_clrTHprice 		= p_pConfigSet->m_clrTHprice		;//당일가격 고가			색 지정값
	m_clrTLprice 		= p_pConfigSet->m_clrTLprice		;//당일가격 저가			색 지정값
	m_clrTEprice 		= p_pConfigSet->m_clrTEprice		;//당일가격 종가			색 지정값
	m_clrTHL 			= p_pConfigSet->m_clrTHL 			;//당일가격 (고+저)/2		색 지정값
	m_clrTSHL 			= p_pConfigSet->m_clrTSHL 			;//당일가격 (시+고+저)/3	색 지정값
	m_clrTHLE 			= p_pConfigSet->m_clrTHLE 			;//당일가격 (고+저+(종*2))/4색 지정값
	m_clrHighLimit		= p_pConfigSet->m_clrHighLimit		;//상한가					색 지정값
	m_clrLowLimit		= p_pConfigSet->m_clrLowLimit		;//하한가					색 지정값
	
	m_clrPivot2Rst 		= p_pConfigSet->m_clrPivot2Rst 		;// 피봇 2차 저항			색 지정값
	m_clrPivot1Rst 		= p_pConfigSet->m_clrPivot1Rst 		;// 피봇 1차 저항			색 지정값
	m_clrPivotGLine 	= p_pConfigSet->m_clrPivotGLine		;// 피봇 기준선				색 지정값
	m_clrPivot1GG 		= p_pConfigSet->m_clrPivot1GG 		;// 피봇 1차 지지			색 지정값
	m_clrPivot2GG 		= p_pConfigSet->m_clrPivot2GG 		;// 피봇 2차 지지			색 지정값
	
	m_clrDMKPreHPrice 	= p_pConfigSet->m_clrDMKPreHPrice	;// Demark 예상고가			색 지정값
	m_clrDMKGLine 		= p_pConfigSet->m_clrDMKGLine 		;// Demark 기준선			색 지정값
	m_clrDMKPreLPrice 	= p_pConfigSet->m_clrDMKPreLPrice	;// Demark 예상저가			색 지정값
	
	m_clr2Pyung1 		= p_pConfigSet->m_clr2Pyung1		;// 이동평균 첫번째 입력값	색 지정값
	m_clr2Pyung2 		= p_pConfigSet->m_clr2Pyung2		;// 이동평균 두번째 입력값	색 지정값
	m_clr2Pyung3 		= p_pConfigSet->m_clr2Pyung3		;// 이동평균 세번째 입력값	색 지정값
	m_clr2Pyung4 		= p_pConfigSet->m_clr2Pyung4		;// 이동평균 네번째 입력값	색 지정값
	
	m_clrPmmDanga 		= p_pConfigSet->m_clrPmmDanga		;// 평균매입 단가			색 지정값
	m_clrPmmPDanga 		= p_pConfigSet->m_clrPmmPDanga		;// 평균매입 단가 +n%		색 지정값
	m_clrPmmMDanga 		= p_pConfigSet->m_clrPmmMDanga		;// 평균매입 단가 -n%		색 지정값


	m_nLineYSprice		= p_pConfigSet->m_nLineYSprice		;//전일가격 시가			라인 두께
	m_nLineYHprice		= p_pConfigSet->m_nLineYHprice		;//전일가격 고가			라인 두께
	m_nLineYLprice		= p_pConfigSet->m_nLineYLprice		;//전일가격 저가			라인 두께
	m_nLineYEprice		= p_pConfigSet->m_nLineYEprice		;//전일가격 종가			라인 두께
	m_nLineYSHL			= p_pConfigSet->m_nLineYSHL			;//전일가격 (시+고+저)/3	라인 두께
	
	m_nLineTSprice		= p_pConfigSet->m_nLineTSprice		;//당일가격 시가			라인 두께
	m_nLineTHprice		= p_pConfigSet->m_nLineTHprice		;//당일가격 고가			라인 두께
	m_nLineTLprice		= p_pConfigSet->m_nLineTLprice		;//당일가격 저가			라인 두께
	m_nLineTEprice		= p_pConfigSet->m_nLineTEprice		;//당일가격 종가			라인 두께
	m_nLineTHL			= p_pConfigSet->m_nLineTHL			;//당일가격 (고+저)/2		라인 두께
	m_nLineTSHL			= p_pConfigSet->m_nLineTSHL			;//당일가격 (시+고+저)/3	라인 두께
	m_nLineTHLE			= p_pConfigSet->m_nLineTHLE			;//당일가격 (고+저+(종*2))/4 라인 두께
	m_nLineHighLimit	= p_pConfigSet->m_nLineHighLimit	;//상한가					라인 두께
	m_nLineLowLimit		= p_pConfigSet->m_nLineLowLimit		;//하한가					라인 두께
	
	m_nLinePivot2Rst	= p_pConfigSet->m_nLinePivot2Rst	;// 피봇 2차 저항			라인 두께
	m_nLinePivot1Rst	= p_pConfigSet->m_nLinePivot1Rst	;// 피봇 1차 저항			라인 두께
	m_nLinePivotGLine	= p_pConfigSet->m_nLinePivotGLine	;// 피봇 기준선				라인 두께
	m_nLinePivot1GG		= p_pConfigSet->m_nLinePivot1GG		;// 피봇 1차 지지			라인 두께
	m_nLinePivot2GG		= p_pConfigSet->m_nLinePivot2GG		;// 피봇 2차 지지			라인 두께
	
	m_nLineDMKPreHPrice = p_pConfigSet->m_nLineDMKPreHPrice	;// Demark 예상고가			라인 두께
	m_nLineDMKGLine		= p_pConfigSet->m_nLineDMKGLine		;// Demark 기준선			라인 두께
	m_nLineDMKPreLPrice = p_pConfigSet->m_nLineDMKPreLPrice	;// Demark 예상저가			라인 두께
	
	m_nLine2Pyung1		= p_pConfigSet->m_nLine2Pyung1		;// 이동평균 첫번째 입력값	라인 두께
	m_nLine2Pyung2		= p_pConfigSet->m_nLine2Pyung2		;// 이동평균 두번째 입력값	라인 두께
	m_nLine2Pyung3		= p_pConfigSet->m_nLine2Pyung3		;// 이동평균 세번째 입력값	라인 두께
	m_nLine2Pyung4		= p_pConfigSet->m_nLine2Pyung4		;// 이동평균 네번째 입력값	라인 두께
	
	m_nLinePmmDanga		= p_pConfigSet->m_nLinePmmDanga		;// 평균매입 단가			라인 두께
	m_nLinePmmPDanga	= p_pConfigSet->m_nLinePmmPDanga	;// 평균매입 단가 +n%		라인 두께
	m_nLinePmmMDanga	= p_pConfigSet->m_nLinePmmMDanga	;// 평균매입 단가 -n%		라인 두께

	m_nDay2Pyung1		= p_pConfigSet->m_nDay2Pyung1		;//단순 이동평균 수치 1
	m_nDay2Pyung2		= p_pConfigSet->m_nDay2Pyung2		;//단순 이동평균 수치 2
	m_nDay2Pyung3		= p_pConfigSet->m_nDay2Pyung3		;//단순 이동평균 수치 3
	m_nDay2Pyung4		= p_pConfigSet->m_nDay2Pyung4		;//단순 이동평균 수치 4


	m_nPmmPDanga		= p_pConfigSet->m_nPmmPDanga;		// 평균매입 단가 +n%		값
	m_nPmmMDanga		= p_pConfigSet->m_nPmmMDanga;		// 평균매입 단가 -n%		값

	m_strAcct			= p_pConfigSet->m_strAcct;			// 계좌 정보

	m_bNotDrawLine		= p_pConfigSet->m_bNotDrawLine;		//기준선을 그릴지 여부

	m_bChkName		= p_pConfigSet->m_bChkName		;		// 정보표시 : 이름	체크 버튼
	m_bChkValue		= p_pConfigSet->m_bChkValue		;		// 정보표시 : 값	체크 버튼
	m_nAlign		= p_pConfigSet->m_nAlign		;		// 정보표시 (왼쪽/오른쪽/양쪽)
	
}


//---------------------------------------------------------
// Author       : Sang-Yun, Nam			Date : 2005/01/28
// Modifier     : 
// Comments		: 설정값을 전부 초기화 한다. 맨처음 로딩시 
//                환경 설정 파일이 없는경우 이 값으로 초기화!!
//---------------------------------------------------------
void CConfigSet::InitMember()
{
	m_bNotDrawLine		= FALSE;//기준선 그린다.

	m_bChkYSprice 		= FALSE;//전일가격 시가				체크 여부
	m_bChkYHprice 		= FALSE;//전일가격 고가				체크 여부
	m_bChkYLprice 		= FALSE;//전일가격 저가				체크 여부
	m_bChkYEprice 		= FALSE;//전일가격 종가				체크 여부
	m_bChkYSHL 			= FALSE;//전일가격 (시+고+저)/3		체크 여부
	
	m_bChkTSprice 		= FALSE;//당일가격 시가				체크 여부
	m_bChkTHprice 		= FALSE;//당일가격 고가				체크 여부
	m_bChkTLprice 		= FALSE;//당일가격 저가				체크 여부
	m_bChkTEprice 		= FALSE;//당일가격 종가				체크 여부
	m_bChkTHL 			= FALSE;//당일가격 (고+저)/2		체크 여부
	m_bChkTSHL 			= FALSE;//당일가격 (시+고+저)/3		체크 여부
	m_bChkTHLE 			= FALSE;//당일가격 (고+저+(종*2))/4 체크 여부
	m_bChkHighLimit		= FALSE;//상한가					체크 여부
	m_bChkLowLimit		= FALSE;//하한가					체크 여부
	
	m_bChkPivot2Rst 	= FALSE;// 피봇 2차 저항 			체크 여부
	m_bChkPivot1Rst 	= FALSE;// 피봇 1차 저항 			체크 여부
	m_bChkPivotGLine 	= FALSE;// 피봇 기준선  			체크 여부
	m_bChkPivot1GG 		= FALSE;// 피봇 1차 지지 			체크 여부
	m_bChkPivot2GG 		= FALSE;// 피봇 2차 지지 			체크 여부
	
	m_bChkDMKPreHPrice 	= FALSE;// Demark 예상고가  		체크 여부
	m_bChkDMKGLine 		= FALSE;// Demark 기준선			체크 여부
	m_bChkDMKPreLPrice 	= FALSE;// Demark 예상저가  		체크 여부
	
	m_bChk2Pyung1 		= FALSE;// 이동평균 첫번째 입력값	체크 여부
	m_bChk2Pyung2 		= FALSE;// 이동평균 두번째 입력값	체크 여부
	m_bChk2Pyung3 		= FALSE;// 이동평균 세번째 입력값	체크 여부
	m_bChk2Pyung4 		= FALSE;// 이동평균 네번째 입력값	체크 여부
	
	m_bChkPmmDanga 		= FALSE;// 평균매입 단가			체크 여부
	m_bChkPmmPDanga 	= FALSE;// 평균매입 단가 +n%		체크 여부
	m_bChkPmmMDanga 	= FALSE;// 평균매입 단가 -n%		체크 여부	
	
	m_clrYSprice 		= RGB(255,0,0);	//전일가격 시가				색 지정값
	m_clrYHprice 		= RGB(255,0,0);	//전일가격 고가				색 지정값
	m_clrYLprice 		= RGB(255,0,0);	//전일가격 저가				색 지정값
	m_clrYEprice 		= RGB(255,0,0);	//전일가격 종가				색 지정값
	m_clrYSHL 			= RGB(255,0,0);	//전일가격 (시+고+저)/3		색 지정값
	
	m_clrTSprice 		= RGB(255,0,0);	//당일가격 시가				색 지정값
	m_clrTHprice 		= RGB(255,0,0);	//당일가격 고가				색 지정값
	m_clrTLprice 		= RGB(255,0,0);	//당일가격 저가				색 지정값
	m_clrTEprice 		= RGB(255,0,0);	//당일가격 종가				색 지정값
	m_clrTHL 			= RGB(255,0,0);	//당일가격 (고+저)/2		색 지정값
	m_clrTSHL 			= RGB(255,0,0);	//당일가격 (시+고+저)/3		색 지정값
	m_clrTHLE 			= RGB(255,0,0);	//당일가격 (고+저+(종*2))/4 색 지정값
	m_clrHighLimit		= RGB(255,0,0);	//상한가					색 지정값
	m_clrLowLimit		= RGB(255,0,0);	//하한가					색 지정값
	
	m_clrPivot2Rst 		= RGB(255,0,0);	// 피봇 2차 저항			색 지정값
	m_clrPivot1Rst 		= RGB(255,0,0);	// 피봇 1차 저항			색 지정값
	m_clrPivotGLine 	= RGB(255,0,0);	// 피봇 기준선				색 지정값
	m_clrPivot1GG 		= RGB(255,0,0);	// 피봇 1차 지지			색 지정값
	m_clrPivot2GG 		= RGB(255,0,0);	// 피봇 2차 지지			색 지정값
	
	m_clrDMKPreHPrice 	= RGB(255,0,0);	// Demark 예상고가			색 지정값
	m_clrDMKGLine 		= RGB(255,0,0);	// Demark 기준선			색 지정값
	m_clrDMKPreLPrice 	= RGB(255,0,0);	// Demark 예상저가			색 지정값
	
	m_clr2Pyung1 		= RGB(255,0,0);	// 이동평균 첫번째 입력값	색 지정값
	m_clr2Pyung2 		= RGB(255,0,0);	// 이동평균 두번째 입력값	색 지정값
	m_clr2Pyung3 		= RGB(255,0,0);	// 이동평균 세번째 입력값	색 지정값
	m_clr2Pyung4 		= RGB(255,0,0);	// 이동평균 네번째 입력값	색 지정값
	
	m_clrPmmDanga 		= RGB(255,0,0);	// 평균매입 단가			색 지정값
	m_clrPmmPDanga 		= RGB(255,0,0);	// 평균매입 단가 +n%		색 지정값
	m_clrPmmMDanga 		= RGB(255,0,0);	// 평균매입 단가 -n%		색 지정값

	m_nLineYSprice		= 1;//전일가격 시가				라인 두께
	m_nLineYHprice		= 1;//전일가격 고가				라인 두께
	m_nLineYLprice		= 1;//전일가격 저가				라인 두께
	m_nLineYEprice		= 1;//전일가격 종가				라인 두께
	m_nLineYSHL			= 1;//전일가격 (시+고+저)/3		라인 두께
	
	m_nLineTSprice		= 1;//당일가격 시가				라인 두께
	m_nLineTHprice		= 1;//당일가격 고가				라인 두께
	m_nLineTLprice		= 1;//당일가격 저가				라인 두께
	m_nLineTEprice		= 1;//당일가격 종가				라인 두께
	m_nLineTHL			= 1;//당일가격 (고+저)/2		라인 두께
	m_nLineTSHL			= 1;//당일가격 (시+고+저)/3		라인 두께
	m_nLineTHLE			= 1;//당일가격 (고+저+(종*2))/4 라인 두께
	m_nLineHighLimit	= 1;//상한가					라인 두께
	m_nLineLowLimit		= 1;//하한가					라인 두께
	
	m_nLinePivot2Rst	= 1;// 피봇 2차 저항 라인 두께
	m_nLinePivot1Rst	= 1;// 피봇 1차 저항 라인 두께
	m_nLinePivotGLine	= 1;// 피봇 기준선	 라인 두께
	m_nLinePivot1GG		= 1;// 피봇 1차 지지 라인 두께
	m_nLinePivot2GG		= 1;// 피봇 2차 지지 라인 두께
	
	m_nLineDMKPreHPrice = 1;// Demark 예상고가  라인 두께
	m_nLineDMKGLine		= 1;// Demark 기준선	라인 두께
	m_nLineDMKPreLPrice = 1;// Demark 예상저가  라인 두께
	
	m_nLine2Pyung1		= 1;// 이동평균 첫번째 입력값 라인 두께
	m_nLine2Pyung2		= 1;// 이동평균 두번째 입력값 라인 두께
	m_nLine2Pyung3		= 1;// 이동평균 세번째 입력값 라인 두께
	m_nLine2Pyung4		= 1;// 이동평균 네번째 입력값 라인 두께
	
	m_nLinePmmDanga		= 1;// 평균매입 단가 라인 두께
	m_nLinePmmPDanga	= 1;// 평균매입 단가 +n% 라인 두께
	m_nLinePmmMDanga	= 1;// 평균매입 단가 -n% 라인 두께

	m_nDay2Pyung1		= 5;	//단순 이동평균 수치 1
	m_nDay2Pyung2		= 20;	//단순 이동평균 수치 2
	m_nDay2Pyung3		= 60;	//단순 이동평균 수치 3
	m_nDay2Pyung4		= 120;	//단순 이동평균 수치 4

	m_clrPmmPDanga 		= 0;	// 평균매입 단가 +n%		값
	m_clrPmmMDanga 		= 0;	// 평균매입 단가 -n%		값

	m_strAcct			= _T("");	

	m_bChkName		= TRUE;		// 정보표시 : 이름	체크 버튼
	m_bChkValue		= TRUE;		// 정보표시 : 값	체크 버튼
	m_nAlign		= 0;		// 정보표시 (왼쪽/오른쪽/양쪽)
	
}


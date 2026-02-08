// ConfigSet.h: interface for the CConfigSet class.
//
//////////////////////////////////////////////////////////////////////
#ifndef __CONFIG_SET_H
#define __CONFIG_SET_H
#endif

#if !defined(AFX_CONFIGSET_H__878DBB4B_C0D1_4B63_B675_C1BB4D6ABD42__INCLUDED_)
#define AFX_CONFIGSET_H__878DBB4B_C0D1_4B63_B675_C1BB4D6ABD42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define DEFAULT_LINE_CLR	RGB(242,159,36) //주황색

//--------------------------------------------------------
// Author      : Sang-Yun, Nam  date : 2005/01/28
// Comments    : 사용자가 설정하고 있는 정보를 가지고 있는 Global Class 
// using way   : 
// See	       : CConfigSet
//---------------------------------------------------------
class CConfigSet  
{
public :
	CConfigSet();
	virtual ~CConfigSet();

//BEGIN ===========================================================
	
public:
	void		InitMember();
	CConfigSet* GetConfigValue(){return this;}
	void		SetConfigValue(const CConfigSet* p_pConfigSet);
public:
	BOOL m_bNotDrawLine;   //기준선 그릴지 여부

	BOOL m_bChkYSprice; //전일가격 시가				체크 여부
	BOOL m_bChkYHprice; //전일가격 고가				체크 여부
	BOOL m_bChkYLprice; //전일가격 저가				체크 여부
	BOOL m_bChkYEprice; //전일가격 종가				체크 여부
	BOOL m_bChkYSHL;	//전일가격 (시+고+저)/3		체크 여부

	BOOL m_bChkTSprice; //당일가격 시가				체크 여부
	BOOL m_bChkTHprice; //당일가격 고가				체크 여부
	BOOL m_bChkTLprice; //당일가격 저가				체크 여부
	BOOL m_bChkTEprice; //당일가격 종가				체크 여부
	BOOL m_bChkTHL;		//당일가격 (고+저)/2		체크 여부
	BOOL m_bChkTSHL;	//당일가격 (시+고+저)/3		체크 여부
	BOOL m_bChkTHLE;	//당일가격 (고+저+(종*2))/4 체크 여부
	BOOL m_bChkHighLimit;	//상한가				체크 여부
	BOOL m_bChkLowLimit;	//하한가				체크 여부

	BOOL m_bChkPivot2Rst;	// 피봇 2차 저항 체크 여부 
	BOOL m_bChkPivot1Rst;	// 피봇 1차 저항 체크 여부
	BOOL m_bChkPivotGLine;	// 피봇 기준선  체크 여부
	BOOL m_bChkPivot1GG;	// 피봇 1차 지지 체크 여부
	BOOL m_bChkPivot2GG;	// 피봇 2차 지지 체크 여부

	BOOL m_bChkDMKPreHPrice;// Demark 예상고가  체크 여부
	BOOL m_bChkDMKGLine;	// Demark 기준선	체크 여부
	BOOL m_bChkDMKPreLPrice;// Demark 예상저가  체크 여부

	BOOL m_bChk2Pyung1;		// 이동평균 첫번째 입력값 체크 여부
	BOOL m_bChk2Pyung2;		// 이동평균 두번째 입력값 체크 여부
	BOOL m_bChk2Pyung3;		// 이동평균 세번째 입력값 체크 여부
	BOOL m_bChk2Pyung4;		// 이동평균 네번째 입력값 체크 여부

	BOOL m_bChkPmmDanga;	// 평균매입 단가 체크 여부
	BOOL m_bChkPmmPDanga;	// 평균매입 단가 +n% 체크 여부
	BOOL m_bChkPmmMDanga;	// 평균매입 단가 -n% 체크 여부

	COLORREF m_clrYSprice;	//전일가격 시가				색 지정값
	COLORREF m_clrYHprice;	//전일가격 고가				색 지정값
	COLORREF m_clrYLprice;	//전일가격 저가				색 지정값
	COLORREF m_clrYEprice;	//전일가격 종가				색 지정값
	COLORREF m_clrYSHL;		//전일가격 (시+고+저)/3		색 지정값

	COLORREF m_clrTSprice;	//당일가격 시가				색 지정값
	COLORREF m_clrTHprice;	//당일가격 고가				색 지정값
	COLORREF m_clrTLprice;	//당일가격 저가				색 지정값
	COLORREF m_clrTEprice;	//당일가격 종가				색 지정값
	COLORREF m_clrTHL;		//당일가격 (고+저)/2		색 지정값
	COLORREF m_clrTSHL;		//당일가격 (시+고+저)/3		색 지정값
	COLORREF m_clrTHLE;		//당일가격 (고+저+(종*2))/4 색 지정값
	COLORREF m_clrHighLimit;//상한가					색 지정값
	COLORREF m_clrLowLimit;	//하한가					색 지정값

	COLORREF m_clrPivot2Rst;	// 피봇 2차 저항 색 지정값
	COLORREF m_clrPivot1Rst;	// 피봇 1차 저항 색 지정값
	COLORREF m_clrPivotGLine;	// 피봇 기준선	 색 지정값
	COLORREF m_clrPivot1GG;		// 피봇 1차 지지 색 지정값
	COLORREF m_clrPivot2GG;		// 피봇 2차 지지 색 지정값

	COLORREF m_clrDMKPreHPrice;	// Demark 예상고가  색 지정값
	COLORREF m_clrDMKGLine;		// Demark 기준선	색 지정값
	COLORREF m_clrDMKPreLPrice;	// Demark 예상저가  색 지정값

	COLORREF m_clr2Pyung1;		// 이동평균 첫번째 입력값 색 지정값
	COLORREF m_clr2Pyung2;		// 이동평균 두번째 입력값 색 지정값
	COLORREF m_clr2Pyung3;		// 이동평균 세번째 입력값 색 지정값
	COLORREF m_clr2Pyung4;		// 이동평균 네번째 입력값 색 지정값
	
	COLORREF m_clrPmmDanga;		// 평균매입 단가 색 지정값
	COLORREF m_clrPmmPDanga;	// 평균매입 단가 +n% 색 지정값
	COLORREF m_clrPmmMDanga;	// 평균매입 단가 -n% 색 지정값

	int m_nLineYSprice;		//전일가격 시가				라인 두께
	int m_nLineYHprice;		//전일가격 고가				라인 두께
	int m_nLineYLprice;		//전일가격 저가				라인 두께
	int m_nLineYEprice;		//전일가격 종가				라인 두께
	int m_nLineYSHL;			//전일가격 (시+고+저)/3		라인 두께
	
	int m_nLineTSprice;		//당일가격 시가				라인 두께
	int m_nLineTHprice;		//당일가격 고가				라인 두께
	int m_nLineTLprice;		//당일가격 저가				라인 두께
	int m_nLineTEprice;		//당일가격 종가				라인 두께
	int m_nLineTHL;			//당일가격 (고+저)/2		라인 두께
	int m_nLineTSHL;			//당일가격 (시+고+저)/3		라인 두께
	int m_nLineTHLE;			//당일가격 (고+저+(종*2))/4 라인 두께
	int m_nLineHighLimit;		//상한가					라인 두께
	int m_nLineLowLimit;		//하한가					라인 두께
	
	int m_nLinePivot2Rst;		// 피봇 2차 저항 라인 두께
	int m_nLinePivot1Rst;		// 피봇 1차 저항 라인 두께
	int m_nLinePivotGLine;		// 피봇 기준선	 라인 두께
	int m_nLinePivot1GG;		// 피봇 1차 지지 라인 두께
	int m_nLinePivot2GG;		// 피봇 2차 지지 라인 두께
	
	int m_nLineDMKPreHPrice;	// Demark 예상고가  라인 두께
	int m_nLineDMKGLine;		// Demark 기준선	라인 두께
	int m_nLineDMKPreLPrice;	// Demark 예상저가  라인 두께
	
	int m_nLine2Pyung1;		// 이동평균 첫번째 입력값 라인 두께
	int m_nLine2Pyung2;		// 이동평균 두번째 입력값 라인 두께
	int m_nLine2Pyung3;		// 이동평균 세번째 입력값 라인 두께
	int m_nLine2Pyung4;		// 이동평균 네번째 입력값 라인 두께
	
	int m_nLinePmmDanga;		// 평균매입 단가 라인 두께
	int m_nLinePmmPDanga;		// 평균매입 단가 +n% 라인 두께
	int m_nLinePmmMDanga;		// 평균매입 단가 -n% 라인 두께

	int m_nDay2Pyung1;			// 이동평균 첫번째 입력 기간
	int m_nDay2Pyung2;			// 이동평균 두번째 입력 기간
	int m_nDay2Pyung3;			// 이동평균 세번째 입력 기간
	int m_nDay2Pyung4;			// 이동평균 네번째 입력 기간
	
	int	 m_nPmmPDanga;			// 평균매입 단가 +n%		값
	int	 m_nPmmMDanga;			// 평균매입 단가 -n%		값

	CString m_strAcct;			// 사용자가 선택한 계좌

	BOOL m_bChkName;			// 정보표시 : 이름	체크 버튼
	BOOL m_bChkValue;			// 정보표시 : 값	체크 버튼
	int	 m_nAlign;				// 정보표시 (왼쪽/오른쪽/양쪽)
	
	
//END ===========================================================

};

#endif // !defined(AFX_CONFIGSET_H__878DBB4B_C0D1_4B63_B675_C1BB4D6ABD42__INCLUDED_)

// HogaData.h: interface for the CHogaData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HOGADATA_H__E646C4BF_16A6_44DB_BCB7_4372C383376E__INCLUDED_)
#define AFX_HOGADATA_H__E646C4BF_16A6_44DB_BCB7_4372C383376E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CHogaData : public CObject  
{
public:	
	int					m_nRow;					// 그리드 열 수
	int					m_nCol;					// 그리드 행 수 
    UINT				m_nMask;

    COLORREF			m_crBkClr;				// 배경색
    COLORREF			m_crFgClr;				// 전경색

	CString				m_strText;				// Text in cell

	CHogaData();
	virtual ~CHogaData();
};



typedef struct _TGRID_1ROWINFO
{
	UINT	nRow;		// Row
	int		nPrice;		// 체결가(상<->하)
	
} TGRID_1ROWINFO, *PTGRID_1ROWINFO;



typedef struct _CHARTDATA
{
	int		nOpen;
	int		nHigh;
	int		nLow;
	int		nClose;

	int		nHighLimit;
	int		nLowLimit;

} CHARTDATA, *PCHARTDATA;



typedef struct __RsvdOdr{
	
	char code[6];			//종목코드 
	char price[9];			//현재가	
	char acct[11];			//계좌

	char odrcon[4];			//조건구분	sm0, so3, so2, so1, sc0, sb1, sb2, sb3
							//			bm0, bo3, bo2, bo1, bc0, bb1, bb2, bb3
	char odrcnt[9];			//주문수량
	char Row[5];			//그리드 Row 정보 
	char odrflag[1];		//주문 유형 u: 목표가 상향 돌파시 주문 전송 , d: 목표가 하향 돌파시 주문 전송

//	//-----------------------------------------------------------------------------
//	// Author		: Dae-Sung, Byun	Date :2005/5/30
//	// Reason		: 매수 매도 구분을 위한 플래그 추가 
//	//-----------------------------------------------------------------------------
//	// start
//	char odrgb[1];			// 주문 구분 ('s' : 매도     'b' : 매수 )
//	// end
} RsvOdr, *PRsvOdr;

#define RSVODR_BUFF_SIZE	sizeof(RsvOdr)



typedef struct __TenHogaData{
	
//	char    offerho3		[ 9];			//매도3호가(원)          
//	char    offerho2		[ 9];			//매도2호가(원)          
//	char    offerho1		[ 9];			//매도1호가(원)          
//	char    bidho1			[ 9];			//매수1호가(원) 
//	char    bidho2			[ 9];			//매수2호가(원) 
//	char    bidho3			[ 9];			//매수3호가(원) 
	
	int		nHighLimit;		// 상한가 영역
	int		nExtraUp;		// 기타매도호가 영역

	int		nOfferho10;		// 매도 10호가
	int		nOfferho9;		// 매도 9호가
	int		nOfferho8;		// 매도 8호가
	int		nOfferho7;		// 매도 7호가
	int		nOfferho6;		// 매도 6호가
	int		nOfferho5;		// 매도 5호가
	int		nOfferho4;		// 매도 4호가
	int		nOfferho3;		// 매도 3호가
	int		nOfferho2;		// 매도 2호가
	int		nOfferho1;		// 매도 1호가

	int		nBidho1;		// 매수 1호가
	int		nBidho2;		// 매수 2호가
	int		nBidho3;		// 매수	3호가
	int		nBidho4;		// 매수	4호가
	int		nBidho5;		// 매수 5호가
	int		nBidho6;		// 매수 6호가
	int		nBidho7;		// 매수 7호가
	int		nBidho8;		// 매수	8호가
	int		nBidho9;		// 매수	9호가
	int		nBidho10;		// 매수 10호가

	int		nExtraDown;		// 기타매수호가 영역
	int		nLowLimit;		// 하한가 영역


} TenHogaData, *PTenHogaData;

#define TEN_HOGADATA_SIZE	sizeof(TenHogaData)



// ***********************************************************************************
//			스탑로스, 주문예약 주문로그 남기기 
// ***********************************************************************************
typedef struct __LOGDATA{
  char   date			[8];
  char   time			[6];
  char   flag			[1]; //0 - 삭제, 1 - 신규접수
  char   jmcode			[6];
  char   scrno			[5];
  char   odrgubun		[1];
  char   odrcnt			[9];
  char   stopprice		[9];
  char   odrprice		[9];
  char   odrpricegubun	[2]; //주문가 구분..01:보통 05:시장가
  char   odrno			[5];
  char   msg			[80];
} LOGDATA, *PLOGDATA;
#define LOGDATA_SIZE sizeof(LOGDATA)

// ************************************************************************************


#endif // !defined(AFX_HOGADATA_H__E646C4BF_16A6_44DB_BCB7_4372C383376E__INCLUDED_)
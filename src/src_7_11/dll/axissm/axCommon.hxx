#ifndef _AXCOMMON_HXX
#define _AXCOMMON_HXX

//
//	status bar
//	operator area pane
//
#define	OA_OPERATOR		5001
#define	OA_MANAGER		5002
#define	OA_SEND			5003
#define	OA_PBPR			5004
#define	OA_ENGHAN		5005
#define	OA_FULLHALF		5006
#define	OA_TIMER		5007
#define	OA_TICKER		5008
#define	OA_TICKER2		5009
#define	OA_ALARM		5010

#define	OA_RTMBASE		6000
#define	OA_RTMK0		1		// KOSPI 종합지수
#define	OA_RTMK2		2		// KOSPI200 종합지수
#define	OA_RTMKX		3		// REAL KOSPI200 종합지수
#define	OA_RTMKQ		4		// KOSDAQ 지수
#define	OA_RTMQ5		5		// KOSDAQ50 지수
#define	OA_RTMQX		6		// REAL KOSDAQ50 종합지수
#define	OA_RTMFJ		7		// 선물최근물 시세(KOSPI200선물지수)
#define OA_RTMSF		8		// S&P 500선물지수(글로백스)
#define OA_RTMNF		9		// 나스탁100선물시세(글로백스)
#define	OA_RTMSH		10		// 투자자 매매현황

#define	OA_CONCLUDE		100		// 사용자별 체결


/*
#define	OA_RTMD0		3		// KOSPI 종합등락
#define	OA_RTMWD		7		// 원/$ 환율
#define	OA_RTMJM		8		// 장운영정보 MSG
#define	OA_RTMQJ		10		// KOSDAQ선물시세
#define	OA_RTMKX		15		// KOSPI200 자체지수(3초지수)
*/

//
//	realtime define symbol
//
const	int sSHK1 = 14;		// 시황 키1
const	int sSHK2 = 15;		// 시황 키2
const	int sSHDA = 16;		// 시황 데이타

const	int sCODX = 21;		// 종목코드
const	int sHANM = 22;		// 종목명
const	int sCURR = 23;		// 현재가
const	int sDIFF = 24;		// 전일대비
const	int sMDGA = 25;		// 매도호가
const	int sMSGA = 26;		// 매수호가
const	int sGVOL = 27;		// 거래량
const	int sGAMT = 28;		// 거래대금(억)
const	int sSIGA = 29;		// 시가
const	int sKOGA = 30;		// 고가
const	int sJEGA = 31;		// 저가
const	int sCVOL = 32;		// 체결거래량
const	int sUDYL = 33;		// 등락율
const	int sCTIM = 34;		// 체결시간
const	int sCCOL = 35;		// 쳐결구분

const	int sSSJS = 252;	// 상승종목수
const	int sSHJS = 251;	// 상한종목수
const	int sBHJS = 253;	// 보합종목수
const	int sHRJS = 255;	// 하락종목수
const	int sHHJS = 254;	// 하한종목수


//
//	investor item
//
#define	investJK	0x00000001	// 증권
#define	investBH	0x00000002	// 보험
#define	investTS	0x00000004	// 투신
#define	investUH	0x00000008	// 은행
#define	investCK	0x00000010	// 종금
#define	investKK	0x00000020	// 기금
#define	investKT	0x00000040	// 기타
#define	investKE	0x00000080	// 개인
#define	investEK	0x00000100	// 외국인
#define	investJU	0x00000200	// 장운용


#endif	// _AXCOMMON_HXX
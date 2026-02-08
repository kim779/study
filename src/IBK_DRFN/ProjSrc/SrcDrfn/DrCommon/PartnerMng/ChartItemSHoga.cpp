// ChartItemSHoga.cpp: implementation of the CChartItemSHoga class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PartnerMng.h"
#include "ChartItemSHoga.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*
					//	종목명,	현재가,	대비구분,대비,	등락율,	거래량,	전일거래량대비,	우선매도호가
char* szHogaField[] = { "1022", "2023", "", "2024", "2033", "2027", "", "2025",
		//	우선매수호가,외국인보유수량,외국인보유비중,호가수신시간,
			"2026",		"2339",			"2333",			"3040",
		//	매도호가1,매도호가수량1,매도직전대비수량1,	매도호가2,매도호가수량2,매도직전대비수량2,
			"3051",		"3041",		"3081",				"3052",		"3042",		"3082",
		//	매도호가3,매도호가수량3,매도직전대비수량3,	매도호가4,매도호가수량4,매도직전대비수량4,
			"3053",		"3043",		"3083",				"3054",		"3044",		"3084",
		//	매도호가5,매도호가수량5,매도직전대비수량5,	매도호가6,매도호가수량6,매도직전대비수량6,
			"3055",		"3045",		"3085",				"3056",		"3046",		"3086",
		//	매도호가7,매도호가수량7,매도직전대비수량7,	매도호가8,매도호가수량8,매도직전대비수량8,
			"3057",		"3047",		"3087",				"3058",		"3048",		"3088",
		//	매도호가9,매도호가수량9,매도직전대비수량9,	매도호가10,매도호가수량10,매도직전대비수량10,
			"3059",		"3049",		"3089",				"3060",		"3050",		"3090",
		//	매수호가1,매수호가수량1,매수직전대비수량1,	매수호가2,매수호가수량2,매수직전대비수량2,
			"3071",		"3061",		"3091",				"3072",		"3062",		"3092",
		//	매수호가3,매수호가수량3,매수직전대비수량3,	매수호가4,매수호가수량4,매수직전대비수량4,
			"3073",		"3063",		"3093",				"3074",		"3064",		"3094",
		//	매수호가5,매수호가수량5,매수직전대비수량5,	매수호가6,매수호가수량6,매수직전대비수량6,
			"3075",		"3065",		"3095",				"3076",		"3066",		"3096",
		//	매수호가7,매수호가수량7,매수직전대비수량7,	매수호가8,매수호가수량8,매수직전대비수량8,
			"3077",		"3067",		"3097",				"3078",		"3068",		"3098",
		//	매수호가9,매수호가수량9,매수직전대비수량9,	매수호가10,매수호가수량10,매수직전대비수량10,
			"3079",		"3069",		"3099",				"3080",		"3070",		"3100",
		//	매도호가총수량,매도호가총수량직전대비,매수호가총수량,매수호가총수량직전대비,예상체결가격,예상체결수량
			"3101",		"3104",		"3106",				"3109",		"3111",		"3112",
		//	예상체결가전일종가대비구분,예상체결가전일종가대비,예상등락율,시간외매도잔량,시간외매수잔량,장구분
			"",		"3115",		"3116",				"3102",		"3107",		"1371",
		//	전일종가, 기준가, 상한가, 하한가, 시가, 고가, 저가, 거래비용,피봇,기준선,1차저항,2차저항
			"2320",	"1336",	"2302",	"2303",	"2029",	"2030",	"2031",	"",	"2423",	"",	"2422",	"2421",
		//	1차지지, 2차지지, D저항, D지지, 액면가, 대용가, 시가총액, 자본금, 상장주식수,PER,PBR,250일최고가
			"2424",	"2425",	"2426",	"2427",	"1337",	"1338",	"2305",	"1333",	"1334",	"5355",	"5352",	"5311",
		//	250일최고가등락율, 250일최고가일자, 250일최저가, 250일최저가등락율, 250일최저가일자
			"5341",	"5321",	"5312",	"5342",	"5322",
		//	거래대금, 거래량회전율, 전일거래량
			"2028",	"2350",	"2321", NULL };
*/
char* szHogaField[] = {
	//	기준가,	예상체결가격,	예상체결수량,	종목명,	현재가,	등락부호,등락폭,	대비율,	누적거래량,	누적거래대금,
		"1336",	"3111",			"3112",			"1022", "2023", "",		"2024", "2033",	"2027",		"2028",
	//	매도호가1,매도수량1,매도호가2,매도수량2,매도호가3,매도수량3,매도호가4,매도수량4,매도호가5,매도수량5,
		"3051",		"3041",	"3052",		"3042",	"3053",		"3043",	"3054",		"3044","3055",		"3045",
	//	매수호가1,매수수량1,매수호가2,매수수량2,매수호가3,매수수량3,매수호가4,매수수량4,매수호가5,매수수량5,
		"3071",		"3061",	"3072",		"3062","3073",		"3063",	"3074",		"3064",	"3075",		"3065",
	//	매도총수량,매수총수량,시간외매도잔량,시간외매수잔량,상한가, 하한가,	예상등락율
		"3101",		"3106",		"3102",		"3107",			"2302",	"2303",	"3116",
	//	고가,	저가,	전일거래량,예상체결가전일종가대비구분
		"2030",	"2031",	"2321",		"", NULL};

CChartItemSHoga::CChartItemSHoga()
{
	m_bSet = FALSE;

	int nIndex = 0;
	while (szHogaField[nIndex] != NULL)
		nIndex++;

	m_nCnt = nIndex;
}

CChartItemSHoga::~CChartItemSHoga()
{

}

void CChartItemSHoga::SetInData(LPk01230In1 pData)
{
	memcpy(&m_InData, pData, sizeof(k01230In1));
	m_bSet = TRUE;
}

// MUltiChart에서의 조회 구조체를 받아서 FID 형태로 변환
// 구성 : 차트조회Grid Inbound + 시세 FID Inbound + GridHeader + Grid Outbound
// winix 플랫폼에서 배열형태로 내려오는 데이터를 Grid 라고 부른다.
CString CChartItemSHoga::Convert()
{
	if(!m_bSet) return "";

	int i;
	CString strData, strCode;

	strCode = CString(m_InData.shcode, sizeof(m_InData.shcode));

	//////////////////////////////////////////////////////////////////////////
	// Inbound 데이터 작성
	//////////////////////////////////////////////////////////////////////////
	// 차트 데이터
	strData += "1301"; // 종목코드 Input 심볼
	strData += sDS;

	strData += strCode;
	strData += sCELL;

	//////////////////////////////////////////////////////////////////////////
	// 종목 시세 OutBound
	for (i=0; i<m_nCnt; i++)
	{
		strData += szHogaField[i];
		strData += sCELL;
	}

	return strData;
}

char* CChartItemSHoga::Convert2Struct(LPBYTE pData, int nLen, int nKey, int &nDataLen)
{
	k01230Out1 stOut;
	CString strNode, strSrc;
	strSrc.Format(_T("%.*s"), nLen, (char*)pData);

	memset(&stOut, 0x20, sizeof(stOut));
	strNode = CDataConverter::Parser(strSrc, sCELL);	// 기준가
	strNode.Remove(_T('-'));
	memcpy(stOut.recprice, strNode, min(strNode.GetLength(), sizeof(stOut.recprice)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 예상체결가격
	strNode.Remove(_T('-'));
	memcpy(stOut.yeprice, strNode, min(strNode.GetLength(), sizeof(stOut.yeprice)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 예상체결수량
	strNode.Remove(_T('-'));
	memcpy(stOut.yevolume, strNode, min(strNode.GetLength(), sizeof(stOut.yevolume)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 종목명
	memcpy(stOut.hname, strNode, min(strNode.GetLength(), sizeof(stOut.hname)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 현재가
	strNode.Remove(_T('-'));
	memcpy(stOut.price, strNode, min(strNode.GetLength(), sizeof(stOut.price)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 등락부호
	memcpy(stOut.sign, strNode, min(strNode.GetLength(), sizeof(stOut.sign)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 등락폭
	memcpy(stOut.change, strNode, min(strNode.GetLength(), sizeof(stOut.change)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 대비율
	memcpy(stOut.rate, strNode, min(strNode.GetLength(), sizeof(stOut.rate)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 누적거래량
	strNode.Remove(_T('-'));
	memcpy(stOut.volume, strNode, min(strNode.GetLength(), sizeof(stOut.volume)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 누적거래대금
	strNode.Remove(_T('-'));
	memcpy(stOut.trade_cost, strNode, min(strNode.GetLength(), sizeof(stOut.trade_cost)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 매도호가1
	strNode.Remove(_T('-'));
	memcpy(stOut.offerho1, strNode, min(strNode.GetLength(), sizeof(stOut.offerho1)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 매도잔량1
	strNode.Remove(_T('-'));
	memcpy(stOut.offerrem1, strNode, min(strNode.GetLength(), sizeof(stOut.offerrem1)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 매도호가2
	strNode.Remove(_T('-'));
	memcpy(stOut.offerho2, strNode, min(strNode.GetLength(), sizeof(stOut.offerho2)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 매도잔량2
	strNode.Remove(_T('-'));
	memcpy(stOut.offerrem2, strNode, min(strNode.GetLength(), sizeof(stOut.offerrem2)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 매도호가3
	strNode.Remove(_T('-'));
	memcpy(stOut.offerho3, strNode, min(strNode.GetLength(), sizeof(stOut.offerho3)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 매도잔량3
	strNode.Remove(_T('-'));
	memcpy(stOut.offerrem3, strNode, min(strNode.GetLength(), sizeof(stOut.offerrem3)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 매도호가4
	strNode.Remove(_T('-'));
	memcpy(stOut.offerho4, strNode, min(strNode.GetLength(), sizeof(stOut.offerho4)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 매도잔량4
	strNode.Remove(_T('-'));
	memcpy(stOut.offerrem4, strNode, min(strNode.GetLength(), sizeof(stOut.offerrem4)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 매도호가5
	strNode.Remove(_T('-'));
	memcpy(stOut.offerho5, strNode, min(strNode.GetLength(), sizeof(stOut.offerho5)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 매도잔량5
	strNode.Remove(_T('-'));
	memcpy(stOut.offerrem5, strNode, min(strNode.GetLength(), sizeof(stOut.offerrem5)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 매수호가1
	strNode.Remove(_T('-'));
	memcpy(stOut.bidho1, strNode, min(strNode.GetLength(), sizeof(stOut.bidho1)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 매수잔량1
	strNode.Remove(_T('-'));
	memcpy(stOut.bidrem1, strNode, min(strNode.GetLength(), sizeof(stOut.bidrem1)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 매수호가2
	strNode.Remove(_T('-'));
	memcpy(stOut.bidho2, strNode, min(strNode.GetLength(), sizeof(stOut.bidho2)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 매수잔량2
	strNode.Remove(_T('-'));
	memcpy(stOut.bidrem2, strNode, min(strNode.GetLength(), sizeof(stOut.bidrem2)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 매수호가3
	strNode.Remove(_T('-'));
	memcpy(stOut.bidho3, strNode, min(strNode.GetLength(), sizeof(stOut.bidho3)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 매수잔량3
	strNode.Remove(_T('-'));
	memcpy(stOut.bidrem3, strNode, min(strNode.GetLength(), sizeof(stOut.bidrem3)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 매수호가4
	strNode.Remove(_T('-'));
	memcpy(stOut.bidho4, strNode, min(strNode.GetLength(), sizeof(stOut.bidho4)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 매수잔량4
	strNode.Remove(_T('-'));
	memcpy(stOut.bidrem4, strNode, min(strNode.GetLength(), sizeof(stOut.bidrem4)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 매수호가5
	strNode.Remove(_T('-'));
	memcpy(stOut.bidho5, strNode, min(strNode.GetLength(), sizeof(stOut.bidho5)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 매수잔량5
	strNode.Remove(_T('-'));
	memcpy(stOut.bidrem5, strNode, min(strNode.GetLength(), sizeof(stOut.bidrem5)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 총매도잔량
	strNode.Remove(_T('-'));
	memcpy(stOut.totofferrem, strNode, min(strNode.GetLength(), sizeof(stOut.totofferrem)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 총매수잔량
	strNode.Remove(_T('-'));
	memcpy(stOut.totbidrem, strNode, min(strNode.GetLength(), sizeof(stOut.totbidrem)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 시간외매도잔량
	strNode.Remove(_T('-'));
	memcpy(stOut.tmofferrem, strNode, min(strNode.GetLength(), sizeof(stOut.tmofferrem)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 시간외 매수잔량
	strNode.Remove(_T('-'));
	memcpy(stOut.tmbidrem, strNode, min(strNode.GetLength(), sizeof(stOut.tmbidrem)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 상한가
	strNode.Remove(_T('-'));
	memcpy(stOut.uplmtprice, strNode, min(strNode.GetLength(), sizeof(stOut.uplmtprice)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 하한가
	strNode.Remove(_T('-'));
	memcpy(stOut.dnlmtprice, strNode, min(strNode.GetLength(), sizeof(stOut.dnlmtprice)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 예상등락율
	memcpy(stOut.yerate, strNode, min(strNode.GetLength(), sizeof(stOut.yerate)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 고가
	strNode.Remove(_T('-'));
	memcpy(stOut.high, strNode, min(strNode.GetLength(), sizeof(stOut.high)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 저가
	strNode.Remove(_T('-'));
	memcpy(stOut.low, strNode, min(strNode.GetLength(), sizeof(stOut.low)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 전일거래량
	strNode.Remove(_T('-'));
	memcpy(stOut.volrate, strNode, min(strNode.GetLength(), sizeof(stOut.volrate)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 예상체결가전일종가대비구분
	memcpy(stOut.jnilysign, strNode, min(strNode.GetLength(), sizeof(stOut.jnilysign)));

	int nSize = sizeof(k01230Out1);

	char* pDataBuf = NULL;
	pDataBuf = new char[nSize+1];
	memset(pDataBuf, 0x00, nSize+1);
	memcpy(pDataBuf, &stOut, sizeof(stOut));

	return pDataBuf;
}

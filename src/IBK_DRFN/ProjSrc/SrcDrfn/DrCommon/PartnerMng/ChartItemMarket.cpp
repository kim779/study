// ChartItemMarket.cpp: implementation of the CChartItemMarket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PartnerMng.h"
#include "ChartItemMarket.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifdef __MULTI_LANG__
	char *szSendTypeTitle[] = {"_OrganPureBuyAccumulation_", "_OrganPureBuy_", "_SuspenseContract_", "_TotalAmount_", "_ForeignHolding_",
					"_ForeignHoldingRate_", "_ForeignPureBuy_", "_VolumeAmount_", "_KspPureBuy_Person_", "_KdqPureBuy_Person_;",
					"_K200PureBuy_Person_", "_FutPureBuy_Person_", "_COptPureBuy_Person_", "_POptPureBuy_Person_",
					"_OrganFinancePureBuy_", "_OrganInsurePureBuy_", "_OrganInvestmentPureBuy_", "_OrganBankPureBuy_", "_OrganEtcFinancePureBuy_",
					"_OrganPensionFundPureBuy_", "_OrganEtcCorpPureBuy_", "_OrganPrivateEquityPureBuy_", "_OrganCountryPureBuy_", "_OrganFinancePureBuyAccum_",
					"_OrganInsurePureBuyAccum_", "_OrganInvestmentPureBuyAccum_", "_OrganBankPureBuyAccum_", "_OrganEtcFinancePureBuyAccum_", 
					"_OrganPensionFundPureBuyAccum_", "_OrganEtcCorpPureBuyAccum_", "_OrganPrivateEquityPureBuyAccum_", "_OrganCountryPureBuyAccum_", "_ADL_","_ADR_",NULL};
#else
	char *szSendTypeTitle[] = {"기관순매수누적", "기관순매수량", "미결제약정", "시가총액", "외국인보유량",
					"외국인보유율", "외국인순매수", "거래대금", "거래소순매수_개인", "코스닥순매수_개인",
					"K200순매수_개인", "선물순매수_개인", "콜옵션순매수_개인", "풋옵션순매수_개인",
					"금융투자순매수", "보험순매수", "투신순매수", "은행순매수", "기타금융순매수", "연기금순매수",
					"기타법인순매수", "사모펀드순매수", "국가순매수", "금융투자누적순매수",
					"보험누적순매수", "투신누적순매수", "은행누적순매수", "기타금융누적순매수", "연기금누적순매수",
					"기타법인누적순매수", "사모펀드누적순매수", "국가누적순매수","ADL","등락주비율", NULL};
#endif

//char *szSendTypeFID[] = {"", "기관순매수량", "미결제약정", "시가총액", "외국인보유량",
//					"외국인보유율", "외국인순매수", "거래대금", "거래소순매수_개인", "코스닥순매수_개인",
//					"K200순매수_개인", "선물순매수_개인", "콜옵션순매수_개인", "풋옵션순매수_개인", NULL};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChartItemMarket::CChartItemMarket()
{
	m_bSet = FALSE;
}

CChartItemMarket::~CChartItemMarket()
{

}

void CChartItemMarket::SetInData(KB_d1015_InRec1* pData)
{
	memcpy(&m_InData, pData, sizeof(KB_d1015_InRec1));

	m_bSet = TRUE;
}

int CChartItemMarket::GetSendDataType()
{
	CString strTemp;
	strTemp = CString(m_InData.dataNm, sizeof(m_InData.dataNm));

#ifdef __MULTI_LANG__
	if (strTemp.Find(_T("_OrganPureBuyAccumulation_")) >= 0)	return TRKEY_STRMARKET_0;
	else if (strTemp.Find(_T("_OrganPureBuy_")) >= 0)			return TRKEY_STRMARKET_1;
	else if (strTemp.Find(_T("_SuspenseContract_")) >= 0)		return TRKEY_STRMARKET_2;
	else if (strTemp.Find(_T("_TotalAmount_")) >= 0)			return TRKEY_STRMARKET_3;
	else if (strTemp.Find(_T("_Foreign Holding_")) >= 0)			return TRKEY_STRMARKET_4;
	else if (strTemp.Find(_T("_Foreign Holding Rate_")) >= 0)		return TRKEY_STRMARKET_5;
	else if (strTemp.Find(_T("_Foreign Pure Buy_")) >= 0)			return TRKEY_STRMARKET_6;
	else if (strTemp.Find(_T("_ForeignPureBuyAccumulation_")) >= 0)			return TRKEY_STRMARKET_7;
	else if (strTemp.Find(_T("_Person PureBuy Accumulation_")) >= 0)		return TRKEY_STRMARKET_8;
	else if (strTemp.Find(_T("_Person PureBuy_")) >= 0)		return TRKEY_STRMARKET_9;
	//	투자자별 : IBK에선 안씀 
//	else if (strTemp.Find(_T("_KspPureBuy_Person_")) >= 0)		return TRKEY_STRMARKET_8;
//	else if (strTemp.Find(_T("_KdqPureBuy_Person_")) >= 0)		return TRKEY_STRMARKET_9;
//	else if (strTemp.Find(_T("_K200PureBuy_Person_")) >= 0)		return TRKEY_STRMARKET_10;
//	else if (strTemp.Find(_T("_FutPureBuy_Person_")) >= 0)		return TRKEY_STRMARKET_11;
//	else if (strTemp.Find(_T("_COptPureBuy_Person_")) >= 0)		return TRKEY_STRMARKET_12;
//	else if (strTemp.Find(_T("_POptPureBuy_Person_")) >= 0)		return TRKEY_STRMARKET_13;
	// 상세기관별
	else if (strTemp.Find(_T("_OrganFinancePureBuy_")) >= 0)			return TRKEY_STRMARKET_10;
	else if (strTemp.Find(_T("_OrganInsurePureBuy_")) >= 0)				return TRKEY_STRMARKET_11;
	else if (strTemp.Find(_T("_OrganInvestmentPureBuy_")) >= 0)			return TRKEY_STRMARKET_12;
	else if (strTemp.Find(_T("_OrganBankPureBuy_")) >= 0)				return TRKEY_STRMARKET_13;
	else if (strTemp.Find(_T("_OrganEtcFinancePureBuy_")) >= 0)			return TRKEY_STRMARKET_14;
	else if (strTemp.Find(_T("_OrganPensionFundPureBuy_")) >= 0)		return TRKEY_STRMARKET_15;
	else if (strTemp.Find(_T("_OrganEtcCorpPureBuy_")) >= 0)			return TRKEY_STRMARKET_16;
	else if (strTemp.Find(_T("_OrganPrivateEquityPureBuy_")) >= 0)		return TRKEY_STRMARKET_17;
	else if (strTemp.Find(_T("_OrganCountryPureBuy_")) >= 0)			return TRKEY_STRMARKET_18;
	else if (strTemp.Find(_T("_OrganFinancePureBuyAccum_")) >= 0)		return TRKEY_STRMARKET_19;
	else if (strTemp.Find(_T("_OrganInsurePureBuyAccum_")) >= 0)		return TRKEY_STRMARKET_20;
	else if (strTemp.Find(_T("_OrganInvestmentPureBuyAccum_")) >= 0)	return TRKEY_STRMARKET_21;
	else if (strTemp.Find(_T("_OrganBankPureBuyAccum_")) >= 0)			return TRKEY_STRMARKET_22;
	else if (strTemp.Find(_T("_OrganEtcFinancePureBuyAccum_")) >= 0)	return TRKEY_STRMARKET_23;
	else if (strTemp.Find(_T("_OrganPensionFundPureBuyAccum_")) >= 0)	return TRKEY_STRMARKET_24;
	else if (strTemp.Find(_T("_OrganEtcCorpPureBuyAccum_")) >= 0)		return TRKEY_STRMARKET_25;
	else if (strTemp.Find(_T("_OrganPrivateEquityPureBuyAccum_")) >= 0)	return TRKEY_STRMARKET_26;
	else if (strTemp.Find(_T("_OrganCountryPureBuyAccum_")) >= 0)		return TRKEY_STRMARKET_27;
	else if (strTemp.Find(_T("_ADL_")) >= 0)		return TRKEY_STRUPMARKET_0;
	else if (strTemp.Find(_T("_ADR_")) >= 0)		return TRKEY_STRUPMARKET_1;
	else if (strTemp.Find(_T("_CreditBalanceRate_")) >= 0)				return TRKEY_STRMARKET_28;
	else if (strTemp.Find(_T("_CreditGivingRate_")) >= 0)				return TRKEY_STRMARKET_29;
	else if (strTemp.Find(_T("_CreditBalanceQty_")) >= 0)				return TRKEY_STRMARKET_30;
	else if (strTemp.Find(_T("_CreditBalanceVariation_")) >= 0)			return TRKEY_STRMARKET_31;
	else if (strTemp.Find(_T("_Foreign+OrganPureBuy_")) >= 0)				return TRKEY_STRMARKET_32;
	else if (strTemp.Find(_T("_Foreign+OrganPureBuyAccumulation_")) >= 0)	return TRKEY_STRMARKET_33;
	
#else
	if (strTemp.Find(_T("기관순매수누적")) >= 0)		return TRKEY_STRMARKET_0;
	else if (strTemp.Find(_T("기관순매수량")) >= 0)		return TRKEY_STRMARKET_1;
	else if (strTemp.Find(_T("미결제약정")) >= 0)		return TRKEY_STRMARKET_2;
	else if (strTemp.Find(_T("시가총액")) >= 0)			return TRKEY_STRMARKET_3;
	else if (strTemp.Find(_T("외국인보유량")) >= 0)		return TRKEY_STRMARKET_4;
	else if (strTemp.Find(_T("외국인보유율")) >= 0)		return TRKEY_STRMARKET_5;
	else if (strTemp.Find(_T("외국인순매수")) >= 0)		return TRKEY_STRMARKET_6;
	else if (strTemp.Find(_T("_ForeignPureBuyAccumulation_")) >= 0)			return TRKEY_STRMARKET_7;
	else if (strTemp.Find(_T("_Person PureBuy Accumulation_")) >= 0)		return TRKEY_STRMARKET_8;
	else if (strTemp.Find(_T("_Person PureBuy_")) >= 0)		return TRKEY_STRMARKET_9;
	//	투자자별 IBK에서 안씀 
//	else if (strTemp.Find(_T("거래소")) >= 0)			return TRKEY_STRMARKET_8;
//	else if (strTemp.Find(_T("코스닥")) >= 0)			return TRKEY_STRMARKET_9;
//	else if (strTemp.Find(_T("K200")) >= 0)				return TRKEY_STRMARKET_10;
//	else if (strTemp.Find(_T("선물")) >= 0)				return TRKEY_STRMARKET_11;
//	else if (strTemp.Find(_T("콜옵션")) >= 0)			return TRKEY_STRMARKET_12;
//	else if (strTemp.Find(_T("풋옵션")) >= 0)			return TRKEY_STRMARKET_13;
	// 상세기관별
	else if (strTemp.Find(_T("금융투자순매수")) >= 0)		return TRKEY_STRMARKET_10;
	else if (strTemp.Find(_T("보험순매수")) >= 0)			return TRKEY_STRMARKET_11;
	else if (strTemp.Find(_T("투신순매수")) >= 0)			return TRKEY_STRMARKET_12;
	else if (strTemp.Find(_T("은행순매수")) >= 0)			return TRKEY_STRMARKET_13;
	else if (strTemp.Find(_T("기타금융순매수")) >= 0)		return TRKEY_STRMARKET_14;
	else if (strTemp.Find(_T("연기금순매수")) >= 0)			return TRKEY_STRMARKET_15;
	else if (strTemp.Find(_T("기타법인순매수")) >= 0)		return TRKEY_STRMARKET_16;
	else if (strTemp.Find(_T("사모펀드순매수")) >= 0)		return TRKEY_STRMARKET_17;
	else if (strTemp.Find(_T("국가순매수")) >= 0)			return TRKEY_STRMARKET_18;
	else if (strTemp.Find(_T("금융투자누적순매수")) >= 0)	return TRKEY_STRMARKET_19;
	else if (strTemp.Find(_T("보험누적순매수")) >= 0)		return TRKEY_STRMARKET_20;
	else if (strTemp.Find(_T("투신누적순매수")) >= 0)		return TRKEY_STRMARKET_21;
	else if (strTemp.Find(_T("은행누적순매수")) >= 0)		return TRKEY_STRMARKET_22;
	else if (strTemp.Find(_T("기타금융누적순매수")) >= 0)	return TRKEY_STRMARKET_23;
	else if (strTemp.Find(_T("연기금누적순매수")) >= 0)		return TRKEY_STRMARKET_24;
	else if (strTemp.Find(_T("기타법인누적순매수")) >= 0)	return TRKEY_STRMARKET_25;
	else if (strTemp.Find(_T("사모펀드누적순매수")) >= 0)	return TRKEY_STRMARKET_26;
	else if (strTemp.Find(_T("국가누적순매수")) >= 0)		return TRKEY_STRMARKET_27;
	else if (strTemp.Find(_T("ADL")) >= 0)						return TRKEY_STRUPMARKET_0;
	else if (strTemp.Find(_T("등락주비율")) >= 0)				return TRKEY_STRUPMARKET_1;
	else if (strTemp.Find(_T("신용잔고율")) >= 0)				return TRKEY_STRMARKET_28;
	else if (strTemp.Find(_T("신용공여율")) >= 0)				return TRKEY_STRMARKET_29;
	else if (strTemp.Find(_T("신용잔고수량")) >= 0)				return TRKEY_STRMARKET_30;
	else if (strTemp.Find(_T("신용잔고증감")) >= 0)				return TRKEY_STRMARKET_31;
	else if (strTemp.Find(_T("외국인+기관순매수")) >= 0)		return TRKEY_STRMARKET_32;
	else if (strTemp.Find(_T("외국인+기관순매수누적")) >= 0)	return TRKEY_STRMARKET_33;
#endif
	return -1;
}

CString CChartItemMarket::GetSendDataType(int nType)
{
	CString strTemp;
	strTemp.Format(_T("%s"), szSendTypeTitle[nType-TRKEY_STRMARKET_0]);

	return strTemp;
}


// MUltiChart에서의 조회 구조체를 받아서 FID 형태로 변환
// 구성 : 차트조회Grid Inbound + 시세 FID Inbound + GridHeader + Grid Outbound
// winix 플랫폼에서 배열형태로 내려오는 데이터를 Grid 라고 부른다.
CString CChartItemMarket::Convert()
{
	if (!m_bSet) return "";

	CString strTemp, strGrid;
	int nCount;

	strTemp = CString(m_InData.num, sizeof(m_InData.num));
	nCount = atoi(strTemp);

	//////////////////////////////////////////////////////////////////////////
	// 그리드 헤더
	GRID_I grid_i;
	memset(&grid_i, 0x20, sizeof(grid_i));

	char szTemp[5];
	grid_i.gdir[0] = _T('1');	//	top.
	if (m_InData.nxtGb[0] == '1')	grid_i.ikey[0] = _T('2');	//	Page Down
	else							grid_i.ikey[0] = _T('0');	//	일반(Next조회).

	sprintf(szTemp, _T("%04d"), nCount);	// 조회 데이터 수
	memcpy(grid_i.nrow, szTemp, sizeof(grid_i.nrow));
	memcpy(grid_i.page, m_InData.nxtKey, 4);
	memcpy(grid_i.save, m_InData.nxtKey+4, sizeof(grid_i.save));

	grid_i.sdir[0] = _T('1');
	grid_i.vflg[0] = _T('1');
	memcpy(grid_i.vrow, _T("999"), sizeof(grid_i.vrow));

	char szGrid_i[128];
	memset(szGrid_i, 0x00, sizeof(szGrid_i));
	memcpy(szGrid_i, (char*)&grid_i, sizeof(grid_i));

	strGrid.Format(_T("%s"), szGrid_i);
	//////////////////////////////////////////////////////////////////////////

	int nDataType = GetSendDataType();

	if (nDataType < 0)
		return _T("");

	return fnStructType1(strGrid, nDataType);
/*
	if (nDataType < TRKEY_STRMARKET_10)//KHD : 시장지표 
		return fnStructType1(strGrid, nDataType);
	else
		return fnStructType2(strGrid, nDataType);
*/
}

CString CChartItemMarket::fnStructType1(CString strGrid, int nType1)
{
	if(!m_bSet) 
		return "";
	CString szDS, szFS, szCOL;
	szDS = sDS;	szFS = sCELL;	szCOL = sCOL;
	CString strData, strCode, strTemp, strDate, strNow, strUnit, strTime, strImgRgb;
	CString stNextKey, stNextData;
	int  nCount, nUnit, nType, nFlag, nMarketType;
	double dUSDollar=1;
	
	strCode	= CDataConverter::Char2String(m_InData.shcode, sizeof(m_InData.shcode));
	strDate	= CDataConverter::Char2String(m_InData.edate, sizeof(m_InData.edate));
//	strNow	= CDataConverter::Char2String(m_InData.nowData, sizeof(m_InData.nowData));
//	strTime	= CDataConverter::Char2String(m_InData.Itime, sizeof(m_InData.Itime));
	nCount	= CDataConverter::Char2Int(m_InData.num, sizeof(m_InData.num));
	nUnit	= CDataConverter::Char2Int(m_InData.unit, sizeof(m_InData.unit));
	nType	= CDataConverter::Char2Int(m_InData.button, sizeof(m_InData.button));	// 틱(0)/분(1)/일(2)/주(3)/월(4)
//	strImgRgb = CDataConverter::Char2String(m_InData.ImgRgb, sizeof(m_InData.ImgRgb));
	nFlag     = CDataConverter::Char2Int(m_InData.nxtGb, sizeof(m_InData.nxtGb));	// 0:
	nMarketType  = CDataConverter::Char2Int(m_InData.type, sizeof(m_InData.type));	// 시장구분

	strUnit.Format(_T("%d"), nUnit);

	//////////////////////////////////////////////////////////////////////////
	//	Inbound Data
	
	//	종목코드 Input 심볼
	if (nMarketType == 0)
	{
		_dataH	Datah;
		FillMemory(&Datah, sz_DATAH, 0x20);
		strData += "1301";	strData += sDS;	strData += strCode;	strData += sCELL;//KHD sFS;
		CString stRowData;
		
		Datah.dindex = GI_DAY;
		nUnit = 1;
		stRowData = _T("?5500");
		
		//NEXTKEY : KHD
		if (nFlag == 0)
		{
			stNextData = "99999999999999";
			memcpy(m_InData.nxtKey, (LPCSTR)(LPCTSTR)stNextData,stNextData.GetLength());
		}
		
		stNextKey = "5324";
		stNextData.Format("%8.8s",m_InData.nxtKey );
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // 넥스트 키값 : KHD 
		
		//////////////////////////////////////////////////////////////////////////
		//	OutBound Data
		strData += "1301";
		strData += sCELL;//KHD sFS;
		
		strData += stNextKey;
		strData += sCELL;
		
		// Row Data 구성
		strData += stRowData;
		strData += sDS;
		
		CString temp;
		//조회데이터 수 
		int GrpHLen = 0;
		temp.Format("%06d", nCount);
		CopyMemory(Datah.count, temp, temp.GetLength());
		Datah.dkey = 0 + 0x30;
		
		CopyMemory(Datah.pday, strDate, strDate.GetLength());
		
		temp.Format("%0*d", sizeof(Datah.lgap), nUnit);// N분봉 
		CopyMemory(Datah.lgap, temp, temp.GetLength() );
		
		char szData_i[sz_DATAH];
		memset(szData_i, 0x20, sz_DATAH);//sizeof(Datah));
		CopyMemory(szData_i, (char*)&Datah, sz_DATAH);//sizeof(Datah));
		szData_i[sz_DATAH-1] = 0x00; // KHD : 맨끝에 0x00문자를 집어 넣는다.
		strData += szData_i;
	}
	else if (nMarketType == 1)
	{
		_dataH	Datah;
		FillMemory(&Datah, sz_DATAH, 0x20);
		strData += "21301";	strData += sDS;	strData += strCode;	strData += sCELL;//KHD sFS;
		CString stRowData;
		
		Datah.dindex = GI_DAY;
		nUnit = 1;
		stRowData = _T("?25500");
		
		//NEXTKEY : KHD
		if (nFlag == 0)
		{
			stNextData = "99999999999999";
			memcpy(m_InData.nxtKey, (LPCSTR)(LPCTSTR)stNextData,stNextData.GetLength());
		}
		
		stNextKey = "25324";
		stNextData.Format("%8.8s",m_InData.nxtKey );
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // 넥스트 키값 : KHD 
		
		//////////////////////////////////////////////////////////////////////////
		//	OutBound Data
		strData += "21301";
		strData += sCELL;//KHD sFS;
		
		strData += stNextKey;
		strData += sCELL;
		
		// Row Data 구성
		strData += stRowData;
		strData += sDS;
		
		CString temp;
		//조회데이터 수 
		int GrpHLen = 0;
		temp.Format("%06d", nCount);
		CopyMemory(Datah.count, temp, temp.GetLength());
		Datah.dkey = 0 + 0x30;
		
		CopyMemory(Datah.pday, strDate, strDate.GetLength());
		
		temp.Format("%0*d", sizeof(Datah.lgap), nUnit);// N분봉 
		CopyMemory(Datah.lgap, temp, temp.GetLength() );
		
		char szData_i[sz_DATAH];
		memset(szData_i, 0x20, sz_DATAH);//sizeof(Datah));
		CopyMemory(szData_i, (char*)&Datah, sz_DATAH);//sizeof(Datah));
		szData_i[sz_DATAH-1] = 0x00; // KHD : 맨끝에 0x00문자를 집어 넣는다.
		strData += szData_i;
	}

	switch (nType1)
	{
	case TRKEY_STRMARKET_0:		//	기관누적순매수 : KHD
		strData += "5302";		strData += sCOL;
		strData += "5512";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_1:		//	기관순매수 : KHD
		strData += "5302";		strData += sCOL;
		strData += "5412";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_2:		//	미결제약정 : KHD : 데이터 제공되야함 
		strData += "5302";		strData += sCOL;
		strData += "35201";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_3:		//	시가총액 : KHD : 안씀 
		strData += "5302";		strData += sCOL;
		strData += "5363";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_4:		//	외국인보유량 : KHD
		strData += "5302";		strData += sCOL;
		strData += "5363";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_5:		//	외국인보유율
		strData += "5302";		strData += sCOL;
		strData += "5373";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_6:		//	외국인순매수
		strData += "5302";		strData += sCOL;
		strData += "5411";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_7:		//	외국인누적순매수
		strData += "5302";		strData += sCOL;
		strData += "5511";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_8:		//	개인누적순매수
		strData += "5302";		strData += sCOL;
		strData += "5508";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_9:		//	개인순매수
		strData += "5302";		strData += sCOL;
		strData += "5408";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_10:	//	금융투자순매수
		strData += "5302";		strData += sCOL;
		strData += "5401";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_11:	//	보험순매수
		strData += "5302";		strData += sCOL;
		strData += "5402";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_12:	//	투신순매수
		strData += "5302";		strData += sCOL;
		strData += "5403";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_13:	//	은행순매수
		strData += "5302";		strData += sCOL;
		strData += "5404";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_14:	//	기타금융순매수
		strData += "5302";		strData += sCOL;
		strData += "5405";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_15:	//	연기금순매수
		strData += "5302";		strData += sCOL;
		strData += "5406";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_16:	//	기타법인순매수
		strData += "5302";		strData += sCOL;
		strData += "5407";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_17:	//	사모펀드순매수
		strData += "5302";		strData += sCOL;
		strData += "5409";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_18:	//	국가순매수
		strData += "5302";		strData += sCOL;
		strData += "5413";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_19:	//	금융투자누적순매수
		strData += "5302";		strData += sCOL;
		strData += "5501";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_20:	//	보험누적순매수
		strData += "5302";		strData += sCOL;
		strData += "5502";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_21:	//	투신누적순매수
		strData += "5302";		strData += sCOL;
		strData += "5503";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_22:	//	은행누적순매수 
		strData += "5302";		strData += sCOL;
		strData += "5504";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_23:	//	기타금융누적순매수  
		strData += "5302";		strData += sCOL;
		strData += "5505";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_24:	//	연기금누적순매수   
		strData += "5302";		strData += sCOL;
		strData += "5506";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_25:	//	기타법인누적순매수
		strData += "5302";		strData += sCOL;
		strData += "5507";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_26:	//	사모펀드누적순매수
		strData += "5302";		strData += sCOL;
		strData += "5509";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_27:	//	국가누적순매수
		strData += "5302";		strData += sCOL;
		strData += "5513";		strData += sCOL;
		break;
	case TRKEY_STRUPMARKET_0:	//	ADL
		strData += "25302";		strData += sCOL;
		strData += "25602";		strData += sCOL;
		break;
	case TRKEY_STRUPMARKET_1:	//	ADR
		strData += "25302";		strData += sCOL;
		strData += "25603";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_28:	//	신용잔고율
		strData += "5302";		strData += sCOL;
		strData += "5601";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_29:	//	신용공여율
		strData += "5302";		strData += sCOL;
		strData += "5602";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_30:	//	신용잔고수량
		strData += "5302";		strData += sCOL;
		strData += "5603";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_31:	//	신용잔고증감
		strData += "5302";		strData += sCOL;
		strData += "5604";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_32:	//	외국인+기관순매수
		strData += "5302";		strData += sCOL;
		strData += "5414";		strData += sCOL;
		break;
	case TRKEY_STRMARKET_33:	//	외국인+기관순매수누적
		strData += "5302";		strData += sCOL;
		strData += "5514";		strData += sCOL;
		break;
	}
	
	strData += szFS;
	return strData;
	

/*	CString strData, strCode, strTemp, strDate;
	CString szDS, szFS, szCOL;
	szDS = sDS;	szFS = sCELL;	szCOL = sCOL;

	strCode = CString(m_InData.shcode, sizeof(m_InData.shcode));
	strCode.TrimLeft(); strCode.TrimRight();

	strDate = CString(m_InData.edate, sizeof(m_InData.edate));
	strDate.TrimLeft(); strDate.TrimRight();

	int nCount	= CDataConverter::Char2Int(m_InData.num, sizeof(m_InData.num));
	CString stCount;
	stCount.Format("%06d", nCount);
	switch (nType)
	{
	case TRKEY_STRMARKET_0:		//	기관순매수누적
		//	Input 심볼	---------->>
		strData += "1301" + szDS + strCode + szFS;		// 종목코드 
		strData += "101306" + szDS + strDate + szFS;	// 조회일자
		//	output 심볼	---------->>
		strData += "1301" + szFS;						// 종목코드 
		strData += "$101810" + szDS;					// 그리드 심볼.
		strData += strGrid;								// 그리드 헤더.
		// Row Data 구성
		strData += "101302" + szCOL;					//	일자
		strData += "101390" + szCOL;					//	누적순매수
		break;
	case TRKEY_STRMARKET_1:		//	기관순매수량
		strData += "1301"	+ szDS + strCode + szFS;	// 종목코드 
		strData += "101306" + szDS + strDate + szFS;	// 조회일자
		strData += "1301" + szFS;						// 종목코드 
		strData += "$101810" + szDS;					// 그리드 심볼.
		strData += strGrid;								// 그리드 헤더.
		strData += "101302" + szCOL;					//	일자
		strData += "101391" + szCOL;					//	순매수
		break;
	case TRKEY_STRMARKET_2:		//	미결제약정
		{
			CStringArray dataArr;
			dataArr.Add(strCode);
			dataArr.Add(strDate);
			dataArr.Add(strGrid);
				
			MakeDataOfSuspensSeContract(strData, &dataArr);
		}
// 		strData += "30301"	+ szDS + strCode + szFS;	// 종목코드 
// 		strData += "103306" + szDS + strDate + szFS;	// 조회일자
// 		strData += "103308" + szDS + "1" + szFS;		// 일주월 구분.
// 		strData += "30301" + szFS;						// 종목코드 
// 		strData += "$103610" + szDS;					// 그리드 심볼.
// 		strData += strGrid;								// 그리드 헤더.
// 		strData += "103302" + szCOL;					//	일자
// 		strData += "103201" + szCOL;					//	미결제 수량
		break;
	case TRKEY_STRMARKET_3:		//	시가총액
		strData += "1301"	+ szDS + strCode + szFS;	// 종목코드 
		strData += "101306" + szDS + strDate + szFS;	// 조회일자
		strData += "1301" + szFS;						// 종목코드 
		strData += "?5500" + szDS;					// 그리드 심볼.
		strData += strGrid;								// 그리드 헤더.
		strData += "101302" + szCOL;					//	일자
		strData += "101305" + szCOL;					//	시가총액
		break;
	case TRKEY_STRMARKET_4:		//	외국인보유량 : KHD
		strData += "1301"	+ szDS + strCode + szFS;	// 종목코드 
		strData += "?5500" + szDS + stCount ;		// 그리드 심볼.
		strData += strDate + szFS;	// 조회일자
		strData += "1301" + szFS;						// 종목코드 
		
	//	strData += strGrid;								// 그리드 헤더.
		strData += "5302" + szCOL;					//	일자
		strData += "5363" + szCOL;					//	보유량
		break;
	case TRKEY_STRMARKET_5:		//	외국인보유율
		strData += "5306"	+ szDS + strCode + szFS;	// 종목코드 
		strData += "101306" + szDS + strDate + szFS;	// 조회일자
		strData += "1301" + szFS;						// 종목코드 
		strData += "$101710" + szDS;					// 그리드 심볼.
		strData += strGrid;								// 그리드 헤더.
		strData += "101302" + szCOL;					//	일자
		strData += "101367" + szCOL;					//	보유비율
		break;
	case TRKEY_STRMARKET_6:		//	외국인순매수
		strData += "1301"	+ szDS + strCode + szFS;	// 종목코드 
		strData += "101306" + szDS + strDate + szFS;	// 조회일자
		strData += "1301" + szFS;						// 종목코드 
		strData += "$101710" + szDS;					// 그리드 심볼.
		strData += strGrid;								// 그리드 헤더.
		strData += "101302" + szCOL;					//	일자
		strData += "101364" + szCOL;					//	순매수
		break;
	case TRKEY_STRMARKET_7:		//	거래대금
		strData += "1301"	+ szDS + strCode + szFS;	// 종목코드 
		strData += "101306" + szDS + strDate + szFS;	// 조회일자
		strData += "1301" + szFS;						// 종목코드 
		strData += "$101610" + szDS;					// 그리드 심볼.
		strData += strGrid;								// 그리드 헤더.
		strData += "101302" + szCOL;					//	일자
		strData += "101028" + szCOL;					//	거래대금
		break;
	}
*/
//	strData += szFS;
//	return strData;
}

CString CChartItemMarket::fnStructType2(CString strGrid, int nType)
{
	CString strData, strCode, strMarket, strDate;
	CString szDS, szFS, szCOL;
	szDS = sDS;	szFS = sCELL;	szCOL = sCOL;

	strCode = CString(m_InData.shcode, sizeof(m_InData.shcode));
	strCode.TrimLeft(); strCode.TrimRight();

	strDate = CString(m_InData.edate, sizeof(m_InData.edate));
	strDate.TrimLeft(); strDate.TrimRight();

	switch (nType)
	{
	case TRKEY_STRMARKET_8:		strMarket = "001";	break;	//	투자자별거래소
	case TRKEY_STRMARKET_9:		strMarket = "201";	break;	//	투자자별코스닥
	case TRKEY_STRMARKET_10:	strMarket = "101";	break;	//	투자자별K200
	case TRKEY_STRMARKET_11:	strMarket = "9101";	break;	//	투자자별선물
	case TRKEY_STRMARKET_12:	strMarket = "9201";	break;	//	투자자별콜옵션
	case TRKEY_STRMARKET_13:	strMarket = "9301";	break;	//	투자자별풋옵션
	}

	//	Input 심볼	---------->>
	strData += "105301"	+ szDS + strMarket + szFS;		// 종목코드
	strData += "105306" + szDS + strDate + szFS;		// 조회일자
	strData += "105309" + szDS + "5" + szFS;			// 매도수 수량(5)/대금(6)
	//	output 심볼	---------->>
	strData += "105301" + szFS;							// 종목코드 
	strData += "$105610" + szDS;						// 그리드 심볼.
	strData += strGrid;									// 그리드 헤더.
	// Row Data 구성
	strData += "105302" + szCOL;						//	일자
	strData += "105318" + szCOL;						//	개인 순매수
	strData += "105319" + szCOL;						//	외국인 순매수
	strData += "105310" + szCOL;						//	기관계 순매수

	strData += szFS;

	return strData;
}

char* CChartItemMarket::Convert2Struct(LPBYTE pData, int nLen, int nKey, int &nDataLen)
{	
	char *pszBuff = NULL;
	int nIndex = 0, nSize=0;
	CString strSrc;
	strSrc.Format(_T("%.*s"), nLen, (char*)pData);
	CString strNode, strCode;
	CChartBozoManager bojoMng;
	CChartBozoInput bojoInput;
	CStringArray arrDate, arrSel1, arrSel2, arrSel3;

	CString strNm1, strNm2, strNm3, strNm4;
	CString strDataNm, strData;
	strDataNm.Format(_T("%.*s"), sizeof(m_InData.dataNm), m_InData.dataNm);
	int nFind = strDataNm.Find(_T(";"));
	if (nFind)
	{
		strNm1 = strDataNm.Left(nFind);
		strDataNm = strDataNm.Mid(nFind+1);
	}
	nFind = strDataNm.Find(_T(";"));
	if (nFind)
	{
		strNm2 = strDataNm.Left(nFind);
		strDataNm = strDataNm.Mid(nFind+1);
	}
	nFind = strDataNm.Find(_T(";"));
	if (nFind)
	{
		strNm3 = strDataNm.Left(nFind);
		strDataNm = strDataNm.Mid(nFind+1);
	}
	nFind = strDataNm.Find(_T(";"));
	if (nFind)
		strNm4 = strDataNm.Left(nFind);
//	strSrc = strSrc.Mid(136);
	if (nKey <= TRKEY_STRMARKET_END)
	{
		strCode = CDataConverter::Parser(strSrc, sCELL);	// 종목코드

		//KHD : NextKey
		CString strNextKey = CDataConverter::Parser(strSrc, sCELL);

		// 그리드 헤더
		GRAPH_IO gridOut;
		memcpy(&gridOut, (LPSTR)(LPCTSTR)strSrc, sizeof(gridOut));
		strSrc = strSrc.Mid(sizeof(gridOut));
		strSrc.TrimLeft(); strSrc.TrimRight();

		while (strSrc.IsEmpty() == FALSE)
		{
			strNode = CDataConverter::Parser(strSrc, sCELL);	// 일자
			arrDate.Add(strNode);

			if (strSrc.IsEmpty() || strNode.GetLength() < 3)	break;
			strNode = CDataConverter::Parser(strSrc, sROW);		// 누적순매수
			arrSel1.Add(strNode);

			nIndex++;
		}

		nDataLen = sizeof(KB_d1015_OutRec1) + nIndex * sizeof(KB_d1015_OutRec2_1);
		pszBuff = new char[nDataLen+1];
		memset(pszBuff, 0x20, nDataLen);
		pszBuff[nDataLen] = 0x00;

		KB_d1015_OutRec_1 *pstOut = NULL;
		pstOut = (KB_d1015_OutRec_1 *)pszBuff;

		memcpy(pstOut->winid, m_InData.winid, sizeof(m_InData.winid));
		memcpy(pstOut->dataNm, m_InData.dataNm, sizeof(m_InData.dataNm));

		if (strNextKey !="99999999999999")// 9가 14자리면 다음키가 없는것.  
			memcpy(pstOut->nxtKey, (LPCTSTR)strNextKey, min(sizeof(pstOut->nxtKey),strNextKey.GetLength()));
		else
			memset(pstOut->nxtKey, 0x20, sizeof(pstOut->nxtKey));

		pstOut->inNumGb[0] = '2';

		memcpy(pstOut->shcode, strCode, min(sizeof(pstOut->shcode), strCode.GetLength()));
		bojoInput.m_sShcode = strCode;

		strNode.Format(_T("%05d"), nIndex);
		memcpy(pstOut->d1015_CHART.chtCnt, strNode, 5);
		int ii= nIndex-1;
		for (int i=0; i<nIndex; i++)
//		for(int i = nIndex-1; i>=0 ; i--)
		{
			KB_d1015_OutRec2_1* pRow = (KB_d1015_OutRec2_1*)&pstOut->d1015_CHART.chart[ii];

			strNode = arrDate.GetAt(i);
			memcpy(pRow->date, strNode, min(sizeof(pRow->date), strNode.GetLength()));

			strNode = arrSel1.GetAt(i);
			if (strNm1.Find(_T("_Foreign Holding Rate___DateTime_")) >= 0 ||
				strNm1.Find(_T("_CreditBalanceRate___DateTime_")) >= 0 ||
				strNm1.Find(_T("_CreditGivingRate___DateTime_")) >= 0)
			{
				double fNode = atoi(strNode)/100.00;
				strNode.Format("%f",fNode);
			}
// 			else
// 			{
// 				strNode+="000";
// 			}
			memcpy(pRow->data1, strNode, min(sizeof(pRow->data1), strNode.GetLength()));
			ii--;
//			OutputDebugString("KHD : 보유량 = " + strNode );
		}

		bojoInput.m_sDataType = _T("0");		//	고정. 차후 필요시 수정.
		bojoInput.m_sMarket = _T("1");			//	고정. 차후 필요시 수정.

		char szKey[5];
//		memset(szKey, 0x00, sizeof(szKey));
//		memcpy(szKey, gridOut.page, sizeof(gridOut.page));
		if (m_InData.nxtGb[0] != _T('0'))
		{
			bojoInput.m_sNxtFlg = _T("1");
			bojoInput.sNxtFlg[0] = _T('1');
		}
		else
		{
			bojoInput.m_sNxtFlg = _T("0");
			bojoInput.sNxtFlg[0] = _T('0');
		}

		bojoInput.m_sTerm = _T("2");				//	일,주,월
		bojoInput.m_sTick = _T("1");

		CString strBozo;
		memset(szKey, 0x00, sizeof(szKey));
		sprintf(szKey, _T("%03d"), 133); // 133은 지표

		nSize = bojoMng.GetChartBozoMsg(szKey, &bojoInput, strNode);
		if (strNm1.Find(_T("_Foreign Holding Rate___DateTime_")) >= 0 ||
			strNm1.Find(_T("_CreditBalanceRate___DateTime_")) >= 0 ||
			strNm1.Find(_T("_CreditGivingRate___DateTime_")) >= 0)
			strData.Format(_T("%.*s:8:YYYYMMDD|%.*s:15:x0.01"), strNm1.GetLength(), strNm1, strNm2.GetLength(), strNm2);
//			strData.Format(_T("자료일자:8:YYYYMMDD|%.*s:15:x0.01"), strNm2.GetLength(), strNm2);
		else
			strData.Format(_T("%.*s:8:YYYYMMDD|%.*s:15:x1"), strNm1.GetLength(), strNm1, strNm2.GetLength(), strNm2);
//			strData.Format(_T("자료일자:8:YYYYMMDD|%.*s:15:x1"), strNm2.GetLength(), strNm2);

		CChartBozoItem *pItem = NULL;
		List_CChartBozoItem arr;
//		if (m_InData.nxtGb[0] == _T('0'))
			pItem = new CChartBozoItem(CChartBozoItem::cbi_add, _T("NEWPACKET"), strData);
//		else
//			pItem = new CChartBozoItem(CChartBozoItem::cbi_add, _T("RESETPACKET"), strData);

		arr.AddTail(pItem);

		nSize = bojoMng.ChangeChartBozoMsg(strNode, &arr);

		memcpy(pstOut->d1015_CHART.chtBjM, (LPSTR)(LPCTSTR)strNode, nSize);		//	보조메시지
		memcpy(pstOut->d1015_CHART.chtBjM, "0396", 4);

		delete pItem;
		pItem = NULL;
		arr.RemoveAll();
	}
	else
	{
		strCode = CDataConverter::Parser(strSrc, sFS);	// 종목코드

		//KHD : NextKey
		CString strNextKey = CDataConverter::Parser(strSrc, sCELL);

		// 그리드 헤더
		GRAPH_IO gridOut;
		memcpy(&gridOut, (LPSTR)(LPCTSTR)strSrc, sizeof(gridOut));
		strSrc = strSrc.Mid(sizeof(gridOut));

		while (strSrc.IsEmpty() == FALSE)
		{
			strNode = CDataConverter::Parser(strSrc, sCELL);	// 일자
			arrDate.Add(strNode);

			if (strSrc.IsEmpty())	break;
			strNode = CDataConverter::Parser(strSrc, sCELL);		// 누적순매수
			arrSel1.Add(strNode);

			if (strSrc.IsEmpty())	break;
			strNode = CDataConverter::Parser(strSrc, sCELL);		// 누적순매수
			arrSel2.Add(strNode);

			if (strSrc.IsEmpty())	break;
			strNode = CDataConverter::Parser(strSrc, sROW);		// 누적순매수
			arrSel3.Add(strNode);

			nIndex++;
		}

		nDataLen = sizeof(KB_d1015_OutRec1) + nIndex * sizeof(KB_d1015_OutRec2_3);
		pszBuff = new char[nDataLen+1];
		memset(pszBuff, 0x20, nDataLen);
		pszBuff[nDataLen] = 0x00;

		KB_d1015_OutRec_3 *pstOut = NULL;
		pstOut = (KB_d1015_OutRec_3 *)pszBuff;

		memcpy(pstOut->winid, m_InData.winid, sizeof(m_InData.winid));
		memcpy(pstOut->dataNm, m_InData.dataNm, sizeof(m_InData.dataNm));

		if (strNextKey !="99999999999999")// 9가 14자리면 다음키가 없는것.  
			memcpy(pstOut->nxtKey, (LPCTSTR)strNextKey, min(sizeof(pstOut->nxtKey),strNextKey.GetLength()));
		else
			memset(pstOut->nxtKey, 0x20, sizeof(pstOut->nxtKey));

		pstOut->inNumGb[0] = '4';

		memcpy(pstOut->shcode, strCode, min(sizeof(pstOut->shcode), strCode.GetLength()));
		bojoInput.m_sShcode = strCode;

		strNode.Format(_T("%05d"), nIndex);
		memcpy(pstOut->d1015_CHART.chtCnt, strNode, 5);

		for (int i=0; i<nIndex; i++)
		{
			KB_d1015_OutRec2_3* pRow = (KB_d1015_OutRec2_3*)&pstOut->d1015_CHART.chart[i];

			strNode = arrDate.GetAt(i);
			memcpy(pRow->date, strNode, min(sizeof(pRow->date), strNode.GetLength()));

			strNode = arrSel1.GetAt(i);
			memcpy(pRow->data1, strNode, min(sizeof(pRow->data1), strNode.GetLength()));

			strNode = arrSel2.GetAt(i);
			memcpy(pRow->data2, strNode, min(sizeof(pRow->data2), strNode.GetLength()));

			strNode = arrSel3.GetAt(i);
			memcpy(pRow->data3, strNode, min(sizeof(pRow->data3), strNode.GetLength()));
		}

		bojoInput.m_sDataType = _T("0");		//	고정. 차후 필요시 수정.
		bojoInput.m_sMarket = _T("1");			//	고정. 차후 필요시 수정.

		char szKey[5];
	//	memset(szKey, 0x00, sizeof(szKey));
	//	memcpy(szKey, gridOut.page, sizeof(gridOut.page));
		if (m_InData.nxtGb[0] != _T('0'))
		{
			bojoInput.m_sNxtFlg = _T("1");
			bojoInput.sNxtFlg[0] = _T('1');
		}
		else
		{
			bojoInput.m_sNxtFlg = _T("0");
			bojoInput.sNxtFlg[0] = _T('0');
		}

		bojoInput.m_sTerm = _T("2");				//	일,주,월
		bojoInput.m_sTick = _T("1");

		CString strBozo;
		memset(szKey, 0x00, sizeof(szKey));
		sprintf(szKey, _T("%03d"), 133); // 133은 지표

		nSize = bojoMng.GetChartBozoMsg(szKey, &bojoInput, strNode);
		strData.Format(_T("%.*s:8:YYYYMMDD|%.*s:15:x1|%.*s:15:x1|%.*s:15:x1"),
					strNm1.GetLength(), strNm1, strNm2.GetLength(), strNm2,
					strNm3.GetLength(), strNm3, strNm4.GetLength(), strNm4);
//		strData.Format(_T("자료일자:8:YYYYMMDD|%.*s:15:x1|%.*s:15:x1|%.*s:15:x1"),
//				strNm2.GetLength(), strNm2, strNm3.GetLength(), strNm3, strNm4.GetLength(), strNm4);

		CChartBozoItem *pItem = NULL;
		List_CChartBozoItem arr;
//		if (m_InData.nxtGb[0] == _T('0'))
			pItem = new CChartBozoItem(CChartBozoItem::cbi_add, _T("NEWPACKET"), strData);
//		else
//			pItem = new CChartBozoItem(CChartBozoItem::cbi_add, _T("RESETPACKET"), strData);

		arr.AddTail(pItem);

		nSize = bojoMng.ChangeChartBozoMsg(strNode, &arr);

		memcpy(pstOut->d1015_CHART.chtBjM, (LPSTR)(LPCTSTR)strNode, nSize);		//	보조메시지
		memcpy(pstOut->d1015_CHART.chtBjM, "0396", 4);

		delete pItem;
		pItem = NULL;
		arr.RemoveAll();
	}

	return pszBuff;
}

//nDataType 1(일), 
void CChartItemMarket::MakeDataOfSuspensSeContract(CString &strData, CStringArray* pDataArr) 
{
	CString strCode = pDataArr->GetAt(0);
	CString strDate = pDataArr->GetAt(1);
	CString strGrid = pDataArr->GetAt(2);

	CString szDS, szFS, szCOL;
	szDS = sDS;	szFS = sFS;	szCOL = sCOL;

	char cDataType = m_InData.button[0];
	char aUnit[3+1] = {0,};
	memcpy(aUnit, m_InData.unit, 3);

	// cDataType : 틱(0)/분(1)/일(2)/주(3)/월(4)/초(6)
	switch(cDataType)
	{
		case '2':	//일
		default:
			strData += "30301"	+ szDS + strCode + szFS;	// 종목코드 
			strData += "103306" + szDS + strDate + szFS;	// 조회일자
			strData += "103308" + szDS + "1" + szFS;		// 일주월 구분.
			strData += "$103610" + szDS;					// 그리드 심볼.
			strData += strGrid;								// 그리드 헤더.
			strData += "103302" + szCOL;					//	일자
			strData += "103201" + szCOL;					//	미결제 수량
			break;

/*		case '0':		//틱
			strData += "30301" + szDS + strCode + szFS;		// 종목코드
			strData += "103304" + szDS + strDate + szFS;	// 기준시간
			strData += "103306" + szDS + strDate + szFS;	// 조회일자
			strData += "103307" + szDS + "1" + szFS;		// 당일구분 1당일 0기준일
			//strData += "103308" + szDS + "1" + szFS;		// 갭.
			strData += "103308" + szDS + aUnit + szFS;		// 갭.
			strData += "30301" + szFS;						// 종목코드
			strData += "$103410" + szDS;					// 그리드 심볼.
			strData += strGrid;								// 그리드 헤더.
			strData += "103302" + szCOL;					// 일자
			strData += "103034" + szCOL;					// 시간
			strData += "103201" + szCOL;					// 미결제 수량
			break;

		case '1':		//분
			strData += "30301" + szDS + strCode + szFS;		// 종목코드
			strData += "103304" + szDS + strDate + szFS;	// 기준시간
			strData += "103306" + szDS + strDate + szFS;	// 조회일자
			strData += "103307" + szDS + "1" + szFS;		// 당일구분 1당일 0기준일부터, 2기준일만
			//strData += "103308" + szDS + "1" + szFS;		// 갭.
			strData += "103308" + szDS + aUnit + szFS;		// 갭.
			strData += "30301" + szFS;						// 종목코드
			strData += "$103510" + szDS;					// 그리드 심볼.
			strData += strGrid;								// 그리드 헤더.
			strData += "103302" + szCOL;					// 일자
			strData += "103034" + szCOL;					// 시간
			strData += "103201" + szCOL;					// 미결제 수량
			break;

		case '6':		//초
			strData += "30301" + szDS + strCode + szFS;		// 종목코드
			strData += "103304" + szDS + "1" + szFS;		// 요청레코드
			strData += "103307" + szDS + "1" + szFS;		// 당일구분 안씀
			//strData += "103308" + szDS + "1" + szFS;		// 갭.
			strData += "103308" + szDS + aUnit + szFS;		// 갭.
			strData += "30301" + szFS;						// 종목코드
			strData += "$103511" + szDS;					// 그리드 심볼.
			strData += strGrid;								// 그리드 헤더.
			strData += "103302" + szCOL;					// 일자
			strData += "103034" + szCOL;					// 시간
			strData += "103201" + szCOL;					// 미결제 수량
			break;
*/
	}
}
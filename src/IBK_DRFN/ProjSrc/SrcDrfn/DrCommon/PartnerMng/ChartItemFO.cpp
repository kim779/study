// ChartItemFO.cpp: implementation of the CChartItemFO class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PartnerMng.h"
#include "ChartItemFO.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//선물 
char* szTickRowFieldFO[] =	{"32302", "32034", "32023", "32029", "32030", "32031", "32032", "32028", "32201",};
char* szSecRowFieldFO[] =	{"36302", "36034", "36023", "36029", "36030", "36031", "36032", "36028", "36201", };
char* szMinRowFieldFO[] =	{"34302", "34034", "34023", "34029", "34030", "34031", "34032", "34028", "34201",};
char* szDayRowFieldFO[] =	{"35302",           "35023", "35029", "35030", "35031", "35027", "35028", "35201",}; // "35201" 미결제 약정 
//옵션
char* szTickRowFieldOption[] =	{"42302", "42034", "42023", "42029", "42030", "42031", "42032", "42028", "42201",};
char* szSecRowFieldOption[] =	{"46302", "46034", "46023", "46029", "46030", "46031", "46032", "46028", "46201",};
char* szMinRowFieldOption[] =	{"44302", "44034", "44023", "44029", "44030", "44031", "44032", "45028", "45201",};
char* szDayRowFieldOption[] =	{"45302",   "45023", "45029", "45030", "45031", "45027", "45028", "45201",};

//상품선물 
char* szTickRowFieldCommodity[] =	{"102302", "102034", "102023", "102029", "102030", "102031", "102032", "105028","102201",};
char* szSecRowFieldCommodity[] =	{"106302", "106034", "106023", "106029", "106030", "106031", "106032", "105028", "106201",};
char* szMinRowFieldCommodity[] =	{"104302", "104034", "104023", "104029", "104030", "104031", "104032", "105028", "104201",};
char* szDayRowFieldCommodity[] =	{"105302",   "105023", "105029", "105030", "105031", "105027", "105028","105201", };

CChartItemFO::CChartItemFO()
{
	m_bSet = FALSE;
}

CChartItemFO::~CChartItemFO()
{

}

void CChartItemFO::SetInData(KB_p0604_InRec1* pData)
{
	memcpy(&m_InData, pData, sizeof(KB_p0604_InRec1));
	m_bSet = TRUE;
}

// MUltiChart에서의 조회 구조체를 받아서 FID 형태로 변환
// 구성 : 차트조회Grid Inbound + 시세 FID Inbound + GridHeader + Grid Outbound
// winix 플랫폼에서 배열형태로 내려오는 데이터를 Grid 라고 부른다.
CString CChartItemFO::Convert()
{
	CString strCode;
	strCode = CString(m_InData.Scode, sizeof(m_InData.Scode));
	strCode.TrimLeft(); strCode.TrimRight();

	if(!m_bSet) return "";

	IMasterDataManagerLast* pMasterDataManager = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);
	m_nCodeType = pMasterDataManager->GetCodeTypeFromCode(strCode);

	switch(m_nCodeType)
	{
		case CDRCodeTypeInfo::CODE_KFUTURE://연결선물?? 
			return ConvertFuture();
			break;
		case CDRCodeTypeInfo::CODE_FUOPT_COMMODITY://상품선물 
			return ConvertCommodity();	
			break;
		case	CDRCodeTypeInfo::CODE_KOPTION:
			return ConvertOption();//옵션일경우 첫자리가 2,3
			break;
		case CDRCodeTypeInfo::CODE_FUTURE_SPREAD:
			return ConvertFuture();
			break;
	}

	
	return "";
}

/**
	@History
		2011.01.04 alzioyes: 상품선물중에서 금(), 미니금(), 돈육()의 경우에는 x1 형태로, 그외는 x0.01형태로 처리해야해서 수정함.
*/
CString CChartItemFO::ConvertCommodity()
{
	CString strData, strCode, strTemp, strImgRgb, strDate, strNow;
	int i, nCount;
	
	strTemp = CString(m_InData.Smaxbong, sizeof(m_InData.Smaxbong));
	nCount = atoi(strTemp);

	strCode = CString(m_InData.Scode, sizeof(m_InData.Scode));
	strCode.TrimLeft(); strCode.TrimRight();

	strTemp = CString(m_InData.Sunit, sizeof(m_InData.Sunit));
	int nUnit = atoi(strTemp);
	if(nUnit > 372)
		nUnit = 372;
	strTemp = CString(m_InData.Sbtngb, sizeof(m_InData.Sbtngb)); // 틱(0)/분(1)/일(2)/주(3)/월(4)
	int nType = atoi(strTemp);

	strImgRgb = CDataConverter::Char2String(m_InData.Simgrgb, sizeof(m_InData.Simgrgb));
	strDate = CString(m_InData.Sdate, sizeof(m_InData.Sdate));//KHD
	strNow	= CDataConverter::Char2String(m_InData.nowData, sizeof(m_InData.nowData));
	int nFlag     = CDataConverter::Char2Int(m_InData.Snextgb, sizeof(m_InData.Snextgb));	// 0:
	//////////////////////////////////////////////////////////////////////////
	// Inbound 데이터 작성
	//////////////////////////////////////////////////////////////////////////

	// 차트 데이터

	strData += "101301"; // 종목코드 Input 심볼
	strData += sDS;

	strData += strCode;
	strData += sCELL;//KHD sFS;

	CString stNextData, stNextKey;
	
	//NEXTKEY : KHD
	if(nFlag == 0)
	{
		stNextData = "99999999999999";
		memcpy(m_InData.Snextkey, (LPCSTR)(LPCTSTR)stNextData,stNextData.GetLength());
	}

	_dataH	Datah;
	FillMemory(&Datah, sz_DATAH, 0x20);
	CString stRowData;
	switch(nType)
	{
	case 0:		// 틱
		Datah.dindex = GI_TICK;
		stRowData = _T("?102500");
		// 		// 당일만조회 Input 심볼 (1:당일) 
 		strData += "102309";	strData += sDS;	strData += strNow;	strData += sCELL;
		//KHD : NEXT		
		stNextKey = "102324";
		stNextData.Format("%28.28s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // 넥스트 키값 : KHD 
		//END
		break;

	case 1:		// 분
		Datah.dindex = GI_MINUTE;
		stRowData = _T("?104500");
		//KHD : NEXT		
		stNextKey = "104324";
		stNextData.Format("%14.14s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // 넥스트 키값 : KHD 
		//END
		// 실/허봉 구분 (0:허봉, 1:실봉)
 		strData += "104308";	strData += sDS;	strData += strImgRgb;	strData += sCELL;
		strData += "104309";//당일만조회 Input 심볼
		strData += sDS;
		strData += strNow;
 		strData += sCELL;

		// 20130530 이상신 : 월물간 갭보정 추가(IBK) (Issue No : 0006572)
		strData += "104339";		strData += sDS;
		strData += CString(m_InData.SgapRev, sizeof(m_InData.SgapRev));	strData += sCELL;
		break;

	case 2:		// 일
		Datah.dindex = GI_DAY;
		nUnit = 1;
		stRowData = _T("?105500");
		//KHD : NEXT		
		stNextKey = "105324";
		stNextData.Format("%8.8s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // 넥스트 키값 : KHD 
		//END
		// 20130530 이상신 : 월물간 갭보정 추가(IBK) (Issue No : 0006572)
		strData += "105339";		strData += sDS;
		strData += CString(m_InData.SgapRev, sizeof(m_InData.SgapRev));	strData += sCELL;
		break;
	case 3:		// 주
		Datah.dindex = GI_WEEK;
		nUnit = 1;
		stRowData = _T("?105500");
		//KHD : NEXT		
		stNextKey = "105324";
		stNextData.Format("%8.8s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // 넥스트 키값 : KHD 
		//END
		// 20130530 이상신 : 월물간 갭보정 추가(IBK) (Issue No : 0006572)
		strData += "105339";		strData += sDS;
		strData += CString(m_InData.SgapRev, sizeof(m_InData.SgapRev));	strData += sCELL;
		break;
	case 4:		// 월
		Datah.dindex = GI_MONTH;
		nUnit = 1;
		stRowData = _T("?105500");
		//KHD : NEXT		
		stNextKey = "105324";
		stNextData.Format("%8.8s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // 넥스트 키값 : KHD 
		//END
		// 20130530 이상신 : 월물간 갭보정 추가(IBK) (Issue No : 0006572)
		strData += "105339";		strData += sDS;
		strData += CString(m_InData.SgapRev, sizeof(m_InData.SgapRev));	strData += sCELL;
		break;
	case 6:		// 초
		Datah.dindex = GI_SEC;
		stRowData = _T("?106500");
		//KHD : NEXT		
		stNextKey = "106324";
		stNextData.Format("%14.14s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // 넥스트 키값 : KHD 
		//END
		// 실/허봉 구분 (0:허봉, 1:실봉)
 		strData += "106308";	strData += sDS;	strData += strImgRgb;	strData += sCELL;
		strData += "106309";//당일만조회 Input 심볼
		strData += sDS;
		strData += strNow;
 		strData += sCELL;

		// 20130530 이상신 : 월물간 갭보정 추가(IBK) (Issue No : 0006572)
		strData += "106339";		strData += sDS;
		strData += CString(m_InData.SgapRev, sizeof(m_InData.SgapRev));	strData += sCELL;
		break;
	}

	//////////////////////////////////////////////////////////////////////////
	// 종목 시세 OutBound : KHD : IBK 맞추기 
						// 단축코드,  이름,  현재가,    전일종가  ,증감?,     등락율, 누적거래량
	char* pFOSymbol[] = { "101301", "101022", "101023", "101336", "101024", "101033", "101027",
						// 상한가,  하한가 ,  시가,  고가,  저가,    	// 전일시가, 전일고가, 전일저가, 누적거래대금
						"101311", "101312", "101029", "101030", "101031","101320", "101321", "101322", "101315", "101201", "101307", "101900", "101901"};
					
	for(i = 0; i < sizeof(pFOSymbol)/sizeof(char*); i++)
	{
		strData += pFOSymbol[i];
		strData += sCELL;//sFS;
	}
	strData += stNextKey;
	strData += sCELL;
	//////////////////////////////////////////////////////////////////////////
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
	memset(szData_i, 0x00, sizeof(Datah));
	memcpy(szData_i, (char*)&Datah, sizeof(Datah));
	szData_i[sz_DATAH-1] = 0x00; // KHD : 맨끝에 0x00문자를 집어 넣는다.
	strData += szData_i;


	int nSize;

	switch(nType)
	{
	case 0:		// 틱
		nSize = sizeof(szTickRowFieldCommodity)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szTickRowFieldCommodity[i];
			strData += sCOL;
		}
		break;

	case 1:		// 분
		nSize = sizeof(szMinRowFieldCommodity)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szMinRowFieldCommodity[i];
			strData += sCOL;
		}
		break;

	case 2:		// 일
	case 3:		// 주
	case 4:		// 월
		nSize = sizeof(szDayRowFieldCommodity)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szDayRowFieldCommodity[i];
			strData += sCOL;
		}
		break;
	case 6:		// 초
		nSize = sizeof(szSecRowFieldCommodity)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szSecRowFieldCommodity[i];
			strData += sCOL;
		}
		break;
	}
	
	strData += sCELL;//sFS;

	return strData;
}
CString CChartItemFO::ConvertFuture()
{
	CString strData, strCode, strTemp, strImgRgb, strDate, strNow;
	int i, nCount;
	
	strTemp = CString(m_InData.Smaxbong, sizeof(m_InData.Smaxbong));
	nCount = atoi(strTemp);

	strCode = CString(m_InData.Scode, sizeof(m_InData.Scode));
	strCode.TrimLeft(); strCode.TrimRight();

	strTemp = CString(m_InData.Sunit, sizeof(m_InData.Sunit));
	int nUnit = atoi(strTemp);
	if(nUnit > 372)
		nUnit = 372;
	strTemp = CString(m_InData.Sbtngb, sizeof(m_InData.Sbtngb)); // 틱(0)/분(1)/일(2)/주(3)/월(4)
	int nType = atoi(strTemp);

	strImgRgb = CDataConverter::Char2String(m_InData.Simgrgb, sizeof(m_InData.Simgrgb));
	strDate = CString(m_InData.Sdate, sizeof(m_InData.Sdate));//KHD
	strNow	= CDataConverter::Char2String(m_InData.nowData, sizeof(m_InData.nowData));
	int nFlag     = CDataConverter::Char2Int(m_InData.Snextgb, sizeof(m_InData.Snextgb));	// 0:
	//////////////////////////////////////////////////////////////////////////
	// Inbound 데이터 작성
	//////////////////////////////////////////////////////////////////////////

	// 차트 데이터

	strData += "31301"; // 종목코드 Input 심볼
	strData += sDS;

	strData += strCode;
	strData += sCELL;//KHD sFS;

	CString stNextData, stNextKey;
	
	//NEXTKEY : KHD
	if(nFlag == 0)
	{
		stNextData = "99999999999999";
		memcpy(m_InData.Snextkey, (LPCSTR)(LPCTSTR)stNextData,stNextData.GetLength());
	}
	_dataH	Datah;
	FillMemory(&Datah, sz_DATAH, 0x20);
	CString stRowData;
	switch(nType)
	{
	case 0:		// 틱
		Datah.dindex = GI_TICK;
		stRowData = _T("?32500");
		strData += "32309";	strData += sDS;	strData += strNow;	strData += sCELL;
		//KHD : NEXT		
		stNextKey = "32324";
		stNextData.Format("%28.28s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // 넥스트 키값 : KHD 
		if (m_InData.Srecordgb[0] == '1')
		{
			int BeforeDay = CDataConverter::Char2Int(m_InData.Sredate, sizeof(m_InData.Sredate));	// 0:
			CString strTime = strTemp = CString(m_InData.Stime, sizeof(m_InData.Stime));
			CString stBefore;

			stBefore = strTime;
			strTime.Format("%d%s",BeforeDay,strTime);//0 : 당일 , 1: 전
			strData += "32341";	strData += sDS;	strData += strTime;	strData += sCELL;
		}
		//END
		break;

	case 1:		// 분
		Datah.dindex = GI_MINUTE;
		stRowData = _T("?34500");
		//KHD : NEXT		
		stNextKey = "34324";
		stNextData.Format("%14.14s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // 넥스트 키값 : KHD 
		//END
		// 실/허봉 구분 (0:허봉, 1:실봉)
 		strData += "34308";	strData += sDS;	strData += strImgRgb;	strData += sCELL;
		strData += "34309";//당일만조회 Input 심볼
		strData += sDS;
		strData += strNow;
 		strData += sCELL;
		if (m_InData.Srecordgb[0] == '1')
		{
			int BeforeDay = CDataConverter::Char2Int(m_InData.Sredate, sizeof(m_InData.Sredate));	// 0:
			CString strTime = strTemp = CString(m_InData.Stime, sizeof(m_InData.Stime));
			CString stBefore;
			
			stBefore = strTime;
			strTime.Format("%d%s",BeforeDay,strTime);//0 : 당일 , 1: 전
			strData += "34341";	strData += sDS;	strData += strTime;	strData += sCELL;
		}
		break;

	case 2:		// 일
		Datah.dindex = GI_DAY;
		nUnit = 1;
		stRowData = _T("?35500");
		//KHD : NEXT		
		stNextKey = "35324";
		stNextData.Format("%8.8s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // 넥스트 키값 : KHD 
		//END
		break;
	case 3:		// 주
		Datah.dindex = GI_WEEK;
		nUnit = 1;
		stRowData = _T("?35500");
		//KHD : NEXT		
		stNextKey = "35324";
		stNextData.Format("%8.8s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // 넥스트 키값 : KHD 
		//END
		break;
	case 4:		// 월
		Datah.dindex = GI_MONTH;
		nUnit = 1;
		stRowData = _T("?35500");
		//KHD : NEXT		
		stNextKey = "35324";
		stNextData.Format("%8.8s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // 넥스트 키값 : KHD 
		//END
		break;
	case 6:		// 초
		// 실/허봉 구분 (0:허봉, 1:실봉)
		Datah.dindex = GI_SEC;
		stRowData = _T("?36500");
		//KHD : NEXT		
		stNextKey = "36324";
		stNextData.Format("%14.14s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // 넥스트 키값 : KHD 
		//END
		// 실/허봉 구분 (0:허봉, 1:실봉)
 		strData += "36308";	strData += sDS;	strData += strImgRgb;	strData += sCELL;
		strData += "36309";//당일만조회 Input 심볼
		strData += sDS;
		strData += strNow;
 		strData += sCELL;
		if (m_InData.Srecordgb[0] == '1')
		{
			int BeforeDay = CDataConverter::Char2Int(m_InData.Sredate, sizeof(m_InData.Sredate));	// 0:
			CString strTime = strTemp = CString(m_InData.Stime, sizeof(m_InData.Stime));
			CString stBefore;
			
			stBefore = strTime;
			strTime.Format("%d%s",BeforeDay,strTime);//0 : 당일 , 1: 전
			strData += "36341";	strData += sDS;	strData += strTime;	strData += sCELL;
		}
		break;
	}

	//////////////////////////////////////////////////////////////////////////
	// 종목 시세 OutBound : KHD : IBK 맞추기 
						// 단축코드,  이름,  현재가, 전일종가,증감, 등락율, 누적거래량
	char* pFOSymbol[] = { "31301", "31022", "31023", "31336", "31024", "31033", "31027",
						// 상한가,  하한가 ,  시가,  고가,  저가,    	// 전일시가, 전일고가, 전일저가, 누적거래대금
						"31311", "31312", "31029", "31030", "31031","31320", "31321", "31322", "35028", "31201", "31307", "31900", "31901" };
					
	for(i = 0; i < sizeof(pFOSymbol)/sizeof(char*); i++)
	{
		strData += pFOSymbol[i];
		strData += sCELL;//sFS;
	}
	strData += stNextKey;
	strData += sCELL;
	//////////////////////////////////////////////////////////////////////////
	// 그리드성 데이터(배열성, 반복데이터) OutBound
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
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
	memset(szData_i, 0x00, sizeof(Datah));
	memcpy(szData_i, (char*)&Datah, sizeof(Datah));
	szData_i[sz_DATAH-1] = 0x00; // KHD : 맨끝에 0x00문자를 집어 넣는다.
	strData += szData_i;


	int nSize;

	switch(nType)
	{
	case 0:		// 틱
		nSize = sizeof(szTickRowFieldFO)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szTickRowFieldFO[i];
			strData += sCOL;
		}
		break;

	case 1:		// 분
		nSize = sizeof(szMinRowFieldFO)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szMinRowFieldFO[i];
			strData += sCOL;
		}
		break;

	case 2:		// 일
	case 3:		// 주
	case 4:		// 월
		nSize = sizeof(szDayRowFieldFO)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szDayRowFieldFO[i];
			strData += sCOL;
		}
		break;
	case 6:		// 초
		nSize = sizeof(szSecRowFieldFO)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szSecRowFieldFO[i];
			strData += sCOL;
		}
		break;
	}
	
	strData += sCELL;//sFS;

	return strData;
}

CString CChartItemFO::ConvertOption()
{
	CString strData, strCode, strTemp, strImgRgb, strDate;
	int i, nCount;
	
	strTemp = CString(m_InData.Smaxbong, sizeof(m_InData.Smaxbong));
	nCount = atoi(strTemp);

	strCode = CString(m_InData.Scode, sizeof(m_InData.Scode));
	strCode.TrimLeft(); strCode.TrimRight();

	strTemp = CString(m_InData.Sunit, sizeof(m_InData.Sunit));
	int nUnit = atoi(strTemp);
	if(nUnit > 372)
		nUnit = 372;

	strTemp = CString(m_InData.Sbtngb, sizeof(m_InData.Sbtngb)); // 틱(0)/분(1)/일(2)/주(3)/월(4)
	int nType = atoi(strTemp);

	strImgRgb = CDataConverter::Char2String(m_InData.Simgrgb, sizeof(m_InData.Simgrgb));
	strDate = CString(m_InData.Sdate, sizeof(m_InData.Sdate));//KHD
	CString strNow	= CDataConverter::Char2String(m_InData.nowData, sizeof(m_InData.nowData));
	int nFlag     = CDataConverter::Char2Int(m_InData.Snextgb, sizeof(m_InData.Snextgb));	// 0:
	//////////////////////////////////////////////////////////////////////////
	// Inbound 데이터 작성
	//////////////////////////////////////////////////////////////////////////

	// 차트 데이터

	strData += "41301"; // 종목코드 Input 심볼
	strData += sDS;

	strData += strCode;
	strData += sCELL;//KHD sFS;

	CString stNextData, stNextKey;
	
	//NEXTKEY : KHD
	if(nFlag == 0)
	{
		stNextData = "99999999999999";
		memcpy(m_InData.Snextkey, (LPCSTR)(LPCTSTR)stNextData,stNextData.GetLength());
	}

	_dataH	Datah;
	FillMemory(&Datah, sz_DATAH, 0x20);
	CString stRowData;
	switch(nType)
	{
	case 0:		// 틱
		Datah.dindex = GI_TICK;
		stRowData = _T("?42500");

		strData += "42309";	strData += sDS;	strData += strNow;	strData += sCELL;//당일
		//KHD : NEXT		
		stNextKey = "42324";
		stNextData.Format("%28.28s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // 넥스트 키값 : KHD 
		//END
		if (m_InData.Srecordgb[0] == '1')
		{
			int BeforeDay = CDataConverter::Char2Int(m_InData.Sredate, sizeof(m_InData.Sredate));	// 0:
			CString strTime = strTemp = CString(m_InData.Stime, sizeof(m_InData.Stime));
			CString stBefore;
			
			stBefore = strTime;
			strTime.Format("%d%s",BeforeDay,strTime);//0 : 당일 , 1: 전
			strData += "42341";	strData += sDS;	strData += strTime;	strData += sCELL;
		}
		break;

	case 1:		// 분
		Datah.dindex = GI_MINUTE;
		stRowData = _T("?44500");
		//KHD : NEXT		
		stNextKey = "44324";
		stNextData.Format("%14.14s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // 넥스트 키값 : KHD 
		//END
		// 실/허봉 구분 (0:허봉, 1:실봉)
  		strData += "44308";	strData += sDS;	strData += strImgRgb;	strData += sCELL;
 		strData += "44309"; // 당일만조회 Input 심볼
 		strData += sDS;
 		strTemp = CString(m_InData.nowData, sizeof(m_InData.nowData));
 		strData += strTemp;
 		strData += sCELL;
		if (m_InData.Srecordgb[0] == '1')
		{
			int BeforeDay = CDataConverter::Char2Int(m_InData.Sredate, sizeof(m_InData.Sredate));	// 0:
			CString strTime = strTemp = CString(m_InData.Stime, sizeof(m_InData.Stime));
			CString stBefore;
			
			stBefore = strTime;
			strTime.Format("%d%s",BeforeDay,strTime);//0 : 당일 , 1: 전
			strData += "44341";	strData += sDS;	strData += strTime;	strData += sCELL;
		}
		break;

	case 2:		// 일
		Datah.dindex = GI_DAY;
		nUnit = 1;
		stRowData = _T("?45500");
		//KHD : NEXT		
		stNextKey = "45324";
		stNextData.Format("%8.8s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // 넥스트 키값 : KHD 
		//END
		break;
	case 3:		// 주
		Datah.dindex = GI_WEEK;
		nUnit = 1;
		stRowData = _T("?45500");
		//KHD : NEXT		
		stNextKey = "45324";
		stNextData.Format("%8.8s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // 넥스트 키값 : KHD 
		//END

		break;
	case 4:		// 월
		Datah.dindex = GI_MONTH;
		nUnit = 1;
		stRowData = _T("?45500");
		//KHD : NEXT		
		stNextKey = "45324";
		stNextData.Format("%8.8s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // 넥스트 키값 : KHD 
		//END

		break;
	case 6:		// 초
		// 실/허봉 구분 (0:허봉, 1:실봉)
		Datah.dindex = GI_SEC;
		stRowData = _T("?46500");
		//KHD : NEXT		
		stNextKey = "46324";
		stNextData.Format("%14.14s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // 넥스트 키값 : KHD 
		//END
		// 실/허봉 구분 (0:허봉, 1:실봉)
  		strData += "46308";	strData += sDS;	strData += strImgRgb;	strData += sCELL;
 		strData += "46309"; // 당일만조회 Input 심볼
 		strData += sDS;
 		strTemp = CString(m_InData.nowData, sizeof(m_InData.nowData));
 		strData += strTemp;
 		strData += sCELL;
		if (m_InData.Srecordgb[0] == '1')
		{
			int BeforeDay = CDataConverter::Char2Int(m_InData.Sredate, sizeof(m_InData.Sredate));	// 0:
			CString strTime = strTemp = CString(m_InData.Stime, sizeof(m_InData.Stime));
			CString stBefore;
			
			stBefore = strTime;
			strTime.Format("%d%s",BeforeDay,strTime);//0 : 당일 , 1: 전
			strData += "46341";	strData += sDS;	strData += strTime;	strData += sCELL;
		}
		break;
	}

	//////////////////////////////////////////////////////////////////////////
	// 종목 시세 OutBound : KHD : IBK 맞추기 
						// 단축코드,  이름,  현재가, 전일종가,증감, 등락율, 누적거래량
	char* pFOSymbol[] = { "41301", "41022", "41023", "41336", "41024", "41033", "41027",
						// 상한가,  하한가 ,  시가,  고가,  저가,    	// 전일시가, 전일고가, 전일저가, 누적거래대금
						"41311", "41312", "41029", "41030", "41031","41320", "41321", "41322", "45028","41201", "41307", "41900", "41901" };
					
	for(i = 0; i < sizeof(pFOSymbol)/sizeof(char*); i++)
	{
		strData += pFOSymbol[i];
		strData += sCELL;//sFS;
	}
	strData += stNextKey;
	strData += sCELL;
	//////////////////////////////////////////////////////////////////////////
	// 그리드성 데이터(배열성, 반복데이터) OutBound
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
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
	memset(szData_i, 0x00, sizeof(Datah));
	memcpy(szData_i, (char*)&Datah, sizeof(Datah));
	szData_i[sz_DATAH-1] = 0x00; // KHD : 맨끝에 0x00문자를 집어 넣는다.
	strData += szData_i;


	int nSize;

	switch(nType)
	{
	case 0:		// 틱
		nSize = sizeof(szTickRowFieldOption)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szTickRowFieldOption[i];
			strData += sCOL;
		}
		break;

	case 1:		// 분
		nSize = sizeof(szMinRowFieldOption)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szMinRowFieldOption[i];
			strData += sCOL;
		}
		break;

	case 2:		// 일
	case 3:		// 주
	case 4:		// 월
		nSize = sizeof(szDayRowFieldOption)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szDayRowFieldOption[i];
			strData += sCOL;
		}
		break;
	case 6:		// 초
		nSize = sizeof(szSecRowFieldOption)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szSecRowFieldOption[i];
			strData += sCOL;
		}
		break;
	}
	
	strData += sCELL;//sFS;

	return strData;
}
char* CChartItemFO::Convert2Struct(LPBYTE pData, int nLen, int nKey, int &nDataLen)
{
	KB_p0604_OutRec1_EX stOut;
	//CString strSrc;
	//strSrc.Format(_T("%.*s"), nLen, (char*)pData);
	CString strSrc = (char*)pData;
	CString strNode, strCode, strLock;
	CString strPrice, strVolume, strVolAmt;
	double dOpen, dHigh, dLow, dPreOpen, dPreHigh, dPreLow, dMaxHigh, dMinLow, dPrice;
	double dPrePrice, dChRate=0.0;
	double dopeninterest = 0.0; //미결제 약정  

	memset(&stOut, 0x20, sizeof(stOut));

// 단축코드,  이름,  현재가, 전일종가,증감, 등락율, 누적거래량, 상한가,  하한가 ,  시가,  고가,  저가,
//{ "30301", "30022", "31023", "31356", "31024", "31033", "31027","30354", "30355", "30358", "30359", "30360", };

	strCode = CDataConverter::Parser(strSrc, sCELL); // 종목코드
	
	int nOption = 0;
	strNode = CDataConverter::Parser(strSrc, sCELL); // 종목명
	memcpy(stOut.name, strNode, min(strNode.GetLength(), sizeof(stOut.name)));

	strPrice = CDataConverter::Parser(strSrc, sCELL); // 현재가

	// 종목코드로 시장정보를 얻어와도 스프레드선물이 선물로 취급되어 케이스별 예외처리.(위닉스플랫폼...)
	// 스프레드 선물일 경우 앞자리의 부호는 색상을 나타냄 (+, -, 0)상승/하락/보합
	// 스프레드 선물 현재가는 음수를 갖을 수 있음. 뒷자리 부호는 실제 현재가격의 부호.
	// 상승/하락 색상은 등락률 부호를 사용하므로 따로 처리 안함.
	// 	if( strPrice.Find("0-") > -1 || strPrice.Find("0+") > -1 || strPrice.Find("--") > -1 || strPrice.Find("-+") > -1 ||
	// 		strPrice.Find("+-") > -1 || strPrice.Find("++") > -1)	
	if( !strPrice.IsEmpty() )
		strPrice = strPrice.Mid( 1 );

	dPrice = atof(strPrice);//KHD
	strPrice.Remove(_T('+'));
	strPrice.Remove(_T('.'));
	int nPrice = atoi(strPrice);
	memcpy(stOut.price, strPrice, min(sizeof(stOut.price), strPrice.GetLength()));

//KHD : 코드타입에 따라 기준가 연산이 틀리다.
//	IMasterDataManagerLast* pMasterDataManager = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);
//	int nCodeType = pMasterDataManager->GetCodeTypeFromCode(m_strCode);

	strNode = CDataConverter::Parser(strSrc, sCELL);
	//기준가 
	if(m_nCodeType == CDRCodeTypeInfo::CODE_FUOPT_COMMODITY)
	{
		dPrePrice = (atof(strNode))/100;
	}
	else
	{
		dPrePrice = atof(strNode);	// 전일종가
		strNode.Format(_T("%d"), dPrePrice*100);
	}
//END 
	memcpy(stOut.preprice, strNode, min(sizeof(stOut.preprice), strNode.GetLength()));

	strNode = CDataConverter::Parser(strSrc, sCELL); // 전일증감
	strNode = strNode.Mid(1);
	strNode.Remove(_T('.'));
	memcpy(stOut.change, strNode, min(sizeof(stOut.change), strNode.GetLength()));
	
	strNode = CDataConverter::Parser(strSrc, sCELL); // 등락율
	dChRate = atof(strNode);
	strNode.Remove(_T('+'));
	strNode.Remove(_T('.'));
	memcpy(stOut.chrate, strNode, min(sizeof(stOut.chrate), strNode.GetLength()));

	strVolume = CDataConverter::Parser(strSrc, sCELL); // 거래량
	memcpy(stOut.volume, strVolume, min(sizeof(stOut.volume), strVolume.GetLength()));

	// Make sign value
	if (m_nCodeType == CDRCodeTypeInfo::CODE_FUOPT_COMMODITY)
	{
		strNode = CDataConverter::Parser(strSrc, sCELL);	dMaxHigh = atof(strNode); // 상한가
		strNode = CDataConverter::Parser(strSrc, sCELL);	dMinLow = atof(strNode); // 하한가
	}
	else
	{
		strNode = CDataConverter::Parser(strSrc, sCELL);	dMaxHigh = fabs(atof(strNode)); // 상한가
		strNode = CDataConverter::Parser(strSrc, sCELL);	dMinLow = fabs(atof(strNode)); // 하한가
	}

	if(abs(nPrice) == (int)(dMaxHigh*100))		stOut.sign[0] = '1'; // UP_LIMIT
	else if(abs(nPrice) == (int)(dMinLow*100))	stOut.sign[0] = '4'; // DOWN_LIMIT
	else if(dChRate < 0)						stOut.sign[0] = '5'; // DOWN
	else if(dChRate == 0)						stOut.sign[0] = '3'; // SAME (BOHAP)
	else										stOut.sign[0] = '2'; // UP

	strNode = CDataConverter::Parser(strSrc, sCELL);	dOpen = atof(strNode.Mid(1));		// 시가
	strNode = CDataConverter::Parser(strSrc, sCELL);	dHigh = atof(strNode.Mid(1));		// 고가
	strNode = CDataConverter::Parser(strSrc, sCELL);	dLow  = atof(strNode.Mid(1));		// 저가
	strNode = CDataConverter::Parser(strSrc, sCELL);	dPreOpen = atof(strNode);	// 전일시가
	strNode = CDataConverter::Parser(strSrc, sCELL);	dPreHigh = atof(strNode);	// 전일고가
	strNode = CDataConverter::Parser(strSrc, sCELL);	dPreLow  = atof(strNode);	// 전일저가

	//>>[A00000651][A00000652]누적거래대금.
	strNode = CDataConverter::Parser(strSrc, sCELL);
	strVolAmt = strNode;
	strVolAmt += _T("00000000");
	//<<[A00000651][A00000652]
	//KHD : 미결제 약정 
	strNode = CDataConverter::Parser(strSrc, sCELL);
	memcpy(stOut.openinterest, strNode, min(sizeof(stOut.openinterest), strNode.GetLength()));

	CString strRealState = CDataConverter::Parser(strSrc, sCELL);   // 선옵 장운영정보(31307, 41307)  	           
																	// 00 : 장개시전                  
																	// 10 : 시가단일가                
																	// 11 : 시간단일가 연장           
																	// 20 : 장중단일가                
																	// 21 : 장중단일가연장            
																	// 30 : 종가단일가                
																	// 40 : 장중                      
																	// 80 : 단위매매체결(주식관련상품)
																	// 90 : 거래정지                  
																	// 99 : 장종료                    
	CString strJangStart = CDataConverter::Parser(strSrc, sCELL);   // 장운영시간시작(HHMM)
	CString strJangEnd   = CDataConverter::Parser(strSrc, sCELL);   // 장운영시간종료(HHMM)
	strJangStart.TrimRight();
	strJangEnd.TrimRight();
	BOOL bSetReal = FALSE;
	if(strRealState.Compare("00") == 0 || strRealState.Compare("10") == 0 || strRealState.Compare("11") == 0)
		bSetReal = TRUE;
	
	if(strJangStart.GetLength() == 4)	strJangStart += "00";
	
	
	CString strResetTime = "";
	if(bSetReal)
	{
		strResetTime = strJangStart;
	}
	CString strBaseTime = strJangStart;
	CString strMarketTime;
	strMarketTime.Format("%4.4s00|%4.4s00", strJangStart, strJangEnd);


	//KHD : NextKey
	CString strNextKey = CDataConverter::Parser(strSrc, sCELL);

	int nSize, nCnt, nType;
	strNode = CString(m_InData.Sbtngb, sizeof(m_InData.Sbtngb)); // 틱(0)/분(1)/일(2)/주(3)/월(4)
	nType = atoi(strNode);
	if(nType == 0) // 틱
		nSize = sizeof(szTickRowFieldFO)/sizeof(char*);
	else if(nType == 1) //분
		nSize = sizeof(szMinRowFieldFO)/sizeof(char*);
	else if(nType == 6) //초
		nSize = sizeof(szSecRowFieldFO)/sizeof(char*);
	else		// 일, 주, 월
		nSize = sizeof(szDayRowFieldFO)/sizeof(char*);
	// 그리드 헤더


 	GRAPH_IO gridOut;
 	strNode = strSrc.Left(sizeof(gridOut));
 	
 	memcpy(&gridOut, strNode, sizeof(gridOut)); 
 	
 	strSrc = strSrc.Mid(sizeof(gridOut));
 
	if (strNextKey == "99999999999999" || atoi(strNextKey) == 0)	// 9가 14자리면 다음키가 없는것.
		memset(stOut.nkey, 0x20, sizeof(stOut.nkey));
	else
		memcpy(stOut.nkey, (LPCTSTR)strNextKey, min(sizeof(stOut.nkey),strNextKey.GetLength()));

	strSrc.Remove('.');
	
	CStringArray arrNode;
	int nNodeIndex = 1;
	strSrc.TrimLeft(); strSrc.TrimRight();

	int nEnd = strSrc.Find(sFS);
	if(nEnd !=-1 )
	{
		strSrc.Delete(nEnd +1, strSrc.GetLength() - nEnd);
	}
	else
	{	
		strSrc+=sCELL;
		strSrc+=sROW;
		strSrc+=sFS;
		strSrc+=sCELL;
	}
	
	while(strSrc.IsEmpty() == FALSE)
	{
		strNode = CDataConverter::Parser(strSrc, (nNodeIndex == nSize)?sROW:sCELL);

		if(strNode != sROW && strNode != sCELL)
			arrNode.Add(strNode);

		if(nNodeIndex == nSize) nNodeIndex = 1;
		else nNodeIndex++;
	}

	// 전체 데이터 수 계산
	nCnt = arrNode.GetSize()/ nSize;
//	nCnt	=	atoi(CString(gridOut.nrec, sizeof(gridOut.nrec)));//CString(stFirstBong.close, sizeof(stFirstBong.close))
	strNode.Format("%05d", nCnt);
	memcpy(stOut.fcnt, strNode, sizeof(stOut.fcnt));

	// 차트 봉 데이터
	int nBongSize = sizeof(KB_p0604_OutRec2);
	int nBufSize = sizeof(KB_p0604_OutRec1_EX) + nBongSize * nCnt;
	int	nPosBong = sizeof(KB_p0604_OutRec1_EX);

	nDataLen = nBufSize;
	
	CString strDate , strTime;
	char* pDataBuf = new char[nBufSize+1];
	memset(pDataBuf, 0x00, sizeof(nBufSize+1));

	KB_p0604_OutRec2 stFirstBong, stBong;
	double dValue=0;
	int nIndex = 0;
	int nRow = 0;
	for ( nRow = 0; nRow < nCnt; nRow++)
	{
		memset(&stBong, 0x20, sizeof(stBong));
		
		// date
		strDate = arrNode.GetAt(nIndex++);

		// time : 틱,초,분일 때
		if(nType < 2 || nType == 6) strTime = arrNode.GetAt(nIndex++); 
		
		if(nType == 0) // DDHHMMSS
			strNode = strDate.Right(2) + strTime;
		else if(nType == 1) //MMDDHHMM
			strNode = strDate.Right(4) + strTime.Left(4);
		else if(nType == 4) //YYYYMM
			strNode = strDate.Left(6);
		else if(nType == 6) //DDHHMMSS
			strNode = strDate.Right(2) + strTime.Left(6);
		else 
			strNode = strDate;
		if(strNode.GetLength() <3) continue;

		memcpy(stBong.date, strNode, min(sizeof(stBong.date), strNode.GetLength()));

		// 종가/체결가
		strNode = arrNode.GetAt(nIndex++);
		memcpy(stBong.close, strNode, min(sizeof(stBong.close), strNode.GetLength()));
		// 시가
		strNode = arrNode.GetAt(nIndex++);
		memcpy(stBong.open, strNode, min(sizeof(stBong.open), strNode.GetLength()));
		// 고가
		strNode = arrNode.GetAt(nIndex++);
		memcpy(stBong.high, strNode, min(sizeof(stBong.high), strNode.GetLength()));
		// 저가
		strNode = arrNode.GetAt(nIndex++);
		memcpy(stBong.low, strNode, min(sizeof(stBong.low), strNode.GetLength()));
		// 거래량
		strNode = arrNode.GetAt(nIndex++);
		memcpy(stBong.volume, strNode, min(sizeof(stBong.volume), strNode.GetLength()));
		//@Solomon	091202SM123	-->
		// 거래대금
		strNode = arrNode.GetAt(nIndex++);
		//dValue = atof(strNode)*100000000;	//[A00000408]거래대금 12 -> 18. (6 -> 8개)
		//strNode.Format(_T("%.0f"), dValue);
		if(nType < 2 || nType == 6)	//틱초분.
		{
		}
		else
		{
			strNode += "00000000";
		}
		memcpy(stBong.value, strNode, min(sizeof(stBong.value), strNode.GetLength()));
		//@Solomon	091202SM123	<--
		//미결제약정 
		strNode = arrNode.GetAt(nIndex++);
		memcpy(stBong.openinterest, strNode, min(sizeof(stBong.openinterest), strNode.GetLength()));

		// 데이터 버퍼에 복사, 일주월은 초,분,틱과 순서가 반대
		//	memcpy(&pDataBuf[nBufSize - (nRow+1)*nBongSize], &stBong, nBongSize);//KHD : 반대로 와서 반대로 세팅 
		memcpy(&pDataBuf[nPosBong + nRow*nBongSize], &stBong, nBongSize);

		if (nRow == 0)
			memcpy(&stFirstBong, &stBong, sizeof(stBong));
	}

	// 20130222 이상신 : 주봉일경우 마지막 데이터의 일자를 설정해 준다.(주 첫 영업일 관련)
	if (nCnt > 0 && nType == 3 && m_InData.Snextgb[0] != _T('1'))
		m_bojoInput.m_sLastDataTime = CString(stBong.date, sizeof(stBong.date));
	else
		m_bojoInput.m_sLastDataTime = _T("");

	// 데이터 가공 : 타사이트는 서버쪽에서 처리
	int nDataType = int(m_InData.SdataType[0] - '0'); 
	if(nDataType > 0)
	{
		CString strData(stFirstBong.close, sizeof(stFirstBong.close));
		int nFirstClose = atoi(strData);
		double dDenom; //분모
		
		switch(nDataType)
		{
		case 1:	dDenom = nFirstClose;			break;	// 첫종가 기준
		case 2:	dDenom = 1;						break;	// $   선옵은 처리없음
		case 3:	dDenom = dOpen*100;		break;	// 당일시가
		case 4:	dDenom = nPrice;				break;	// 현재가
		case 5:	dDenom = dPrePrice*100;	break;	// 전일종가
		}

//		전일종가가 안들어 올경우 비정상적으로 그려진다.
//		테스트하기 위해 시가로 셋팅하여 그려보려 한것임.
//		if (dDenom == 0)
//			dDenom = atoi(strOpen);

		for(nRow = 0; nRow < nCnt; nRow++)
		{
			if(nDataType == 2) break;

			memcpy(&stBong, &pDataBuf[nPosBong + nRow*nBongSize], nBongSize);
			// 시가
			strData = CDataConverter::Get_PercentData(stBong.open, dDenom, sizeof(stBong.open));
			memcpy(stBong.open, strData, min(strData.GetLength(), sizeof(stBong.open)));
			// 고가
			strData = CDataConverter::Get_PercentData(stBong.high, dDenom, sizeof(stBong.high));
			memcpy(stBong.high, strData, min(strData.GetLength(), sizeof(stBong.high)));
			// 저가
			strData = CDataConverter::Get_PercentData(stBong.low, dDenom, sizeof(stBong.low));
			memcpy(stBong.low, strData, min(strData.GetLength(), sizeof(stBong.low)));
			// 종가
			strData = CDataConverter::Get_PercentData(stBong.close, dDenom, sizeof(stBong.close));
			memcpy(stBong.close, strData, min(strData.GetLength(), sizeof(stBong.close)));

			memcpy(&pDataBuf[nPosBong + nRow*nBongSize], &stBong, nBongSize);
		}
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 차트보조 메시지 만들기
	CChartBozoManager bojoMng;
	m_bojoInput.m_sShcode = strCode;
//	m_bojoInput.m_sPrice = strPrice;		// 현재가
	m_bojoInput.m_sPrice.Format(_T("%.2f"), dPrice);	// 현재가 
	m_bojoInput.m_sVolume = strVolume;		// 거래량
// 		m_bojoInput.m_sUpLmtPrice.Format(_T("%.0f"), dMaxHigh*100);	// 상한가
// 		m_bojoInput.m_sDnLmtPrice.Format(_T("%.0f"), dMinLow*100);	// 하한가
// 		m_bojoInput.m_sOpen.Format(_T("%.0f"), dOpen*100);		// 시가
// 		m_bojoInput.m_sHigh.Format(_T("%.0f"), dHigh*100);		// 고가
// 		m_bojoInput.m_sLow.Format(_T("%.0f"), dLow*100);			// 저가
// 		//@Solomon:091123SM068 -->
// 		m_bojoInput.m_sPreOpen.Format(_T("%.0f"), dPreOpen*100);		// 전일시가
// 		m_bojoInput.m_sPreHigh.Format(_T("%.0f"), dPreHigh*100);		// 전일고가
// 		m_bojoInput.m_sPreLow.Format(_T("%.0f"), dPreLow*100);		// 전일저가
// 		//@Solomon:091123SM068 <--
// 		m_bojoInput.m_sPreprice.Format(_T("%.0f"), dPrePrice*100);	// 전일종가
	m_bojoInput.m_sUpLmtPrice.Format(_T("%.2f"), dMaxHigh);	// 상한가
	m_bojoInput.m_sDnLmtPrice.Format(_T("%.2f"), dMinLow);	// 하한가
	m_bojoInput.m_sOpen.Format(_T("%.2f"), dOpen);		// 시가
	m_bojoInput.m_sHigh.Format(_T("%.2f"), dHigh);		// 고가
	m_bojoInput.m_sLow.Format(_T("%.2f"), dLow);			// 저가
	//@Solomon:091123SM068 -->
	m_bojoInput.m_sPreOpen.Format(_T("%.2f"), dPreOpen);		// 전일시가
	m_bojoInput.m_sPreHigh.Format(_T("%.2f"), dPreHigh);		// 전일고가
	m_bojoInput.m_sPreLow.Format(_T("%.2f"), dPreLow);		// 전일저가
	//@Solomon:091123SM068 <--
	m_bojoInput.m_sPreprice.Format(_T("%.2f"), dPrePrice);	// 전일종가
//	m_bojoInput.m_sPreVolume = strPreVolume;	// 전일거래량
	m_bojoInput.m_sStartTime = strResetTime;	// 거래시작시간(HHMMSS) <<!>>
	m_bojoInput.m_sFirstClose = CString(stFirstBong.close, sizeof(stFirstBong.close)) ;	// 조회중 첫봉의 종가.
	m_bojoInput.m_sFirstClose.TrimRight();

	m_bojoInput.m_sDataType = _T("0");		//	고정. 차후 필요시 수정.
	m_bojoInput.m_sMarket = _T("1");			//	고정. 차후 필요시 수정.
	m_bojoInput.m_sNxtFlg = m_InData.Snextgb[0];

	m_bojoInput.m_sTerm.Format("%d", nType);
	m_bojoInput.m_sTick = CString(m_InData.Sunit, sizeof(m_InData.Sunit));
	if(nType == 1)
	{
		//{{JS.Kim_20100903 Solomon 60분봉 이상인 경우 시간으로 단위 변경하여야 함
		int nTick = atoi(m_bojoInput.m_sTick);
		if( nTick >= 60 )
			m_bojoInput.m_sTick.Format("%d", (nTick/ 60) * 100 + nTick % 60);
		//}}
		m_bojoInput.m_szBaseTime	= strBaseTime;	
	}
	else if (nType >= 2 && nType <= 5)		//	일,주,월,년
		m_bojoInput.m_sTick = _T("1");

	m_bojoInput.m_sVolAmt = strVolAmt;	//[A00000651][A00000652]누적거래대금.
	m_bojoInput.m_szMarketTime	= strMarketTime;	//장운영정보.

	CString strBozo;
	char szKey[4];
	memset(szKey, 0x00, sizeof(szKey));
	sprintf(szKey, _T("%03d"), 30); // 103은 선옵

	nSize = bojoMng.GetChartBozoMsg(szKey, &m_bojoInput, strNode, nOption);
	memcpy(stOut.bojomsg, strNode, nSize);
	memcpy(stOut.bojomsg, "0396", 4);

	memcpy(pDataBuf, &stOut, sizeof(stOut));
	return pDataBuf;
}

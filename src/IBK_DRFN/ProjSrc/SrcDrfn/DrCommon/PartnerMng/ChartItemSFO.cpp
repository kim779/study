// ChartItemSFO.cpp: implementation of the CChartItemSFO class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PartnerMng.h"
#include "ChartItemSFO.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
char* szTickRowFieldSFO[] =	{"62302", "62034", "62023", "62029", "62030", "62031", "62032", "65028","65201", };
char* szSecRowFieldSFO[] =	{"66302", "66034", "66023", "66029", "66030", "66031", "66032", "65028","65201", };
char* szMinRowFieldSFO[] =	{"64302", "64034", "64023", "64029", "64030", "64031", "64032", "65028","65201", };
char* szDayRowFieldSFO[] =	{"65302",           "65023", "65029", "65030", "65031", "65027", "65028","65201", };

CChartItemSFO::CChartItemSFO()
{
	m_bSet = FALSE;
}

CChartItemSFO::~CChartItemSFO()
{

}

void CChartItemSFO::SetInData(KB_d1013_InRec1* pData)
{
	memcpy(&m_InData, pData, sizeof(KB_d1013_InRec1));
	m_bSet = TRUE;
}

// MUltiChart에서의 조회 구조체를 받아서 FID 형태로 변환
// 구성 : 차트조회Grid Inbound + 시세 FID Inbound + GridHeader + Grid Outbound
// winix 플랫폼에서 배열형태로 내려오는 데이터를 Grid 라고 부른다.
CString CChartItemSFO::Convert()
{
	if(!m_bSet) return "";

	CString strData, strCode, strTemp, strDate, strImgRgb;
	int i, nCount;
	
	strTemp = CString(m_InData.Smaxbong, sizeof(m_InData.Smaxbong));
	nCount = atoi(strTemp);

	strCode = CString(m_InData.Scode, sizeof(m_InData.Scode));
	strCode.TrimLeft(); strCode.TrimRight();

	strTemp = CString(m_InData.Sunit, sizeof(m_InData.Sunit));
	int nUnit = atoi(strTemp);

	strTemp = CString(m_InData.Sbtngb, sizeof(m_InData.Sbtngb)); // 틱(0)/분(1)/일(2)/주(3)/월(4)
	int nType = atoi(strTemp);

	strDate	= CDataConverter::Char2String(m_InData.Sdate, sizeof(m_InData.Sdate));
	strImgRgb = CDataConverter::Char2String(m_InData.Simgrgb, sizeof(m_InData.Simgrgb));
	int nFlag     = CDataConverter::Char2Int(m_InData.Snextgb, sizeof(m_InData.Snextgb));	// 0:
	CString strNow	= CDataConverter::Char2String(m_InData.nowData, sizeof(m_InData.nowData));

	//////////////////////////////////////////////////////////////////////////
	// Inbound 데이터 작성
	//////////////////////////////////////////////////////////////////////////

	// 차트 데이터
	strData += "61301"; // 종목코드 Input 심볼
	strData += sDS;

	strData += strCode;
	strData += sCELL;

	CString stNextData, stNextKey;
	
	//NEXTKEY : KHD
	if(nFlag == 0)
	{
		stNextData = "99999999999999";
		memcpy(m_InData.Snextkey, (LPCSTR)(LPCTSTR)stNextData,stNextData.GetLength());
	}

	CString stRowData;
	_dataH	Datah;
	FillMemory(&Datah, sz_DATAH, 0x20);

	switch(nType)
	{
	case 0:		// 틱
		Datah.dindex = GI_TICK;
		stRowData = _T("?62500");
		strData += "62309";	strData += sDS;	strData += strNow;	strData += sCELL;
		//KHD : NEXT		
		stNextKey = "62324";
		stNextData.Format("%28.28s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // 넥스트 키값 : KHD 
		//END
		break;

	case 1:		// 분
		//
		Datah.dindex = GI_MINUTE;
		stRowData = _T("?64500");	
		//KHD : NEXT		
		stNextKey = "64324";
		stNextData.Format("%14.14s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // 넥스트 키값 : KHD 
		//END
		strData += "64308";	strData += sDS;	strData += strImgRgb;	strData += sCELL;
		strData += "64309"; // 당일만조회 Input 심볼
		strData += sDS;
		strTemp = CString(m_InData.nowData, sizeof(m_InData.nowData));
		strData += strTemp;
 		strData += sCELL;

		// 20130530 이상신 : 월물간 갭보정 추가(IBK) (Issue No : 0006572)
		strData += "64339";		strData += sDS;
		strData += CString(m_InData.SgapRev, sizeof(m_InData.SgapRev));	strData += sCELL;

		break;

	case 2:		// 일
		Datah.dindex = GI_DAY;
		nUnit = 1;
		stRowData = _T("?65500");
		//KHD : NEXT		
		stNextKey = "65324";
		stNextData.Format("%8.8s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // 넥스트 키값 : KHD 
		//END
		// 20130530 이상신 : 월물간 갭보정 추가(IBK) (Issue No : 0006572)
		strData += "65339";		strData += sDS;
		strData += CString(m_InData.SgapRev, sizeof(m_InData.SgapRev));	strData += sCELL;
		break;
	case 3:		// 주
		Datah.dindex = GI_WEEK;
		nUnit = 1;
		stRowData = _T("?65500");
		//KHD : NEXT		
		stNextKey = "65324";
		stNextData.Format("%8.8s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // 넥스트 키값 : KHD 
		//END
		// 20130530 이상신 : 월물간 갭보정 추가(IBK) (Issue No : 0006572)
		strData += "65339";		strData += sDS;
		strData += CString(m_InData.SgapRev, sizeof(m_InData.SgapRev));	strData += sCELL;
		break;
	case 4:		// 월
		Datah.dindex = GI_MONTH;
		nUnit = 1;
		stRowData = _T("?65500");
		//KHD : NEXT		
		stNextKey = "65324";
		stNextData.Format("%8.8s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // 넥스트 키값 : KHD 
		//END
		// 20130530 이상신 : 월물간 갭보정 추가(IBK) (Issue No : 0006572)
		strData += "65339";		strData += sDS;
		strData += CString(m_InData.SgapRev, sizeof(m_InData.SgapRev));	strData += sCELL;
		break;
	case 6:		// 초
		Datah.dindex = GI_SEC;
		stRowData = _T("?66500");	
		//KHD : NEXT		
		stNextKey = "66324";
		stNextData.Format("%14.14s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // 넥스트 키값 : KHD 
		//END
		strData += "66308";	strData += sDS;	strData += strImgRgb;	strData += sCELL;
		strData += "66309"; // 당일만조회 Input 심볼
		strData += sDS;
		strTemp = CString(m_InData.nowData, sizeof(m_InData.nowData));
		strData += strTemp;
 		strData += sCELL;

		// 20130530 이상신 : 월물간 갭보정 추가(IBK) (Issue No : 0006572)
		strData += "66339";		strData += sDS;
		strData += CString(m_InData.SgapRev, sizeof(m_InData.SgapRev));	strData += sCELL;

		break;
	}

	//////////////////////////////////////////////////////////////////////////
	// 종목 시세 OutBound
						// 단축코드,  이름,  현재가, 전일종가,증감, 등락율, 누적거래량
	char* pFOSymbol[] = { "61301", "61022", "61023", "61336", "61024", "61033", "61027",
						// 상한가,  하한가 ,  시가,  고가,  저가,
							"61311", "61312", "61029", "61030", "61031",
						// 전일시가, 전일고가, 전일저가, 누적거래대금, 기준가//-> IBK에 요청
							//"61320", "61321", "61322", "61028", "61038"};
							"61320", "61321", "61322",            "61336", "61307", "61900", "61901"};
//							"30354", "30355", "30358", "30359", "30360", };

	for(i = 0; i < sizeof(pFOSymbol)/sizeof(char*); i++)
	{
		strData += pFOSymbol[i];
		strData += sCELL;
	}

	strData += stNextKey;
	strData += sCELL;
	//////////////////////////////////////////////////////////////////////////
	// 그리드성 데이터(배열성, 반복데이터) OutBound함
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Row Data 구성
	int nSize;
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
	switch(nType)
	{
	case 0:		// 틱
		nSize = sizeof(szTickRowFieldSFO)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szTickRowFieldSFO[i];
			strData += sCOL;
		}
		break;

	case 1:		// 분
		nSize = sizeof(szMinRowFieldSFO)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szMinRowFieldSFO[i];
			strData += sCOL;
		}
		break;

	case 2:		// 일
	case 3:		// 주
	case 4:		// 월
		nSize = sizeof(szDayRowFieldSFO)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szDayRowFieldSFO[i];
			strData += sCOL;
		}
		break;
	case 6:		// 초
		nSize = sizeof(szSecRowFieldSFO)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szSecRowFieldSFO[i];
			strData += sCOL;
		}
		break;
	}
	
	strData += sCELL;

	return strData;
}

char* CChartItemSFO::Convert2Struct(LPBYTE pData, int nLen, int nKey, int &nDataLen)
{
	KB_d1013_OutRec1_EX stOut;
	CString strSrc;
	strSrc.Format(_T("%.*s"), nLen, (char*)pData);
	CString strNode, strCode, strLock;
	CString strPrice, strPrePrice, strVolume, strVolAmt;
	CString strHigh, strLow, strOpen, strHighLimit, strLowLimit;
	CString strPreHigh, strPreLow, strPreOpen, strStandard;
	double dChRate=0.0;

	memset(&stOut, 0x20, sizeof(stOut));

// 단축코드,  이름,  현재가, 전일종가,증감, 등락율, 누적거래량
//{ "30301", "30022", "31023", "31356", "31024", "31033", "31027",
// 상한가,  하한가 ,  시가,  고가,  저가,
//	"30354", "30355", "30358", "30359", "30360", };

	strCode = CDataConverter::Parser(strSrc, sCELL); // 종목코드

	strNode = CDataConverter::Parser(strSrc, sCELL); // 종목명
	memcpy(stOut.name, strNode, min(strNode.GetLength(), sizeof(stOut.name)));

	strPrice = CDataConverter::Parser(strSrc, sCELL); // 현재가
	strPrice.Remove(_T('+')); strPrice.Remove(_T('-')); strPrice.Remove(_T('.'));
	int nPrice = atoi(strPrice);
	memcpy(stOut.price, strPrice, min(sizeof(stOut.price), strPrice.GetLength()));

	strPrePrice = CDataConverter::Parser(strSrc, sCELL); // 전일종가
	strPrePrice.Remove(_T('.'));
	memcpy(stOut.preprice, strPrePrice, min(sizeof(stOut.preprice), strPrePrice.GetLength()));

	strNode = CDataConverter::Parser(strSrc, sCELL); // 전일증감
	strNode = strNode.Mid(1);
	strNode.Remove(_T('.'));
	memcpy(stOut.change, strNode, min(sizeof(stOut.change), strNode.GetLength()));
	
	strNode = CDataConverter::Parser(strSrc, sCELL); // 등락율
	dChRate = atof(strNode);
	strNode.Remove(_T('+'));/*strNode.Remove(_T('-'));*/strNode.Remove(_T('.'));
	memcpy(stOut.chrate, strNode, min(sizeof(stOut.chrate), strNode.GetLength()));

	strVolume = CDataConverter::Parser(strSrc, sCELL); // 거래량
	memcpy(stOut.volume, strVolume, min(sizeof(stOut.volume), strVolume.GetLength()));

	// Make sign value
	int nHigh, nLow;
	strHighLimit = CDataConverter::Parser(strSrc, sCELL); // 상한가
	strHighLimit.Remove(_T('.'));
	nHigh = atoi(strHighLimit);

	strLowLimit = CDataConverter::Parser(strSrc, sCELL); // 하한가
	strLowLimit.Remove(_T('.'));
	nLow = atoi(strLowLimit);

	if(abs(nPrice) == nHigh)		stOut.sign[0] = '1'; // UP_LIMIT
	else if(abs(nPrice) == nLow)	stOut.sign[0] = '4'; // DOWN_LIMIT
	else if(dChRate < 0)			stOut.sign[0] = '5'; // DOWN
	else if(dChRate == 0)			stOut.sign[0] = '3'; // SAME (BOHAP)
	else							stOut.sign[0] = '2'; // UP

	strOpen = CDataConverter::Parser(strSrc, sCELL); // 시가
	strHigh = CDataConverter::Parser(strSrc, sCELL); // 고가
	strLow = CDataConverter::Parser(strSrc, sCELL); // 저가
	strOpen.Remove(_T('.'));
	strHigh.Remove(_T('.'));
	strLow.Remove(_T('.'));
 
	strPreOpen = CDataConverter::Parser(strSrc, sCELL);		// 전일시가
	strPreOpen.Remove(_T('+'));	strPreOpen.Remove(_T('-'));
	strPreHigh = CDataConverter::Parser(strSrc, sCELL);		// 전일고가
	strPreHigh.Remove(_T('+'));	strPreHigh.Remove(_T('-'));
	strPreLow = CDataConverter::Parser(strSrc, sCELL);		// 전일저가
	strPreLow.Remove(_T('+'));	strPreLow.Remove(_T('-'));

	//>>[A00000651][A00000652]누적거래대금.
//	strNode = CDataConverter::Parser(strSrc, sCELL);
//	strVolAmt = strNode;
	strVolAmt += _T("00000");
	//<<[A00000651][A00000652
	strStandard = CDataConverter::Parser(strSrc, sCELL);//기준가 ;

	CString strRealState = CDataConverter::Parser(strSrc, sCELL);   //주식선물 장운영정보(61307)	
																	// 00 : 장개시전
																	// 10 : 시가단일가       
																	// 11 : 시가단일가연장   
																	// 20 : 장중단일가       
																	// 21 : 장중단일가 연장  
																	// 30 : 종가단일가       
																	// 40 : 장중            
																	// 99 : 장종료           
	CString strJangStart = CDataConverter::Parser(strSrc, sCELL);   // 장운영시간시작(HHMM)
	CString strJangEnd   = CDataConverter::Parser(strSrc, sCELL);   // 장운영시간종료(HHMM)
	strJangStart.TrimRight();
	strJangEnd.TrimRight();
// 	{
// 		CString strDebug;
// 		strDebug.Format("[DRCS]주식선물 종목코드[%s] 장운영정보:[%s]", strCode, strRealState);
// 		OutputDebugString(strDebug);
// 	}

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
		nSize = sizeof(szTickRowFieldSFO)/sizeof(char*);
	else if(nType == 1) //분
		nSize = sizeof(szMinRowFieldSFO)/sizeof(char*);
	else if(nType == 6) //초
		nSize = sizeof(szSecRowFieldSFO)/sizeof(char*);
	else		// 일, 주, 월
		nSize = sizeof(szDayRowFieldSFO)/sizeof(char*);
	
	// 그리드 헤더
	GRAPH_IO gridOut;
	strNode = strSrc.Left(sizeof(gridOut));
	memcpy(&gridOut, strNode, sizeof(gridOut)); 
	
	strSrc = strSrc.Mid(sizeof(gridOut));

	if (strNextKey =="99999999999999" || atoi(strNextKey) == 0)// 9가 14자리면 다음키가 없는것.  
		memset(stOut.nkey, 0x20, sizeof(stOut.nkey));
	else
		memcpy(stOut.nkey, (LPCTSTR)strNextKey, min(sizeof(stOut.nkey),strNextKey.GetLength()));

	strSrc.Remove('.');
	
	CStringArray arrNode;
	int nNodeIndex = 1;
	while(strSrc.IsEmpty() == FALSE)
	{
		strNode = CDataConverter::Parser(strSrc, (nNodeIndex == nSize)?sROW:sCELL);
		arrNode.Add(strNode);

		if(nNodeIndex == nSize) nNodeIndex = 1;
		else nNodeIndex++;
	}

	// 전체 데이터 수 계산
	nCnt = arrNode.GetSize()/ nSize;
	strNode.Format("%05d", nCnt);
	memcpy(stOut.fcnt, strNode, sizeof(stOut.fcnt));

	// 차트 봉 데이터
	int nBongSize = sizeof(KB_d1013_OutRec2);
	int nBufSize = sizeof(KB_d1013_OutRec1_EX) + nBongSize * nCnt;
	int	nPosBong = sizeof(KB_d1013_OutRec1_EX);

	nDataLen = nBufSize;
	
	CString strDate = "00000000", strTime;
	char* pDataBuf = new char[nBufSize+1];
	KB_d1013_OutRec2 stFirstBong, stBong;

	double dValue=0;
	int nIndex = 0, nRow;
	for(nRow = nCnt-1; nRow >= 0; nRow--)
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
		// 거래대금
		//@Solomon	091202SM123	-->
		strNode = arrNode.GetAt(nIndex++);
		//dValue = atof(strNode)*1000000;
		//dValue = atof(strNode)*100000;	//[A00000408]거래대금 12 -> 18. (6 -> 5개)
		//strNode.Format(_T("%.0f"), dValue);
		strNode += "00000";
// 		if(nRow==nCnt-1)
// 		{
// 			CString szDebug;
// 			szDebug.Format("[SMalzio] SFO거래대금=%s", strNode);
// 			OutputDebugString(szDebug);
// 		}
		memcpy(stBong.value, strNode, min(sizeof(stBong.value), strNode.GetLength()));
		//@Solomon	091202SM123	<--
		//미결제약정 
		strNode = arrNode.GetAt(nIndex++);
		memcpy(stBong.openinterest, strNode, min(sizeof(stBong.openinterest), strNode.GetLength()));

		// 데이터 버퍼에 복사, 일주월은 초,분,틱과 순서가 반대
		//memcpy(&pDataBuf[nPosBong + nRow*nBongSize], &stBong, nBongSize);
		memcpy(&pDataBuf[nBufSize - (nRow+1)*nBongSize], &stBong, nBongSize);//KHD : 반대로 와서 반대로 세팅 
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
		case 1:	dDenom = nFirstClose; break;	// 첫종가 기준
		case 2:	dDenom = 1; break;				// $   선옵은 처리없음
		case 3:	dDenom = atof(strOpen); break;	// 당일시가
		case 4:	dDenom = nPrice; break;			// 현재가
		case 5:	dDenom = atof(strPrePrice); break;		// 전일종가
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
	m_bojoInput.m_sPrice = strPrice;		// 현재가
	m_bojoInput.m_sVolume = strVolume;		// 거래량
	strHighLimit.Remove('-');
	strLowLimit.Remove('-');
	strLow.Remove('-');
	m_bojoInput.m_sUpLmtPrice = strHighLimit;	// 상한가
	m_bojoInput.m_sDnLmtPrice = strLowLimit;	// 하한가
	m_bojoInput.m_sOpen = strOpen;		// 시가
	m_bojoInput.m_sHigh = strHigh;		// 고가
	m_bojoInput.m_sLow = strLow;			// 저가
	//@Solomon:091123SM068 -->
	m_bojoInput.m_sPreOpen = strPreOpen;		// 전일시가
	m_bojoInput.m_sPreHigh = strPreHigh;		// 전일고가
	m_bojoInput.m_sPreLow = strPreLow;		// 전일저가
	//@Solomon:091123SM068 <--
	m_bojoInput.m_sPreprice = strPrePrice;	// 전일종가
//	m_bojoInput.m_sPreVolume = strPreVolume;	// 전일거래량
	m_bojoInput.m_sStartTime = strResetTime;	// 거래시작시간(HHMMSS) <<!>>
	m_bojoInput.m_sFirstClose = CString(stFirstBong.close, sizeof(stFirstBong.close)) ;	// 조회중 첫봉의 종가.
	m_bojoInput.m_sFirstClose.TrimRight();
	m_bojoInput.m_sBaseValue = strPrePrice;//strStandard;

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
	// 주식선옵은 FID 에서 선옵과 같은것으로 사용하나,
	// 보조메시지 옵션은 다르므로 134은 FID에서 사용하지 않는 번호를 임의로 사용하였음.
	sprintf(szKey, _T("%03d"), 134);
	
	nSize = bojoMng.GetChartBozoMsg(szKey, &m_bojoInput, strNode);
	memcpy(stOut.bojomsg, strNode, nSize);
	memcpy(stOut.bojomsg, "0396", 4);

	memcpy(pDataBuf, &stOut, sizeof(stOut));

	return pDataBuf;
}

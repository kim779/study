// ChartItemUpjong.cpp: implementation of the CChartItemUpjong class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PartnerMng.h"
#include "ChartItemUpjong.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
char* szTickRowFieldUJ[] ={"22302", "22034", "22023", "22029", "22030", "22031", "22032", "22028", };
char* szSecRowFieldUJ[] = {"26302", "26034", "26023", "26029", "26030", "26031", "26032", "26028", };
char* szMinRowFieldUJ[] = {"24302", "24034", "24023", "24029", "24030", "24031", "24032", "24028", };
char* szDayRowFieldUJ[] = {"25302",          "25023", "25029", "25030", "25031", "25027", "25028", }; // IBK는 25이다. 

CChartItemUpjong::CChartItemUpjong()
{
	m_bSet = FALSE;
}

CChartItemUpjong::~CChartItemUpjong()
{

}

void CChartItemUpjong::SetInData(KB_p0603_InRec1* pData)
{
	memcpy(&m_InData, pData, sizeof(KB_p0603_InRec1));
	m_bSet = TRUE;
}

// MUltiChart에서의 조회 구조체를 받아서 FID 형태로 변환
// 구성 : 차트조회Grid Inbound + 시세 FID Inbound + GridHeader + Grid Outbound
// winix 플랫폼에서 배열형태로 내려오는 데이터를 Grid 라고 부른다.
CString CChartItemUpjong::Convert()
{
	if(!m_bSet) 
		return "";

	CString strData, strCode, strTemp, strImgRgb, strDate;
	int i, nCount;
	
	strTemp = CString(m_InData.Smaxbong, sizeof(m_InData.Smaxbong));
	nCount = atoi(strTemp);

	strCode = CString(m_InData.Supcode, sizeof(m_InData.Supcode));
	strCode.TrimLeft(); strCode.TrimRight();

	strTemp = CString(m_InData.Sunit, sizeof(m_InData.Sunit));
	int nUnit = atoi(strTemp);

	strTemp = CString(m_InData.Sbtngb, sizeof(m_InData.Sbtngb)); // 틱(0)/분(1)/일(2)/주(3)/월(4)
	int nType = atoi(strTemp);
	strDate	= CDataConverter::Char2String(m_InData.Sdate, sizeof(m_InData.Sdate));
	strImgRgb = CDataConverter::Char2String(m_InData.ImgRgb, sizeof(m_InData.ImgRgb));
	CString strNow	= CDataConverter::Char2String(m_InData.nowData, sizeof(m_InData.nowData));
	int nFlag     = CDataConverter::Char2Int(m_InData.Snextgb, sizeof(m_InData.Snextgb));	// 0:
	//////////////////////////////////////////////////////////////////////////
	// Inbound 데이터 작성
	//////////////////////////////////////////////////////////////////////////

	// 차트 데이터
	strData += "21301"; // 종목코드 Input 심볼
	strData += sDS;

	strData += strCode;
	strData +=  sCELL;//KHD sFS;

	CString stNextData, stNextKey;
	
	//NEXTKEY : KHD
	if(nFlag == 0)
	{
		stNextData = "99999999999999";
		memcpy(m_InData.Snextkey, (LPCSTR)(LPCTSTR)stNextData,stNextData.GetLength());
	}
	//환율
	CString stRowData;
	_dataH	Datah;
	FillMemory(&Datah, sz_DATAH, 0x20);
	switch(nType)
	{
	case 0:		// 틱
		Datah.dindex = GI_TICK;
		stRowData = _T("?22500");
	
		strData += "22309";	strData += sDS;	strData += strNow;	strData += sCELL;//당일
//KHD : NEXT		
		stNextKey = "22324";
		stNextData.Format("%20.20s",m_InData.Snextkey);
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // 넥스트 키값 : KHD 
//END	
		break;

	case 1:		// 분
		Datah.dindex = GI_MINUTE;
		stRowData = _T("?24500");
		
		stNextKey = "24324";
		stNextData.Format("%14.14s",m_InData.Snextkey );
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // 넥스트 키값 : KHD 

		// 실/허봉 구분 (0:허봉, 1:실봉)
		strData += "24308";	strData += sDS;	strData += strImgRgb;	strData += sCELL;
		//당일만 조회 
		strData += "24309"; // 당일만조회 Input 심볼
		strData += sDS;
		strTemp = CString(m_InData.nowData, sizeof(m_InData.nowData));
		strData += strTemp;
 		strData += sCELL;

		break;

	case 2:		// 일
		Datah.dindex = GI_DAY;
		stRowData = _T("?25500");
		nUnit = 1;
		stNextKey = "25324";
		stNextData.Format("%8.8s",m_InData.Snextkey );
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // 넥스트 키값 : KHD 

		break;
	case 3:		// 주
		Datah.dindex = GI_WEEK;
		stRowData = _T("?25500");
		nUnit = 1;
		stNextKey = "25324";
		stNextData.Format("%8.8s",m_InData.Snextkey );
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // 넥스트 키값 : KHD 

		break;
	case 4:		// 월
		Datah.dindex = GI_MONTH;
		stRowData = _T("?25500");
		nUnit = 1;
		stNextKey = "25324";
		stNextData.Format("%8.8s",m_InData.Snextkey );
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // 넥스트 키값 : KHD 

		break;
	case 5:		// 년
		Datah.dindex = GI_YEAR;
		stRowData = _T("?25500");
		nUnit = 1;
		stNextKey = "25324";
		stNextData.Format("%8.8s",m_InData.Snextkey );
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // 넥스트 키값 : KHD 

		break;
	case 6:		// 초
		Datah.dindex = GI_SEC;
		stRowData = _T("?26500");
		stNextKey = "26324";
		stNextData.Format("%14.14s",m_InData.Snextkey );
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // 넥스트 키값 : KHD 

		// 실/허봉 구분 (0:허봉, 1:실봉)
		strData += "26308";	strData += sDS;	strData += strImgRgb;	strData += sCELL;
		//당일만 조회 
		strData += "26309"; // 당일만조회 Input 심볼
		strData += sDS;
		strTemp = CString(m_InData.nowData, sizeof(m_InData.nowData));
		strData += strTemp;
 		strData += sCELL;
		break;
	}

	//////////////////////////////////////////////////////////////////////////
	// 종목 시세 OutBound
						//  단축코드,  이름,  현재지수, 전일지수, 증감,  등락율, 거래량, 전일거래량(누적거래대금)
	char* pUpjongSymbol[] = { "21301", "21022", "21023", "21313", "21024", "21033", "21027", "21028",
						//  시가지수, 고가지수, 저가지수,  전일시가, 전일고가, 전일저가,
							"21029", "21030", "21031", "21320", "21321", "21322", "21307" ,"21900", "21901" };

	for(i = 0; i < sizeof(pUpjongSymbol)/sizeof(char*); i++)
	{
		strData += pUpjongSymbol[i];
		strData += sCELL;//KHDsFS;
	}
	strData += stNextKey;
	strData += sCELL;
	//////////////////////////////////////////////////////////////////////////
	// 그리드성 데이터(배열성, 반복데이터) OutBound
	//////////////////////////////////////////////////////////////////////////

	// 챠트 심볼
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
		nSize = sizeof(szTickRowFieldUJ)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szTickRowFieldUJ[i];
			strData += sCOL;
		}
		break;

	case 1:		// 분
		nSize = sizeof(szMinRowFieldUJ)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szMinRowFieldUJ[i];
			strData += sCOL;
		}
		break;

	case 2:		// 일
	case 3:		// 주
	case 4:		// 월
	case 5:		// 년
		nSize = sizeof(szDayRowFieldUJ)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szDayRowFieldUJ[i];
			strData += sCOL;
		}
		break;
	case 6:		// 초
		nSize = sizeof(szSecRowFieldUJ)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szSecRowFieldUJ[i];
			strData += sCOL;
		}
		break;
	}
	
	strData += sCELL;//sFS;

	return strData;
}

char* CChartItemUpjong::Convert2Struct(LPBYTE pData, int nLen, int nKey, int &nDataLen)
{
	KB_p0603_OutRec1_EX stOut;
	CString strSrc;
	strSrc.Format(_T("%.*s"), nLen, (char*)pData);
	CString strNode, strCode, strLock;
	CString strPrice, strVolume, strPreVolume;

	double dOpen, dHigh, dLow, dPreOpen, dPreHigh, dPreLow;//, dMaxHigh, dMinLow;
	double dPrePrice;

	memset(&stOut, 0x20, sizeof(stOut));

//  단축코드,  이름,  현재지수, 전일지수, 증감,  등락율, 거래량, 전일거래량
//{ "20301", "20022", "20023", "20306", "20024", "20033", "20027", "20307",
//  시가지수, 고가지수, 저가지수,
//	"20029", "20030", "20031", };

	strCode = CDataConverter::Parser(strSrc, sCELL); // 종목코드

	strNode = CDataConverter::Parser(strSrc, sCELL); // 종목명
	memcpy(stOut.name, strNode, min(strNode.GetLength(), sizeof(stOut.name)));

	strPrice = CDataConverter::Parser(strSrc, sCELL); // 현재가
	int nPrice = atoi(strPrice);
	strPrice.Remove(_T('+')); strPrice.Remove(_T('-')); strPrice.Remove(_T('.'));
	memcpy(stOut.price, strPrice, min(sizeof(stOut.price), strPrice.GetLength()));
	
	strNode = CDataConverter::Parser(strSrc, sCELL);	dPrePrice = fabs(atof(strNode));	// 전일종가
	strNode.Format(_T("%d"), dPrePrice*100);
	memcpy(stOut.preprice, strNode, min(sizeof(stOut.preprice), strNode.GetLength()));

	strNode = CDataConverter::Parser(strSrc, sCELL); // 전일증감
	strNode = strNode.Mid(1);
	strNode.Remove(_T('.'));
	memcpy(stOut.change, strNode, min(sizeof(stOut.change), strNode.GetLength()));
	
	strNode = CDataConverter::Parser(strSrc, sCELL); // 등락율
	strNode.Remove(_T('+'));/*strNode.Remove(_T('-'));*/
	strNode.Remove(_T('.'));
	memcpy(stOut.chrate, strNode, min(sizeof(stOut.chrate), strNode.GetLength()));

	strVolume = CDataConverter::Parser(strSrc, sCELL); // 거래량
	memcpy(stOut.volume, strVolume, min(sizeof(stOut.volume), strVolume.GetLength()));

	strPreVolume = CDataConverter::Parser(strSrc, sCELL); // 전일거래량

	// Make sign value
	if(nPrice < 0)		stOut.sign[0] = '5'; // DOWN
	else if(abs(nPrice) == (int)(dPrePrice*100))	stOut.sign[0] = '3'; // Same (BOHAP)
	else stOut.sign[0] = '2'; // UP
	
	strNode = CDataConverter::Parser(strSrc, sCELL);	dOpen = fabs(atof(strNode));	// 시가
	strNode = CDataConverter::Parser(strSrc, sCELL);	dHigh = fabs(atof(strNode));	// 고가
	strNode = CDataConverter::Parser(strSrc, sCELL);	dLow = fabs(atof(strNode));		// 저가
	strNode = CDataConverter::Parser(strSrc, sCELL);	dPreOpen = fabs(atof(strNode));	// 전일시가
	strNode = CDataConverter::Parser(strSrc, sCELL);	dPreHigh = fabs(atof(strNode));	// 전일고가
	strNode = CDataConverter::Parser(strSrc, sCELL);	dPreLow = fabs(atof(strNode));	// 전일저가
	
	CString strRealState = CDataConverter::Parser(strSrc, sCELL);   //장운영 정보
																	// 00:장개시전                    
																	// 10:시가단일가                  
																	// 11:시가단일가연장              
																	// 20:장중단일가                  
																	// 21:장중단일가연장              
																	// 30:종가단일가                  
																	// 40:장중                        
																	// 50:장개시전시간외              
																	// 60:장종료후시간외접수          
																	// 65:장종료후시간외              
																	// 70:시간외단일가                
																	// 80:단위매매체결                
																	// 90:거래정지                    
																	// 99:장종료              
	CString strJangStart = CDataConverter::Parser(strSrc, sCELL);   // 장운영시간시작(HHMM)
	CString strJangEnd   = CDataConverter::Parser(strSrc, sCELL);   // 장운영시간종료(HHMM)
	strJangStart.TrimRight();
	strJangEnd.TrimRight();
	
	BOOL bSetReal = FALSE;
	if(strRealState.Compare("00") == 0 || strRealState.Compare("10") == 0 || strRealState.Compare("11") == 0 || strRealState.Compare("50") == 0)
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
		nSize = sizeof(szTickRowFieldUJ)/sizeof(char*);
	else if(nType == 1) //분
		nSize = sizeof(szMinRowFieldUJ)/sizeof(char*);
	else if(nType == 6) //초
		nSize = sizeof(szSecRowFieldUJ)/sizeof(char*);
	else		// 일, 주, 월, 년
		nSize = sizeof(szDayRowFieldUJ)/sizeof(char*);
	
	// 그리드 헤더
	GRAPH_IO gridOut;
	strNode = strSrc.Left(sizeof(gridOut));
	memcpy(&gridOut, strNode, sizeof(gridOut)); 
	
	strSrc = strSrc.Mid(sizeof(gridOut));
	//091027-alzioyes.기존 DRFN차트에서 소숫점없이 데이터를 받았다. 통일.
	strSrc.Remove('.');
	
	if (strNextKey =="99999999999999" || atoi(strNextKey) == 0)// 9가 14자리면 다음키가 없는것.  
		memset(stOut.nkey, 0x20, sizeof(stOut.nkey));
	else
		memcpy(stOut.nkey, (LPCTSTR)strNextKey, min(sizeof(stOut.nkey),strNextKey.GetLength()));
	//END

	CStringArray arrNode;
	int nNodeIndex = 1;
	while(strSrc.IsEmpty() == FALSE)
	{	
		strNode = CDataConverter::Parser(strSrc, (nNodeIndex == nSize)?sROW:sCELL);
	//	strNode.TrimLeft(); strNode.TrimRight();
		arrNode.Add(strNode);

		if(nNodeIndex == nSize) nNodeIndex = 1;
		else nNodeIndex++;
	}

	// 전체 데이터 수 계산
	nCnt = arrNode.GetSize()/ nSize;
	strNode.Format("%05d", nCnt);
	memcpy(stOut.fcnt, strNode, sizeof(stOut.fcnt));

	// 차트 봉 데이터
	int nBongSize = sizeof(KB_p0603_OutRec2);
	int nBufSize = sizeof(KB_p0603_OutRec1_EX) + nBongSize * nCnt;
	int	nPosBong = sizeof(KB_p0603_OutRec1_EX);

	nDataLen = nBufSize;
	
	CString strDate, strTime;
	char* pDataBuf = new char[nBufSize+1];
	KB_p0603_OutRec2 stFirstBong, stBong;

	double dValue=0;
	int nIndex = 0;
	int nRow = 0;
	for( nRow = 0; nRow < nCnt; nRow++)
	{
		memset(&stBong, 0x20, sizeof(stBong));
		
		// date
		strDate = arrNode.GetAt(nIndex++);
		
		// time : 틱,초,분일 때
		if (nType < 2 || nType == 6)
			strTime = arrNode.GetAt(nIndex++); 
		
		if (nType == 0) // DDHHMMSS
			strNode = strDate.Right(2) + strTime;
		else if (nType == 1) //MMDDHHMM
			strNode = strDate.Right(4) + strTime.Left(4);
		else if (nType == 4) //YYYYMM
			strNode = strDate.Left(6);
		else if (nType == 6) //DDHHMMSS
			strNode = strDate.Right(2) + strTime.Left(6);
		else if (nType == 5) // 년.
		{
			if (nRow != nCnt-1)
				strNode.Format(_T("%s12"), strDate.Left(4));
			else
				strNode = strDate.Left(6);
		}
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
		//strNode.Format(_T("%.0f"), dValue);
		strNode += "000000";
// 		if(nRow==0)
// 		{
// 			CString szDebug;
// 			szDebug.Format("[SMalzio] UP거래대금=%s", strNode);
// 			OutputDebugString(szDebug);
// 		}
		memcpy(stBong.value, strNode, min(sizeof(stBong.value), strNode.GetLength()));
		//@Solomon	091202SM123	<--

		// 데이터 버퍼에 복사, 일주월은 초,분,틱과 순서가 반대
		memcpy(&pDataBuf[nPosBong + nRow*nBongSize], &stBong, nBongSize);

		// 첫 데이터를 저장해 놓는다.
		if(nRow == 0)
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
		KB_p0602_OutRec2 stBong;
		
		switch(nDataType)
		{
		case 1:	dDenom = nFirstClose;		break;	// 첫종가 기준
		case 2:	dDenom = 1;					break;	// $  업종은 처리없음
		case 3:	dDenom = dOpen*100;			break;	// 당일시가
		case 4:	dDenom = nPrice;			break;	// 현재가
		case 5:	dDenom = dPrePrice*100;		break;	// 전일종가
		}

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
//	m_bojoInput.m_sUpLmtPrice.Format(_T("%.0f"), dMaxHigh*100);	// 상한가
//	m_bojoInput.m_sDnLmtPrice.Format(_T("%.0f"), dMinLow*100);	// 하한가
	m_bojoInput.m_sOpen.Format(_T("%.0f"), dOpen*100);			// 시가
	m_bojoInput.m_sHigh.Format(_T("%.0f"), dHigh*100);			// 고가
	m_bojoInput.m_sLow.Format(_T("%.0f"), dLow*100);			// 저가
	//@Solomon:091123SM068 -->
	m_bojoInput.m_sPreOpen.Format(_T("%.0f"), dPreOpen*100);	// 전일시가
	m_bojoInput.m_sPreHigh.Format(_T("%.0f"), dPreHigh*100);	// 전일고가
	m_bojoInput.m_sPreLow.Format(_T("%.0f"), dPreLow*100);		// 전일저가
	//@Solomon:091123SM068 <--
	m_bojoInput.m_sPreprice.Format(_T("%.0f"), dPrePrice*100);	// 전일종가
	m_bojoInput.m_sPreVolume = strPreVolume;	// 전일거래량
	m_bojoInput.m_sStartTime = strResetTime;	// 거래시작시간(HHMMSS) <<!>>
	m_bojoInput.m_sFirstClose = CString(stFirstBong.close, sizeof(stFirstBong.close)) ;	// 조회중 첫봉의 종가.

	m_bojoInput.m_sDataType = _T("0");		//	고정. 차후 필요시 수정.
	m_bojoInput.m_sMarket = _T("1");			//	고정. 차후 필요시 수정.
	m_bojoInput.m_sNxtFlg = m_InData.Snextgb[0];

	m_bojoInput.m_sTerm.Format("%d", nType);
	m_bojoInput.m_sTick = CString(m_InData.Sunit, sizeof(m_InData.Sunit));
	if (nType == 1)
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

	m_bojoInput.m_szMarketTime	= strMarketTime;	//장운영정보.

	CString strBozo;
	char szKey[4];
	memset(szKey, 0x00, sizeof(szKey));
	sprintf(szKey, _T("%03d"), 20); // 102은 업종
	
	nSize = bojoMng.GetChartBozoMsg(szKey, &m_bojoInput, strNode);
	//nSize = bojoMng.GetChartBozoMsg(szKey, &m_bojoInput, strNode, 1);

	memcpy(stOut.bojomsg, strNode, nSize);
	memcpy(stOut.bojomsg, "0396", 4);

	memcpy(pDataBuf, &stOut, sizeof(stOut));

	return pDataBuf;
}

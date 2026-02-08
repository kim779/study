// ChartItemForeign.cpp: implementation of the CChartItemForeign class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PartnerMng.h"
#include "ChartItemForeign.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//@IBK : 일.주.월 데이터 밖에 없다. 추후 추가되면 한다. 2012
#ifndef LOCALTIME_SET
char* szTickRowFieldForeign[] ={"104302", "104034", "104035", "104023", "104029", "104030", "104031", "104032", };
char* szSecRowFieldForeign[] ={ "104302", "104034", "104035", "104023", "104029", "104030", "104031", "104032", };
char* szMinRowFieldForeign[] = {"104302", "104034", "104035", "104023", "104029", "104030", "104031", "104032", };
char* szDayRowFieldForeign[] = {"52302",					  "52023", "52029", "52030", "52031", "52027", };
#else
char* szTickRowFieldForeign[] ={"104302", "104034", "104023", "104029", "104030", "104031", "104032", };
char* szSecRowFieldForeign[] ={ "104302", "104034", "104023", "104029", "104030", "104031", "104032", };
char* szMinRowFieldForeign[] = {"104302", "104034", "104023", "104029", "104030", "104031", "104032", };
char* szDayRowFieldForeign[] = {"52302",			"52023", "52029", "52030", "52031", "52027", };
#endif

CChartItemForeign::CChartItemForeign()
{
	m_bSet = FALSE;
}

CChartItemForeign::~CChartItemForeign()
{

}

void CChartItemForeign::SetInData(KB_p0606_InRec1* pData)
{
	memcpy(&m_InData, pData, sizeof(KB_p0606_InRec1));
	m_bSet = TRUE;
}

CString CChartItemForeign::Convert()
{
	if(!m_bSet) return "";

//	50021
//	104309 // 1: 한국시간, 0:현지시간.

	CString strData, strCode, strTemp, strDate;
	int i, nCount;
	
	strTemp = CString(m_InData.num, sizeof(m_InData.num));
	nCount = atoi(strTemp);

	strCode = CString(m_InData.Shcode, sizeof(m_InData.Shcode));
	strCode.TrimLeft(); strCode.TrimRight();

	strTemp = CString(m_InData.unit, sizeof(m_InData.unit));
	int nUnit = atoi(strTemp);

	strTemp = CString(m_InData.button, sizeof(m_InData.button)); // 틱(0)/분(1)/일(2)/주(3)/월(4)
	int nType = atoi(strTemp);

	strDate	= CDataConverter::Char2String(m_InData.date, sizeof(m_InData.date));
	int nFlag     = CDataConverter::Char2Int(m_InData.nflag, sizeof(m_InData.nflag));	// 0:
	//////////////////////////////////////////////////////////////////////////
	// Inbound 데이터 작성
	//////////////////////////////////////////////////////////////////////////

	// 차트 데이터
	strData += "51301"; // 종목코드 Input 심볼
	strData += sDS;

	strData += strCode;
	strData += sCELL;
	CString stRowData;
	_dataH	Datah;
	FillMemory(&Datah, sz_DATAH, 0x20);
	//NEXTKEY : KHD
	CString stNextData, stNextKey, stNextKey2;
	if(nFlag == 0)
	{
		stNextData = "99999999999999";
		memcpy(m_InData.nkey, (LPCSTR)(LPCTSTR)stNextData,stNextData.GetLength());
	}

	//KHD : 현재 틱. 분. 초 데이터 없음 IBK에서 제공해주지 않음. 
	if(nType== 0 || nType == 1 || nType == 6) return "";
	switch(nType)
	{
	case 0:		// 틱
/*		strData += "104306"; // 조회일자 Input 심볼
		strData += sDS;
		strTemp = CString(m_InData.date, sizeof(m_InData.date));
		strData += strTemp;
		strData += sFS;
		
		strData += "104307"; // 당일만조회 Input 심볼
		strData += sDS;
		strTemp = CString(m_InData.nowData, sizeof(m_InData.nowData));
		strData += strTemp;
		strData += sFS;
*/
/*	KHD	strData += "104308"; // GAP Input 심볼
		strData += sDS;
		strTemp.Format("%d", nUnit);
		strData += strTemp;
		strData += sFS;
#ifndef LOCALTIME_SET
		strData += "104309";
		strData += sDS;
		strTemp.Format("%c", m_InData.timeGB[0]);
		strData += strTemp;
		strData += sFS;
#endif
*/
		break;

	case 1:		// 분 //KHD : 만약 분으로 오면 강제로 일주기로 바꾼다.
	
		break;
/*		strData += "104306"; // 조회일자 Input 심볼
		strData += sDS;
		strTemp = " ";//CString(m_InData.date, sizeof(m_InData.date));
		strData += strTemp;
		strData += sFS;

		strData += "104307"; // 당일만조회 Input 심볼
		strData += sDS;
		strTemp = CString(m_InData.nowData, sizeof(m_InData.nowData));
		strData += strTemp;
		strData += sFS;
*/
/*	KHD	strData += "104308"; // 분봉갭 Input 심볼
		strData += sDS;
		strTemp.Format("%d", nUnit);
		strData += strTemp;
		strData += sFS;

#ifndef LOCALTIME_SET
		strData += "104309";
		strData += sDS;
		strTemp.Format("%c", m_InData.timeGB[0]);
		strData += strTemp;
		strData += sFS;
#endif
*/
		break;

	case 2:		// 일
		Datah.dindex = GI_DAY;
		nUnit = 1;
		stRowData = _T("?52500");
		stNextKey = "52324";
		stNextData.Format("%8.8s",m_InData.nkey );
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // 넥스트 키값 : KHD 

		break;
	case 3:		// 주
		Datah.dindex = GI_WEEK;
		nUnit = 1;
		stRowData = _T("?52500");
		stNextKey = "52324";
		stNextData.Format("%8.8s",m_InData.nkey );
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // 넥스트 키값 : KHD 

		break;
	case 4:		// 월
		Datah.dindex = GI_MONTH;
		nUnit = 1;
		stRowData = _T("?52500");
		stNextKey = "52324";
		stNextData.Format("%8.8s",m_InData.nkey );
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // 넥스트 키값 : KHD 

		break;

/*		strData += "104306"; // 조회끝일자 Input 심볼  <<!>> 끝날짜가 없음
		strData += sDS;
		strTemp = CString(m_InData.date, sizeof(m_InData.date));
		strData += strTemp;
		strData += sFS;
*/
/* KHD		strData += "104308"; // 일(1)주(2)월(3)구분 Input 심볼
		strData += sDS;
		strTemp = (nType==2) ? "1":(nType==3 ? "2":"3");
		strData += strTemp;
		strData += sFS;*/
/*
		// 선옵은 필요없을것 같음	
		strData += "104309"; // 연결선물여부 Input 심볼
		strData += sDS;
		strTemp = " "; //CString(m_InData.modGb, sizeof(m_InData.modGb));
		strData += strTemp;
		strData += sFS;
*/
		break;
	case 6:		// 초
/*KHD
		strData += "101307"; // 당일 구분 (1:당일, 0:아님) Input 심볼
		strData += sDS;
		strTemp = CString(m_InData.nowData, sizeof(m_InData.nowData));
		strData += strTemp;
		strData += sFS;
		
		strData += "101308"; // n 초 Input 심볼
		strData += sDS;
		strTemp.Format("%d", nUnit);
		strData += strTemp;
		strData += sFS;

#ifndef LOCALTIME_SET
		strData += "104309";
		strData += sDS;
		strTemp.Format("%c", m_InData.timeGB[0]);
		strData += strTemp;
		strData += sFS;
#endif
		*/
		break;
	}

	//////////////////////////////////////////////////////////////////////////
	// 종목 시세 OutBound
						// 단축코드,  이름,  현재가, 전일종가,증감, 등락율, 누적거래량
	char* pFOSymbol[] = { "51301", "51022", "51023", "51313", "51024", "51033", "51027",
						// 시가,  고가,  저가,// 전일시가,  전일고가,  전일저가,
						"51029", "51030", "51031"};// "51029", "51030", "51031", };
						// 상한가,  하한가 ,  시가,  고가,  저가,
//							"30354", "30355", "30358", "30359", "30360", };

	for(i = 0; i < sizeof(pFOSymbol)/sizeof(char*); i++)
	{
		strData += pFOSymbol[i];
		strData += sCELL;
	}

	strData += stNextKey;
	strData += sCELL;
	//////////////////////////////////////////////////////////////////////////
	// 그리드성 데이터(배열성, 반복데이터) OutBound
	//////////////////////////////////////////////////////////////////////////

	// 챠트 심볼


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
		nSize = sizeof(szTickRowFieldForeign)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szTickRowFieldForeign[i];
			strData += sCOL;
		}
		break;

	case 1:		// 분
		nSize = sizeof(szMinRowFieldForeign)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szMinRowFieldForeign[i];
			strData += sCOL;
		}
		break;

	case 2:		// 일
	case 3:		// 주
	case 4:		// 월
		nSize = sizeof(szDayRowFieldForeign)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szDayRowFieldForeign[i];
			strData += sCOL;
		}
		break;
	case 6:		// 초
		nSize = sizeof(szSecRowFieldForeign)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szSecRowFieldForeign[i];
			strData += sCOL;
		}
		break;
	}

	strData += sCELL;

	return strData;
}

char* CChartItemForeign::Convert2Struct(LPBYTE pData, int nLen, int nKey, int &nDataLen)
{
	KB_p0606_OutRec1_EX stOut;
	CString strSrc;
	strSrc.Format(_T("%.*s"), nLen, (char*)pData);
	CString strNode, strCode, strLock;
	CString strPrice, strPrePrice, strVolume;
	CString strHigh, strLow, strOpen, strHighLimit, strLowLimit;
	CString strPreHigh, strPreLow, strPreOpen;

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
	double dwRate = atof(strNode);
	strNode.Remove(_T('+'));/*strNode.Remove(_T('-'));*/strNode.Remove(_T('.'));
	memcpy(stOut.chrate, strNode, min(sizeof(stOut.chrate), strNode.GetLength()));

	strVolume = CDataConverter::Parser(strSrc, sCELL); // 거래량
	memcpy(stOut.volume, strVolume, min(sizeof(stOut.volume), strVolume.GetLength()));

	//[CSA00000065] Make sign value
	if(dwRate < 0)			stOut.sign[0] = '5'; // DOWN
	else if(dwRate > 0) stOut.sign[0] = '2'; // UP
	else stOut.sign[0] = '3'; // SAME (BOHAP)

	strOpen = CDataConverter::Parser(strSrc, sCELL); // 시가
	strHigh = CDataConverter::Parser(strSrc, sCELL); // 고가
	strLow = CDataConverter::Parser(strSrc, sCELL); // 저가
	strOpen.Remove(_T('.'));
	strHigh.Remove(_T('.'));
	strLow.Remove(_T('.'));
/*
	strPreOpen = CDataConverter::Parser(strSrc, sFS);		// 전일시가
	strPreOpen.Remove(_T('+'));
	strPreHigh = CDataConverter::Parser(strSrc, sFS);		// 전일고가
	strPreHigh.Remove(_T('+'));
	strPreLow = CDataConverter::Parser(strSrc, sFS);		// 전일저가
	strPreLow.Remove(_T('+'));
*/	
	//KHD : NextKey
	CString strNextKey = CDataConverter::Parser(strSrc, sCELL);

	int nSize, nCnt, nType;
	strNode = CString(m_InData.button, sizeof(m_InData.button)); // 틱(0)/분(1)/일(2)/주(3)/월(4)
	nType = atoi(strNode);
	if(nType == 0) // 틱
		nSize = sizeof(szTickRowFieldForeign)/sizeof(char*);
	else if(nType == 1) //분
		nSize = sizeof(szMinRowFieldForeign)/sizeof(char*);
	else if(nType == 6) //초
		nSize = sizeof(szSecRowFieldForeign)/sizeof(char*);
	else		// 일, 주, 월
		nSize = sizeof(szDayRowFieldForeign)/sizeof(char*);
	
	// 그리드 헤더
	GRAPH_IO gridOut;
	strNode = strSrc.Left(sizeof(gridOut));
	memcpy(&gridOut, strNode, sizeof(gridOut)); 
	
	strSrc = strSrc.Mid(sizeof(gridOut));

	if (strNextKey == "99999999999999" || atoi(strNextKey) == 0)	// 9가 14자리면 다음키가 없는것.
		memset(stOut.nkey, 0x20, sizeof(stOut.nkey));
	else
		memcpy(stOut.nkey, (LPCTSTR)strNextKey, min(sizeof(stOut.nkey),strNextKey.GetLength()));
		//END
	
	CStringArray arrNode;
	int nNodeIndex = 1;
	while(strSrc.IsEmpty() == FALSE)
	{
		strNode = CDataConverter::Parser(strSrc, (nNodeIndex == nSize)?sROW:sCELL);
		strNode.TrimLeft(); strNode.TrimRight();
		arrNode.Add(strNode);

		if(nNodeIndex == nSize) nNodeIndex = 1;
		else nNodeIndex++;
	}

	// 전체 데이터 수 계산
	nCnt = arrNode.GetSize()/ nSize;
	strNode.Format("%05d", nCnt);
	memcpy(stOut.fcnt, strNode, sizeof(stOut.fcnt));

	// 차트 봉 데이터
	int nBongSize = sizeof(KB_p0606_OutRec2);
	int nBufSize = sizeof(KB_p0606_OutRec1_EX) + nBongSize * nCnt;
	int	nPosBong = sizeof(KB_p0606_OutRec1_EX);

	nDataLen = nBufSize;
	
	CString strDate = "00000000", strTime;
	char* pDataBuf = new char[nBufSize+1];
	KB_p0606_OutRec2 stFirstBong;

	char cTimeGB = m_InData.timeGB[0];
	int nIndex = 0;
	IMasterDataManagerLast* pMasterDataManager = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);
	int nCodeType = pMasterDataManager->GetCodeTypeFromCode(strCode);

	KB_p0606_OutRec2 stBong;
	int nRow = 0;
	for( nRow = 0; nRow < nCnt; nRow++)
	{
		memset(&stBong, 0x20, sizeof(stBong));
		
		// date
		strDate = arrNode.GetAt(nIndex++);

		// time : 틱,초,분일 때
		if	(nType < 2 || nType == 6)
		{
			strTime = arrNode.GetAt(nIndex++);	// 한국시간
#ifndef LOCALTIME_SET
			if (cTimeGB == '1')
				strNode = arrNode.GetAt(nIndex++);	// 현지시간 사용안함.
			else
				strTime = arrNode.GetAt(nIndex++);	// 현지시간 사용.
#endif
		}
		
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
		//strNode.Format("%d", atoi(strNode));
		//KHD : 뒤에 쓸데없는 2자리가 더와서 임의로 삭제한다. : 이부분 IBK에서 확인해주기로 했음 ( 2012.2.10)
		if(strNode.GetLength() > 0)
		{
		//	strNode.TrimRight("0");
		//	strNode+="0";
		//	strNode.Delete(strNode.GetLength()-2,2);

		}
		//END 
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

		// 데이터 버퍼에 복사, 일주월은 초,분,틱과 순서가 반대
	//	if (nType == 6)
	//		memcpy(&pDataBuf[nPosBong + nRow*nBongSize], &stBong, nBongSize);
	//	else
	//		memcpy(&pDataBuf[nBufSize - (nRow+1)*nBongSize], &stBong, nBongSize);
		memcpy(&pDataBuf[nPosBong + nRow*nBongSize], &stBong, nBongSize);//KHD : 반대로 와서 반대로 세팅 
		// 첫 데이터를 저장해 놓는다.
		if ( nRow == 0)
			memcpy(&stFirstBong, &stBong, sizeof(stBong));
	}

	// 20130222 이상신 : 주봉일경우 마지막 데이터의 일자를 설정해 준다.(주 첫 영업일 관련)
	if (nCnt > 0 && nType == 3 && m_InData.nflag[0] != _T('1'))
		m_bojoInput.m_sLastDataTime = CString(stBong.date, sizeof(stBong.date));
	else
		m_bojoInput.m_sLastDataTime = _T("");

	int nDataType = int(m_InData.dataType[0] - '0'); 
	if(nDataType > 0)
	{
		CString strData(stFirstBong.close, sizeof(stFirstBong.close));
		int nFirstClose = (int)fabs(atof(strData));
		double dDenom,dPrePrice,dOpen; //분모
		KB_p0602_OutRec2 stBong;
		dPrePrice = fabs(atof(strPrePrice));
		dOpen = fabs(atof(strOpen));
		switch(nDataType)
		{
		case 1:	dDenom = nFirstClose;		break;	// 첫종가 기준
		case 2:	dDenom = 1;					break;	// $  업종은 처리없음
		case 3:	dDenom = dOpen;			break;	// 당일시가
		case 4:	dDenom = nPrice;			break;	// 현재가
		case 5:	dDenom = dPrePrice;		break;	// 전일종가
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
//	m_bojoInput.m_sUpLmtPrice = strHighLimit;	// 상한가
//	m_bojoInput.m_sDnLmtPrice = strLowLimit;	// 하한가
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
//	m_bojoInput.m_sStartTime = "090000";	// 거래시작시간(HHMMSS) <<!>>
	m_bojoInput.m_sFirstClose = CString(stFirstBong.close, sizeof(stFirstBong.close)) ;	// 조회중 첫봉의 종가.

	m_bojoInput.m_sDataType = _T("0");		//	고정. 차후 필요시 수정.
	m_bojoInput.m_sMarket = _T("1");			//	고정. 차후 필요시 수정.
	m_bojoInput.m_sNxtFlg = m_InData.nflag[0];

	if(nType == 0)
	{
		m_bojoInput.m_sTerm = "0";	//	틱
		m_bojoInput.m_sTick = CString(m_InData.unit, sizeof(m_InData.unit));
	}
	else if(nType == 1)
	{
		m_bojoInput.m_sTerm = "1";	//	분
		m_bojoInput.m_sTick = CString(m_InData.unit, sizeof(m_InData.unit));
		//{{JS.Kim_20100903 Solomon 60분봉 이상인 경우 시간으로 단위 변경하여야 함
		int nTick = atoi(m_bojoInput.m_sTick);
		if( nTick >= 60 )
			m_bojoInput.m_sTick.Format("%d", (nTick/ 60) * 100 + nTick % 60);
		//}}
	}
	else if(nType == 6)
	{
		m_bojoInput.m_sTerm = "6";	//	초
		m_bojoInput.m_sTick = CString(m_InData.unit, sizeof(m_InData.unit));
	}
	else if(nType >= 2)
	{
		m_bojoInput.m_sTerm.Format("%d", nType);	//	일,주,월
		m_bojoInput.m_sTick = _T("1");
	}

	CString strBozo;
	char szKey[4];
	memset(szKey, 0x00, sizeof(szKey));
	sprintf(szKey, _T("%03d"), 104); // 103은 선옵
	
	nSize = bojoMng.GetChartBozoMsg(szKey, &m_bojoInput, strNode);
	memcpy(stOut.bojomsg, strNode, nSize);
	memcpy(stOut.bojomsg, "0396", 4);

	memcpy(pDataBuf, &stOut, sizeof(stOut));

	return pDataBuf;
}

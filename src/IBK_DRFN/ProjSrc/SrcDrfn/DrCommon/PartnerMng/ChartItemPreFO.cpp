// ChartItemPreFO.cpp: implementation of the CChartItemPreFO class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PartnerMng.h"
#include "ChartItemPreFO.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
char* szTickRowFieldPreFO[] =	{"108302", "108034", "108023", "108029", "108030", "108031", "108032", "108028", };
char* szSecRowFieldPreFO[] =	{"108302", "108034", "108023", "108029", "108030", "108031", "108032", "108028", };
char* szMinRowFieldPreFO[] =	{"108302", "108034", "108023", "108029", "108030", "108031", "108032", "108028", };
char* szDayRowFieldPreFO[] =	{"108302",           "108023", "108029", "108030", "108031", "108027", "108028", };

CChartItemPreFO::CChartItemPreFO()
{
	m_bSet = FALSE;
}

CChartItemPreFO::~CChartItemPreFO()
{

}

void CChartItemPreFO::SetInData(KB_p0604_InRec1* pData)
{
	memcpy(&m_InData, pData, sizeof(KB_p0604_InRec1));
	m_bSet = TRUE;
}

// MUltiChart에서의 조회 구조체를 받아서 FID 형태로 변환
// 구성 : 차트조회Grid Inbound + 시세 FID Inbound + GridHeader + Grid Outbound
// winix 플랫폼에서 배열형태로 내려오는 데이터를 Grid 라고 부른다.
CString CChartItemPreFO::Convert()
{
	if(!m_bSet) return "";

	CString strData, strCode, strTemp;
	int i, nCount;
	
	strTemp = CString(m_InData.Smaxbong, sizeof(m_InData.Smaxbong));
	nCount = atoi(strTemp);

	strCode = CString(m_InData.Scode, sizeof(m_InData.Scode));
	strCode.TrimLeft(); strCode.TrimRight();

	strTemp = CString(m_InData.Sunit, sizeof(m_InData.Sunit));
	int nUnit = atoi(strTemp);

	strTemp = CString(m_InData.Sbtngb, sizeof(m_InData.Sbtngb)); // 틱(0)/분(1)/일(2)/주(3)/월(4)
	int nType = atoi(strTemp);

	//////////////////////////////////////////////////////////////////////////
	// Inbound 데이터 작성
	//////////////////////////////////////////////////////////////////////////

	// 차트 데이터
	strData += "108301"; // 종목코드 Input 심볼
	strData += sDS;

	strData += strCode;
	strData += sFS;

	switch(nType)
	{
	case 0:		// 틱
		strData += "108306"; // 조회일자 Input 심볼
		strData += sDS;
		strTemp = CString(m_InData.Sdate, sizeof(m_InData.Sdate));
		strData += strTemp;
		strData += sFS;
		
		strData += "108307"; // 당일만조회 Input 심볼
		strData += sDS;
		strTemp = CString(m_InData.nowData, sizeof(m_InData.nowData));
		strData += strTemp;
		strData += sFS;

		strData += "108308"; // GAP Input 심볼
		strData += sDS;
		strTemp.Format("%d", nUnit);
		strData += strTemp;
		strData += sFS;

		if (m_InData.Srecordgb[0] == '1')
		{
			strData += "108304";		//	복기차트 조회 시간
			strData += sDS;
			strTemp = CString(m_InData.Stime, sizeof(m_InData.Stime));
			strData += strTemp;
			strData += sFS;
		}
		break;

	case 1:		// 분
		strData += "108306"; // 조회일자 Input 심볼
		strData += sDS;
		strTemp = CString(m_InData.Sdate, sizeof(m_InData.Sdate));
		strData += strTemp;
		strData += sFS;

		strData += "108307"; // 당일만조회 Input 심볼
		strData += sDS;
		strTemp = CString(m_InData.nowData, sizeof(m_InData.nowData));
		strData += strTemp;
		strData += sFS;

		strData += "108308"; // 분봉갭 Input 심볼
		strData += sDS;
		strTemp.Format("%d", nUnit);
		strData += strTemp;
		strData += sFS;

		if (m_InData.Srecordgb[0] == '1')
		{
			strData += "108304";		//	복기차트 조회 시간
			strData += sDS;
			strTemp = CString(m_InData.Stime, sizeof(m_InData.Stime));
			strData += strTemp;
			strData += sFS;
		}
		break;

	case 2:		// 일
	case 3:		// 주
	case 4:		// 월
// 		strData += "101305"; // 조회시작일자 Input 심볼
// 		strData += sDS;
// 		strTemp = CString(m_InData.date, sizeof(m_InData.date));
// 		strData += strTemp;
// 		strData += sFS;

		strData += "108306"; // 조회끝일자 Input 심볼  <<!>> 끝날짜가 없음
		strData += sDS;
		strTemp = CString(m_InData.Sdate, sizeof(m_InData.Sdate));
		strData += strTemp;
		strData += sFS;
		
		strData += "108308"; // 일(1)주(2)월(3)구분 Input 심볼
		strData += sDS;
		strTemp = (nType==2) ? "1":(nType==3 ? "2":"3");
		strData += strTemp;
		strData += sFS;

		// 선옵은 필요없을것 같음	
		strData += "108309"; // 연결선물여부 Input 심볼
		strData += sDS;
		strTemp = " "; //CString(m_InData.modGb, sizeof(m_InData.modGb));
		strData += strTemp;
		strData += sFS;
		break;
	case 6:		// 초
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

		break;
	}

	//////////////////////////////////////////////////////////////////////////
	// 종목 시세 OutBound
						// 단축코드,  이름
	char* pFOSymbol[] = { "108301", "108022", NULL};

//	for (i = 0; i < sizeof(pFOSymbol)/sizeof(char*); i++)
	int nIndex = 0;
	while (pFOSymbol[nIndex] != NULL)
	{
		strData += pFOSymbol[nIndex];
		strData += sFS;
		nIndex++;
	}

	//////////////////////////////////////////////////////////////////////////
	// 그리드성 데이터(배열성, 반복데이터) OutBound
	//////////////////////////////////////////////////////////////////////////

	// 챠트 심볼
	strData += _T('$');
	if(nType == 0) strData += "108410";
	else if(nType == 1) strData += "108510";
	else if(nType == 6) strData += "108511";
	else strData += "108610";
	strData += sDS;

	//////////////////////////////////////////////////////////////////////////
	// 그리드 헤더
	GRID_I grid_i;
	memset(&grid_i, 0x20, sizeof(grid_i));

	char szTemp[5];
	grid_i.gdir[0] = _T('1');	//	top.
	if (m_InData.Snextgb[0] == '1')	grid_i.ikey[0] = _T('2');	//	Page Down
	else							grid_i.ikey[0] = _T('0');	//	일반(Next조회).

	sprintf(szTemp, _T("%04d"), nCount);	// 조회 데이터 수
	memcpy(grid_i.nrow, szTemp, sizeof(grid_i.nrow));

	memcpy(grid_i.page, m_InData.Snextkey, 4);
	memcpy(grid_i.save, m_InData.Snextkey+4, sizeof(grid_i.save));

	grid_i.sdir[0] = _T('1');
	grid_i.vflg[0] = _T('1');
	memcpy(grid_i.vrow, _T("999"), sizeof(grid_i.vrow));

	char szGrid_i[128];
	memset(szGrid_i, 0x00, sizeof(szGrid_i));
	memcpy(szGrid_i, (char*)&grid_i, sizeof(grid_i));

	strData += szGrid_i;
	//////////////////////////////////////////////////////////////////////////
	// Row Data 구성
	int nSize;

	switch(nType)
	{
	case 0:		// 틱
		nSize = sizeof(szTickRowFieldPreFO)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szTickRowFieldPreFO[i];
			strData += sCOL;
		}
		break;

	case 1:		// 분
		nSize = sizeof(szMinRowFieldPreFO)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szMinRowFieldPreFO[i];
			strData += sCOL;
		}
		break;

	case 2:		// 일
	case 3:		// 주
	case 4:		// 월
		nSize = sizeof(szDayRowFieldPreFO)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szDayRowFieldPreFO[i];
			strData += sCOL;
		}
		break;
	case 6:		// 초
		nSize = sizeof(szSecRowFieldPreFO)/sizeof(char*);
		for(i = 0; i < nSize; i++)
		{
			strData += szSecRowFieldPreFO[i];
			strData += sCOL;
		}
		break;
	}
	
	strData += sFS;

	return strData;
}

char* CChartItemPreFO::Convert2Struct(LPBYTE pData, int nLen, int nKey, int &nDataLen)
{
	KB_p0604_OutRec1_EX stOut;
	CString strSrc;
	strSrc.Format(_T("%*.*s"), nLen, nLen, (char*)pData);
	CString strNode, strCode, strLock;
	CString strPrice, strPrePrice, strVolume;
	CString strHigh, strLow, strOpen, strHighLimit, strLowLimit;
	CString strPreHigh, strPreLow, strPreOpen;
	int nPrice=0;

	memset(&stOut, 0x20, sizeof(stOut));

	strCode = CDataConverter::Parser(strSrc, sFS); // 종목코드

	strNode = CDataConverter::Parser(strSrc, sFS); // 종목명
	memcpy(stOut.name, strNode, min(strNode.GetLength(), sizeof(stOut.name)));

	int nSize, nCnt, nType;
	strNode = CString(m_InData.Sbtngb, sizeof(m_InData.Sbtngb)); // 틱(0)/분(1)/일(2)/주(3)/월(4)
	nType = atoi(strNode);
	if(nType == 0) // 틱
		nSize = sizeof(szTickRowFieldPreFO)/sizeof(char*);
	else if(nType == 1) //분
		nSize = sizeof(szMinRowFieldPreFO)/sizeof(char*);
	else if(nType == 6) //초
		nSize = sizeof(szSecRowFieldPreFO)/sizeof(char*);
	else		// 일, 주, 월
		nSize = sizeof(szDayRowFieldPreFO)/sizeof(char*);
	
	// 그리드 헤더
	GRID_O gridOut;
	strNode = strSrc.Left(sizeof(gridOut));
	memcpy(&gridOut, strNode, sizeof(gridOut)); 
	
	strSrc = strSrc.Mid(sizeof(gridOut));

	if ((gridOut.xpos[0] & 0x01) || (gridOut.xpos[0] & 0x02))
		memcpy(stOut.nkey, gridOut.page, sizeof(stOut.nkey));

	//091027-alzioyes.기존 DRFN차트에서 소숫점없이 데이터를 받았다. 통일.
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
	int nBongSize = sizeof(KB_p0604_OutRec2);
	int nBufSize = sizeof(KB_p0604_OutRec1_EX) + nBongSize * nCnt;
	int	nPosBong = sizeof(KB_p0604_OutRec1_EX);

	nDataLen = nBufSize;
	
	CString strDate = "00000000", strTime;
	char* pDataBuf = new char[nBufSize+1];
	KB_p0604_OutRec2 stFirstBong, stBong;

	double dValue=0;
	int nIndex = 0, nRow;
//	for(nRow = 0; nRow < nCnt; nRow++)
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

		strNode.TrimLeft();	strNode.TrimRight();
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
		//dValue = atof(strNode)*100000000;	//[A00000408]거래대금 12 -> 18. (6 -> 8개)
		//strNode.Format(_T("%.0f"), dValue);
		strNode += "00000000";
// 		if(nRow==nCnt-1)
// 		{
// 			CString szDebug;
// 			szDebug.Format("[SMalzio] PreFO거래대금=%s", strNode);
// 			OutputDebugString(szDebug);
// 		}
		memcpy(stBong.value, strNode, min(sizeof(stBong.value), strNode.GetLength()));
		//@Solomon	091202SM123	<--

		memcpy(&pDataBuf[nPosBong + nRow*nBongSize], &stBong, nBongSize);
		if (nRow == 0)
			memcpy(&stFirstBong, &stBong, sizeof(stBong));
/*
		if (nType == 1)
			memcpy(&pDataBuf[nPosBong + nRow*nBongSize], &stBong, nBongSize);
		else
			memcpy(&pDataBuf[nBufSize - (nRow+1)*nBongSize], &stBong, nBongSize);

		// 첫 데이터를 저장해 놓는다.
		if ((nType == 1 && nRow == 0) || (nType != 1 && nRow == nCnt-1))
			memcpy(&stFirstBong, &stBong, sizeof(stBong));
*/
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
//	m_bojoInput.m_sStartTime = "090000";	// 거래시작시간(HHMMSS) <<!>>
	m_bojoInput.m_sFirstClose = CString(stFirstBong.close, sizeof(stFirstBong.close)) ;	// 조회중 첫봉의 종가.

	m_bojoInput.m_sDataType = _T("0");		//	고정. 차후 필요시 수정.
	m_bojoInput.m_sMarket = _T("1");			//	고정. 차후 필요시 수정.
	m_bojoInput.m_sNxtFlg = m_InData.Snextgb[0];

	if(nType == 0)
	{
		m_bojoInput.m_sTerm = "0";	//	틱
		m_bojoInput.m_sTick = CString(m_InData.Sunit, sizeof(m_InData.Sunit));
	}
	else if(nType == 1)
	{
		m_bojoInput.m_sTerm = "1";	//	분
		m_bojoInput.m_sTick = CString(m_InData.Sunit, sizeof(m_InData.Sunit));
		//{{JS.Kim_20100903 Solomon 60분봉 이상인 경우 시간으로 단위 변경하여야 함
		int nTick = atoi(m_bojoInput.m_sTick);
		if( nTick >= 60 )
			m_bojoInput.m_sTick.Format("%d", (nTick/ 60) * 100 + nTick % 60);
		//}}
	}
	else if(nType == 6)
	{
		m_bojoInput.m_sTerm = "6";	//	초
		m_bojoInput.m_sTick = CString(m_InData.Sunit, sizeof(m_InData.Sunit));
	}
	else if(nType >= 2)
	{
		m_bojoInput.m_sTerm.Format("%d", nType);	//	일,주,월
		m_bojoInput.m_sTick = _T("1");
	}

	CString strBozo;
	char szKey[4];
	memset(szKey, 0x00, sizeof(szKey));
	sprintf(szKey, _T("%03d"), 30); // 103은 선옵
	
	nSize = bojoMng.GetChartBozoMsg(szKey, &m_bojoInput, strNode);
	memcpy(stOut.bojomsg, strNode, nSize);
	memcpy(stOut.bojomsg, "0396", 4);

	memcpy(pDataBuf, &stOut, sizeof(stOut));

	return pDataBuf;
}

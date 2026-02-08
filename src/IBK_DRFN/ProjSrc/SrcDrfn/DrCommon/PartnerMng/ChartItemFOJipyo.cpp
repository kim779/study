// ChartItemFOJipyo.cpp: implementation of the ChartItemFOJipyo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PartnerMng.h"
#include "ChartItemFOJipyo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
char* szJipyoRowField[] = {"109302", "109034", "109023", "109029", "109030", "109031", NULL};

CChartItemFOJipyo::CChartItemFOJipyo()
{
	m_nRowCnt = 0;
	while (szJipyoRowField[m_nRowCnt] != NULL)
		m_nRowCnt++;

	m_bSet = FALSE;
}

CChartItemFOJipyo::~CChartItemFOJipyo()
{

}

void CChartItemFOJipyo::SetInData(KB_p0515_InRec* pData)
{
	memcpy(&m_InData, pData, sizeof(KB_p0515_InRec));
	m_bSet = TRUE;
}

// MUltiChart에서의 조회 구조체를 받아서 FID 형태로 변환
// 구성 : 차트조회Grid Inbound + 시세 FID Inbound + GridHeader + Grid Outbound
// winix 플랫폼에서 배열형태로 내려오는 데이터를 Grid 라고 부른다.
CString CChartItemFOJipyo::Convert()
{
	if(!m_bSet) return "";

	CString strData, strCode, strTemp;
	int i, nCount;
	
	strTemp = CString(m_InData.maxbong, sizeof(m_InData.maxbong));
	nCount = atoi(strTemp);

	strCode = CString(m_InData.code, sizeof(m_InData.code));
	strCode.TrimLeft(); strCode.TrimRight();

	strTemp = CString(m_InData.unit, sizeof(m_InData.unit));
	int nUnit = atoi(strTemp);

	strTemp = CString(m_InData.btngb, sizeof(m_InData.btngb)); // 틱(0)/분(1)/일(2)/주(3)/월(4)
	int nType = atoi(strTemp);

	//////////////////////////////////////////////////////////////////////////
	// Inbound 데이터 작성
	//////////////////////////////////////////////////////////////////////////

	// 차트 데이터
	strData += "109301"; // 종목코드 Input 심볼
	strData += sDS;
	strData += strCode;
	strData += sFS;

	strData += "109306"; // 조회일자 Input 심볼
	strData += sDS;
	strTemp = CString(m_InData.date, sizeof(m_InData.date));
	strData += strTemp;
	strData += sFS;

	switch (nType)
	{
	case 0:	break;	// 틱
	case 6:	break;	// 초
	case 1:		// 분
		strData += "109308"; // 분봉갭 Input 심볼
		strData += sDS;
		strTemp.Format("%d", nUnit);
		strData += strTemp;
		strData += sFS;
		break;

	case 2:		// 일
	case 3:		// 주
	case 4:		// 월
		strData += "109308"; // 일(1)주(2)월(3)구분 Input 심볼
		strData += sDS;
		strTemp = (nType==2) ? "1":(nType==3 ? "2":"3");
		strData += strTemp;
		strData += sFS;
		break;
	}
	//////////////////////////////////////////////////////////////////////////
	// 종목 시세 OutBound
						//	단축코드, 이름, 현재가, 전일종가, 증감, 등락율, 시가, 고가, 저가,
	char* pStockSymbol[] = { "109301", "109022", "109023", "109320", "109024", "109033", "109029", "109030", "2031", NULL};

	int nIndex = 0;
	while (pStockSymbol[nIndex] != NULL)
	{
		strData += pStockSymbol[nIndex++];
		strData += sFS;
	}

	//////////////////////////////////////////////////////////////////////////
	// 그리드성 데이터(배열성, 반복데이터) OutBound

	// 챠트 심볼
	strData += _T('$');
	if (nType == 1)	strData += "109510";
	else			strData += "109610";
	strData += sDS;

	//////////////////////////////////////////////////////////////////////////
	// 그리드 헤더
	GRID_I grid_i;
	memset(&grid_i, 0x20, sizeof(grid_i));

	char szTemp[5];
	grid_i.gdir[0] = _T('1');	//	top.
	if (m_InData.nextgb[0] == '1')	grid_i.ikey[0] = _T('2');	//	Page Down
	else							grid_i.ikey[0] = _T('0');	//	일반(Next조회).

	sprintf(szTemp, _T("%04d"), nCount);	// 조회 데이터 수
	memcpy(grid_i.nrow, szTemp, sizeof(grid_i.nrow));

	memcpy(grid_i.page, m_InData.nextkey, 4);
	memcpy(grid_i.save, m_InData.nextkey+4, sizeof(grid_i.save));

	grid_i.sdir[0] = _T('1');
	grid_i.vflg[0] = _T('1');
	memcpy(grid_i.vrow, _T("999"), sizeof(grid_i.vrow));

	char szGrid_i[128];
	memset(szGrid_i, 0x00, sizeof(szGrid_i));
	memcpy(szGrid_i, (char*)&grid_i, sizeof(grid_i));

	strData += szGrid_i;
	//////////////////////////////////////////////////////////////////////////
	// Row Data 구성
	switch (nType)
	{
	case 0:	break;	// 틱
	case 6:	break;	// 초
	case 2:		// 일
	case 3:		// 주
	case 4:		// 월
	case 1:		// 분
		for (i = 0; i < m_nRowCnt; i++)
		{
			strData += szJipyoRowField[i];
			strData += sCOL;
		}
		break;
	}
	strData += sFS;

	return strData;
}

char* CChartItemFOJipyo::Convert2Struct(LPBYTE pData, int nLen, int nKey, int &nDataLen)
{
	KB_p0515_OutRec stOut;
	CString strSrc;
	strSrc.Format(_T("%.*s"), nLen, (char*)pData);
	CString strNode, strCode, strLock;
	CString strPrice, strPrePrice, strVolume, strPreVolume;
	CString strHigh, strLow, strOpen, strHighLimit, strLowLimit;
	CString strPreHigh, strPreLow, strPreOpen;
	int nPrice;

	memset(&stOut, 0x20, sizeof(stOut));

	strCode = CDataConverter::Parser(strSrc, sFS); // 종목코드

	strNode = CDataConverter::Parser(strSrc, sFS); // 종목명
	memcpy(stOut.name, strNode, min(strNode.GetLength(), sizeof(stOut.name)));

	strPrice = CDataConverter::Parser(strSrc, sFS); // 현재가
	nPrice = atoi(strPrice);
	strPrice.Remove(_T('+'));
	memcpy(stOut.price, strPrice, min(sizeof(stOut.price), strPrice.GetLength()));

	strPrePrice = CDataConverter::Parser(strSrc, sFS); // 전일종가
	memcpy(stOut.preprice, strPrePrice, min(sizeof(stOut.preprice), strPrePrice.GetLength()));

	strNode = CDataConverter::Parser(strSrc, sFS); // 전일증감
	strNode = strNode.Mid(1);
	memcpy(stOut.change, strNode, min(sizeof(stOut.change), strNode.GetLength()));
	
	strNode = CDataConverter::Parser(strSrc, sFS); // 등락율
	strNode.Remove(_T('+'));strNode.Remove(_T('-'));strNode.Remove(_T('.'));
	memcpy(stOut.chrate, strNode, min(sizeof(stOut.chrate), strNode.GetLength()));

	strOpen = CDataConverter::Parser(strSrc, sFS); // 시가
	strOpen.Remove(_T('+')); strOpen.Remove(_T('-'));
	strHigh = CDataConverter::Parser(strSrc, sFS); // 고가
	strHigh.Remove(_T('+')); strHigh.Remove(_T('-'));
	strLow = CDataConverter::Parser(strSrc, sFS); // 저가
	strLow.Remove(_T('+')); strLow.Remove(_T('-'));
/*
	strPreOpen = CDataConverter::Parser(strSrc, sFS);		// 전일시가
	strPreOpen.Remove(_T('+')); strPreOpen.Remove(_T('-'));
	strPreHigh = CDataConverter::Parser(strSrc, sFS);		// 전일고가
	strPreHigh.Remove(_T('+')); strPreHigh.Remove(_T('-'));
	strPreLow = CDataConverter::Parser(strSrc, sFS);		// 전일저가
	strPreLow.Remove(_T('+')); strPreLow.Remove(_T('-'));
*/
	int nCnt, nType;
	strNode = CString(m_InData.btngb, sizeof(m_InData.btngb)); // 틱(0)/분(1)/일(2)/주(3)/월(4)
	nType = atoi(strNode);

	// 그리드 헤더
	GRAPH_IO gridOut;
	strNode = strSrc.Left(sizeof(gridOut));
	memcpy(&gridOut, strNode, sizeof(gridOut)); 

	strSrc = strSrc.Mid(sizeof(gridOut));
	strSrc.Remove('.');

//	if ((gridOut.xpos[0] & 0x01) || (gridOut.xpos[0] & 0x02))
//		memcpy(stOut.nkey, gridOut.page, sizeof(stOut.nkey));

	CStringArray arrNode;
	int nNodeIndex = 1;
	while(strSrc.IsEmpty() == FALSE)
	{
		strNode = CDataConverter::Parser(strSrc, (nNodeIndex == m_nRowCnt)?sROW:sCELL);
		arrNode.Add(strNode);

		if(nNodeIndex == m_nRowCnt) nNodeIndex = 1;
		else nNodeIndex++;
	}

	// 전체 데이터 수 계산
	nCnt = arrNode.GetSize() / m_nRowCnt;
	strNode.Format("%05d", nCnt);
	memcpy(stOut.fcnt, strNode, sizeof(stOut.fcnt));

	// 차트 봉 데이터
	int nBongSize = sizeof(KB_p0515_OutRecArr);
	int	nPosBong = sizeof(KB_p0515_OutRec);
	int nBufSize = nPosBong + nBongSize * nCnt;

	nDataLen = nBufSize;

	CString strDate, strTime;
	char* pDataBuf = new char[nBufSize+1];
	KB_p0515_OutRecArr stFirstBong, stBong;

	int nIndex = 0;
	for (int nRow = 0; nRow < nCnt; nRow++)
	{
		memset(&stBong, 0x20, sizeof(stBong));
		
		// date
		strDate = arrNode.GetAt(nIndex++);
		strTime = arrNode.GetAt(nIndex++); 
		if (nType == 0) // DDHHMMSS
			strNode = strDate.Right(2) + strTime;
		else if (nType == 1) //MMDDHHMM
			strNode = strDate.Right(4) + strTime.Left(4);
		else if (nType == 4) //YYYYMM
			strNode = strDate.Left(6);
		else if (nType == 6) //DDHHMMSS
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

		memcpy(&pDataBuf[nBufSize - (nRow+1)*nBongSize], &stBong, nBongSize);
		memcpy(&stFirstBong, &stBong, sizeof(stBong));
	}

	// 20130222 이상신 : 주봉일경우 마지막 데이터의 일자를 설정해 준다.(주 첫 영업일 관련)
	if (nCnt > 0 && nType == 3 && m_InData.nextgb[0] != _T('1'))
		m_bojoInput.m_sLastDataTime = CString(stBong.date, sizeof(stBong.date));
	else
		m_bojoInput.m_sLastDataTime = _T("");

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
	m_bojoInput.m_sPreVolume = strPreVolume;	// 전일거래량
//	m_bojoInput.m_sStartTime = strMarketStartTime;//"090000";	// 거래시작시간(HHMMSS) <<!>>
	m_bojoInput.m_sFirstClose = CString(stFirstBong.close, sizeof(stFirstBong.close)) ;	// 조회중 첫봉의 종가.

	m_bojoInput.m_sDataType = _T("0");		//	고정. 차후 필요시 수정.
	m_bojoInput.m_sMarket = _T("1");			//	고정. 차후 필요시 수정.
	m_bojoInput.m_sNxtFlg = m_InData.nextgb[0];

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
	//20110124_alzioyes:FO차트(FUOPT_JIPYO_CHART) 리얼처리루틴 수정.
	sprintf(szKey, _T("%03d"), 109);
	
	int nSize = bojoMng.GetChartBozoMsg(szKey, &m_bojoInput, strNode);
	memcpy(stOut.bojomsg, strNode, nSize);
	memcpy(stOut.bojomsg, "0396", 4);

	memcpy(pDataBuf, &stOut, sizeof(stOut));

	return pDataBuf;
}

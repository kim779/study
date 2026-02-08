// ChartReplay.cpp: implementation of the CChartReplay class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PartnerMng.h"
#include "ChartReplay.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
char* szReplayRowField[] = {"302", "034", "023", "032", NULL};

CChartReplay::CChartReplay()
{
	int nIndex=0;
	while (szReplayRowField[nIndex] != NULL)
		nIndex++;

	m_nRepColCnt = nIndex;
}

CChartReplay::~CChartReplay()
{

}

void CChartReplay::SetInData(KB_d1016_InRec1* pData)
{
	memcpy(&m_InData, pData, sizeof(KB_d1016_InRec1));
	m_bSet = TRUE;
}

// typedef struct
// {
//     char    shcode[16];                            /* 종목코드(16)					 */
//     char    remindDate[2];                         /* 복기시작n일전(2)                */
//     char    remindTime[6];                         /* 복기시간(hhmmss)(6)             */
// }KB_d1016_InRec1;

CString CChartReplay::Convert()
{
	if(!m_bSet) return "";
	m_nFutOpt = STOCK;

	CString strData, strCode, strTemp, strFirstTime;
	int i;

	strCode = CString(m_InData.shcode, sizeof(m_InData.shcode));
	strCode.TrimLeft(); strCode.TrimRight();
	
	if (strCode.GetLength() < 6) return "";
	else if (IS_FUT_CODE_3(strCode.Left(3)))//선물일때.. // 0024696: 파생상품 코드체계 변경 제도개선
		m_nFutOpt = FUTURE;
	else if(IS_CALL_CODE_3(strCode.Left(3)) || IS_PUT_CODE_3(strCode.Left(3)))
		m_nFutOpt = OPTION;
	
	strTemp = CString(m_InData.remindTime, sizeof(m_InData.remindTime));
	strTemp.TrimLeft(); strTemp.TrimRight();
	strFirstTime = strTemp;

	if (strTemp.IsEmpty())		m_nPage = 0;
	else						m_nPage++;

//////////////////////////////////////////////////////////////////////////
//	Inbound 데이터 작성
//////////////////////////////////////////////////////////////////////////
	_dataH	Datah;
	FillMemory(&Datah, sz_DATAH, 0x20);
	//	종목코드
	if (m_nFutOpt == FUTURE)	strData += "31301";	
	else if (m_nFutOpt == OPTION)	strData += "41301";		
	else			strData += "1301";
	strData += sDS;
	strData += strCode;
	strData += sCELL;

	if (m_nFutOpt == FUTURE)	strData += "32340"; // 당일/전일구분 Input 심볼
	else if (m_nFutOpt == OPTION)	strData += "42340"; // 당일/전일구분 Input 심볼
	else			strData += "2340"; // 당일/전일구분 Input 심볼
	strData += sDS;
	strTemp = CString(m_InData.remindDate, sizeof(m_InData.remindDate));
	if (atoi(strTemp) == 0)	
		strTemp.Format("%d%s", 0, strFirstTime); // 1 : 전일, 0 : 당일
	else	
		strTemp.Format("%d%s", 1, strFirstTime);
	strData += strTemp;
	strData += sCELL;

//Data 세팅 : KHD 
	CString stRowData, stNextData, stNextKey;
	Datah.dindex = GI_TICK;
	Datah.dunit = GI_TICK;
	if(m_nFutOpt == FUTURE)
	{
		stRowData = _T("?32500");
		stNextKey = "32324";
		stNextData.Format("%28.28s",m_InData.nkey );
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // 넥스트 키값 : KHD 
		
		strData += "31301";
		strData += sCELL;
		strData += "31900";
		strData += sCELL;
		strData += "31901";
		strData += sCELL;
	}
	else if(m_nFutOpt == OPTION)
	{
		stRowData = _T("?42500");
		stNextKey = "42324";
		stNextData.Format("%28.28s",m_InData.nkey );
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // 넥스트 키값 : KHD 
		
		strData += "41301";
		strData += sCELL;
		strData += "41900";
		strData += sCELL;
		strData += "41901";
		strData += sCELL;
	}
	else//주식일경우 
	{
		stRowData = _T("?2500");
		stNextKey = "2324";
		stNextData.Format("%28.28s",m_InData.nkey );
		strData += stNextKey; strData += sDS;	strData += stNextData;	strData += sCELL; // 넥스트 키값 : KHD 
		
		strData += "1301";
		strData += sCELL;
		strData += "1900";
		strData += sCELL;
		strData += "1901";
		strData += sCELL;
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
	temp.Format("%06d", 5000);
	CopyMemory(Datah.count, temp, temp.GetLength());
	Datah.dkey = 0 + 0x30;
	//CString strDate	= CDataConverter::Char2String(m_InData.date, sizeof(m_InData.date));
	//CopyMemory(Datah.pday, strDate, strDate.GetLength());
	
	temp.Format("%0*d", sizeof(Datah.lgap), 1);// 1틱 
	CopyMemory(Datah.lgap, temp, temp.GetLength() );
	
	char szData_i[sz_DATAH];
	memset(szData_i, 0x20, sz_DATAH);//sizeof(Datah));
	CopyMemory(szData_i, (char*)&Datah, sz_DATAH);//sizeof(Datah));
	szData_i[sz_DATAH-1] = 0x00; // KHD : 맨끝에 0x00문자를 집어 넣는다.
	strData += szData_i;
	for (i=0; i<m_nRepColCnt; i++)
	{
		if (m_nFutOpt == FUTURE)	strData += "32";
		else if(m_nFutOpt == OPTION) strData += "42";
		else			strData += "5";

		strData += szReplayRowField[i];
		strData += sCOL;
	}
	strData += sCELL;

	return strData;
}

char* CChartReplay::Convert2Struct(LPBYTE pData, int nLen, int nKey, int &nDataLen)
{
	KB_d1016_OutRec1 stOut;
	CString strSrc;
	strSrc.Format(_T("%.*s"), nLen, (char*)pData);
	CString strNode, strCode, strStartTime, strEndTime;

	memset(&stOut, 0x20, sizeof(stOut));

	memcpy(stOut.remindDate, m_InData.remindDate, sizeof(stOut.remindDate));

	strCode = CDataConverter::Parser(strSrc, sCELL); // 종목코드
	strStartTime = CDataConverter::Parser(strSrc, sCELL) + _T("00");
	strEndTime = CDataConverter::Parser(strSrc, sCELL) + _T("00");
	//KHD : NextKey
	CString strNextKey = CDataConverter::Parser(strSrc, sCELL);
	if (strNextKey !="99999999999999")// 9가 14자리면 다음키가 없는것.  
	{
		//	CString strKey;
		//	LPTSTR lpszKey = strKey.GetBuffer(sizeof(stOut.nkey) + 1);
		//	::memcpy(lpszKey, strNextKey, sizeof(stOut.nkey));
		//	strNextKey.Format("%s", lpszKey);
		
		memcpy(stOut.nkey, (LPCTSTR)strNextKey, min(sizeof(stOut.nkey),strNextKey.GetLength()));
	}
	else
	{
		memset(stOut.nkey, 0x20, sizeof(stOut.nkey));
	}
	//KHD :
	// 그리드 헤더
	GRAPH_IO gridOut;
	strNode = strSrc.Left(sizeof(gridOut));
	memcpy(&gridOut, strNode, sizeof(gridOut));

	strSrc = strSrc.Mid(sizeof(gridOut));
	
 
//	if ((gridOut.xpos[0] & 0x01) || (gridOut.xpos[0] & 0x02))
//		memcpy(stOut.nkey, gridOut.page, sizeof(stOut.nkey));

	//091027-alzioyes.기존 DRFN차트에서 소숫점없이 데이터를 받았다. 통일.
	strSrc.Remove('.');

	CStringArray arrNode;
	int nNodeIndex = 1;
	while (strSrc.IsEmpty() == FALSE)
	{
		strNode = CDataConverter::Parser(strSrc, (nNodeIndex == m_nRepColCnt)?sROW:sCELL);
		arrNode.Add(strNode);

		if (nNodeIndex == m_nRepColCnt) nNodeIndex = 1;
		else nNodeIndex++;
	}

	// 전체 데이터 수 계산
	int nCnt = arrNode.GetSize() / m_nRepColCnt;
	strNode.Format("%05d", nCnt);
	memcpy(stOut.fcnt, strNode, sizeof(stOut.fcnt));

	// 차트 봉 데이터
	int nBongSize = sizeof(KB_d1016_OutRec2);
	int nBufSize = sizeof(KB_d1016_OutRec1) + nBongSize * nCnt;
	int	nPosBong = sizeof(KB_d1016_OutRec1);

	nDataLen = nBufSize;

	CString strDate = "00000000", strTime;
	char* pDataBuf = new char[nBufSize+1];
	memset(pDataBuf, 0x20, nBufSize+1);
	KB_d1016_OutRec2 stFirstBong;

	int nIndex = 0;
	for (int nRow=0; nRow<nCnt; nRow++)
	{
		KB_d1016_OutRec2 stBong;
		memset(&stBong, 0x20, sizeof(stBong));

		// date
		strDate = arrNode.GetAt(nIndex++);
//		OutputDebugString("KHD : " + strDate);
		memcpy(stBong.date, strDate, min(sizeof(stBong.date), strDate.GetLength()));
		// time
		strTime = arrNode.GetAt(nIndex++);
//			OutputDebugString("KHD : " + strTime);
		memcpy(stBong.time, strTime, min(sizeof(stBong.time), strTime.GetLength()));
		// 종가/체결가
		strNode = arrNode.GetAt(nIndex++);
		memcpy(stBong.close, strNode, min(sizeof(stBong.close), strNode.GetLength()));
		// 거래량
		strNode = arrNode.GetAt(nIndex++);
		memcpy(stBong.cvol, strNode, min(sizeof(stBong.cvol), strNode.GetLength()));

		// 데이터 버퍼에 복사, 일주월은 초,분,틱과 순서가 반대
		memcpy(&pDataBuf[nPosBong + nRow*nBongSize], &stBong, nBongSize);

		if (nRow == 0)
			memcpy(&stFirstBong, &stBong, sizeof(stBong));
	}

	char szBojo[400+1];
	memset(szBojo, 0x20, sizeof(szBojo));

	if (atoi(strEndTime) < 153000)
		strEndTime = _T("153000");

	sprintf(szBojo, _T("0400SC=%s@MARKETTIME=%s|%s@"), (LPCSTR)(LPCTSTR)strCode,
						(LPCSTR)(LPCTSTR)strStartTime, (LPCSTR)(LPCTSTR)strEndTime);
	memcpy(stOut.bojo, szBojo, min(strlen(szBojo), sizeof(stOut.bojo)));

	memcpy(pDataBuf, &stOut, sizeof(stOut));

	return pDataBuf;
}

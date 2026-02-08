// ChartItemDollar.cpp: implementation of the CChartItemDollar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PartnerMng.h"
#include "ChartItemDollar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
char* szDollarSymbol[] = { "51301", "51022", "51023", NULL};

CChartItemDollar::CChartItemDollar()
{
	m_bSet = FALSE;
	m_nCnt = 0;
	while (szDollarSymbol[m_nCnt] != NULL)		m_nCnt++;
}

CChartItemDollar::~CChartItemDollar()
{
}

void CChartItemDollar::SetInData(KB_d1018_InRec* pData)
{
	memcpy(&m_InData, pData, sizeof(KB_d1018_InRec));
	m_bSet = TRUE;
}

// MUltiChart에서의 조회 구조체를 받아서 FID 형태로 변환
// 구성 : 차트조회Grid Inbound + 시세 FID Inbound + GridHeader + Grid Outbound
// winix 플랫폼에서 배열형태로 내려오는 데이터를 Grid 라고 부른다.
CString CChartItemDollar::Convert()
{
	if (!m_bSet) return "";

	CString strCode, strData;
	int i;

	strCode	= CDataConverter::Char2String(m_InData.code, sizeof(m_InData.code));
	//////////////////////////////////////////////////////////////////////////
	//	Inbound Data
	//	종목코드 Input 심볼
	strData += "51301";	strData += sDS;	strData += strCode;	strData += sCELL;//sFS;

	//////////////////////////////////////////////////////////////////////////
	//	OutBound Data
	for (i=0; i<m_nCnt; i++)
	{
		strData += szDollarSymbol[i];
		strData += sCELL;//KHDsFS;
	}

	return strData;
}

char* CChartItemDollar::Convert2Struct(LPBYTE pData, int nLen, int nKey, int &nDataLen)
{
	KB_d1018_OutRec stOut;
	CString strSrc;
	strSrc.Format(_T("%.*s"), nLen, (char*)pData);

	CString strNode, strCode;
	int nBufSize = sizeof(KB_d1018_OutRec);

	memset(&stOut, 0x20, sizeof(stOut));

	strCode = CDataConverter::Parser(strSrc, sCELL); // 종목코드

	strNode = CDataConverter::Parser(strSrc, sCELL); // 종목명
	memcpy(stOut.name, strNode, min(strNode.GetLength(), sizeof(stOut.name)));

	strNode = CDataConverter::Parser(strSrc, sCELL); // 현재가
	memcpy(stOut.price, strNode, min(sizeof(stOut.price), strNode.GetLength()));

	char* pDataBuf = new char[nBufSize+1];
	memcpy(pDataBuf, &stOut, sizeof(stOut));

	nDataLen = nBufSize;

	return pDataBuf;
}

// ChartItemFOForeignHoga.cpp: implementation of the CChartItemFOForeignHoga class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PartnerMng.h"
#include "ChartItemFOForeignHoga.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// 기준가, 현재가
// 매도호가5, 매도잔량5, 매도호가4, 매도잔량4, 매도호가3, 매도잔량3, 매도호가2, 매도잔량2, 매도호가1, 매도잔량1
// 매수호가1, 매수잔량1, 매수호가2, 매수잔량2, 매수호가3, 매수잔량3, 매수호가4, 매수잔량4, 매수호가5, 매수잔량5
// 매도 총 잔량, 매수 총 잔량, 고가, 저가, 등락폭, 누적거래량
char* szFOFoHogaField[] = {"70319","71023",
						"72051","72041","72052","72042","72053","72043","72054","72044","72055", "72045",
						"72071","72061","72072","72062","72073","72063","72074","72064","72075", "72065",
						"72101","72106","71030","71031","71033","71027",
						NULL};

CChartItemFOForeignHoga::CChartItemFOForeignHoga()
{
	m_bSet = FALSE;
	
	int nIndex = 0;
	while (szFOFoHogaField[nIndex] != NULL)
		nIndex++;

	m_nCnt = nIndex;
}

CChartItemFOForeignHoga::~CChartItemFOForeignHoga()
{

}

void CChartItemFOForeignHoga::SetInData(HCQ01112_IN* pData)
{
	memcpy(&m_InData, pData, sizeof(HCQ01112_IN));
	m_bSet = TRUE;
}

// MUltiChart에서의 조회 구조체를 받아서 FID 형태로 변환
// 구성 : 차트조회Grid Inbound + 시세 FID Inbound + GridHeader + Grid Outbound
// winix 플랫폼에서 배열형태로 내려오는 데이터를 Grid 라고 부른다.
CString CChartItemFOForeignHoga::Convert()
{
	if(!m_bSet) return "";
	
	int i;
	CString strData, strCode;

	strCode = CString(m_InData.jmcode, sizeof(m_InData.jmcode));

	//////////////////////////////////////////////////////////////////////////
	// Inbound 데이터 작성
	//////////////////////////////////////////////////////////////////////////
	// 차트 데이터
	strData += "70301"; // 종목코드 Input 심볼
	strData += sDS;

	strData += strCode;
	strData += sFS;

	//////////////////////////////////////////////////////////////////////////
	// 종목 시세 OutBound
	for (i=0; i<m_nCnt; i++)
	{
		strData += szFOFoHogaField[i];
		strData += sFS;
	}

	return strData;
}

char* CChartItemFOForeignHoga::Convert2Struct(LPBYTE pData, int nLen, int nKey, int &nDataLen)
{
	HCQ01112_OUT stOut;
	CString strNode, strSrc;
	strSrc.Format(_T("%.*s"), nLen, (char*)pData);

	memset(&stOut, 0x20, sizeof(stOut));
	memcpy(stOut.jmcode, m_InData.jmcode, sizeof(stOut.jmcode));

	strNode = CDataConverter::Parser(strSrc, sFS);	// 기준가
	memcpy(stOut.jsprice, strNode, min(strNode.GetLength(), sizeof(stOut.jsprice)));
	
	strNode = CDataConverter::Parser(strSrc, sFS);	// 현재가
	memcpy(stOut.price, strNode, min(strNode.GetLength(), sizeof(stOut.price)));

	strNode = CDataConverter::Parser(strSrc, sFS);	// 매도호가5
	memcpy(stOut.mdhoga5, strNode, min(strNode.GetLength(), sizeof(stOut.mdhoga5)));
	strNode = CDataConverter::Parser(strSrc, sFS);	// 매도잔량5
	memcpy(stOut.mdhorem5, strNode, min(strNode.GetLength(), sizeof(stOut.mdhorem5)));

	strNode = CDataConverter::Parser(strSrc, sFS);	// 매도호가4
	memcpy(stOut.mdhoga4, strNode, min(strNode.GetLength(), sizeof(stOut.mdhoga4)));
	strNode = CDataConverter::Parser(strSrc, sFS);	// 매도잔량4
	memcpy(stOut.mdhorem4, strNode, min(strNode.GetLength(), sizeof(stOut.mdhorem4)));
	
	strNode = CDataConverter::Parser(strSrc, sFS);	// 매도호가3
	memcpy(stOut.mdhoga3, strNode, min(strNode.GetLength(), sizeof(stOut.mdhoga3)));
	strNode = CDataConverter::Parser(strSrc, sFS);	// 매도잔량3
	memcpy(stOut.mdhorem3, strNode, min(strNode.GetLength(), sizeof(stOut.mdhorem3)));
	
	strNode = CDataConverter::Parser(strSrc, sFS);	// 매도호가2
	memcpy(stOut.mdhoga2, strNode, min(strNode.GetLength(), sizeof(stOut.mdhoga2)));
	strNode = CDataConverter::Parser(strSrc, sFS);	// 매도잔량2
	memcpy(stOut.mdhorem2, strNode, min(strNode.GetLength(), sizeof(stOut.mdhorem2)));
	
	strNode = CDataConverter::Parser(strSrc, sFS);	// 매도호가1
	memcpy(stOut.mdhoga1, strNode, min(strNode.GetLength(), sizeof(stOut.mdhoga1)));
	strNode = CDataConverter::Parser(strSrc, sFS);	// 매도잔량1
	memcpy(stOut.mdhorem1, strNode, min(strNode.GetLength(), sizeof(stOut.mdhorem1)));

	strNode = CDataConverter::Parser(strSrc, sFS);	// 매수호가1
	memcpy(stOut.mshoga1, strNode, min(strNode.GetLength(), sizeof(stOut.mshoga1)));
	strNode = CDataConverter::Parser(strSrc, sFS);	// 매수잔량1
	memcpy(stOut.mshorem1, strNode, min(strNode.GetLength(), sizeof(stOut.mshorem1)));
	
	strNode = CDataConverter::Parser(strSrc, sFS);	// 매수호가2
	memcpy(stOut.mshoga2, strNode, min(strNode.GetLength(), sizeof(stOut.mshoga2)));
	strNode = CDataConverter::Parser(strSrc, sFS);	// 매수잔량2
	memcpy(stOut.mshorem2, strNode, min(strNode.GetLength(), sizeof(stOut.mshorem2)));
	
	strNode = CDataConverter::Parser(strSrc, sFS);	// 매수호가3
	memcpy(stOut.mshoga3, strNode, min(strNode.GetLength(), sizeof(stOut.mshoga3)));
	strNode = CDataConverter::Parser(strSrc, sFS);	// 매수잔량3
	memcpy(stOut.mshorem3, strNode, min(strNode.GetLength(), sizeof(stOut.mshorem3)));
	
	strNode = CDataConverter::Parser(strSrc, sFS);	// 매수호가4
	memcpy(stOut.mshoga4, strNode, min(strNode.GetLength(), sizeof(stOut.mshoga4)));
	strNode = CDataConverter::Parser(strSrc, sFS);	// 매수잔량4
	memcpy(stOut.mshorem4, strNode, min(strNode.GetLength(), sizeof(stOut.mshorem4)));
	
	strNode = CDataConverter::Parser(strSrc, sFS);	// 매수호가5
	memcpy(stOut.mshoga5, strNode, min(strNode.GetLength(), sizeof(stOut.mshoga5)));
	strNode = CDataConverter::Parser(strSrc, sFS);	// 매수잔량5
	memcpy(stOut.mshorem5, strNode, min(strNode.GetLength(), sizeof(stOut.mshorem5)));

	// 매도 총 잔량
	strNode = CDataConverter::Parser(strSrc, sFS);
	memcpy(stOut.totmdhorem, strNode, min(strNode.GetLength(), sizeof(stOut.totmdhorem)));
	// 매수 총 잔량
	strNode = CDataConverter::Parser(strSrc, sFS);
	memcpy(stOut.totmshorem, strNode, min(strNode.GetLength(), sizeof(stOut.totmshorem)));
	// 고가
	strNode = CDataConverter::Parser(strSrc, sFS);
	memcpy(stOut.high, strNode, min(strNode.GetLength(), sizeof(stOut.high)));
	// 저가
	strNode = CDataConverter::Parser(strSrc, sFS);
	memcpy(stOut.low, strNode, min(strNode.GetLength(), sizeof(stOut.low)));
	// 등락부호
//	strNode = CDataConverter::Parser(strSrc, sFS);
//	memcpy(stOut.sign, strNode, min(strNode.GetLength(), sizeof(stOut.sign)));
	// 등락폭
	strNode = CDataConverter::Parser(strSrc, sFS);
	memcpy(stOut.change, strNode, min(strNode.GetLength(), sizeof(stOut.change)));
	// 누적거래량
	strNode = CDataConverter::Parser(strSrc, sFS);
	memcpy(stOut.volume, strNode, min(strNode.GetLength(), sizeof(stOut.volume)));

	int nSize = sizeof(HCQ01112_OUT);
	
	char* pDataBuf = NULL;
	pDataBuf = new char[nSize+1];
	memset(pDataBuf, 0x00, nSize+1);
	memcpy(pDataBuf, &stOut, sizeof(stOut));
	
	nDataLen = sizeof(HCQ01112_OUT);

	return pDataBuf;
}

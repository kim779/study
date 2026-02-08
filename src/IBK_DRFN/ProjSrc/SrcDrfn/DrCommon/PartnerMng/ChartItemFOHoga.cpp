// ChartItemFOHoga.cpp: implementation of the CChartItemFOHoga class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PartnerMng.h"
#include "ChartItemFOHoga.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
						//기준가,	상한가,		하한가,		현재가,	
char* szFOHogaField[] = {"30319",	"30354",	"30355",	"31023",
					//	매도호가1,	매도호가2,	매도호가3,	매도호가1,	매도호가2,	매도호가3,
						"32051",	"32052",	"32053",	"32071",	"32072",	"32073",	NULL};
//기준가,	상한가,		하한가,		현재가,	
char* szOptionHogaField[] = {"40319",	"40354",	"40355",	"41023",
//	매도호가1,	매도호가2,	매도호가3,	매도호가1,	매도호가2,	매도호가3,
"42051",	"42052",	"42053",	"42071",	"42072",	"42073",	NULL};

CChartItemFOHoga::CChartItemFOHoga()
{
	m_bSet = FALSE;

	int nIndex = 0;
	while (szFOHogaField[nIndex] != NULL)
		nIndex++;

	m_nCnt = nIndex;
}

CChartItemFOHoga::~CChartItemFOHoga()
{

}

void CChartItemFOHoga::SetInData(KB_f0201_InRec1* pData)
{
	memcpy(&m_InData, pData, sizeof(KB_f0201_InRec1));
	m_bSet = TRUE;
}

// MUltiChart에서의 조회 구조체를 받아서 FID 형태로 변환
// 구성 : 차트조회Grid Inbound + 시세 FID Inbound + GridHeader + Grid Outbound
// winix 플랫폼에서 배열형태로 내려오는 데이터를 Grid 라고 부른다.
CString CChartItemFOHoga::Convert()
{
	if(!m_bSet) return "";
	CString strCode;
	strCode = CString(m_InData.futcode, sizeof(m_InData.futcode));
	IMasterDataManagerLast* pMasterDataManager = (IMasterDataManagerLast*)AfxGetPctrInterface(UUID_IMasterDataManager);
	m_nCodeType = pMasterDataManager->GetCodeTypeFromCode(strCode);
	
	switch(m_nCodeType)
	{
	case CDRCodeTypeInfo::CODE_KFUTURE://연결선물?? 
		return ConvertFuture();
		break;
	case CDRCodeTypeInfo::CODE_FUOPT_COMMODITY://상품선물 
	//	return ConvertCommodity();	
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

CString CChartItemFOHoga::ConvertFuture()
{
	CString strData, strCode;
	strCode = CString(m_InData.futcode, sizeof(m_InData.futcode));
	
	//////////////////////////////////////////////////////////////////////////
	// Inbound 데이터 작성
	//////////////////////////////////////////////////////////////////////////
	// 차트 데이터
	strData += "30301"; // 종목코드 Input 심볼
	strData += sDS;
	
	strData += strCode;
	strData += sCELL;
	
	//////////////////////////////////////////////////////////////////////////
	// 종목 시세 OutBound
	for (int i=0; i<m_nCnt; i++)
	{
		strData += szFOHogaField[i];
		strData += sCELL;
	}

	return strData;
}

CString CChartItemFOHoga::ConvertOption()
{
	CString strData, strCode;
	strCode = CString(m_InData.futcode, sizeof(m_InData.futcode));
	
	//////////////////////////////////////////////////////////////////////////
	// Inbound 데이터 작성
	//////////////////////////////////////////////////////////////////////////
	// 차트 데이터
	strData += "40301"; // 종목코드 Input 심볼
	strData += sDS;
	
	strData += strCode;
	strData += sCELL;
	
	//////////////////////////////////////////////////////////////////////////
	// 종목 시세 OutBound
	for (int i=0; i<m_nCnt; i++)
	{
		strData += szOptionHogaField[i];
		strData += sCELL;
	}
	return strData;
}

char* CChartItemFOHoga::Convert2Struct(LPBYTE pData, int nLen, CString szTRCode, int &nDataLen)
{
	KB_f0201_OutRec1 stOut;
	CString strNode, strSrc;
	strSrc.Format(_T("%.*s"), nLen, (char*)pData);

	memset(&stOut, 0x20, sizeof(stOut));

	memcpy(stOut.futcode, m_InData.futcode, sizeof(stOut.futcode));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 기준가
	strNode.Remove('.');
	memcpy(stOut.recprice, strNode, min(strNode.GetLength(), sizeof(stOut.recprice)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 상한가
	strNode.Remove('.');
	memcpy(stOut.uplmtprice, strNode, min(strNode.GetLength(), sizeof(stOut.uplmtprice)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 하한가
	strNode.Remove('.');
	memcpy(stOut.dnlmtprice, strNode, min(strNode.GetLength(), sizeof(stOut.dnlmtprice)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 현재가
	strNode.Remove('.');
	memcpy(stOut.price, strNode, min(strNode.GetLength(), sizeof(stOut.price)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 매도호가1
	strNode.Remove('.');
	memcpy(stOut.offerho1, strNode, min(strNode.GetLength(), sizeof(stOut.offerho1)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 매도호가2
	strNode.Remove('.');
	memcpy(stOut.offerho2, strNode, min(strNode.GetLength(), sizeof(stOut.offerho2)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 매도호가3
	strNode.Remove('.');
	memcpy(stOut.offerho3, strNode, min(strNode.GetLength(), sizeof(stOut.offerho3)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 매수호가1
	strNode.Remove('.');
	memcpy(stOut.bidho1, strNode, min(strNode.GetLength(), sizeof(stOut.bidho1)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 매수호가2
	strNode.Remove('.');
	memcpy(stOut.bidho2, strNode, min(strNode.GetLength(), sizeof(stOut.bidho2)));

	strNode = CDataConverter::Parser(strSrc, sCELL);	// 매수호가3
	strNode.Remove('.');
	memcpy(stOut.bidho3, strNode, min(strNode.GetLength(), sizeof(stOut.bidho3)));

	int nSize = sizeof(KB_f0201_OutRec1);

	char* pDataBuf = NULL;
	pDataBuf = new char[nSize+1];
	memset(pDataBuf, 0x00, nSize+1);
	memcpy(pDataBuf, &stOut, sizeof(stOut));

	return pDataBuf;
}

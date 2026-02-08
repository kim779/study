// DataConverter.cpp: implementation of the CDataConverter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PartnerMng.h"
#include "DataConverter.h"

#include "../../chart/COMMONTR/KB_d1017.h"

#include "../../inc/IMainInfoMng.h"

#include "../../ForSite/FS_Env.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define CALC_CHGRATE(x,y) (double)(((double)((double)(x)-(double)(y))*100.0)/((double)y))

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDataConverter::CDataConverter()
{

}

CDataConverter::~CDataConverter()
{

}

int CDataConverter::GetCodeType(CString strCode)
{
	char pzType[3];
	int nLength = strCode.GetLength(), nType = -1;
	
	if(nLength < 3) return ERROR_CODE;
	
	memcpy(pzType, (LPSTR)(LPCTSTR)strCode, 3);
	
	if(nLength == 6 && pzType[1] == '6' && pzType[2] == '9')
		nType = ETF_CODE;
	else if(nLength == 6 && strCode.GetAt(strCode.GetLength() - 1 ) == '0')
		nType = STOCK_CODE;
	else if(nLength >= 8 && IS_FUT_CODE_1(pzType[0]) && pzType[1] == '0' && pzType[2] == '1') // 0024696: 파생상품 코드체계 변경 제도개선
		nType = FUTURE_CODE;
	else if(nLength == 8 && IS_CALL_CODE_1(pzType[0]) && pzType[1] == '0' && pzType[2] == '1')  // 0024696: 파생상품 코드체계 변경 제도개선
		nType = OPTION_CALL_CODE;
	else if(nLength == 8 && IS_PUT_CODE_1(pzType[0]) && pzType[1] == '0' && pzType[2] == '1')  // 0024696: 파생상품 코드체계 변경 제도개선
		nType = OPTION_PUT_CODE;
	else if(nLength == 8 && (IS_CALL_CODE_1(pzType[0]) || IS_PUT_CODE_1(pzType[0])))  // 0024696: 파생상품 코드체계 변경 제도개선
		nType = STOCK_OPT_CODE;
	else if(nLength >= 8 && IS_FUT_CODE_1(pzType[0]) && pzType[1] == '0' && pzType[2] == '3')  // 0024696: 파생상품 코드체계 변경 제도개선
		nType = STAR_FUTURE_CODE;
	
	return nType;
}

CString CDataConverter::ConvertToFid(CString strTR, void* pSendData, int nLen)
{
	CString strSendData;

	if(strTR == QUERY_STRJONGMOK)
	{
		m_Stock.SetInData((KB_p0602_InRec1*)pSendData);
		strSendData = m_Stock.Convert();
	}
	else if(strTR == QUERY_STRUPJONG)
	{
		m_Upjong.SetInData((KB_p0603_InRec1*)pSendData);
		strSendData = m_Upjong.Convert();
	}
	else if(strTR == QUERY_STRFUTOPT)
	{
		m_FutOpt.SetInData((KB_p0604_InRec1*)pSendData);
		strSendData = m_FutOpt.Convert();
	}
	else if(strTR == QUERY_STRSTOCKFUTOPT)
	{
		m_StockFO.SetInData((KB_d1013_InRec1*)pSendData);
		strSendData = m_StockFO.Convert();
	}
	else if(strTR == QUERY_STRFOREIGN)
	{
		m_Foreign.SetInData((KB_p0606_InRec1*)pSendData);
		strSendData = m_Foreign.Convert();
	}
	else if(strTR == QUERY_STRREPLAY)
	{
		m_Replay.SetInData((KB_d1016_InRec1*)pSendData);
		strSendData = m_Replay.Convert();
	}
	else if(strTR == QUERY_STRMARKET)
	{
		m_Market.SetInData((KB_d1015_InRec1*)pSendData);
		strSendData = m_Market.Convert();
	}
	else if(strTR == QUERY_STOCK_HOGA)
	{
		m_SHoga.SetInData((LPk01230In1)pSendData);
		strSendData = m_SHoga.Convert();
	}
	else if(strTR == QUERY_FUTOPT_HOGA)
	{
		m_FOHoga.SetInData((KB_f0201_InRec1*)pSendData);
		strSendData = m_FOHoga.Convert();
	}
	else if(strTR == QUERY_STRFOJIPYO)
	{
		m_FoJipyo.SetInData((KB_p0515_InRec*)pSendData);
		strSendData = m_FoJipyo.Convert();
	}
	else if(strTR == QUERY_STRPREFUTOPT)
	{
		m_PreFutOpt.SetInData((KB_p0604_InRec1*)pSendData);
		strSendData = m_PreFutOpt.Convert();
	}
	else if(strTR == QUERY_STRDOLLAR)
	{
		m_Dollar.SetInData((KB_d1018_InRec*)pSendData);
		strSendData = m_Dollar.Convert();
	}
	else if(strTR == QUERY_STRFOFOREIGN)
	{
		m_FOForeign.SetInData((HCQ03010_IN*)pSendData);
		strSendData = m_FOForeign.Convert();
	}
	else if(strTR == QUERY_STRFOFOREIGN_HOGA)
	{
		m_FOForeignHoga.SetInData((HCQ01112_IN*)pSendData);
		strSendData = m_FOForeignHoga.Convert();
	}
	//<<20100315_JS.Kim 솔로몬
	else if(strTR == QUERY_STRFOFOREIGN_CHE2)
	{
		m_FOForeignChe2.SetInData((FX_CHE_RQ_INPUT *)pSendData);
		m_FOForeignChe2.SetTR(strTR);
		strSendData = m_FOForeignChe2.Convert();
	}
	else if(strTR == QUERY_STRFOFOREIGN_JAN)
	{
		m_FOForeignJan.SetInData((FX_CHE_RQ_INPUT *)pSendData);
		m_FOForeignJan.SetTR(strTR);
		strSendData = m_FOForeignJan.Convert();
	}
	else if(strTR == QUERY_STRFX)
	{
		m_FX.SetInData((SM_fx112_InRec1*)pSendData);
		strSendData = m_FX.Convert();
	}
	else if(strTR == QUERY_STRFX_CHE)
	{
		m_FXChe.SetInData((FX_CHE_RQ_INPUT *)pSendData);
		m_FXChe.SetTR(strTR);
		strSendData = m_FXChe.Convert();
	}
	else if(strTR == QUERY_STRFX_CHE2)
	{
		m_FXChe2.SetInData((FX_CHE_RQ_INPUT *)pSendData);
		m_FXChe2.SetTR(strTR);
		strSendData = m_FXChe2.Convert();
	}
	else if(strTR == QUERY_STRFX_JAN)
	{
		m_FXJan.SetInData((FX_CHE_RQ_INPUT *)pSendData);
		m_FXJan.SetTR(strTR);
		strSendData = m_FXJan.Convert();
	}
//KHD
// 	else if(strTR == QUERY_STRPLUS_STOCK)
// 	{
// 		m_Plus_Stock.SetInData((CStringArray*)pSendData);
// 		strSendData = m_Plus_Stock.Convert();
// 	}
// 	else if(strTR == QUERY_STRPLUS_MARKET)
// 	{
// 		m_Plus_Stock.SetInitMarketList((INT)pSendData);
// 		strSendData = m_Plus_Stock.Convert_Market();
// 	}
// 	else if(strTR == QUERY_STRPLUS_KOSPI ||strTR == QUERY_STRPLUS_KOSDAK ||strTR == QUERY_STRPLUS_FO  )
// 	{
// 		//KHD :시장 지수 
// 		strSendData = m_Plus_Stock.Convert_Investor((char*)pSendData);
// //		strSendData = m_Plus_Stock.Convert_Market();
// 	}
// 	else if(strTR == QUERY_STRPLUS_SIGNAL)
// 	{
// 	//	m_Plus_Stock.SetInitMarketList((INT)pSendData);
// 		strSendData = m_Plus_Stock.Convert_Maret_Signal((char*)pSendData);
// 	}
//END
	//>>

	return strSendData;
}

char* CDataConverter::Convert2Struct(LPBYTE pData, int nLen, int nKey, int &nDataLen)
{
	char* pDataBuf = NULL;

	switch (nKey)
	{
	case TRKEY_STRJONGMOK:
		pDataBuf = m_Stock.Convert2Struct(pData, nLen, nKey, nDataLen);
		break;
	case TRKEY_STRUPJONG:
		pDataBuf = m_Upjong.Convert2Struct(pData, nLen, nKey, nDataLen);
		break;
	case TRKEY_STRFUTOPT:
		pDataBuf = m_FutOpt.Convert2Struct(pData, nLen, nKey, nDataLen);
		break;
	case TRKEY_STRSTOCKFUTOPT:
		pDataBuf = m_StockFO.Convert2Struct(pData, nLen, nKey, nDataLen);
		break;
	case TRKEY_STRFOREIGN:
		pDataBuf = m_Foreign.Convert2Struct(pData, nLen, nKey, nDataLen);
		break;
	case TRKEY_STRREPLAY:
		pDataBuf = m_Replay.Convert2Struct(pData, nLen, nKey, nDataLen);
		break;
	case TRKEY_STOCK_HOGA:
		pDataBuf = m_SHoga.Convert2Struct(pData, nLen, nKey, nDataLen);
		break;
	case TRKEY_STRFOJIPYO:
		pDataBuf = m_FoJipyo.Convert2Struct(pData, nLen, nKey, nDataLen);
		break;
	case TRKEY_STRPREFUTOPT:
		pDataBuf = m_PreFutOpt.Convert2Struct(pData, nLen, nKey, nDataLen);
		break;
	case TRKEY_STRDOLLAR:
		pDataBuf = m_Dollar.Convert2Struct(pData, nLen, nKey, nDataLen);
		break;
	case TRKEY_STRFOFOREIGN:
		pDataBuf = m_FOForeign.Convert2Struct(pData, nLen, nKey, nDataLen);
		break;
	case TRKEY_STRFOFOREIGN_HOGA:
		pDataBuf = m_FOForeignHoga.Convert2Struct(pData, nLen, nKey, nDataLen);
		break;
	//<<20100315_JS.Kim 솔로몬
	case TRKEY_STRFOFOREIGN_CHE2:
		pDataBuf = m_FOForeignChe2.Convert2Struct(pData, nLen, nKey, nDataLen);
		break;
	case TRKEY_STRFOFOREIGN_JAN:
		pDataBuf = m_FOForeignJan.Convert2Struct(pData, nLen, nKey, nDataLen);
		break;
	case TRKEY_STRFX:
		pDataBuf = m_FX.Convert2Struct(pData, nLen, nKey, nDataLen);
		break;
	case TRKEY_STRFX_CHE:
		pDataBuf = m_FXChe.Convert2Struct(pData, nLen, nKey, nDataLen);
		break;
	case TRKEY_STRFX_CHE2:
		pDataBuf = m_FXChe2.Convert2Struct(pData, nLen, nKey, nDataLen);
		break;
	case TRKEY_STRFX_JAN:
		pDataBuf = m_FXJan.Convert2Struct(pData, nLen, nKey, nDataLen);
		break;
	//>>
//KHD :
//KHD
//	case TRKEY_PLUS_STOCK:
// 		pDataBuf = m_Plus_Stock.Convert2Struct(pData, nLen, nKey, nDataLen);
// 		break;
// 	case TRKEY_PLUS_MARKET:
// 		pDataBuf = m_Plus_Stock.Convert2Struct_Market(pData, nLen, nKey, nDataLen);
// 		break;
// 	case TRKEY_PLUS_KOSPI:
// 	case TRKEY_PLUS_KOSDAK:
// 	case TRKEY_PLUS_FO:
// 		pDataBuf = m_Plus_Stock.Convert2Struct_Investor(pData, nLen, nKey, nDataLen);
// 		break;
// 	case TRKEY_PLUS_SIGNAL:
// 		pDataBuf = m_Plus_Stock.Convert2Struct_Signal(pData, nLen, nKey, nDataLen);
// 	
// 		break;
//END	
	}

	if (nKey >= TRKEY_STRMARKET_0 && nKey <= TRKEY_STRMARKET_END)
		pDataBuf = m_Market.Convert2Struct(pData, nLen, nKey, nDataLen);

	return pDataBuf;
}

char* CDataConverter::Convert2Struct(LPBYTE pData, int nLen, CString strTR, int &nDataLen)
{
	char* pDataBuf = NULL;
/*
	if (strTR.CompareNoCase(QUERY_STRJONGMOK) == 0)
		pDataBuf = m_Stock.Convert2Struct(pData, nLen, TRKEY_STRJONGMOK, nDataLen);
	else if (strTR.CompareNoCase(QUERY_STRUPJONG) == 0)
		pDataBuf = m_Stock.Convert2Struct(pData, nLen, TRKEY_STRUPJONG, nDataLen);
	else if (strTR.CompareNoCase(QUERY_STRFUTOPT) == 0)
		pDataBuf = m_Stock.Convert2Struct(pData, nLen, TRKEY_STRFUTOPT, nDataLen);
	else if (strTR.CompareNoCase(QUERY_STRSTOCKFUTOPT) == 0)
		pDataBuf = m_Stock.Convert2Struct(pData, nLen, TRKEY_STRSTOCKFUTOPT, nDataLen);
	else if (strTR.CompareNoCase(QUERY_STRFOREIGN) == 0)
		pDataBuf = m_Stock.Convert2Struct(pData, nLen, TRKEY_STRFOREIGN, nDataLen);
	else if (strTR.CompareNoCase(QUERY_STRREPLAY) == 0)
		pDataBuf = m_Stock.Convert2Struct(pData, nLen, TRKEY_STRREPLAY, nDataLen);
	else if (strTR.CompareNoCase(QUERY_STOCK_HOGA) == 0)
		pDataBuf = m_Stock.Convert2Struct(pData, nLen, TRKEY_STOCK_HOGA, nDataLen);
	else if (strTR.CompareNoCase(QUERY_STRFOJIPYO) == 0)
		pDataBuf = m_Stock.Convert2Struct(pData, nLen, TRKEY_STRFOJIPYO, nDataLen);
	else if (strTR.CompareNoCase(QUERY_STRPREFUTOPT) == 0)
		pDataBuf = m_Stock.Convert2Struct(pData, nLen, TRKEY_STRPREFUTOPT, nDataLen);
	else if (strTR.CompareNoCase(QUERY_STRDOLLAR) == 0)
		pDataBuf = m_Stock.Convert2Struct(pData, nLen, TRKEY_STRDOLLAR, nDataLen);
	else if (strTR.CompareNoCase(QUERY_STRMARKET) == 0)
		pDataBuf = m_Stock.Convert2Struct(pData, nLen, , nDataLen);
*/
	if (strTR.CompareNoCase(QUERY_STOCK_HOGA) == 0)
		pDataBuf = m_SHoga.Convert2Struct(pData, nLen, TRKEY_STOCK_HOGA, nDataLen);
	else if (strTR.CompareNoCase(QUERY_FUTOPT_HOGA) == 0)
		pDataBuf = m_FOHoga.Convert2Struct(pData, nLen, strTR, nDataLen);

	return pDataBuf;
}

//@Solomon:091123SM068	-->
CChartBozoInput CDataConverter::GetBozoData(int nKey)
{
	CChartBozoInput input;
	switch (nKey)
	{
	case TRKEY_STRJONGMOK:		return m_Stock.GetBozoInputData();
	case TRKEY_STRUPJONG:		return m_Upjong.GetBozoInputData();
	case TRKEY_STRFUTOPT:		return m_FutOpt.GetBozoInputData();
	case TRKEY_STRSTOCKFUTOPT:	return m_StockFO.GetBozoInputData();
	case TRKEY_STRFOREIGN:		return m_Foreign.GetBozoInputData();
	case TRKEY_STRFOJIPYO:		return m_FoJipyo.GetBozoInputData();
	case TRKEY_STRPREFUTOPT:	return m_PreFutOpt.GetBozoInputData();
	case TRKEY_STRREPLAY:
	case TRKEY_STOCK_HOGA:
	case TRKEY_STRDOLLAR:		return input;
	case TRKEY_STRFOFOREIGN:		return m_FOForeign.GetBozoInputData();
	case TRKEY_STRFOFOREIGN_HOGA:	return m_FOForeignHoga.GetBozoInputData();
	case TRKEY_STRFX:				return m_FX.GetBozoInputData();			//20100315_JS.Kim 솔로몬
	}

	if (nKey >= TRKEY_STRMARKET_0 && nKey <= TRKEY_STRMARKET_13)
		return input;

	return input;
}

char* CDataConverter::GetConv2StructGuideLine(int nKey, int &nDataLen)
{
	BOOL bRet = TRUE, bDouble = FALSE;
	CChartBozoInput input;
	switch (nKey)
	{
	case TRKEY_STRJONGMOK:		input = m_Stock.GetBozoInputData();		break;
	case TRKEY_STRUPJONG:		input = m_Upjong.GetBozoInputData();	bDouble = TRUE;	break;
	case TRKEY_STRFUTOPT:		input = m_FutOpt.GetBozoInputData();	bDouble = FALSE;	break;
	case TRKEY_STRSTOCKFUTOPT:	input = m_StockFO.GetBozoInputData();	break;
	case TRKEY_STRFOREIGN:		input = m_Foreign.GetBozoInputData();	break;
	case TRKEY_STRFOFOREIGN:	input = m_FOForeign.GetBozoInputData();	break;
	case TRKEY_STRFX:			input = m_FX.GetBozoInputData();		break;//20100326_JS.Kim
	case TRKEY_STRREPLAY:
	case TRKEY_STOCK_HOGA:
	case TRKEY_STRFOJIPYO:
	case TRKEY_STRPREFUTOPT:
	default:					bRet = FALSE;		break;
	}

	if (!bRet)
	{
		nDataLen = -1;
		return "";
	}

	KB_d1017_OutRec1 outRec;
	memset(&outRec, 0x20, sizeof(KB_d1017_OutRec1));
	if (bDouble)
	{
		CString strData;
		double dValue = 0;
		dValue = atof(input.m_sPreOpen) / 100.0;		strData.Format(_T("%.2f"), dValue);
		memcpy(outRec.preopen,	(LPCSTR)(LPCTSTR)strData,	min(sizeof(outRec.preopen),	 strData.GetLength()));
		dValue = atof(input.m_sPreHigh) / 100.0;		strData.Format(_T("%.2f"), dValue);
		memcpy(outRec.prehigh,	(LPCSTR)(LPCTSTR)strData,	min(sizeof(outRec.prehigh),	 strData.GetLength()));
		dValue = atof(input.m_sPreLow) / 100.0;		strData.Format(_T("%.2f"), dValue);
		memcpy(outRec.prelow,	(LPCSTR)(LPCTSTR)strData,	min(sizeof(outRec.prelow),	 strData.GetLength()));
		dValue = atof(input.m_sPreprice) / 100.0;		strData.Format(_T("%.2f"), dValue);
		memcpy(outRec.preclose, (LPCSTR)(LPCTSTR)strData,	min(sizeof(outRec.preclose), strData.GetLength()));
		dValue = atof(input.m_sOpen) / 100.0;		strData.Format(_T("%.2f"), dValue);
		memcpy(outRec.open,		(LPCSTR)(LPCTSTR)strData,	min(sizeof(outRec.open),	 strData.GetLength()));
		dValue = atof(input.m_sHigh) / 100.0;		strData.Format(_T("%.2f"), dValue);
		memcpy(outRec.high,		(LPCSTR)(LPCTSTR)strData,	min(sizeof(outRec.high),	 strData.GetLength()));
		dValue = atof(input.m_sLow) / 100.0;		strData.Format(_T("%.2f"), dValue);
		memcpy(outRec.low,		(LPCSTR)(LPCTSTR)strData,	min(sizeof(outRec.low),		 strData.GetLength()));
		dValue = atof(input.m_sPrice) / 100.0;		strData.Format(_T("%.2f"), dValue);
		memcpy(outRec.close,	(LPCSTR)(LPCTSTR)strData,	min(sizeof(outRec.close),	 strData.GetLength()));
		dValue = atof(input.m_sUpLmtPrice) / 100.0;		strData.Format(_T("%.2f"), dValue);
		memcpy(outRec.hprice,	(LPCSTR)(LPCTSTR)strData,	min(sizeof(outRec.hprice),	 strData.GetLength()));
		dValue = atof(input.m_sDnLmtPrice) / 100.0;		strData.Format(_T("%.2f"), dValue);
		memcpy(outRec.lprice,	(LPCSTR)(LPCTSTR)strData,	min(sizeof(outRec.lprice),	 strData.GetLength()));
	}
	else
	{
		memcpy(outRec.preopen,	input.m_sPreOpen,	min(sizeof(outRec.preopen),	 input.m_sPreOpen.GetLength()));
		memcpy(outRec.prehigh,	input.m_sPreHigh,	min(sizeof(outRec.prehigh),	 input.m_sPreHigh.GetLength()));
		memcpy(outRec.prelow,	input.m_sPreLow,	min(sizeof(outRec.prelow),	 input.m_sPreLow.GetLength()));
		memcpy(outRec.preclose, input.m_sPreprice,	min(sizeof(outRec.preclose), input.m_sPreprice.GetLength()));
		memcpy(outRec.open,		input.m_sOpen,		min(sizeof(outRec.open),	 input.m_sOpen.GetLength()));
		memcpy(outRec.high,		input.m_sHigh,		min(sizeof(outRec.high),	 input.m_sHigh.GetLength()));
		memcpy(outRec.low,		input.m_sLow,		min(sizeof(outRec.low),		 input.m_sLow.GetLength()));
		memcpy(outRec.close,	input.m_sPrice,		min(sizeof(outRec.close),	 input.m_sPrice.GetLength()));
		memcpy(outRec.hprice,	input.m_sUpLmtPrice,min(sizeof(outRec.hprice),	 input.m_sUpLmtPrice.GetLength()));
		memcpy(outRec.lprice,	input.m_sDnLmtPrice,min(sizeof(outRec.lprice),	 input.m_sDnLmtPrice.GetLength()));
	}

	int nSize = sizeof(KB_d1017_OutRec1);
	char *pszBuff = new char[nSize+1];
	memcpy(pszBuff, &outRec, sizeof(KB_d1017_OutRec1));

	nDataLen = nSize;

	return pszBuff;
}
//@Solomon:091123SM068	<--

CString CDataConverter::GetTrNo(int nKey)
{
	switch(nKey)
	{
	case TRKEY_STRJONGMOK:
		return QUERY_STRJONGMOK;
	case TRKEY_STRUPJONG:
		return QUERY_STRUPJONG;
	case TRKEY_STRPREFUTOPT:
		return QUERY_STRPREFUTOPT;
	case TRKEY_STRFUTOPT:
		return QUERY_STRFUTOPT;
	case TRKEY_STRSTOCKFUTOPT:
		return QUERY_STRSTOCKFUTOPT;
	case TRKEY_STRFOREIGN:
		return QUERY_STRFOREIGN;
	case TRKEY_STRREPLAY:
		return QUERY_STRREPLAY;
	case TRKEY_STRFOJIPYO:
		return QUERY_STRFOJIPYO;
	case TRKEY_STRDOLLAR:
		return QUERY_STRDOLLAR;
	case TRKEY_STOCK_HOGA:
		return QUERY_STOCK_HOGA;
	case TRKEY_STRFOFOREIGN:
		return QUERY_STRFOFOREIGN;
	case TRKEY_STRFOFOREIGN_HOGA:
		return QUERY_STRFOFOREIGN_HOGA;
	//<<20100315_JS.Kim 솔로몬
	case TRKEY_STRFOFOREIGN_CHE2:
		return QUERY_STRFOFOREIGN_CHE2;
	case TRKEY_STRFOFOREIGN_JAN:
		return QUERY_STRFOFOREIGN_JAN;

	case TRKEY_STRFX:
		return QUERY_STRFX;
	case TRKEY_STRFX_CHE:
		return QUERY_STRFX_CHE;
	case TRKEY_STRFX_CHE2:
		return QUERY_STRFX_CHE2;
	case TRKEY_STRFX_JAN:
		return QUERY_STRFX_JAN;
	case TRKEY_PLUS_STOCK:
		return QUERY_STRPLUS_STOCK;//KHD
	case TRKEY_PLUS_MARKET:
		return QUERY_STRPLUS_MARKET;//KHD
	case TRKEY_PLUS_KOSPI:
		return QUERY_STRPLUS_KOSPI;//KHD
	case TRKEY_PLUS_KOSDAK:
		return QUERY_STRPLUS_KOSDAK;//KHD
	case TRKEY_PLUS_FO:
		return QUERY_STRPLUS_FO;//KHD
	case TRKEY_PLUS_SIGNAL:
		return QUERY_STRPLUS_SIGNAL;//KHD


	//>>
	}

	if (nKey >= TRKEY_STRMARKET_0 && nKey <= TRKEY_STRMARKET_END)
		return QUERY_STRMARKET;

	return "";
}

CString CDataConverter::Parser(CString &strSrc, CString strSub)
{
	CString strTemp, strData;
	int nIndex = strSrc.Find(strSub);
	if (nIndex == -1)	
	{
		strTemp = strSrc;
		strSrc.Empty();
		return strTemp;
	}

	strTemp = strSrc.Left(nIndex);
	int nSubLen = strSub.GetLength();
	strData = strSrc.Mid(nIndex + nSubLen);
//	strSrc.Format("%s", strData);
	strSrc = strData;

	return strTemp;
}

CString CDataConverter::Get_RateData(char* szSrc, double dDenom, int nSize)
{
	CString strData(szSrc, nSize);
	
	double dX = atoi(strData);
	
	strData.Format("%10.2f", dX / dDenom);
	
	strData.Remove('.');
	
	return strData;
}

CString CDataConverter::Get_PercentData(char* szSrc, double dDenom, int nSize)
{
	CString strData(szSrc, nSize);
	CString strDa1(szSrc,nSize);
	double dX = atof(strData);
	
	strData.Format("%10.2f", CALC_CHGRATE(dX,dDenom)*100);
	//strData.Remove('.');
	return strData;
}

CString CDataConverter::Char2String(char* pszData, int nSize)
{
	CString strData = CString(pszData, nSize);
	strData.TrimRight();	strData.TrimLeft();

	return strData;
}

int CDataConverter::Char2Int(char* pszData, int nSize)
{
	CString strData;
	strData.Format(_T("%.*s"), nSize, pszData);

	return atoi(strData);
}

double CDataConverter::Char2Double(char* pszData, int nSize)
{
	CString strData;
	strData.Format(_T("%.*s"), nSize, pszData);

	return atof(strData);
}

long CDataConverter::Char2Long(char* pszData, int nSize)
{
	CString strData;
	strData.Format(_T("%.*s"), nSize, pszData);

	return atol(strData);
}

int CDataConverter::Parser2(char *pszBuff, char cSep, CString &strData)
{
	strData.Empty();
	int nDataIdx=0;
	char szData[1024];
	int nMaxSize = sizeof(szData);

	memset(szData, 0x00, sizeof(szData));

	for (int nIndex=0; nIndex<nMaxSize; nIndex++)
	{
		if (pszBuff[nIndex] == 0x00)
			break;

		if (pszBuff[nIndex] == cSep)
			break;

		szData[nDataIdx++] = pszBuff[nIndex];
	}

	strData.Format(_T("%s"), szData);
	return nDataIdx;
}

//*******************************************************************/
/*! Function Name : TbondPrcLToF
/*! Function      : 미국채 표시변환 TB Price long -> float decimal
/*! Param         : CString strDecimalGB, CString strData
/*! Return        : CString
/*! Comment       : 
//******************************************************************/
BOOL CDataConverter::TbondPrcLToF(char cType, int nPcnt, double dRate, CString &strData)
{
	if (cType <= '9')
		return FALSE;

	CString strInData = strData;
	if (strData.GetLength() <= 0)
	{
		strData = _T("");
		return TRUE;
	}

//	소수부 정수갯수, 소수부 소수갯수	예시) 9999'99.9
//	int nFcnt, nHcnt;
	CString strFvalue, strHvalue;

	int nFind = strInData.Find(_T("`"));
	if (nFind < 0)
		nFind = strInData.Find(_T("'"));

	strFvalue = strInData.Left(nFind);
	strHvalue = strInData.Mid(nFind+1);
/*
	if (cType == 'K')
	{
		//if (strHvalue.Right(1).CompareNoCase(_T("5")))
		if (strHvalue.Right(1) == '2' || strHvalue.Right(1) == '7')
			strHvalue += _T("5");
	}
*/
	double dRetData = (atof(strFvalue) + atof(strHvalue) * dRate);// * pow(10, nPcnt);
	strData.Format(_T("%f"), dRetData);

	return TRUE;
}

//<<20100419_JS.Kim 맵차트에서 사용하는 CWinixConverter::GetDecimalRate() 를 가져옴
/*
char CDataConverter::GetDecimalRate(CString strDecimalGB, int &nPcnt, double &dRate)
{
	if (strDecimalGB.GetLength() <= 0)
		return -1;

	nPcnt = 0;
	dRate = 1.0;

	char cRet = '0';
	int nType = atoi(strDecimalGB);

	if (nType >= 1 && nType <= 10)
	{
		nPcnt = nType-1;
		dRate = 1.0/pow(10, nPcnt);
		cRet += nPcnt;
	}
	else
	{
		cRet = strDecimalGB.GetAt(0);
		switch (cRet)
		{
		case 'A':	nPcnt = 1;	dRate = 1.0/2.0;	break;	//nFcnt = 1;	nHcnt = 0;	//	9999'9
		case 'B':	nPcnt = 2;	dRate = 1.0/4.0;	break;	//nFcnt = 1;	nHcnt = 0;	//	9999'9
		case 'C':	nPcnt = 3;	dRate = 1.0/8.0;	break;	//nFcnt = 1;	nHcnt = 0;	//	9999'9
		case 'D':	nPcnt = 4;	dRate = 1.0/16.0;	break;	//nFcnt = 2;	nHcnt = 0;	//	9999'99
		case 'E':	nPcnt = 5;	dRate = 1.0/32.0;	break;	//nFcnt = 2;	nHcnt = 0;	//	9999'99
		case 'F':	nPcnt = 6;														//	9999'99
		case 'I':	nPcnt = 6;	dRate = 1.0/64.0;	break;	//nFcnt = 2;	nHcnt = 1;	//	9999'99.9
		case 'J':	nPcnt = 7;
		case 'K':	nPcnt = 7;	dRate = 1.0/128.0;	break;	//nFcnt = 2;	nHcnt = 1;	//	9999'99.9
			break;
//		case 'G':			break;
		default:	return ' ';
		}
	}

	return cRet;
}
*/
char CDataConverter::GetDecimalRate(CString strDecimalGB, int &nPcnt, double &dRate, CString &strResult)
{
	if (strDecimalGB.GetLength() <= 0)
		return -1;

	int nFcnt=0, nHcnt=0;
	nPcnt = 0;
	dRate = 1.0;

	char cRet = '0';
	int nType = atoi(strDecimalGB);

	if (nType >= 1 && nType <= 10)
	{
		nPcnt = nType-1;
		dRate = 1.0/pow(10, nPcnt);
		cRet += nPcnt;
		strResult.Format("× %0.*f", nPcnt, 1 / pow(10, nPcnt));
	}
	else
	{
		cRet = strDecimalGB.GetAt(0);
		switch (cRet)
		{
		case 'A':	nPcnt = 1;	dRate = 1.0/2.0;	nFcnt = 1;	nHcnt = 0;	break;	//	9999'9
		case 'B':	nPcnt = 2;	dRate = 1.0/4.0;	nFcnt = 1;	nHcnt = 0;	break;	//	9999'9
		case 'C':	nPcnt = 3;	dRate = 1.0/8.0;	nFcnt = 1;	nHcnt = 0;	break;	//	9999'9
		case 'D':	nPcnt = 4;	dRate = 1.0/16.0;	nFcnt = 2;	nHcnt = 0;	break;	//	9999'99
		case 'E':	nPcnt = 5;	dRate = 1.0/32.0;	nFcnt = 2;	nHcnt = 0;	break;	//	9999'99
		case 'F':	nPcnt = 6;	dRate = 1.0/64.0;	nFcnt = 2;	nHcnt = 0;	break;	//	9999'99
//		case 'I':	nPcnt = 6;	dRate = 1.0/64.0;	nFcnt = 2;	nHcnt = 1;	break;	//	9999'99.9
//		case 'J':	nPcnt = 7;
//		case 'K':	nPcnt = 7;	dRate = 1.0/128.0;	nFcnt = 2;	nHcnt = 1;	break;	//	9999'99.9
//		case 'G':			break;
		case 'I':	nPcnt = 6;	dRate = 1.0/32.0;	nFcnt = 2;	nHcnt = 1;	break;	//	9999'99.9
		case 'J':	nPcnt = 7;	dRate = 1.0/64.0;	nFcnt = 2;	nHcnt = 1;	break;	//	9999'99.9
		case 'K':	nPcnt = 7;	dRate = 1.0/32.0;	nFcnt = 2;	nHcnt = 1;	break;	//	9999'99.9
		default:	return ' ';
		}

		strResult.Format("× 1/%d\'%d.%d", (int)(dRate/(dRate*dRate)), nFcnt+nHcnt, nHcnt);
	}

	return cRet;
}
//>>

double CDataConverter::DataSeparation(CString strData, int nMask)
{
	strData.TrimLeft();
	strData.TrimRight();

	if (strData.IsEmpty())
		return 99999999.;

	if (nMask <= 0)
//		nMask = 1;
		return atof(strData);

	double dMask = pow(10, nMask);

	return (atof(strData) * dMask);
}

CString CDataConverter::GetTimeIndexInfo(int nIndex, int nType)
{
//	IMainInfoManager* pMng = (IMainInfoManager*)AfxGetPctrInterface(UUID_IMainInfoManager);
//	if (pMng == NULL)
//		return _T("");

	CString strFileName, strTemp, strKey, strData, strTitle, strGmt, strSummerTime, strCityName;

	char szItem[MAX_PATH];
	int nCount;

	GetDir_DrfnRoot(strFileName);
	strFileName += "\\Icss\\timesetup.ini";
//	strFileName = pMng->GetRootDir();
//	strFileName += "\\hanaro\\timesetup.ini";

	CString strSection = "TIME";
	nCount = GetPrivateProfileInt(strSection, "COUNT", 0, strFileName);

	if (nCount < nIndex)
		return "";

	strKey.Format("%d", nIndex);
	memset(szItem, 0, MAX_PATH);
	GetPrivateProfileString(strSection, strKey, "", szItem, MAX_PATH, strFileName);

	strTemp = szItem;
	int nCnt = strTemp.Find(",");
	strTitle= strTemp.Left(nCnt);	
	strTemp = strTemp.Mid(nCnt+1);	
	nCnt	= strTemp.Find(",");
	strGmt	= strTemp.Left(nCnt);

	if (strTemp.GetLength() < nCnt+1)
	{
		if (nIndex == -1 && (nType != 1 || nType != 2))
			return "원래시간대";

		return "";
	}
	
	strSummerTime = strTemp.Mid(nCnt+1);

	// 도시명만 취득한다.
	nCnt = strTitle.Find('(');	
	strCityName = strTitle.Left(nCnt);	

	switch (nType)
	{
	case 1 :		//	시간차
		if (strSummerTime == "1")
			strData.Format("%d", atoi(strGmt)+1);
		else
			strData.Format("%d", atoi(strGmt));

		break;

	case 2 :		//	섬머타임 여부
		strData = strData;
		break;

	default :		//	타이틀
		if (nIndex == -1 && (nType != 1 || nType != 2))
			return "원래시간대";
		else
			strData = strTitle;

		break;
	}

	return strData;
}

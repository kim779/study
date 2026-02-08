// DataConverter.h: interface for the CDataConverter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATACONVERTER_H__E0D2E922_ED2B_4C25_A711_4CD7811C5133__INCLUDED_)
#define AFX_DATACONVERTER_H__E0D2E922_ED2B_4C25_A711_4CD7811C5133__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////////
#include "../../../SrcSite/include/axisfire.h"
#include "../../../SrcSite/include/axisGrid.h"
#include "../../../SrcSite/include/axisgenv.h"
#include "../../Chart/commontr/TRDefine.h"

#include "ChartItemFO.h"
#include "ChartItemForeign.h"
#include "ChartItemSFO.h"
#include "ChartItemStock.h"
#include "ChartItemSHoga.h"
#include "ChartItemUpjong.h"
#include "ChartItemMarket.h"
#include "ChartReplay.h"
#include "ChartItemFOJipyo.h"
#include "ChartItemPreFO.h"
#include "ChartItemDollar.h"
#include "ChartItemFOHoga.h"
#include "ChartItemFX.h"
#include "ChartItemFXChe.h"
#include "ChartItemFXJan.h"

#include "../../chart/commontr/TranStruct_GNF.h"
#include "ChartItemFOForeign.h"
#include "ChartItemFOForeignHoga.h"

#include <math.h>

//////////////////////////////////////////////////////////////////////////
//>> 종목코드 구분
#define ERROR_CODE				-1
#define	STOCK_CODE				1
#define	STOCK_OPT_CODE			2
#define	FUTURE_CODE				3
#define	OPTION_PUT_CODE			4
#define	OPTION_CALL_CODE		5
#define	ETF_CODE				6
#define	STAR_FUTURE_CODE		7

// Tr Header

class CDataConverter  
{
public:
	CDataConverter();
	virtual ~CDataConverter();

	int GetCodeType(CString strCode);
	CString ConvertToFid(CString strTR, void* pSendData, int nLen);
	char* Convert2Struct(LPBYTE pData, int nLen, int nKey, int &nDataLen);
	char* Convert2Struct(LPBYTE pData, int nLen, CString strTR, int &nDataLen);
	CString GetTrNo(int nKey);

	static CString	Parser(CString &strSrc, CString strSub);
	static CString	Get_PercentData(char* szSrc, double nDenom, int nSize);
	static int		Parser2(char *pszBuff, char cSep, CString &strData);
	static BOOL		TbondPrcLToF(char cType, int nPcnt, double dRate, CString &strData);
	static char		GetDecimalRate(CString strDecimalGB, int &nPcnt, double &dRate, CString &strResult);
	static double	DataSeparation(CString strData, int nMask);
	static CString	GetTimeIndexInfo(int nIndex, int nType);

	int		GetMarketTRKey()	{ return m_Market.GetSendDataType();	};

	//@Solomon:091123SM068	-->
	CChartBozoInput GetBozoData(int nKey);
	char*			GetConv2StructGuideLine(int nKey, int &nDataLen);
	//@Solomon:091123SM068	<--

	static CString GetHogaTick(CString strCode, CString strPrice);

private:
	CChartItemFO		m_FutOpt;
	CChartItemForeign	m_Foreign;
	CChartItemSFO		m_StockFO;
	CChartItemStock		m_Stock;
	CChartItemSHoga		m_SHoga;
	CChartItemUpjong	m_Upjong;
	CChartItemMarket	m_Market;
	CChartReplay		m_Replay;
	CChartItemFOJipyo	m_FoJipyo;
	CChartItemPreFO		m_PreFutOpt;
	CChartItemDollar	m_Dollar;
	CChartItemFOHoga	m_FOHoga;

	CChartItemFOForeign	m_FOForeign;
	CChartItemFOForeignHoga	m_FOForeignHoga;
	CChartItemFXChe		m_FOForeignChe2;
	CChartItemFXJan		m_FOForeignJan;

	CChartItemFX		m_FX;
	CChartItemFXChe		m_FXChe;
	CChartItemFXChe		m_FXChe2;
	CChartItemFXJan		m_FXJan;
	
//KHD
//	CPlusConditionStock m_Plus_Stock;
//END
public:
	static	CString		Char2String(char* pszData, int nSize);
	static	int			Char2Int(char* pszData, int nSize);
	static	double		Char2Double(char* pszData, int nSize);
	static	long		Char2Long(char* pszData, int nSize);
	static	CString		Get_RateData(char* szSrc, double dDenom, int nSize);

};

#endif // !defined(AFX_DATACONVERTER_H__E0D2E922_ED2B_4C25_A711_4CD7811C5133__INCLUDED_)

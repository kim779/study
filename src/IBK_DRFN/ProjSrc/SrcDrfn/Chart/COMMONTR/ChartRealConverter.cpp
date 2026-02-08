#include "stdafx.h"
#include "./ChartRealConverter.h"
#include "../CommonTR/KXS3.h"	//주식
#include "../CommonTR/KXYS.h"	//주식
#include "../CommonTR/EWs3.h"	//주식
#include "../CommonTR/IXIJ.h"	//업종
#include "../CommonTR/FFC0.h"	//선물
#include "../CommonTR/OOC0.h"	//옵션
#include "../CommonTR/JFC0.h"	//주식선물
#include "../CommonTR/JOC0.h"	//주식옵션
#include "../CommonTR/HWFJ.h"	//해외
#include "../CommonTR/GTFJ.h"	//상품선물
#include "../CommonTR/FOJS.h"	//FO 지수

// --------------------------------------------------------------------------------------
// 주식
BOOL CChartRealConverter::Convert_KXS3(LPCSTR pRealStruct, CRealData_Common& realData_Common)
{
	LPKXS3_OutBlock pInput = (LPKXS3_OutBlock)pRealStruct;
	char sTmp[20+1];

	memcpy(sTmp, pInput->kshcode, sizeof(pInput->kshcode));	/*  [종목코드]				*/
	sTmp[sizeof(pInput->kshcode)]=0;
	realData_Common.m_strCode = sTmp;
	realData_Common.m_strCode.TrimLeft(); realData_Common.m_strCode.TrimRight();
	
	memcpy(sTmp, pInput->chetime, sizeof(pInput->chetime));	/*  [체결시간]				*/
	sTmp[sizeof(pInput->chetime)]=0;
	realData_Common.m_strTime = sTmp;
	realData_Common.m_strTime.TrimLeft(); realData_Common.m_strTime.TrimRight();
	
	memcpy(sTmp, pInput->sign, sizeof(pInput->sign));		/*  [1:상한]				*/
	sTmp[sizeof(pInput->sign)]=0;
	realData_Common.m_strSign = sTmp;
	realData_Common.m_strSign.TrimLeft(); realData_Common.m_strSign.TrimRight();

	memcpy(sTmp, pInput->price, sizeof(pInput->price));		/*  [현재가,     원]		*/
	sTmp[sizeof(pInput->price)]=0;
	realData_Common.m_strPrice = sTmp;
	realData_Common.m_strPrice.TrimLeft(); realData_Common.m_strPrice.TrimRight();

	memcpy(sTmp, pInput->change, sizeof(pInput->change));	/*  [전일대비,   원]		*/
	sTmp[sizeof(pInput->change)]=0;
	realData_Common.m_strChange = sTmp;
	realData_Common.m_strChange.TrimLeft(); realData_Common.m_strChange.TrimRight();

	memcpy(sTmp, pInput->drate, sizeof(pInput->drate));		/*  [등락율]				*/
	sTmp[sizeof(pInput->drate)]=0;
	realData_Common.m_strChrate = sTmp;
	realData_Common.m_strChrate.TrimLeft(); realData_Common.m_strChrate.TrimRight();

	memcpy(sTmp, pInput->volume, sizeof(pInput->volume));	/*  [누적거래량, 주]		*/
	sTmp[sizeof(pInput->volume)]=0;
	realData_Common.m_strVolume = sTmp;
	realData_Common.m_strVolume.TrimLeft(); realData_Common.m_strVolume.TrimRight();

	memcpy(sTmp, pInput->open, sizeof(pInput->open));		/*  [시가,       원]		*/
	sTmp[sizeof(pInput->open)]=0;
	realData_Common.m_strOpen = sTmp;
	realData_Common.m_strOpen.TrimLeft(); realData_Common.m_strOpen.TrimRight();

	memcpy(sTmp, pInput->high, sizeof(pInput->high));		/*  [고가,       원]		*/
	sTmp[sizeof(pInput->high)]=0;
	realData_Common.m_strHigh = sTmp;
	realData_Common.m_strHigh.TrimLeft(); realData_Common.m_strHigh.TrimRight();

	memcpy(sTmp, pInput->low, sizeof(pInput->low));			/*  [저가,       원]		*/
	sTmp[sizeof(pInput->low)]=0;
	realData_Common.m_strLow = sTmp;
	realData_Common.m_strLow.TrimLeft(); realData_Common.m_strLow.TrimRight();
	
	memcpy(sTmp, pInput->value, sizeof(pInput->value));		/*  [거래대금,       원]	*/
	sTmp[sizeof(pInput->value)]=0;
	realData_Common.m_strValue = sTmp;
	realData_Common.m_strValue.TrimLeft(); realData_Common.m_strValue.TrimRight();

	realData_Common.m_nMarketType = CMasterTypeInfo::STOCK_CHART;
	return TRUE;
}

BOOL CChartRealConverter::Convert_KXYS(LPCSTR pRealStruct, CRealData_Common& realData_Common)
{
	LPKXYS_OutBlock pInput = (LPKXYS_OutBlock)pRealStruct;
	char sTmp[20+1];

	memcpy(sTmp, pInput->kshcode, sizeof(pInput->kshcode));	/*  [종목코드]				*/
	sTmp[sizeof(pInput->kshcode)]=0;
	realData_Common.m_strCode = sTmp;
	realData_Common.m_strCode.TrimLeft(); realData_Common.m_strCode.TrimRight();
	
	memcpy(sTmp, pInput->hotime, sizeof(pInput->hotime));	/*  [체결시간]				*/
	sTmp[sizeof(pInput->hotime)]=0;
	realData_Common.m_strTime = sTmp;
	realData_Common.m_strTime.TrimLeft(); realData_Common.m_strTime.TrimRight();
	
	memcpy(sTmp, pInput->jnilysign, sizeof(pInput->jnilysign));		/*  [1:상한]				*/
	sTmp[sizeof(pInput->jnilysign)]=0;
	realData_Common.m_strSign = sTmp;
	realData_Common.m_strSign.TrimLeft(); realData_Common.m_strSign.TrimRight();

	memcpy(sTmp, pInput->yeprice, sizeof(pInput->yeprice));		/*  [현재가,     원]		*/
	sTmp[sizeof(pInput->yeprice)]=0;
	realData_Common.m_strPrice = sTmp;
	realData_Common.m_strPrice.TrimLeft(); realData_Common.m_strPrice.TrimRight();

	memcpy(sTmp, pInput->jnilychange, sizeof(pInput->jnilychange));	/*  [전일대비,   원]		*/
	sTmp[sizeof(pInput->jnilychange)]=0;
	realData_Common.m_strChange = sTmp;
	realData_Common.m_strChange.TrimLeft(); realData_Common.m_strChange.TrimRight();

	memcpy(sTmp, pInput->jnilydrate, sizeof(pInput->jnilydrate));		/*  [등락율]				*/
	sTmp[sizeof(pInput->jnilydrate)]=0;
	realData_Common.m_strChrate = sTmp;
	realData_Common.m_strChrate.TrimLeft(); realData_Common.m_strChrate.TrimRight();

	memcpy(sTmp, pInput->yevolume, sizeof(pInput->yevolume));	/*  [누적거래량, 주]		*/
	sTmp[sizeof(pInput->yevolume)]=0;
	realData_Common.m_strVolume = sTmp;
	realData_Common.m_strVolume.TrimLeft(); realData_Common.m_strVolume.TrimRight();

	realData_Common.m_strOpen = _T("0");
	realData_Common.m_strHigh = _T("0");
	realData_Common.m_strLow = _T("0");
	realData_Common.m_strValue = _T("0");

	realData_Common.m_nMarketType = CMasterTypeInfo::STOCK_CHART;
	return TRUE;
}

// ELW
BOOL CChartRealConverter::Convert_EWs3(LPCSTR pRealStruct, CRealData_Common& realData_Common)
{
	LPEWs3_OutBlock pInput = (LPEWs3_OutBlock)pRealStruct;
	char sTmp[20+1];

	memcpy(sTmp, pInput->kshcode, sizeof(pInput->kshcode));	/*  [종목코드]				*/
	sTmp[sizeof(pInput->kshcode)]=0;
	realData_Common.m_strCode = sTmp;
	realData_Common.m_strCode.TrimLeft(); realData_Common.m_strCode.TrimRight();
	
	memcpy(sTmp, pInput->chetime, sizeof(pInput->chetime));	/*  [체결시간]				*/
	sTmp[sizeof(pInput->chetime)]=0;
	realData_Common.m_strTime = sTmp;
	realData_Common.m_strTime.TrimLeft(); realData_Common.m_strTime.TrimRight();
	
	memcpy(sTmp, pInput->sign, sizeof(pInput->sign));		/*  [1:상한]				*/
	sTmp[sizeof(pInput->sign)]=0;
	realData_Common.m_strSign = sTmp;
	realData_Common.m_strSign.TrimLeft(); realData_Common.m_strSign.TrimRight();

	memcpy(sTmp, pInput->price, sizeof(pInput->price));		/*  [현재가,     원]		*/
	sTmp[sizeof(pInput->price)]=0;
	realData_Common.m_strPrice = sTmp;
	realData_Common.m_strPrice.TrimLeft(); realData_Common.m_strPrice.TrimRight();

	memcpy(sTmp, pInput->change, sizeof(pInput->change));	/*  [전일대비,   원]		*/
	sTmp[sizeof(pInput->change)]=0;
	realData_Common.m_strChange = sTmp;
	realData_Common.m_strChange.TrimLeft(); realData_Common.m_strChange.TrimRight();

	memcpy(sTmp, pInput->drate, sizeof(pInput->drate));		/*  [등락율]				*/
	sTmp[sizeof(pInput->drate)]=0;
	realData_Common.m_strChrate = sTmp;
	realData_Common.m_strChrate.TrimLeft(); realData_Common.m_strChrate.TrimRight();

	memcpy(sTmp, pInput->volume, sizeof(pInput->volume));	/*  [누적거래량, 주]		*/
	sTmp[sizeof(pInput->volume)]=0;
	realData_Common.m_strVolume = sTmp;
	realData_Common.m_strVolume.TrimLeft(); realData_Common.m_strVolume.TrimRight();

	memcpy(sTmp, pInput->open, sizeof(pInput->open));		/*  [시가,       원]		*/
	sTmp[sizeof(pInput->open)]=0;
	realData_Common.m_strOpen = sTmp;
	realData_Common.m_strOpen.TrimLeft(); realData_Common.m_strOpen.TrimRight();

	memcpy(sTmp, pInput->high, sizeof(pInput->high));		/*  [고가,       원]		*/
	sTmp[sizeof(pInput->high)]=0;
	realData_Common.m_strHigh = sTmp;
	realData_Common.m_strHigh.TrimLeft(); realData_Common.m_strHigh.TrimRight();

	memcpy(sTmp, pInput->low, sizeof(pInput->low));			/*  [저가,       원]		*/
	sTmp[sizeof(pInput->low)]=0;
	realData_Common.m_strLow = sTmp;
	realData_Common.m_strLow.TrimLeft(); realData_Common.m_strLow.TrimRight();
	
	memcpy(sTmp, pInput->value, sizeof(pInput->value));		/*  [거래대금,       원]	*/
	sTmp[sizeof(pInput->value)]=0;
	realData_Common.m_strValue = sTmp;
	realData_Common.m_strValue.TrimLeft(); realData_Common.m_strValue.TrimRight();

	realData_Common.m_nMarketType = CMasterTypeInfo::STOCK_CHART;
	return TRUE;
}

// --------------------------------------------------------------------------------------
// 업종
BOOL CChartRealConverter::Convert_IXIJ(LPCSTR pRealStruct, CRealData_Common& realData_Common)
{
	LPIXIJ_OutBlock pInput = (LPIXIJ_OutBlock)pRealStruct;
	char sTmp[20+1];

	memcpy(sTmp, pInput->kupcode, sizeof(pInput->kupcode));		/*  [업종코드]			*/
	sTmp[sizeof(pInput->kupcode)]=0;
	realData_Common.m_strCode = sTmp;
	realData_Common.m_strCode.TrimLeft(); realData_Common.m_strCode.TrimRight();
	
	memcpy(sTmp, pInput->time, sizeof(pInput->time));			/*  [시간]				*/
	sTmp[sizeof(pInput->time)]=0;
	realData_Common.m_strTime = sTmp;
	realData_Common.m_strTime.TrimLeft(); realData_Common.m_strTime.TrimRight();
	
	memcpy(sTmp, pInput->sign, sizeof(pInput->sign));			/*  [2:상승 3:보합 5:하락]	*/
	sTmp[sizeof(pInput->sign)]=0;
	realData_Common.m_strSign = sTmp;
	realData_Common.m_strSign.TrimLeft(); realData_Common.m_strSign.TrimRight();

	memcpy(sTmp, pInput->jisu, sizeof(pInput->jisu));			/*  [지수]				*/
	sTmp[sizeof(pInput->jisu)]=0;
	realData_Common.m_strPrice = sTmp;
	realData_Common.m_strPrice.TrimLeft(); realData_Common.m_strPrice.TrimRight();

	memcpy(sTmp, pInput->change, sizeof(pInput->change));		/*  [전일비]			*/
	sTmp[sizeof(pInput->change)]=0;
	realData_Common.m_strChange = sTmp;
	realData_Common.m_strChange.TrimLeft(); realData_Common.m_strChange.TrimRight();

	memcpy(sTmp, pInput->drate, sizeof(pInput->drate));			/*  [등락율]			*/
	sTmp[sizeof(pInput->drate)]=0;
	realData_Common.m_strChrate = sTmp;
	realData_Common.m_strChrate.TrimLeft(); realData_Common.m_strChrate.TrimRight();

	memcpy(sTmp, pInput->volume, sizeof(pInput->volume));		/*  [거래량]			*/
	sTmp[sizeof(pInput->volume)]=0;
	realData_Common.m_strVolume = sTmp;
	realData_Common.m_strVolume.TrimLeft(); realData_Common.m_strVolume.TrimRight();

	memcpy(sTmp, pInput->openjisu, sizeof(pInput->openjisu));	/*  [시가지수]			*/
	sTmp[sizeof(pInput->openjisu)]=0;
	realData_Common.m_strOpen = sTmp;
	realData_Common.m_strOpen.TrimLeft(); realData_Common.m_strOpen.TrimRight();

	memcpy(sTmp, pInput->highjisu, sizeof(pInput->highjisu));	/*  [고가지수]			*/
	sTmp[sizeof(pInput->highjisu)]=0;
	realData_Common.m_strHigh = sTmp;
	realData_Common.m_strHigh.TrimLeft(); realData_Common.m_strHigh.TrimRight();

	memcpy(sTmp, pInput->lowjisu, sizeof(pInput->lowjisu));		/*  [저가지수]			*/
	sTmp[sizeof(pInput->lowjisu)]=0;
	realData_Common.m_strLow = sTmp;
	realData_Common.m_strLow.TrimLeft(); realData_Common.m_strLow.TrimRight();
	
	memcpy(sTmp, pInput->value, sizeof(pInput->value));		/*  [거래대금,       원]	*/
	sTmp[sizeof(pInput->value)]=0;
	realData_Common.m_strValue = sTmp;
	realData_Common.m_strValue.TrimLeft(); realData_Common.m_strValue.TrimRight();

	realData_Common.m_nMarketType = CMasterTypeInfo::UPJONG_CHART;
	return TRUE;
}

// --------------------------------------------------------------------------------------
// 선물
BOOL CChartRealConverter::Convert_FFC0(LPCSTR pRealStruct, CRealData_Common& realData_Common)
{
	LPFFC0_OutBlock pInput = (LPFFC0_OutBlock)pRealStruct;
	char sTmp[20+1];

	memcpy(sTmp, pInput->kfutcode, sizeof(pInput->kfutcode));	/*  [선물종목코드]				*/
	sTmp[sizeof(pInput->kfutcode)]=0;
	realData_Common.m_strCode = sTmp;
	realData_Common.m_strCode.TrimLeft(); realData_Common.m_strCode.TrimRight();
	
	memcpy(sTmp, pInput->chetime, sizeof(pInput->chetime));	/*  [체결시간]				*/
	sTmp[sizeof(pInput->chetime)]=0;
	realData_Common.m_strTime = sTmp;
	realData_Common.m_strTime.TrimLeft(); realData_Common.m_strTime.TrimRight();
	
	memcpy(sTmp, pInput->sign, sizeof(pInput->sign));		/*  [1:상한]				*/
	sTmp[sizeof(pInput->sign)]=0;
	realData_Common.m_strSign = sTmp;
	realData_Common.m_strSign.TrimLeft(); realData_Common.m_strSign.TrimRight();

	memcpy(sTmp, pInput->price, sizeof(pInput->price));		/*  [현재가,     원]		*/
	sTmp[sizeof(pInput->price)]=0;
	realData_Common.m_strPrice = sTmp;
	realData_Common.m_strPrice.TrimLeft(); realData_Common.m_strPrice.TrimRight();

	memcpy(sTmp, pInput->change, sizeof(pInput->change));	/*  [전일대비,   원]		*/
	sTmp[sizeof(pInput->change)]=0;
	realData_Common.m_strChange = sTmp;
	realData_Common.m_strChange.TrimLeft(); realData_Common.m_strChange.TrimRight();

	memcpy(sTmp, pInput->drate, sizeof(pInput->drate));		/*  [등락율]				*/
	sTmp[sizeof(pInput->drate)]=0;
	realData_Common.m_strChrate = sTmp;
	realData_Common.m_strChrate.TrimLeft(); realData_Common.m_strChrate.TrimRight();

	memcpy(sTmp, pInput->volume, sizeof(pInput->volume));	/*  [누적거래량, 주]		*/
	sTmp[sizeof(pInput->volume)]=0;
	realData_Common.m_strVolume = sTmp;
	realData_Common.m_strVolume.TrimLeft(); realData_Common.m_strVolume.TrimRight();

	memcpy(sTmp, pInput->open, sizeof(pInput->open));		/*  [시가,       원]		*/
	sTmp[sizeof(pInput->open)]=0;
	realData_Common.m_strOpen = sTmp;
	realData_Common.m_strOpen.TrimLeft(); realData_Common.m_strOpen.TrimRight();

	memcpy(sTmp, pInput->high, sizeof(pInput->high));		/*  [고가,       원]		*/
	sTmp[sizeof(pInput->high)]=0;
	realData_Common.m_strHigh = sTmp;
	realData_Common.m_strHigh.TrimLeft(); realData_Common.m_strHigh.TrimRight();

	memcpy(sTmp, pInput->low, sizeof(pInput->low));			/*  [저가,       원]		*/
	sTmp[sizeof(pInput->low)]=0;
	realData_Common.m_strLow = sTmp;
	realData_Common.m_strLow.TrimLeft(); realData_Common.m_strLow.TrimRight();
	
	memcpy(sTmp, pInput->value, sizeof(pInput->value));		/*  [거래대금,       원]	*/
	sTmp[sizeof(pInput->value)]=0;
	realData_Common.m_strValue = sTmp;
	realData_Common.m_strValue.TrimLeft(); realData_Common.m_strValue.TrimRight();

	realData_Common.m_nMarketType = CMasterTypeInfo::FUTOPT_CHART;
	return TRUE;
}

// --------------------------------------------------------------------------------------
// 옵션
BOOL CChartRealConverter::Convert_OOC0(LPCSTR pRealStruct, CRealData_Common& realData_Common)
{
	LPOOC0_OutBlock pInput = (LPOOC0_OutBlock)pRealStruct;
	char sTmp[20+1];

	memcpy(sTmp, pInput->koptcode, sizeof(pInput->koptcode));	/*  [종목코드]				*/
	sTmp[sizeof(pInput->koptcode)]=0;
	realData_Common.m_strCode = sTmp;
	realData_Common.m_strCode.TrimLeft(); realData_Common.m_strCode.TrimRight();
	
	memcpy(sTmp, pInput->chetime, sizeof(pInput->chetime));	/*  [체결시간]				*/
	sTmp[sizeof(pInput->chetime)]=0;
	realData_Common.m_strTime = sTmp;
	realData_Common.m_strTime.TrimLeft(); realData_Common.m_strTime.TrimRight();
	
	memcpy(sTmp, pInput->sign, sizeof(pInput->sign));		/*  [1:상한]				*/
	sTmp[sizeof(pInput->sign)]=0;
	realData_Common.m_strSign = sTmp;
	realData_Common.m_strSign.TrimLeft(); realData_Common.m_strSign.TrimRight();

	memcpy(sTmp, pInput->price, sizeof(pInput->price));		/*  [현재가,     원]		*/
	sTmp[sizeof(pInput->price)]=0;
	realData_Common.m_strPrice = sTmp;
	realData_Common.m_strPrice.TrimLeft(); realData_Common.m_strPrice.TrimRight();

	memcpy(sTmp, pInput->change, sizeof(pInput->change));	/*  [전일대비,   원]		*/
	sTmp[sizeof(pInput->change)]=0;
	realData_Common.m_strChange = sTmp;
	realData_Common.m_strChange.TrimLeft(); realData_Common.m_strChange.TrimRight();

	memcpy(sTmp, pInput->drate, sizeof(pInput->drate));		/*  [등락율]				*/
	sTmp[sizeof(pInput->drate)]=0;
	realData_Common.m_strChrate = sTmp;
	realData_Common.m_strChrate.TrimLeft(); realData_Common.m_strChrate.TrimRight();

	memcpy(sTmp, pInput->volume, sizeof(pInput->volume));	/*  [누적거래량, 주]		*/
	sTmp[sizeof(pInput->volume)]=0;
	realData_Common.m_strVolume = sTmp;
	realData_Common.m_strVolume.TrimLeft(); realData_Common.m_strVolume.TrimRight();

	memcpy(sTmp, pInput->open, sizeof(pInput->open));		/*  [시가,       원]		*/
	sTmp[sizeof(pInput->open)]=0;
	realData_Common.m_strOpen = sTmp;
	realData_Common.m_strOpen.TrimLeft(); realData_Common.m_strOpen.TrimRight();

	memcpy(sTmp, pInput->high, sizeof(pInput->high));		/*  [고가,       원]		*/
	sTmp[sizeof(pInput->high)]=0;
	realData_Common.m_strHigh = sTmp;
	realData_Common.m_strHigh.TrimLeft(); realData_Common.m_strHigh.TrimRight();

	memcpy(sTmp, pInput->low, sizeof(pInput->low));			/*  [저가,       원]		*/
	sTmp[sizeof(pInput->low)]=0;
	realData_Common.m_strLow = sTmp;
	realData_Common.m_strLow.TrimLeft(); realData_Common.m_strLow.TrimRight();
	
	memcpy(sTmp, pInput->value, sizeof(pInput->value));		/*  [거래대금,       원]	*/
	sTmp[sizeof(pInput->value)]=0;
	realData_Common.m_strValue = sTmp;
	realData_Common.m_strValue.TrimLeft(); realData_Common.m_strValue.TrimRight();

	realData_Common.m_nMarketType = CMasterTypeInfo::FUTOPT_CHART;
	return TRUE;
}

// --------------------------------------------------------------------------------------
// 주식선물
BOOL CChartRealConverter::Convert_JFC0(LPCSTR pRealStruct, CRealData_Common& realData_Common)
{
	LPJFC0_OutBlock pInput = (LPJFC0_OutBlock)pRealStruct;
	char sTmp[20+1];

	memcpy(sTmp, pInput->kfutcode, sizeof(pInput->kfutcode));	/*  [선물종목코드]				*/
	sTmp[sizeof(pInput->kfutcode)]=0;
	realData_Common.m_strCode = sTmp;
	realData_Common.m_strCode.TrimLeft(); realData_Common.m_strCode.TrimRight();
	
	memcpy(sTmp, pInput->chetime, sizeof(pInput->chetime));	/*  [체결시간]				*/
	sTmp[sizeof(pInput->chetime)]=0;
	realData_Common.m_strTime = sTmp;
	realData_Common.m_strTime.TrimLeft(); realData_Common.m_strTime.TrimRight();
	
	memcpy(sTmp, pInput->sign, sizeof(pInput->sign));		/*  [1:상한]				*/
	sTmp[sizeof(pInput->sign)]=0;
	realData_Common.m_strSign = sTmp;
	realData_Common.m_strSign.TrimLeft(); realData_Common.m_strSign.TrimRight();

	memcpy(sTmp, pInput->price, sizeof(pInput->price));		/*  [현재가,     원]		*/
	sTmp[sizeof(pInput->price)]=0;
	realData_Common.m_strPrice = sTmp;
	realData_Common.m_strPrice.TrimLeft(); realData_Common.m_strPrice.TrimRight();

	memcpy(sTmp, pInput->change, sizeof(pInput->change));	/*  [전일대비,   원]		*/
	sTmp[sizeof(pInput->change)]=0;
	realData_Common.m_strChange = sTmp;
	realData_Common.m_strChange.TrimLeft(); realData_Common.m_strChange.TrimRight();

	memcpy(sTmp, pInput->drate, sizeof(pInput->drate));		/*  [등락율]				*/
	sTmp[sizeof(pInput->drate)]=0;
	realData_Common.m_strChrate = sTmp;
	realData_Common.m_strChrate.TrimLeft(); realData_Common.m_strChrate.TrimRight();

	memcpy(sTmp, pInput->volume, sizeof(pInput->volume));	/*  [누적거래량, 주]		*/
	sTmp[sizeof(pInput->volume)]=0;
	realData_Common.m_strVolume = sTmp;
	realData_Common.m_strVolume.TrimLeft(); realData_Common.m_strVolume.TrimRight();

	memcpy(sTmp, pInput->open, sizeof(pInput->open));		/*  [시가,       원]		*/
	sTmp[sizeof(pInput->open)]=0;
	realData_Common.m_strOpen = sTmp;
	realData_Common.m_strOpen.TrimLeft(); realData_Common.m_strOpen.TrimRight();

	memcpy(sTmp, pInput->high, sizeof(pInput->high));		/*  [고가,       원]		*/
	sTmp[sizeof(pInput->high)]=0;
	realData_Common.m_strHigh = sTmp;
	realData_Common.m_strHigh.TrimLeft(); realData_Common.m_strHigh.TrimRight();

	memcpy(sTmp, pInput->low, sizeof(pInput->low));			/*  [저가,       원]		*/
	sTmp[sizeof(pInput->low)]=0;
	realData_Common.m_strLow = sTmp;
	realData_Common.m_strLow.TrimLeft(); realData_Common.m_strLow.TrimRight();
	
	memcpy(sTmp, pInput->value, sizeof(pInput->value));		/*  [거래대금,       원]	*/
	sTmp[sizeof(pInput->value)]=0;
	realData_Common.m_strValue = sTmp;
	realData_Common.m_strValue.TrimLeft(); realData_Common.m_strValue.TrimRight();

	realData_Common.m_nMarketType = CMasterTypeInfo::STOCKFUTOPT_CHART;
	return TRUE;
}

// --------------------------------------------------------------------------------------
// 주식옵션
BOOL CChartRealConverter::Convert_JOC0(LPCSTR pRealStruct, CRealData_Common& realData_Common)
{
	LPJOC0_OutBlock pInput = (LPJOC0_OutBlock)pRealStruct;
	char sTmp[20+1];

	memcpy(sTmp, pInput->koptcode, sizeof(pInput->koptcode));	/*  [종목코드]				*/
	sTmp[sizeof(pInput->koptcode)]=0;
	realData_Common.m_strCode = sTmp;
	realData_Common.m_strCode.TrimLeft(); realData_Common.m_strCode.TrimRight();
	
	memcpy(sTmp, pInput->chetime, sizeof(pInput->chetime));	/*  [체결시간]				*/
	sTmp[sizeof(pInput->chetime)]=0;
	realData_Common.m_strTime = sTmp;
	realData_Common.m_strTime.TrimLeft(); realData_Common.m_strTime.TrimRight();
	
	memcpy(sTmp, pInput->sign, sizeof(pInput->sign));		/*  [1:상한]				*/
	sTmp[sizeof(pInput->sign)]=0;
	realData_Common.m_strSign = sTmp;
	realData_Common.m_strSign.TrimLeft(); realData_Common.m_strSign.TrimRight();

	memcpy(sTmp, pInput->price, sizeof(pInput->price));		/*  [현재가,     원]		*/
	sTmp[sizeof(pInput->price)]=0;
	realData_Common.m_strPrice = sTmp;
	realData_Common.m_strPrice.TrimLeft(); realData_Common.m_strPrice.TrimRight();

	memcpy(sTmp, pInput->change, sizeof(pInput->change));	/*  [전일대비,   원]		*/
	sTmp[sizeof(pInput->change)]=0;
	realData_Common.m_strChange = sTmp;
	realData_Common.m_strChange.TrimLeft(); realData_Common.m_strChange.TrimRight();

	memcpy(sTmp, pInput->drate, sizeof(pInput->drate));		/*  [등락율]				*/
	sTmp[sizeof(pInput->drate)]=0;
	realData_Common.m_strChrate = sTmp;
	realData_Common.m_strChrate.TrimLeft(); realData_Common.m_strChrate.TrimRight();

	memcpy(sTmp, pInput->volume, sizeof(pInput->volume));	/*  [누적거래량, 주]		*/
	sTmp[sizeof(pInput->volume)]=0;
	realData_Common.m_strVolume = sTmp;
	realData_Common.m_strVolume.TrimLeft(); realData_Common.m_strVolume.TrimRight();

	memcpy(sTmp, pInput->open, sizeof(pInput->open));		/*  [시가,       원]		*/
	sTmp[sizeof(pInput->open)]=0;
	realData_Common.m_strOpen = sTmp;
	realData_Common.m_strOpen.TrimLeft(); realData_Common.m_strOpen.TrimRight();

	memcpy(sTmp, pInput->high, sizeof(pInput->high));		/*  [고가,       원]		*/
	sTmp[sizeof(pInput->high)]=0;
	realData_Common.m_strHigh = sTmp;
	realData_Common.m_strHigh.TrimLeft(); realData_Common.m_strHigh.TrimRight();

	memcpy(sTmp, pInput->low, sizeof(pInput->low));			/*  [저가,       원]		*/
	sTmp[sizeof(pInput->low)]=0;
	realData_Common.m_strLow = sTmp;
	realData_Common.m_strLow.TrimLeft(); realData_Common.m_strLow.TrimRight();
	
	memcpy(sTmp, pInput->value, sizeof(pInput->value));		/*  [거래대금,       원]	*/
	sTmp[sizeof(pInput->value)]=0;
	realData_Common.m_strValue.TrimLeft(); realData_Common.m_strValue.TrimRight();

	realData_Common.m_nMarketType = CMasterTypeInfo::STOCKFUTOPT_CHART;
	return TRUE;
}

// 해외
BOOL CChartRealConverter::Convert_HWFJ(LPCSTR pRealStruct, CRealData_Common& realData_Common)
{
	LPHWFJ_OutBlock pInput = (LPHWFJ_OutBlock)pRealStruct;
	CString strTmp;
	char sTmp[20+1];

	memcpy(sTmp, pInput->symbol, sizeof(pInput->symbol));	/*  [심볼코드]				*/
	sTmp[sizeof(pInput->symbol)]=0;
	realData_Common.m_strCode = sTmp;
	realData_Common.m_strCode.TrimLeft(); realData_Common.m_strCode.TrimRight();
	
	memcpy(sTmp, pInput->time, sizeof(pInput->time));	/*  [시간(현지시간)]				*/
	sTmp[sizeof(pInput->time)]=0;
	realData_Common.m_strTime = sTmp;
	realData_Common.m_strTime.TrimLeft(); realData_Common.m_strTime.TrimRight();
	
	memcpy(sTmp, pInput->sign, sizeof(pInput->sign));		/*  [전일대비구분]				*/
	sTmp[sizeof(pInput->sign)]=0;
	realData_Common.m_strSign = sTmp;
	realData_Common.m_strSign.TrimLeft(); realData_Common.m_strSign.TrimRight();

	memcpy(sTmp, pInput->price, sizeof(pInput->price));		/*  [현재가]		*/
	sTmp[sizeof(pInput->price)]=0;
	strTmp.Format("%lf", atof(sTmp)*100.);
	realData_Common.m_strPrice.Format("%ld", atol(strTmp));
	realData_Common.m_strPrice.TrimLeft(); realData_Common.m_strPrice.TrimRight();

	memcpy(sTmp, pInput->change, sizeof(pInput->change));	/*  [전일대비]		*/
	sTmp[sizeof(pInput->change)]=0;
	strTmp.Format("%lf", atof(sTmp)*100.);
	realData_Common.m_strChange.Format("%ld", atol(strTmp));
	realData_Common.m_strChange.TrimLeft(); realData_Common.m_strChange.TrimRight();

	memcpy(sTmp, pInput->drate, sizeof(pInput->drate));		/*  [등락율]				*/
	sTmp[sizeof(pInput->drate)]=0;
	strTmp.Format("%lf", atof(sTmp)*100.);
	realData_Common.m_strChrate.Format("%ld", atol(strTmp));
	realData_Common.m_strChrate.TrimLeft(); realData_Common.m_strChrate.TrimRight();

	memcpy(sTmp, pInput->volume, sizeof(pInput->volume));	/*  [거래량]		*/
	sTmp[sizeof(pInput->volume)]=0;
	realData_Common.m_strVolume = sTmp;
	realData_Common.m_strVolume.TrimLeft(); realData_Common.m_strVolume.TrimRight();

	memcpy(sTmp, pInput->open, sizeof(pInput->open));		/*  [시가]		*/
	sTmp[sizeof(pInput->open)]=0;
	strTmp.Format("%lf", atof(sTmp)*100.);
	realData_Common.m_strOpen.Format("%ld", atol(strTmp));
	realData_Common.m_strOpen.TrimLeft(); realData_Common.m_strOpen.TrimRight();

	memcpy(sTmp, pInput->high, sizeof(pInput->high));		/*  [고가]		*/
	sTmp[sizeof(pInput->high)]=0;
	strTmp.Format("%lf", atof(sTmp)*100.);
	realData_Common.m_strHigh.Format("%ld", atol(strTmp));
	realData_Common.m_strHigh.TrimLeft(); realData_Common.m_strHigh.TrimRight();

	memcpy(sTmp, pInput->low, sizeof(pInput->low));			/*  [저가]		*/
	sTmp[sizeof(pInput->low)]=0;
	strTmp.Format("%lf", atof(sTmp)*100.);
	realData_Common.m_strLow.Format("%ld", atol(strTmp));
	realData_Common.m_strLow.TrimLeft(); realData_Common.m_strLow.TrimRight();
	
	realData_Common.m_strValue = "0";

	realData_Common.m_nMarketType = CMasterTypeInfo::FOREIGN_CHART;
	return TRUE;
}


// 상품선물
BOOL CChartRealConverter::Convert_GTFJ(LPCSTR pRealStruct, CRealData_Common& realData_Common)
{
	LPGTFJ_OutBlock pInput = (LPGTFJ_OutBlock)pRealStruct;
	CString strTmp;
	char sTmp[20+1];
	
	memcpy(sTmp, pInput->symbol, sizeof(pInput->symbol));	/*  [심볼코드]				*/
	sTmp[sizeof(pInput->symbol)]=0;
	realData_Common.m_strCode = sTmp;
	realData_Common.m_strCode.TrimLeft(); realData_Common.m_strCode.TrimRight();
	
	memcpy(sTmp, pInput->time, sizeof(pInput->time));	/*  [시간(현지시간)]				*/
	sTmp[sizeof(pInput->time)]=0;
	realData_Common.m_strTime = sTmp;
	realData_Common.m_strTime.TrimLeft(); realData_Common.m_strTime.TrimRight();
	
	memcpy(sTmp, pInput->sign, sizeof(pInput->sign));		/*  [전일대비구분]				*/
	sTmp[sizeof(pInput->sign)]=0;
	realData_Common.m_strSign = sTmp;
	realData_Common.m_strSign.TrimLeft(); realData_Common.m_strSign.TrimRight();
	
	memcpy(sTmp, pInput->price, sizeof(pInput->price));		/*  [현재가]		*/
	sTmp[sizeof(pInput->price)]=0;
	strTmp.Format("%lf", atof(sTmp)*100.);
	realData_Common.m_strPrice.Format("%ld", atol(strTmp));
	realData_Common.m_strPrice.TrimLeft(); realData_Common.m_strPrice.TrimRight();
	
	memcpy(sTmp, pInput->change, sizeof(pInput->change));	/*  [전일대비]		*/
	sTmp[sizeof(pInput->change)]=0;
	strTmp.Format("%lf", atof(sTmp)*100.);
	realData_Common.m_strChange.Format("%ld", atol(strTmp));
	realData_Common.m_strChange.TrimLeft(); realData_Common.m_strChange.TrimRight();
	
	memcpy(sTmp, pInput->drate, sizeof(pInput->drate));		/*  [등락율]				*/
	sTmp[sizeof(pInput->drate)]=0;
	strTmp.Format("%lf", atof(sTmp)*100.);
	realData_Common.m_strChrate.Format("%ld", atol(strTmp));
	realData_Common.m_strChrate.TrimLeft(); realData_Common.m_strChrate.TrimRight();
	
	memcpy(sTmp, pInput->volume, sizeof(pInput->volume));	/*  [거래량]		*/
	sTmp[sizeof(pInput->volume)]=0;
	realData_Common.m_strVolume = sTmp;
	realData_Common.m_strVolume.TrimLeft(); realData_Common.m_strVolume.TrimRight();
	
	memcpy(sTmp, pInput->open, sizeof(pInput->open));		/*  [시가]		*/
	sTmp[sizeof(pInput->open)]=0;
	strTmp.Format("%lf", atof(sTmp)*100.);
	realData_Common.m_strOpen.Format("%ld", atol(strTmp));
	realData_Common.m_strOpen.TrimLeft(); realData_Common.m_strOpen.TrimRight();
	
	memcpy(sTmp, pInput->high, sizeof(pInput->high));		/*  [고가]		*/
	sTmp[sizeof(pInput->high)]=0;
	strTmp.Format("%lf", atof(sTmp)*100.);
	realData_Common.m_strHigh.Format("%ld", atol(strTmp));
	realData_Common.m_strHigh.TrimLeft(); realData_Common.m_strHigh.TrimRight();
	
	memcpy(sTmp, pInput->low, sizeof(pInput->low));			/*  [저가]		*/
	sTmp[sizeof(pInput->low)]=0;
	strTmp.Format("%lf", atof(sTmp)*100.);
	realData_Common.m_strLow.Format("%ld", atol(strTmp));
	realData_Common.m_strLow.TrimLeft(); realData_Common.m_strLow.TrimRight();
	
	realData_Common.m_strValue = "0";
	
	realData_Common.m_nMarketType = CMasterTypeInfo::FOREIGN_CHART;
	return TRUE;
}



// FO 지수
BOOL CChartRealConverter::Convert_FOJS(LPCSTR pRealStruct, CRealData_Common& realData_Common)
{
	LPFOJS_OutBlock pInput = (LPFOJS_OutBlock)pRealStruct;
	CString strTmp;
	char sTmp[20+1];
	
	memcpy(sTmp, pInput->symbol, sizeof(pInput->symbol));	/*  [심볼코드]				*/
	sTmp[sizeof(pInput->symbol)]=0;
	realData_Common.m_strCode = sTmp;
	realData_Common.m_strCode.TrimLeft(); realData_Common.m_strCode.TrimRight();
	
	memcpy(sTmp, pInput->time, sizeof(pInput->time));	/*  [시간(현지시간)]				*/
	sTmp[sizeof(pInput->time)]=0;
	realData_Common.m_strTime = sTmp;
	realData_Common.m_strTime.TrimLeft(); realData_Common.m_strTime.TrimRight();
	
	memcpy(sTmp, pInput->sign, sizeof(pInput->sign));		/*  [전일대비구분]				*/
	sTmp[sizeof(pInput->sign)]=0;
	realData_Common.m_strSign = sTmp;
	realData_Common.m_strSign.TrimLeft(); realData_Common.m_strSign.TrimRight();
	
	memcpy(sTmp, pInput->price, sizeof(pInput->price));		/*  [현재가]		*/
	sTmp[sizeof(pInput->price)]=0;
	strTmp.Format("%lf", atof(sTmp)*100.);
	realData_Common.m_strPrice.Format("%ld", atol(strTmp));
	realData_Common.m_strPrice.TrimLeft(); realData_Common.m_strPrice.TrimRight();
	
	memcpy(sTmp, pInput->change, sizeof(pInput->change));	/*  [전일대비]		*/
	sTmp[sizeof(pInput->change)]=0;
	strTmp.Format("%lf", atof(sTmp)*100.);
	realData_Common.m_strChange.Format("%ld", atol(strTmp));
	realData_Common.m_strChange.TrimLeft(); realData_Common.m_strChange.TrimRight();
	
	memcpy(sTmp, pInput->drate, sizeof(pInput->drate));		/*  [등락율]				*/
	sTmp[sizeof(pInput->drate)]=0;
	strTmp.Format("%lf", atof(sTmp)*100.);
	realData_Common.m_strChrate.Format("%ld", atol(strTmp));
	realData_Common.m_strChrate.TrimLeft(); realData_Common.m_strChrate.TrimRight();
	
	memcpy(sTmp, pInput->volume, sizeof(pInput->volume));	/*  [거래량]		*/
	sTmp[sizeof(pInput->volume)]=0;
	realData_Common.m_strVolume = sTmp;
	realData_Common.m_strVolume.TrimLeft(); realData_Common.m_strVolume.TrimRight();
	
	memcpy(sTmp, pInput->open, sizeof(pInput->open));		/*  [시가]		*/
	sTmp[sizeof(pInput->open)]=0;
	strTmp.Format("%lf", atof(sTmp)*100.);
	realData_Common.m_strOpen.Format("%ld", atol(strTmp));
	realData_Common.m_strOpen.TrimLeft(); realData_Common.m_strOpen.TrimRight();
	
	memcpy(sTmp, pInput->high, sizeof(pInput->high));		/*  [고가]		*/
	sTmp[sizeof(pInput->high)]=0;
	strTmp.Format("%lf", atof(sTmp)*100.);
	realData_Common.m_strHigh.Format("%ld", atol(strTmp));
	realData_Common.m_strHigh.TrimLeft(); realData_Common.m_strHigh.TrimRight();
	
	memcpy(sTmp, pInput->low, sizeof(pInput->low));			/*  [저가]		*/
	sTmp[sizeof(pInput->low)]=0;
	strTmp.Format("%lf", atof(sTmp)*100.);
	realData_Common.m_strLow.Format("%ld", atol(strTmp));
	realData_Common.m_strLow.TrimLeft(); realData_Common.m_strLow.TrimRight();
	
	realData_Common.m_strValue = "0";
	
	realData_Common.m_nMarketType = CMasterTypeInfo::FOREIGN_CHART;
	return TRUE;
}

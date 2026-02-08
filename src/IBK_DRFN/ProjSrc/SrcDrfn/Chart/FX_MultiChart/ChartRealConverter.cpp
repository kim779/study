#include "stdafx.h"
#include "./ChartRealConverter.h"
#include "../CommonTR/EUREAL_KXH3.h"	//주식
#include "../CommonTR/EUREAL_IXIJ.h"	//업종
#include "../CommonTR/EUREAL_FFC0.h"	//선물
#include "../CommonTR/EUREAL_OOC0.h"	//옵션
#include "../CommonTR/EUREAL_HWFJ.h"	//해외

// --> [Edit]  강지원 2008/09/10		( 만들다 실패함... ㅠ.ㅠ 그냥 이함수 안부르고 DRDS에서 바로 처리함...  )
//#include "../CommonTR/FXREAL_a.h"		// FX 시세 리얼

// --------------------------------------------------------------------------------------
// FX시세
BOOL CChartRealConverter::Convert_a(LPCSTR pRealStruct, CRealData_Common& realData_Common, short nTime)
{
//	stFXREAL_a* pInput = (stFXREAL_a*)pRealStruct;
//	char sTmp[20+1];
//	CString strTemp = "";
//
//	memcpy(sTmp, pInput->shcode, sizeof(pInput->shcode));	/*  [단축코드]				*/
//	sTmp[sizeof(pInput->shcode)]=0;
//	realData_Common.m_strCode = sTmp;
//	realData_Common.m_strCode.TrimLeft(); realData_Common.m_strCode.TrimRight();
//	
////	if ( nTime == 0 )	//	일자
////	{
////		memcpy(sTmp, pInput->chetime, sizeof(pInput->chetime));	/*  [체결시간]				*/
////		sTmp[sizeof(pInput->chetime)]=0;
////		realData_Common.m_strTime = sTmp;
////		realData_Common.m_strTime.TrimLeft(); realData_Common.m_strTime.TrimRight();
////	}
////	else				//	시간
//	{
//		memcpy(sTmp, pInput->real_time, sizeof(pInput->real_time));	/*  [체결시간]				*/
//		sTmp[sizeof(pInput->real_time)]=0;
//		realData_Common.m_strTime = sTmp;
//		realData_Common.m_strTime.TrimLeft(); realData_Common.m_strTime.TrimRight();
//	}
//	
//	memcpy(sTmp, pInput->bid_sign, sizeof(pInput->bid_sign));		/*  [1:상한]				*/
//	sTmp[sizeof(pInput->bid_sign)]=0;
//	realData_Common.m_strSign = sTmp;
//	realData_Common.m_strSign.TrimLeft(); realData_Common.m_strSign.TrimRight();
//
//	memcpy(sTmp, pInput->bidclose_price, sizeof(pInput->bidclose_price));		/*  [현재가,     원]		*/
//	sTmp[sizeof(pInput->bidclose_price)]=0;
//	realData_Common.m_strPrice = sTmp;
//	realData_Common.m_strPrice.TrimLeft(); realData_Common.m_strPrice.TrimRight();
//
//	memcpy(sTmp, pInput->bid_change_price, sizeof(pInput->bid_change_price));	/*  [전일대비,   원]		*/
//	sTmp[sizeof(pInput->bid_change_price)]=0;
//	realData_Common.m_strChange = sTmp;
//	realData_Common.m_strChange.TrimLeft(); realData_Common.m_strChange.TrimRight();
//
////	memcpy(sTmp, pInput->drate, sizeof(pInput->drate));		/*  [등락율]				*/
////	sTmp[sizeof(pInput->drate)]=0;
//	realData_Common.m_strChrate = "";
//	realData_Common.m_strChrate.TrimLeft(); realData_Common.m_strChrate.TrimRight();
//
//	strTemp.Format("%s", pInput->bid_volume);
////	memcpy(sTmp, pInput->volume, sizeof(pInput->volume));	/*  [누적거래량, 주]		*/
////	sTmp[sizeof(pInput->volume)]=0;
////	realData_Common.m_strVolume = sTmp;
//	realData_Common.m_strVolume = strTemp;
//	realData_Common.m_strVolume.TrimLeft(); realData_Common.m_strVolume.TrimRight();
//
//	memcpy(sTmp, pInput->bidopen_price, sizeof(pInput->bidopen_price));		/*  [시가,       원]		*/
//	sTmp[sizeof(pInput->bidopen_price)]=0;
//	realData_Common.m_strOpen = sTmp;
//	realData_Common.m_strOpen.TrimLeft(); realData_Common.m_strOpen.TrimRight();
//
//	memcpy(sTmp, pInput->bidhigh_price, sizeof(pInput->bidhigh_price));		/*  [고가,       원]		*/
//	sTmp[sizeof(pInput->bidhigh_price)]=0;
//	realData_Common.m_strHigh = sTmp;
//	realData_Common.m_strHigh.TrimLeft(); realData_Common.m_strHigh.TrimRight();
//
//	memcpy(sTmp, pInput->bidlow_price, sizeof(pInput->bidlow_price));			/*  [저가,       원]		*/
//	sTmp[sizeof(pInput->bidlow_price)]=0;
//	realData_Common.m_strLow = sTmp;
//	realData_Common.m_strLow.TrimLeft(); realData_Common.m_strLow.TrimRight();
	
	return TRUE;
}
// <-- [Edit]  강지원 2008/09/10

// --------------------------------------------------------------------------------------
// 주식
BOOL CChartRealConverter::Convert_KXS3(LPCSTR pRealStruct, CRealData_Common& realData_Common)
{
	stEUREAL_KXH3* pInput = (stEUREAL_KXH3*)pRealStruct;
	char sTmp[20+1];

	memcpy(sTmp, pInput->kshcode, sizeof(pInput->kshcode));	/*  [단축코드]				*/
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
	
	return TRUE;
}

// --------------------------------------------------------------------------------------
// 업종
BOOL CChartRealConverter::Convert_IXIJ(LPCSTR pRealStruct, CRealData_Common& realData_Common)
{
	stEUREAL_IXIJ* pInput = (stEUREAL_IXIJ*)pRealStruct;
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
	
	return TRUE;
}

// --------------------------------------------------------------------------------------
// 선물
BOOL CChartRealConverter::Convert_FFC0(LPCSTR pRealStruct, CRealData_Common& realData_Common)
{
	stEUREAL_FFC0* pInput = (stEUREAL_FFC0*)pRealStruct;
	char sTmp[20+1];

	memcpy(sTmp, pInput->kfutcode, sizeof(pInput->kfutcode));	/*  [선물단축코드]				*/
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
	
	return TRUE;
}

// --------------------------------------------------------------------------------------
// 옵션
BOOL CChartRealConverter::Convert_OOC0(LPCSTR pRealStruct, CRealData_Common& realData_Common)
{
	stEUREAL_OOC0* pInput = (stEUREAL_OOC0*)pRealStruct;
	char sTmp[20+1];

	memcpy(sTmp, pInput->koptcode, sizeof(pInput->koptcode));	/*  [단축코드]				*/
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
	
	return TRUE;
}

// 해외
BOOL CChartRealConverter::Convert_HWFJ(LPCSTR pRealStruct, CRealData_Common& realData_Common)
{
	stEUREAL_HWFJ* pInput = (stEUREAL_HWFJ*)pRealStruct;
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
	realData_Common.m_strPrice = sTmp;
	realData_Common.m_strPrice.TrimLeft(); realData_Common.m_strPrice.TrimRight();

	memcpy(sTmp, pInput->change, sizeof(pInput->change));	/*  [전일대비]		*/
	sTmp[sizeof(pInput->change)]=0;
	realData_Common.m_strChange = sTmp;
	realData_Common.m_strChange.TrimLeft(); realData_Common.m_strChange.TrimRight();

	memcpy(sTmp, pInput->drate, sizeof(pInput->drate));		/*  [등락율]				*/
	sTmp[sizeof(pInput->drate)]=0;
	realData_Common.m_strChrate = sTmp;
	realData_Common.m_strChrate.TrimLeft(); realData_Common.m_strChrate.TrimRight();

	memcpy(sTmp, pInput->volume, sizeof(pInput->volume));	/*  [거래량]		*/
	sTmp[sizeof(pInput->volume)]=0;
	realData_Common.m_strVolume = sTmp;
	realData_Common.m_strVolume.TrimLeft(); realData_Common.m_strVolume.TrimRight();

	memcpy(sTmp, pInput->open, sizeof(pInput->open));		/*  [시가]		*/
	sTmp[sizeof(pInput->open)]=0;
	realData_Common.m_strOpen = sTmp;
	realData_Common.m_strOpen.TrimLeft(); realData_Common.m_strOpen.TrimRight();

	memcpy(sTmp, pInput->high, sizeof(pInput->high));		/*  [고가]		*/
	sTmp[sizeof(pInput->high)]=0;
	realData_Common.m_strHigh = sTmp;
	realData_Common.m_strHigh.TrimLeft(); realData_Common.m_strHigh.TrimRight();

	memcpy(sTmp, pInput->low, sizeof(pInput->low));			/*  [저가]		*/
	sTmp[sizeof(pInput->low)]=0;
	realData_Common.m_strLow = sTmp;
	realData_Common.m_strLow.TrimLeft(); realData_Common.m_strLow.TrimRight();
	
	return TRUE;
}
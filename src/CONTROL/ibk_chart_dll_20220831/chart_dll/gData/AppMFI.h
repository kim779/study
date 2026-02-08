#if !defined(AFX_APPMFI_H__9070B1EE_3F7D_4EC5_896B_849439919F73__INCLUDED_)
#define AFX_APPMFI_H__9070B1EE_3F7D_4EC5_896B_849439919F73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineData.h"
///////////////////////////////////////////////////////////////////////////////
// MFI (Money Flow Index)
// 주식 거래를 위한 자금이 유입되거나 유출되는 양을 측정해 주는 지표로서
// 추세 전환시기를 예측하거나 시세의 과열 및 침체정도를 파악하는데 유용
// 공식 : TP(Typical Price)
// TP = (고가+저가+종가) / 3
// Money Flow = TP * 거래량
// PositiveMoneyFlow : 당일 TP > 전일 TP 일때 MoneyFlow의 합
// NegativeMoneyFlow : 당일 TP < 전일 TP 일때 MoneyFlow의 합
// Money Ratio : PositiveMoneyFlow / NegativeMoneyFlow
// Money Flow Index = 100 - 100 / (1 + MoneyRatio)
///////////////////////////////////////////////////////////////////////////////
class CAppMFI : public CLineData  
{
public:
	CAppMFI(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppMFI();

protected:
	bool CalcuData(bool bShift = false, bool bLast = false);	
};

#endif // !defined(AFX_APPMFI_H__9070B1EE_3F7D_4EC5_896B_849439919F73__INCLUDED_)

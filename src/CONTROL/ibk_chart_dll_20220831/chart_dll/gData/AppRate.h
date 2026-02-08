#if !defined(AFX_APPRATE_H__4932F483_2F91_4B7B_BB2B_5CEC052AB31E__INCLUDED_)
#define AFX_APPRATE_H__4932F483_2F91_4B7B_BB2B_5CEC052AB31E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineData.h"

/*
거래량회전율은 당일의 거래량 규모를 기준으로 하여 상장주식 총수가 연간 몇 회전하고 있는가를 
나타낸 것이다. 회전율 100%의 의미는 상장주식 총수가 연간 1회전하는 것을 의미한다. 
과거의 검증으로 볼 때 50%를 기준으로 130% 이상이 단기경계, 20% 수준이면 바닥으로 본다
*/

// 회전율
// 공식: ( (당일거래량*입회일수(300)) / 상장주식총수 ) * 100

class CAppRate : public CLineData  
{
public:
	CAppRate(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppRate();

protected:
	bool CalcuData(bool bShift = false, bool bLast = false);
};

#endif // !defined(AFX_APPRATE_H__4932F483_2F91_4B7B_BB2B_5CEC052AB31E__INCLUDED_)

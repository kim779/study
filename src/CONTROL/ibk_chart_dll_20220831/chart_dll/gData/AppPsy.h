#if !defined(AFX_APPPSY_H__367CC2DC_F8B9_43A0_8D1A_BCD37487C054__INCLUDED_)
#define AFX_APPPSY_H__367CC2DC_F8B9_43A0_8D1A_BCD37487C054__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineData.h"

// 심리도
// 공식: ( n일간 상승일수/n ) * 100 
// n의 기본값은 10
// 중요값: 25,75

class CAppPsy : public CLineData  
{
public:
	CAppPsy(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppPsy();

protected:
	bool CalcuData(bool bShift = false, bool bLast = false);		
};

#endif // !defined(AFX_APPPSY_H__367CC2DC_F8B9_43A0_8D1A_BCD37487C054__INCLUDED_)

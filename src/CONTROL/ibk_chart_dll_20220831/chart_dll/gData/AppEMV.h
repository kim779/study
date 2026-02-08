#if !defined(AFX_CAPPEMV_H__A60D21FF_C28D_453E_9FF2_A5091AD78028__INCLUDED_)
#define AFX_CAPPEMV_H__A60D21FF_C28D_453E_9FF2_A5091AD78028__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineData.h"

/*
Arm's Ease of Movement
Val1 = (( 당일고가 + 당일저가 ) / 2 - (전일고가 + 전일저가) / 2) / ((거래량) / (당일고가 - 당일저가))
EMV = Val1 의 n일 지수 이평
*/

class CAppEMV : public CLineData  
{
public:
	CAppEMV(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppEMV();

protected:
	bool CalcuData(bool bShift = false, bool bLast = false);

};

#endif // !defined(AFX_CAPPEMV_H__A60D21FF_C28D_453E_9FF2_A5091AD78028__INCLUDED_)

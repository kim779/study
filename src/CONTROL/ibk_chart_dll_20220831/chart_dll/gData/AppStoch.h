#if !defined(AFX_APPSTOCH_H__4F5871D2_30BE_46E7_9751_1ACD4E9763EF__INCLUDED_)
#define AFX_APPSTOCH_H__4F5871D2_30BE_46E7_9751_1ACD4E9763EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineData.h"

//	Stochastic
//	fast %K : [ (당일종가 - N일동안의 최저가) / 
//		(n일동안의 최고가 - N일동안의 최저가) ] * 100
//	fast %D(= slow %K) : fast %K의 M일이평
//	slow %D : fast %D의 L일이평

//	fast : N:5, M:3
//	slow : N:5, M:3, L:3

class CAppStoch : public CLineData  
{
public:
	CAppStoch(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppStoch();

protected:
	bool CalcuData(bool bShift = false, bool bLast = false);		
};

#endif // !defined(AFX_APPSTOCH_H__4F5871D2_30BE_46E7_9751_1ACD4E9763EF__INCLUDED_)

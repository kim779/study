#if !defined(AFX_APPSONA_H__C2B92BBB_EAAE_49B2_ABF6_D6BC45606E2C__INCLUDED_)
#define AFX_APPSONA_H__C2B92BBB_EAAE_49B2_ABF6_D6BC45606E2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineData.h"

// Sona chart
// 공식: [(당일 n1일 지수이평 - 전일 n1일 지수이평) / 전일 n1일 지수이평] * 100
//       Sona's MA: Sona/m
//
// input : n1

class CAppSona : public CLineData  
{
public:
	CAppSona(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppSona();

protected:
	bool CalcuData(bool bShift = false, bool bLast = false);
	double	m_prevVal[2];
};

#endif // !defined(AFX_APPSONA_H__C2B92BBB_EAAE_49B2_ABF6_D6BC45606E2C__INCLUDED_)

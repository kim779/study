// AppSonaM.h: interface for the CAppSonaM class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APPSONAM_H__378616D1_21BB_495F_BFE2_F85B9A9DB6EF__INCLUDED_)
#define AFX_APPSONAM_H__378616D1_21BB_495F_BFE2_F85B9A9DB6EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineData.h"

// Sona chart
// 공식: (당일 n1일 지수이평 - n2일전 n1일 지수이평) * 100
//       Sona's MA: Sona/m
//
// input : n1,n2

class CAppSonaM : public CLineData  
{
public:
	CAppSonaM(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppSonaM();

protected:
	bool CalcuData(bool bShift = false, bool bLast = false);
	double	m_prevVal[2];
};

#endif // !defined(AFX_APPSONAM_H__378616D1_21BB_495F_BFE2_F85B9A9DB6EF__INCLUDED_)

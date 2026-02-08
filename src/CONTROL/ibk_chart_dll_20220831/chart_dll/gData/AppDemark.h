#if !defined(AFX_APPDEMARK_H__33D2E4D0_1B2F_4C7A_829C_35ADD26B9B05__INCLUDED_)
#define AFX_APPDEMARK_H__33D2E4D0_1B2F_4C7A_829C_35ADD26B9B05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineData.h"

//////////////////////////////////////////////////////////////////////////
//	Demark 
//////////////////////////////////////////////////////////////////////////
//	타입 : 라인
//	개수 : 1
//	비고 : 
//////////////////////////////////////////////////////////////////////////
//
//	변수)
//
//	공식)
//	if (fpr(1) > epr(1))
//		X = (hpr(1) + lpr(1) + epr(1) + lpr(1)) / 2
//	else if (fpr(1) < epr(1))
//		X = (hpr(1) + lpr(1) + epr(1) + hpr(1)) / 2
//	else if (fpr(1) == epr(1))
//		X = (hpr(1) + lpr(1) + epr(1) + epr(1)) / 2
//
//	DemarkHigh = X - lpr(1)
//	DemarkLow = X - hpr(1)
//
//////////////////////////////////////////////////////////////////////////

class CAppDemark : public CLineData  
{
public:
	CAppDemark(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppDemark();

private:	
	bool CalcuData(bool bShift = false, bool bLast = false);	
};

#endif // !defined(AFX_APPDEMARK_H__33D2E4D0_1B2F_4C7A_829C_35ADD26B9B05__INCLUDED_)

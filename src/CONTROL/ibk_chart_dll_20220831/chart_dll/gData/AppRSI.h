#if !defined(AFX_APPRSI_H__1A611B45_40DF_4EA7_AEC2_A165E36AAE11__INCLUDED_)
#define AFX_APPRSI_H__1A611B45_40DF_4EA7_AEC2_A165E36AAE11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineData.h"

class CAppRSI : public CLineData  
{
public:
	CAppRSI(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppRSI();

protected:
	bool CalcuData(bool bShift = false, bool bLast = false);		
};

#endif // !defined(AFX_APPRSI_H__1A611B45_40DF_4EA7_AEC2_A165E36AAE11__INCLUDED_)

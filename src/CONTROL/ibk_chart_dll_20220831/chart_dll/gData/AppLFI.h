#if !defined(AFX_APPLFI_H__9E0283B9_17BF_4EFA_80BE_E7654ED3974D__INCLUDED_)
#define AFX_APPLFI_H__9E0283B9_17BF_4EFA_80BE_E7654ED3974D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineData.h"
//	LFI chart
//	공식: val = 종가[t] - 시가[t]
//	      LFI[t] = (val > 0)? 
//	               LFI[t-1] + val*3 : LFI[t-1] + val*2
class CAppLFI : public CLineData  
{
public:
	CAppLFI(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppLFI();

protected:
	bool CalcuData(bool bShift = false, bool bLast = false);
};

#endif // !defined(AFX_APPLFI_H__9E0283B9_17BF_4EFA_80BE_E7654ED3974D__INCLUDED_)

#if !defined(AFX_APPNCO_H__C3E1CE57_45B4_4CE4_BC7E_E573000EF82B__INCLUDED_)
#define AFX_APPNCO_H__C3E1CE57_45B4_4CE4_BC7E_E573000EF82B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineData.h"

class CAppNCO : public CLineData  
{
public:
	CAppNCO(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppNCO();

protected:
	bool CalcuData(bool bShift = false, bool bLast = false);		
};

#endif // !defined(AFX_APPNCO_H__C3E1CE57_45B4_4CE4_BC7E_E573000EF82B__INCLUDED_)

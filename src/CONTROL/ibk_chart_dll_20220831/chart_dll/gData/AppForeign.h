// AppForeign.h: interface for the CAppForeign class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APPFOREIGN_H__FC15B3F1_6A24_41D3_AD9A_E2A980DE3E03__INCLUDED_)
#define AFX_APPFOREIGN_H__FC15B3F1_6A24_41D3_AD9A_E2A980DE3E03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineData.h"

class CAppForeign : public CLineData  
{
public:
	CAppForeign(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppForeign();

protected:
	bool CalcuData(bool bShift = false, bool bLast = false);		
	virtual bool CalculateMinMax(int start, int end, double *Min, double *Max, bool bForce = false);
};

#endif // !defined(AFX_APPFOREIGN_H__FC15B3F1_6A24_41D3_AD9A_E2A980DE3E03__INCLUDED_)

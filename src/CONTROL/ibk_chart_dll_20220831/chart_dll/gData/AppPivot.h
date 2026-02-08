#if !defined(AFX_APPPIVOT_H__C1B97911_6782_4C2B_891A_84BFB5F43F95__INCLUDED_)
#define AFX_APPPIVOT_H__C1B97911_6782_4C2B_891A_84BFB5F43F95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineData.h"

class CAppPivot : public CLineData  
{
public:
	CAppPivot(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppPivot();

private:
	bool CalcuData(bool bShift = false, bool bLast = false);	
};

#endif // !defined(AFX_APPPIVOT_H__C1B97911_6782_4C2B_891A_84BFB5F43F95__INCLUDED_)

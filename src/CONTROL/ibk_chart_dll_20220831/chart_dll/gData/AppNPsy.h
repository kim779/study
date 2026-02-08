#if !defined(AFX_APPNPSY_H__A9F8B38F_F761_4B47_A98A_D0C8FDAB448B__INCLUDED_)
#define AFX_APPNPSY_H__A9F8B38F_F761_4B47_A98A_D0C8FDAB448B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineData.h"

class CAppNPsy : public CLineData  
{
public:
	CAppNPsy(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppNPsy();

protected:
	bool CalcuData(bool bShift = false, bool bLast = false);		
};

#endif // !defined(AFX_APPNPSY_H__A9F8B38F_F761_4B47_A98A_D0C8FDAB448B__INCLUDED_)

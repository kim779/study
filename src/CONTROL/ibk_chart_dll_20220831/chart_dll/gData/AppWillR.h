#if !defined(AFX_APPWILLR_H__D7C53905_7497_4C34_BC3D_B9E0DE3FD54F__INCLUDED_)
#define AFX_APPWILLR_H__D7C53905_7497_4C34_BC3D_B9E0DE3FD54F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineData.h"

class CAppWillR : public CLineData  
{
public:
	CAppWillR(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppWillR();

protected:
	bool CalcuData(bool bShift = false, bool bLast = false);
};

#endif // !defined(AFX_APPWILLR_H__D7C53905_7497_4C34_BC3D_B9E0DE3FD54F__INCLUDED_)

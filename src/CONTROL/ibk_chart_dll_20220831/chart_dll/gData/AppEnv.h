#if !defined(AFX_APPENV_H__FF123526_E508_457B_86D7_59924BC048D8__INCLUDED_)
#define AFX_APPENV_H__FF123526_E508_457B_86D7_59924BC048D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineData.h"

class CAppEnv : public CLineData  
{
public:
	CAppEnv(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppEnv();

protected:
	bool CalcuData(bool bShift = false, bool bLast = false);		
};

#endif // !defined(AFX_APPENV_H__FF123526_E508_457B_86D7_59924BC048D8__INCLUDED_)

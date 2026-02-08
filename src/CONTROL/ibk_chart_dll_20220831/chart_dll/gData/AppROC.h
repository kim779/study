#if !defined(AFX_APPROC_H__C0C9E458_66FC_4042_B292_43415E7F03E2__INCLUDED_)
#define AFX_APPROC_H__C0C9E458_66FC_4042_B292_43415E7F03E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineData.h"

class CAppROC : public CLineData  
{
public:
	CAppROC(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppROC();

protected:
	bool CalcuData(bool bShift = false, bool bLast = false);
};

#endif // !defined(AFX_APPROC_H__C0C9E458_66FC_4042_B292_43415E7F03E2__INCLUDED_)

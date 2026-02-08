#if !defined(AFX_APPMMT_H__8C785DEA_F58B_4242_84E1_B269C5F9C6FE__INCLUDED_)
#define AFX_APPMMT_H__8C785DEA_F58B_4242_84E1_B269C5F9C6FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineData.h"

class CAppMmt : public CLineData  
{
public:
	CAppMmt(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppMmt();

protected:
	bool CalcuData(bool bShift = false, bool bLast = false);	
};

#endif // !defined(AFX_APPMMT_H__8C785DEA_F58B_4242_84E1_B269C5F9C6FE__INCLUDED_)

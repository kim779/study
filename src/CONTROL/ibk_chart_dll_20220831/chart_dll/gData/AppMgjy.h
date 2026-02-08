// AppMgjy.h: interface for the CAppMgjy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APPMGJY_H__F7C2DF02_DEF9_41B9_BE9E_2F473A8CDFF1__INCLUDED_)
#define AFX_APPMGJY_H__F7C2DF02_DEF9_41B9_BE9E_2F473A8CDFF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineData.h"

class CAppMgjy : public CLineData  
{
public:
	CAppMgjy(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppMgjy();

protected:
	bool CalcuData(bool bShift = false, bool bLast = false);		
};

#endif // !defined(AFX_APPMGJY_H__F7C2DF02_DEF9_41B9_BE9E_2F473A8CDFF1__INCLUDED_)

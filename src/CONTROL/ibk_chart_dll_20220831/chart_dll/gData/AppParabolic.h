#if !defined(AFX_APPPARABOLIC_H__C158D228_F44A_441F_9960_BE4BD969DB03__INCLUDED_)
#define AFX_APPPARABOLIC_H__C158D228_F44A_441F_9960_BE4BD969DB03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineData.h"

class CAppParabolic : public CLineData  
{
public:
	CAppParabolic(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppParabolic();

private:
	bool CalcuData(bool bShift = false, bool bLast = false);
	enum	{tendencyNO = 0, tendencyUP = 1, tendencyUPDN = 2, tendencyDN = 3, tendencyDNUP = 4} m_tendency;
};

#endif // !defined(AFX_APPPARABOLIC_H__C158D228_F44A_441F_9960_BE4BD969DB03__INCLUDED_)

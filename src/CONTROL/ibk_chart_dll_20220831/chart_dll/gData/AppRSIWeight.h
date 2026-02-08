// AppRSIWeight.h: interface for the CAppRSIWeight class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APPRSIWEIGHT_H__F3AD70F1_5E4F_478E_8D48_C07A5F4AAE00__INCLUDED_)
#define AFX_APPRSIWEIGHT_H__F3AD70F1_5E4F_478E_8D48_C07A5F4AAE00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineData.h"

class CAppRSIWeight : public CLineData  
{
public:
	CAppRSIWeight(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppRSIWeight();

protected:
	bool CalcuData(bool bShift = false, bool bLast = false);

private:
	double m_dPreviousLossAverage;
	double m_dPreviousGainAverage;
	double m_dLossAveragePreviousInterval;
	double m_dGainAveragePreviousInterval;
};

#endif // !defined(AFX_APPRSIWEIGHT_H__F3AD70F1_5E4F_478E_8D48_C07A5F4AAE00__INCLUDED_)

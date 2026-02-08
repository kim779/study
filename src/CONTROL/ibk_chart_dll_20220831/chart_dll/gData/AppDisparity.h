#if !defined(AFX_APPDISPARITY_H__1E137340_40EC_4245_B0EC_B4AE0D13FED6__INCLUDED_)
#define AFX_APPDISPARITY_H__1E137340_40EC_4245_B0EC_B4AE0D13FED6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineData.h"

// 이격률의 격차가 너무 작을 때를 위해서 100을 임의로 곱하고 다시 나누어 보여준다.

class CAppDisparity : public CLineData  
{
public:
	CAppDisparity(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppDisparity();	

protected:
	bool CalcuData(bool bShift = false, bool bLast = false);	
	double	m_prevVal[2];
};

#endif // !defined(AFX_APPDISPARITY_H__1E137340_40EC_4245_B0EC_B4AE0D13FED6__INCLUDED_)

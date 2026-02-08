// LineData.h: interface for the CLineData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINEDATA_H__3150AA17_E636_48C8_AFC3_780056111973__INCLUDED_)
#define AFX_LINEDATA_H__3150AA17_E636_48C8_AFC3_780056111973__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AppData.h"

class CLineData : public CAppData  
{
public:
	CLineData(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CLineData();

protected:
	bool	m_bShift;
	int	m_aiNLimit[12];	

protected:
	virtual bool Calculate(int start, int end, bool bForce = true);
	virtual bool CalculateMinMax(int start, int end, double *Min, double *Max, bool bForce = false);
	virtual bool CalcuData(bool bShift = false, bool bLast = false) { return true; } 
	virtual bool CalculateLastIndex(bool bShift, bool bIncrease);
	virtual void CalcuLastData(bool bShift);
	virtual void ShiftData();
	virtual void CLineData::IncreaseData();

	int CalcEMA(double *source, double *dest, int count, int start, int size, bool bLast);
	int CalcEMA(double *source, double* prev, double *dest, int size);

};

#endif // !defined(AFX_LINEDATA_H__3150AA17_E636_48C8_AFC3_780056111973__INCLUDED_)

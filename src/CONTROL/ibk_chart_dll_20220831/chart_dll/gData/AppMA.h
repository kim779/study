// AppMA.h: interface for the CAppMA class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APPMA_H__F8FD9705_B11E_4ACB_A281_6276DFB48773__INCLUDED_)
#define AFX_APPMA_H__F8FD9705_B11E_4ACB_A281_6276DFB48773__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AppData.h"
#include "../../h/axisgenv.h"

class CAppMA : public CAppData  
{
public:
	CAppMA(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppMA();

	double GetVal(int lindex, int vindex);
	bool	Calculate(int start, int end, bool bForce = true);
	bool	CalculateMinMax(int start, int end, double *Min, double *Max, bool bForce = false);
	bool	CalculateLastIndex(bool bShift, bool bIncrease);

	struct	_maData
	{
		double	ma[MAX_LINE];
	};

protected:
	int	CalculateSMA(int idx, int nDay);
	int	CalculateSWMA(int idx, int nDay);
	int	CalculateEMA(int idx, int nDay);
	int	CalculateLastSMA(int idx, int nDay);
	int	CalculateLastSWMA(int idx, int nDay);
	int	CalculateLastEMA(int idx, int nDay);

private:
	CArray	<_maData*, _maData*>		m_arMA;
};

#endif // !defined(AFX_APPMA_H__F8FD9705_B11E_4ACB_A281_6276DFB48773__INCLUDED_)

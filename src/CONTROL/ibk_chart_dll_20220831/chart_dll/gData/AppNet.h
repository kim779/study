// AppNet.h: interface for the CAppNet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APPNET_H__CAB3C780_9103_4F72_B246_5CB61E0EF90D__INCLUDED_)
#define AFX_APPNET_H__CAB3C780_9103_4F72_B246_5CB61E0EF90D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AppData.h"

class CAppNet : public CAppData  
{
public:
	CAppNet(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppNet();

	double GetVal(int lindex, int vindex);
	bool	Calculate(int start, int end, bool bForce = true);
	bool	CalculateMinMax(int start, int end, double *Min, double *Max, bool bForce = false);
	bool	CalculateLastIndex(bool bShift, bool bIncrease);	

	struct	_netData
	{
		double	ma[30];
	};

protected:
	int	CalculateSMA(int idx, int nDay);
	int	CalculateSWMA(int idx, int nDay);
	int	CalculateEMA(int idx, int nDay);
	int	CalculateLastSMA(int idx, int nDay);
	int	CalculateLastSWMA(int idx, int nDay);
	int	CalculateLastEMA(int idx, int nDay);

	int	m_dayMAV;
	int	m_gap; 
	int	m_netCnt;

private:
	CArray	<_netData *, _netData *>	m_netQue;
	int	m_pIndex[30];
};

#endif // !defined(AFX_APPNET_H__CAB3C780_9103_4F72_B246_5CB61E0EF90D__INCLUDED_)

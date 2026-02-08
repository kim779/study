#if !defined(AFX_APPSIGMA_H__6E88C771_3AC1_4FCD_9E1F_88F24EFF69A6__INCLUDED_)
#define AFX_APPSIGMA_H__6E88C771_3AC1_4FCD_9E1F_88F24EFF69A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineData.h"

// 시그마차트
// 공식: S = SQRT( Σ(종가 - 이평)²/ (n - 1) )
//	 SIGMA = ( (종가 - 이평) / S ) * 100		
//	n: 기본값 20
class CAppSigma : public CLineData  
{
public:
	CAppSigma(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppSigma();

protected:
	bool CalcuData(bool bShift = false, bool bLast = false);
	double	m_prevVal[2];
};

#endif // !defined(AFX_APPSIGMA_H__6E88C771_3AC1_4FCD_9E1F_88F24EFF69A6__INCLUDED_)

#if !defined(AFX_APPCV_H__CBE1335A_99BA_4C97_9665_C1B510E3D624__INCLUDED_)
#define AFX_APPCV_H__CBE1335A_99BA_4C97_9665_C1B510E3D624__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineData.h"

//////////////////////////////////////////////////////////////////////////
//	Chaikin's Volatility
//////////////////////////////////////////////////////////////////////////
//	타입 : 라인
//	개수 : 1
//	비고 : 
//////////////////////////////////////////////////////////////////////////
//
//	변수)
//	n : 기간
//
//	공식)
//	HLma = EMA(H-L, n)
//	HLmama = EMA(HLma, n)
//	CV = (HLma-HLmama)/HLmama * 100
//
//////////////////////////////////////////////////////////////////////////

class CAppCV : public CLineData  
{
public:
	CAppCV(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppCV();

protected:
	bool CalcuData(bool bShift = false, bool bLast = false);
	double GetnDayVal(int nStart, int nday);

	double	m_prevVal[6];	
};

#endif // !defined(AFX_APPCV_H__CBE1335A_99BA_4C97_9665_C1B510E3D624__INCLUDED_)

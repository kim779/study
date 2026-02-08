#if !defined(AFX_APPCO_H__A1603A16_8A37_4D69_97B5_AE2D2C1124E9__INCLUDED_)
#define AFX_APPCO_H__A1603A16_8A37_4D69_97B5_AE2D2C1124E9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineData.h"

//////////////////////////////////////////////////////////////////////////
//	Chaikin's Oscillator
//////////////////////////////////////////////////////////////////////////
//	타입 : 라인
//	개수 : 1
//	비고 : 
//////////////////////////////////////////////////////////////////////////
//
//	변수)
//	n1 : 단기기간
//	n2 : 장기기간
//
//	공식)
//		(종가- 저가)-(고가-종가)
//	CAD = ────────────── × 거래량의 누적치
//		        고가-저가
//	CO = 단기 이평 - 장기이평
//
//////////////////////////////////////////////////////////////////////////

class CAppCO : public CLineData  
{
public:
	CAppCO(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppCO();

protected:
	bool CalcuData(bool bShift = false, bool bLast = false);	
	double	m_prevVal[6];
};

#endif // !defined(AFX_APPCO_H__A1603A16_8A37_4D69_97B5_AE2D2C1124E9__INCLUDED_)

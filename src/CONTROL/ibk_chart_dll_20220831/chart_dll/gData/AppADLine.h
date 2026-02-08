#if !defined(AFX_APPADLINE_H__65C3F0FD_EDC8_4215_9A15_340EC2D75BD2__INCLUDED_)
#define AFX_APPADLINE_H__65C3F0FD_EDC8_4215_9A15_340EC2D75BD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineData.h"

//////////////////////////////////////////////////////////////////////////
//	A/D Line(Accumulation/Distribution)
//////////////////////////////////////////////////////////////////////////
//	타입 : 라인
//	개수 : 1
//	비고 : 고가와 저가가 같을 경우 누적이 되지 않는다(skip).
//////////////////////////////////////////////////////////////////////////
//	변수)
//
//	공식)
//
//		    (epr - lpr) - (hpr - epr)
//	ADLine = ─────────────── * trn 의 누적치
//		           (hpr - lpr)	
//
//////////////////////////////////////////////////////////////////////////


class CAppADLine : public CLineData  
{
public:
	CAppADLine(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppADLine();	

protected:
	bool CalcuData(bool bShift = false, bool bLast = false);	
};

#endif // !defined(AFX_APPADLINE_H__65C3F0FD_EDC8_4215_9A15_340EC2D75BD2__INCLUDED_)

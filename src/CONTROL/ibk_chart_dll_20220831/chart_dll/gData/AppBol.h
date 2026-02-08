#if !defined(AFX_APPBOL_H__69D7D08D_92B9_4B7B_9F43_4114AFACC039__INCLUDED_)
#define AFX_APPBOL_H__69D7D08D_92B9_4B7B_9F43_4114AFACC039__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineData.h"

//////////////////////////////////////////////////////////////////////////
//	Bollinger Band
//////////////////////////////////////////////////////////////////////////
//	타입 : 라인
//	개수 : 3
//	비고 : 
//////////////////////////////////////////////////////////////////////////
//	변수)
//	n : 기간
//	Multi : 승수
//
//	공식)
//	HighLine = MidLine + Multi * std(MidLine, n)	// MidLine의 n일간의 표준편차를 구하여 
//							// 승수배하여 더한다
//	MidLine = MA(epr, n)				// n일간의 종가 이평
//	LowLine = MidLine - Multi * std(MidLine, n)	// MidLine의 n일간의 표준편차를 구하여 
//							// 승수배하여 뺀다
//      
//      
//
//////////////////////////////////////////////////////////////////////////

class CAppBol : public CLineData  
{
public:
	CAppBol(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppBol();

protected:
	bool CalcuData(bool bShift = false, bool bLast = false);	
};

#endif // !defined(AFX_APPBOL_H__69D7D08D_92B9_4B7B_9F43_4114AFACC039__INCLUDED_)

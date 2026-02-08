// AppBandWidth.h: interface for the CAppBandWidth class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APPBANDWIDTH_H__95638802_71F7_4CE3_831E_678546B15B7E__INCLUDED_)
#define AFX_APPBANDWIDTH_H__95638802_71F7_4CE3_831E_678546B15B7E__INCLUDED_

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
//      Band Width = (볼린저밴드 상한선-볼린저밴드 하한선) / 볼린저밴드 중심선
//      
//
//////////////////////////////////////////////////////////////////////////

class CAppBandWidth : public CLineData  
{
public:
	CAppBandWidth(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppBandWidth();

protected:
	bool CalcuData(bool bShift = false, bool bLast = false);

	double	*m_pBollinger[3];

};

#endif // !defined(AFX_APPBANDWIDTH_H__95638802_71F7_4CE3_831E_678546B15B7E__INCLUDED_)

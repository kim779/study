#if !defined(AFX_APPVR_H__8888BE46_CB6C_4B32_B312_7CCF90B4EF5C__INCLUDED_)
#define AFX_APPVR_H__8888BE46_CB6C_4B32_B312_7CCF90B4EF5C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineData.h"

// n일동안 주가상승일의 거래량합계 + 보합일의 거래량합계 * 0.5
// ----------------------------------------------------------- * 100
// n일동안 주가하락일의 거래량합계 + 보합일의 거래량합계 * 0.5

//	중요값	: 70,150,450 
//	n: 기본값 20

class CAppVR : public CLineData  
{
public:
	CAppVR(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppVR();

protected:
	bool CalcuData(bool bShift = false, bool bLast = false);
};

#endif // !defined(AFX_APPVR_H__8888BE46_CB6C_4B32_B312_7CCF90B4EF5C__INCLUDED_)

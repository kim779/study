#if !defined(AFX_APPDMI_H__709A0CD6_9B69_4DCA_9AAE_390C8C4DB562__INCLUDED_)
#define AFX_APPDMI_H__709A0CD6_9B69_4DCA_9AAE_390C8C4DB562__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineData.h"

//	** DMI **
//	rf1)	pDM = abs(고가(t) - 고가(t-1) )	
//		mDM = abs( 저가(t) - 저가(t-1) )
//
//	rf2)	p = 고가(t) - 고가(t-1)
//		m = 저가(t) - 저가(t-1)
//		pDM = ( p > 0 )? p:0;
//		mDM = ( m < 0 )? abs(m):0;
//
//	rf1)	CL  = abs( 종가(t) - 종가(t-1) )
//		TR = MAX(pDM, mDM, CL)
//	rf2)	TR = MAX( abs(고가(t) - 저가(t)), abs(고가(t) - 종가(t-1)),
//			abs(저가(t-1) - 종가(t-1)) )
//	
//	1.pDI(상승탄력) : (pDM이평 / TR이평) * 100
//	2.mDI(하락탄력) : (mDM이평 / TR이평) * 100

//	** ADX **
//	dDI = abs(pDI - mDI)
//	sDI = pDI + mDI
//	DX  = (dDI / sDI) * 100
//	ADX = ΣDX / n		n은 DMI와 같은 기간을 사용
//	n: 기본값 14

class CAppDMI : public CLineData  
{
public:
	CAppDMI(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppDMI();	

protected:
	bool CalcuData(bool bShift = false, bool bLast = false);
	double	m_prevVal[6];
};

#endif // !defined(AFX_APPDMI_H__709A0CD6_9B69_4DCA_9AAE_390C8C4DB562__INCLUDED_)

#if !defined(AFX_APPOBV_H__5FA5E91E_15A3_4F7A_9BD7_ECC44793AE50__INCLUDED_)
#define AFX_APPOBV_H__5FA5E91E_15A3_4F7A_9BD7_ECC44793AE50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineData.h"

// O.B.V
// 전일대비주가상승일의 거래량누계 - 전일대비주가하락일의 거래량누계

class CAppOBV : public CLineData  
{
public:
	CAppOBV(class COrgData *pOrgData, int dKey, int gKind, struct _chartinfo *pCInfo, int cOption);
	virtual ~CAppOBV();

protected:
	bool CalcuData(bool bShift = false, bool bLast = false);
};

#endif // !defined(AFX_APPOBV_H__5FA5E91E_15A3_4F7A_9BD7_ECC44793AE50__INCLUDED_)

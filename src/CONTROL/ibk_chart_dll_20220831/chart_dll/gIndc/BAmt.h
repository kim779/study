#if !defined(AFX_BAMT_H__F883B4FE_B017_45C7_84A6_EBC3330907B1__INCLUDED_)
#define AFX_BAMT_H__F883B4FE_B017_45C7_84A6_EBC3330907B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BongBase.h"

class CBAmt : public CBongBase  
{
public:
	CBAmt(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CBAmt();

	void	DrawGraph(CDC* pDC);
	void	DrawChart(CDC* pDC);
	bool	CalculateMinMax(bool bForce = false);
	bool	IsChangeMinMaxRTM(bool bShift = false, bool bIncrease = false);
	CString	GetDisplayPosData(CPoint pt, bool bGrid = true);
	CString	ExcelData(int idx);

	virtual	void	DrawLegend(CDC* pDC);
};

#endif // !defined(AFX_BAMT_H__F883B4FE_B017_45C7_84A6_EBC3330907B1__INCLUDED_)

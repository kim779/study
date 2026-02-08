#if !defined(AFX_BVOLUME_H__9A46BB43_9B45_41E2_ABBA_C26578D2F9C1__INCLUDED_)
#define AFX_BVOLUME_H__9A46BB43_9B45_41E2_ABBA_C26578D2F9C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BongBase.h"

class CBVolume : public CBongBase  
{
public:
	CBVolume(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CBVolume();

	void	DrawGraph(CDC* pDC);
	void	DrawChart(CDC* pDC);
	bool	CalculateMinMax(bool bForce = false);
	bool	IsChangeMinMaxRTM(bool bShift = false, bool bIncrease = false);
	CString	GetDisplayPosData(CPoint pt, bool bGrid = true);
	CString	ExcelData(int idx) { return ""; }

	virtual void	DrawLegend(CDC* pDC);
};

#endif // !defined(AFX_BVOLUME_H__9A46BB43_9B45_41E2_ABBA_C26578D2F9C1__INCLUDED_)

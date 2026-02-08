#pragma once

#include "Grp_Base.h"

class CGrp_Vol  : public CGrp_Base
{
public:
	CGrp_Vol(CWnd *pView, class CGrpWnd *pGrpWnd, char *pInfo);
	virtual ~CGrp_Vol();

	void	DrawGraph(CDC *pDC);
	void	DrawVolume(CDC *pDC);
	bool	CalculateMinMax();
	bool	IsChangeMinMax(bool bShift = false);
	CString	GetDisplayPosData(CPoint pt);
	void	DrawLegend(CDC* pDC);
	CString	GetExcelData(int idx);
};

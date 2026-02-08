#pragma once

#include "Grp_Base.h"

class CGrp_Line1 : public CGrp_Base  
{
public:
	CGrp_Line1(CWnd *pView, class CGrpWnd *pGrpWnd, char *pInfo);
	virtual ~CGrp_Line1();

	void	DrawGraph(CDC *pDC);
	bool	CalculateMinMax();
	bool	IsChangeMinMax(bool bShift = false);
	CString	GetDisplayPosData(CPoint pt);
	CString	Setdouble(CString data);

private:
	void	DrawLine(CDC *pDC);
	void	PDrawLine(CDC *pDC);
	void	DrawPrePrc(CDC *pDC);
	void	DrawLegend(CDC* pDC);
	void	DrawMinMaxDay(CDC* pDC);
	CString	GetExcelData(int idx);	

private:
	struct _cgIndex	m_MaxDayIndex, m_MinDayIndex;
	CPoint	m_MaxDayPos, m_MinDayPos;
	double	m_dispMaxVal, m_dispMinVal;

};
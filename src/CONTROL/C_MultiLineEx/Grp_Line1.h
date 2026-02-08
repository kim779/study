#pragma once

#include "Grp_Base.h"
#include <afxtempl.h>
class CGrp_Line1 : public CGrp_Base  
{
public:
	CGrp_Line1(CWnd *pView, class CGrpWnd *pGrpWnd, char *pInfo, char* dInfo);
	virtual ~CGrp_Line1();

	void	DrawGraph(CDC *pDC);
	bool	CalculateMinMax();
	bool	IsChangeMinMax(bool bShift = false);
	CString	GetDisplayPosData(CPoint pt);
	CString	GetExcelData(int idx, int idx1);

private:
	void	DrawLine(CDC *pDC);	
	void	DrawLegend(CDC* pDC);	
	void	DrawColorLine(CDC* pDC, CPoint ptTo, COLORREF color, UINT nStyle, int nWidth);
	COLORREF GetColorReverse(COLORREF color);

private:
	struct _cgIndex	m_MaxDayIndex, m_MinDayIndex;
	CPoint	m_MaxDayPos, m_MinDayPos;
	double	m_dispMaxVal, m_dispMinVal;
	CArray <COLORREF, COLORREF>	m_arColor;

};

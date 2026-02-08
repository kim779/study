// Grp_Juga.h: interface for the CGrp_Juga class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRP_JUGA_H__37D54FAF_5698_48CE_B0D8_2B26418676C2__INCLUDED_)
#define AFX_GRP_JUGA_H__37D54FAF_5698_48CE_B0D8_2B26418676C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Grp_Base.h"

class CGrp_Juga  : public CGrp_Base
{
public:
	CGrp_Juga(CWnd *pView, class CGrpWnd *pGrpWnd, char *pInfo);
	virtual ~CGrp_Juga();

	void	DrawGraph(CDC *pDC);
	bool	CalculateMinMax();
	bool	IsChangeMinMax(bool bShift = false);
	CString	GetDisplayPosData(CPoint pt);
	CString	GetExcelData(int idx);

private:
	void	DrawBong(CDC *pDC);
	void	DrawBar(CDC *pDC);
	void	DrawLine(CDC *pDC);
	void	DrawMinMaxDay(CDC* pDC);

private:
	struct _cgIndex	m_MaxDayIndex, m_MinDayIndex;
	CPoint	m_MaxDayPos, m_MinDayPos;
	double	m_dispMaxVal, m_dispMinVal;
	int	m_clrUp, m_clrDn;
};

#endif // !defined(AFX_GRP_JUGA_H__37D54FAF_5698_48CE_B0D8_2B26418676C2__INCLUDED_)

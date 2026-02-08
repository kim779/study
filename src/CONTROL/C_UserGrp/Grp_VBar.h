// Grp_VBar.h: interface for the CGrp_VBar class.
//
//////////////////////////////////////////////////////////////////////
#pragma once
#include "Grp_Base.h"

class CGrp_VBar : public CGrp_Base
{
public:
	CGrp_VBar(CWnd *pView, class CGrpWnd *pGrpWnd, char *pInfo);
	virtual ~CGrp_VBar();

	bool	CalculateMinMax();
	void	DrawName(CDC *pDC);
private:
	bool	m_bAllSameValue;	// all plus or all minus

	void	DrawChart(CDC* pDC);
	void	Draw2DBarChart(CDC *pDC);
	void	Draw3DBarChart(CDC *pDC);
	void	DrawXBaseBar(CDC *pDC);
};


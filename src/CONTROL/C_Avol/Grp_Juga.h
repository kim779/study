#pragma once

#include "Grp_Base.h"
#include <afxtempl.h>

class CGrp_Juga  : public CGrp_Base
{
public:
	CGrp_Juga(CWnd* pView, class CGrpWnd *pGrpWnd, char *pInfo);
	virtual ~CGrp_Juga();

	void	DrawGraph(CDC *pDC);
	bool	CalculateMinMax();
	CString	GetDisplayPosData(CPoint pt);
	CString	GetExcelData(int idx);

private:
	bool	CreateData();
	void	DrawBong(CDC *pDC);
	void	DrawLine(CDC *pDC);
	void	DrawBar(CDC *pDC);
	void	DrawMinMaxDay(CDC* pDC);
	void	DrawAvol(CDC* pDC);

private:
	struct _cgIndex	m_MaxDayIndex, m_MinDayIndex;
	CPoint	m_MaxDayPos, m_MinDayPos;
	double	m_dispMaxVal, m_dispMinVal;

// for maemul
	struct _local_data
	{
		double		percent;
		double		amount;
	};

	void	removeData();
	double		m_maxCURR;
	double		m_minCURR;
	double		m_fUnit;
	int		m_nCurr;
	int		m_nMaxIndex;
	double		m_MaxVolume;
	COLORREF	m_clrNORM;
	COLORREF	m_clrCURR;
	COLORREF	m_clrMAX;
	int		m_iDrawHeight;
	int		m_iDrawWidth;	
	bool		m_bCalculated;
	//CArray<struct _local_data*, struct _local_data*>	m_arData;
	std::vector<std::unique_ptr<struct _local_data>> _vData;


	int	getYPos(double val);		
	void	SetMember();
	void	drawChart(CDC* pDC);	
	void	FindMinMax();	
	void	DrawAvolTick(CDC* pDC, int tkInfo);
	bool	Draw3DBar(CDC* pDC, CRect rc, int Gap, COLORREF orgColor);
	void	Get3DRGB(COLORREF color[3]);
};

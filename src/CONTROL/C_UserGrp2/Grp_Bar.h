// Grp_Bar.h: interface for the CGrp_Bar class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "Grp_Base.h"
#include <afxtempl.h>

struct _3NInfo
{
	double	val[3]{};		// up/dn/eq
	int	top{};		// rect - top
	int	bottom{};		// rect - bottom
	double	dUpTick{};	// tick top value
	double	dDnTick{};	// tick bottom value
};

class CGrp_Bar : public CGrp_Base  
{
public:
	CGrp_Bar(CWnd *pView, class CGrpWnd *pGrpWnd, char *pInfo);
	virtual ~CGrp_Bar();

	void	DrawGraph(CDC *pDC);
	CString	GetDisplayPosHead(CPoint pt);
	CString	GetDisplayPosData(CPoint pt);
	CString	GetExcelHead(int idx);
	CString	GetExcelData(int idx);
	void	DrawTickBottom(CDC* pDC);
	void	DrawName(CDC *pDC);

	bool	CalculateMinMax();
	void	GetMinMaxValue(double& dMin, double& dMax);

private:
	int	m_pColor[3]{};		// 상승, 하락, 보합
	CString	m_pName[3];
	std::vector<std::unique_ptr<struct _3NInfo>>	m_arNInfo;

	//void	DrawChart(CDC* pDC);
	bool	Calculate2D1N();
	bool	Calculate2D3N();
	void	Draw2D1NBar(CDC *pDC);	// 1쌍의 체결 그래프
	void	Draw2D3NBar(CDC *pDC);	// 3쌍의 체결 그래프
	void	DrawTickHorizon2DNBar(CDC* pDC, double Max, double Min, int tickinfo);
	int	GetXPos(double val);
};
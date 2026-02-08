// Grp_Tick.h: interface for the CGrp_Tick class.
//
//////////////////////////////////////////////////////////////////////
#pragma once
#include "../../h/axiscgrp.h"

class CGrp_Tick  
{
public:
	CGrp_Tick(CWnd *pView, class CGrpWnd *pGrpWnd, char *pInfo);
	virtual ~CGrp_Tick();

public:
	void	DrawGraph(CDC *pDC);
	bool	CalculateMinMax();
	bool	IsChangeMinMax();
	CString	GetDisplayPosData(CPoint pt);
	void	Resize();

	void	DrawTickRight(CDC* pDC, double Max, double Min);
	void	DrawTickLeft(CDC* pDC, double Max, double Min);
	void	DrawTickBottom(CDC *pDC);
	void	FormatVariableComma(CString &dispTxt, double dVal, int point, int maxDigit = 10);
	int	GetTickCount(int dir);
	bool	calculateDisplay(double div, double Max, double Min, double& drawMinValue, 
			int dispTICK, double* dispValue, int& dispCount);
	void	GetMaxStr(CString &sMax);
	void	GetMinStr(CString &sMin);

private:
	void	DrawBong(CDC *pDC);
	void	DrawBar(CDC *pDC);
	void	DrawLine(CDC *pDC);
	void	DrawPrePrc(CDC *pDC);

private:
	CWnd		*m_pView;
	class CGrpWnd	*m_pGrpWnd;

	int	m_gKind;
	int	m_dKind;
	int	m_dIndex;

	CRect	m_DrawRect;
	COLORREF	m_tRGB;
	COLORREF	m_pRGB;

	double	m_Max;
	double	m_Min;

	struct _cgIndex	m_MaxDayIndex, m_MinDayIndex;
	CPoint	m_MaxDayPos, m_MinDayPos;
	double	m_dispMaxVal, m_dispMinVal;
	CC_TickApp	*m_pApp;

};

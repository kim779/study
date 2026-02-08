// Grp_Tick.h: interface for the CGrp_Tick class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "../../h/axiscgrp.h"

class CGrp_Tick  
{
public:
	CGrp_Tick(CWnd *pView, class CGrpWnd *pGrpWnd, int index, char *pInfo);
	virtual ~CGrp_Tick();

public:
	void	DrawGraph(CDC *pDC);
	bool	CalculateMinMax();
	bool	IsChangeMinMax();
	CString	GetDisplayPosData(CPoint pt);
	CString	GetExcelHead(int idx);
	CString	GetExcelData(int idx);
	void	Resize();

	void	DrawTickRight(CDC* pDC, double Max, double Min, int digit);
	void	DrawTickLeft(CDC* pDC, double Max, double Min, int digit);
	void	DrawTickBottom(CDC *pDC);
	void	FormatVariableComma(CString &dispTxt, double dVal, int point, int maxDigit = 10);
	int	GetTickCount(int dir);
	bool	calculateDisplay(double div, double Max, double Min, double& drawMinValue, 
			int dispTICK, double* dispValue, int& dispCount);
	void	GetMaxStr(CString &sMax);
	void	GetMinStr(CString &sMin);

private:
	void	DrawLine(CDC *pDC);
	void	DrawPrePrc(CDC *pDC);
	void	DrawVol(CDC *pDC);
	void	DrawName(CDC *pDC);

private:
	CWnd		*m_pView;
	class CGrpWnd	*m_pGrpWnd;

	int	m_gKind;
	int	m_dKind;
	int	m_dIndex;
	int	m_Index;

	CRect	m_DrawRect;
	CRect	m_orgDrawRect;
	COLORREF	m_tRGB;
	COLORREF	m_pRGB;

	double	m_Max;
	double	m_Min;

	CC_TickExApp	*m_pApp;

};

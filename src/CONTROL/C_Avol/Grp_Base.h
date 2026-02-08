#pragma once

#include "../../h/axiscgrp.h"

class CGrp_Base  
{
public:
	CGrp_Base(CWnd* pView, class CGrpWnd *pGrpWnd, char *pInfo);
	virtual ~CGrp_Base();

	struct _gInfo *	m_pGinfo;
	
public:
	CRect	DisplayRect(CDC *pDC, int idx, CString dispStr, CPoint dispPt);
	virtual void	DrawGraph(CDC *pDC) {}
	virtual bool	CalculateMinMax() { return false; }
	virtual bool	IsChangeMinMax(bool bShift = false);
	virtual void	GetMinMaxValue(double& dMin, double& dMax);
	virtual CString	GetDisplayPosHead(CPoint pt);
	virtual CString	GetDisplayPosData(CPoint pt)	{ return ""; }
	virtual CString	GetExcelHead(int idx);
	virtual CString	GetExcelData(int idx)	{ return ""; }
	void	Resize();

	void	DrawTickHorizon(CDC* pDC, double Max, double Min, int tickinfo);
	void	DrawTickBottom(CDC *pDC);
	void	FormatVariableComma(CString &dispTxt, double dVal, int point, int maxDigit = 10);
	int	GetTickCount(int dir);
	bool	calculateDisplay(double div, double Max, double Min, double& drawMinValue, 
			int dispTICK, double* dispValue, int& dispCount);

	int	GetRgnKey() { return m_rKey; }
	int	GetGrpKind() { return m_gKind; }	

protected:
	class CGrpWnd	*m_pGrpWnd;

	int	m_gKind;
	int	m_rKey;
	int	m_dKey;
	int	m_dKind;
	int	m_dIndex;

	CRect	m_RgnRect;
	CRect	m_DrawRect;	
	COLORREF	m_tRGB;
	COLORREF	m_pRGB;

	double	m_Max;
	double	m_Min;

	CC_AVolApp*	m_pApp;
	CWnd*		m_pView;
};

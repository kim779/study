// Grp_Base.h: interface for the CGrp_Base class.
//
#pragma once

#include "../../h/axiscgrp.h"
#define	VOLUNIT	1000000
class CGrp_Base  
{
public:
	CGrp_Base(CWnd *pView, class CGrpWnd *pGrpWnd, char *pInfo, char* dInfo);
	virtual ~CGrp_Base();
	
public:
	CRect DisplayRect(CDC *pDC, int idx, CString dispStr, CPoint dispPt);
	virtual void	DrawGraph(CDC *pDC) {}
	virtual bool	CalculateMinMax() { return false; }
	virtual bool	IsChangeMinMax(bool bShift = false) { return false; }
	virtual void	GetMinMaxValue(double& dMin, double& dMax);
	virtual CString	GetDisplayPosHead(CPoint pt);
	virtual CString	GetDisplayPosData(CPoint pt)	{ return ""; }
	virtual CString	GetExcelHead(int idx);
	virtual CString	GetExcelData(int idx, int idx1)	{ return ""; }
	void	Resize();

	void	DrawTickHorizon(CDC* pDC, double Max, double Min, int tickinfo);
	void	DrawTickBottom(CDC *pDC);
	int	DrawTickIndex(CDC* pDC, CString dispText, COLORREF tColor, bool bColor, bool bIndex = false, CPoint pt = CPoint(-1,-1));
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

	CRect	m_DrawRect;
	CRect	m_RgnRect;
	COLORREF	m_tRGB;
	COLORREF	m_pRGB;

	double	m_Max;
	double	m_Min;

	int	m_digit;
	int	m_index;
	int	m_indexInRGN;
	CString	m_szName;
	CString	m_szCode;
	CString m_szRsym;
	double	m_jgga;
	CC_MultiLineExApp*	m_pApp;
	CWnd*			m_pView;
};

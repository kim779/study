// GrpLine.h: interface for the CGrpLine class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

class CGrpLine  
{
public:
	CGrpLine(CWnd *pView, CWnd* pMainWnd, class CGrpWnd *pGrpWnd, int order);
	virtual ~CGrpLine();

public:
	void	DrawGraph(CDC *pDC);
	void	DrawLine(CDC *pDC);
	bool	CalculateMinMax();
	bool	IsChangeMinMax(bool bShift = false);
	void	GetMinMaxValue(double& dMin, double& dMax);
	CString	GetDisplayPosHead(CPoint pt);
	CString	GetDisplayPosData(CPoint pt);
	void	Resize();

	void	DrawZeroLine(CDC *pDC, double Max, double Min);
	void	DrawTickHorizon(CDC* pDC, double Max, double Min, int tickinfo);
	void	DrawTickBottom(CDC *pDC);
	CRect	DisplayRect(CDC *pDC, int idx, CString dispStr, CPoint dispPt);
	int	DrawTickIndex(CDC* pDC, CString dispText, COLORREF tColor, bool bColor, bool bIndex = false, CPoint pt = CPoint(-1,-1));
	void	FormatVariableComma(CString &dispTxt, double dVal, int point, int maxDigit = 10);
	int	GetTickCount(int dir);
	bool	calculateDisplay(double div, double Max, double Min, double& drawMinValue, 
			int dispTICK, double* dispValue, int& dispCount);

protected:
	class CGrpWnd	*m_pGrpWnd;

	int	m_Order;
	CRect	m_DrawRect;

	double	m_Max;
	double	m_Min;

	CCx_MiniChartApp	*m_pApp;
	CWnd		*m_pView;
	CWnd		*m_pMainWnd;
};
// Grp_Base.h: interface for the CGrp_Base class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

class CGrp_Base  
{
public:
	CGrp_Base(CWnd *pView, class CGrpWnd *pGrpWnd, char *pInfo);
	virtual ~CGrp_Base();
	
public:
	CRect DisplayRect(CDC *pDC, int idx, CString dispStr, CPoint dispPt);
	virtual void	DrawGraph(CDC *pDC);
	virtual void	DrawChart(CDC* pDC) {}
	virtual bool	CalculateMinMax();
	virtual bool	IsChangeMinMax(bool bShift = false);
	virtual void	GetMinMaxValue(double& dMin, double& dMax);
	virtual CString	GetDisplayPosHead(CPoint pt);
	virtual CString	GetDisplayPosData(CPoint pt);
	virtual CString	GetExcelHead(int idx);
	virtual CString	GetExcelData(int idx);
	virtual	void	DrawTickBottom(CDC* pDC);
	virtual	void	DrawName(CDC *pDC);
	void	Resize();

	void	DrawTickHorizon(CDC* pDC, double Max, double Min, int tickinfo);
	void	DrawDateBottom(CDC* pDC);
	void	DrawNameBottom(CDC* pDC);
	void	DrawVerticNameBottom(CDC* pDC);
	void	DrawVerticalNumber(CDC* pDC, CString dispStr, CRect drawRC);
	int	DrawTickIndex(CDC* pDC, CString dispText, COLORREF tColor);
	void	FormatVariableComma(CString &dispTxt, double dVal, int point, int maxDigit = 12);
	int	GetTickCount(int dir);
	bool	calculateDisplay(double div, double Max, double Min, double& drawMinValue, 
			int dispTICK, double* dispValue, int& dispCount);
	bool	Draw3DPole(CDC* pDC, CRect rc, int xGap, int yGap, COLORREF orgColor);
	bool	Draw3DRevPole(CDC* pDC, CRect rc, int xGap, int yGap, COLORREF orgColor);
	static int DrawVerticalString(CDC* pDC, CString dispStr, CRect drawRC, bool bDraw = true);
	void	Get3DRGB(COLORREF color[3]);
	void	SetBgColor(COLORREF pRGB)	{ m_pRGB = pRGB; }

	int	GetRgnKey() { return m_rKey; }
	int	GetGrpKind() { return m_gKind; }
	int	GetMinMaxIndex() { return m_mIndex; }

	bool m_bHide;	// original foreground color

protected:
	class CGrpWnd	*m_pGrpWnd;
	int		m_gKind;	// graph kind
	int		m_rKey;		// region key
	int		m_dKey;		// data key
	int		m_dIndex;	// 일주월분틱...
	int		m_mIndex;	// 최대최소 색인(0/1)
	int		m_lIndex;	// label 색인(0,1)
	int		m_TickInfo;	// 좌우 틱정보
	int		m_LineWidth;	// Line Graph Width
	int		m_LineStyle;	// Line Graph Style

	CRect		m_DrawRect;	// Chart's Drawing Rect
	CRect		m_RgnRect;	// Original Drawing Rect
	COLORREF	m_tRGB;		// foreground color
	COLORREF	m_pRGB;		// background color
	COLORREF	m_clrUp;
	COLORREF	m_clrDn;

	double		m_Max;
	double		m_Min;
	CC_UserGrpApp*	m_pApp;	
	CWnd*		m_pView;
};

#if !defined(AFX_VRCLOCK_H__56864B4E_6A91_4EEA_BED8_90625034EC1D__INCLUDED_)
#define AFX_VRCLOCK_H__56864B4E_6A91_4EEA_BED8_90625034EC1D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VarsBase.h"

class CVRClock : public CVarsBase  
{
public:
	CVRClock(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CVRClock();
	
	CString	GetXposValue(CPoint pt);
	void	DrawGraph(CDC* pDC);
	void	DrawChart(CDC* pDC);
	bool	CalculateMinMax(bool bForce = false);
	bool	IsChangeMinMaxRTM(bool bShift = false, bool bIncrease = false);
	CString	GetDisplayPosHeader(CPoint pt, bool bGrid = true);
	CString GetDisplayPosData(CPoint pt, bool bGrid = true);
	void	SetMember();
	bool	CreateData();
	int	GetYPos(double val);
	int	GetXPos(double val);
	void	FindMinMax(double &Min, double &Max, double *pArray, int Size);
	void	ReviseHorizon(double& Max, double& Min);
private:

	bool	CalculateDisplay(double &div, double Max, double Min, double& drawMinValue, int dispTICK,
				double* dispValue, double* logValue, int& dispCount);
	void DrawBottomTick(CDC *pDC);
	CString GetRClockUnit(double Max);
	void FormatRClockComma(CString &dispTxt, double dVal, double Max, int maxDigit = 7);
	COLORREF m_pLineColor[5];
	int	m_pLineStyle[5];
	int	m_pLineWidth[5];

	int	m_UnitSize;		// n value
	double	*m_pRClockPrice;	// Price data array
	double	*m_pRClockVolume;	// Volume Data Array
	int	m_StartInx;		// Skipped Data Count
	bool	m_bCalculated;
	int	m_iDrawHeight, m_iDrawWidth;	//Drawing Height,Width
};

#endif // !defined(AFX_VRCLOCK_H__56864B4E_6A91_4EEA_BED8_90625034EC1D__INCLUDED_)

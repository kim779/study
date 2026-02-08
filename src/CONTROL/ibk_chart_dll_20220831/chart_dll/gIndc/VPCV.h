#if !defined(AFX_VPCV_H__798217B4_4C0E_492F_91A9_40B7D8F2378B__INCLUDED_)
#define AFX_VPCV_H__798217B4_4C0E_492F_91A9_40B7D8F2378B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VarsBase.h"

class CVPCV : public CVarsBase  
{
public:
	CVPCV(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CVPCV();

	CString	GetXposValue(CPoint pt);
	void	DrawGraph(CDC* pDC);
	void	DrawChart(CDC* pDC);
	bool	CalculateMinMax(bool bForce = false);
	bool	IsChangeMinMaxRTM(bool bShift = false, bool bIncrease = false);
	CString	GetDisplayPosHeader(CPoint pt, bool bGrid = true);
	CString GetDisplayPosData(CPoint pt, bool bGrid = true);
	bool	CreateData();
	void	SetMember();
	int	GetYPos(double val);
	int	GetXPos(double val);
	void	FindMinMax(double &Min, double &Max, double *pArray, int Size);

private:

	bool	CalculateDisplay(double &div, double Max, double Min, double& drawMinValue, int dispTICK,
				double* dispValue, double* logValue, int& dispCount);
	void DrawBottomTick(CDC *pDC);

	void FormatPCVComma(CString &dispTxt, double dVal, double Max, int maxDigit);
	CString GetPCVUnit(double Max);
	
	COLORREF m_pLineColor[5];
	int	m_pLineStyle[5];
	int	m_pLineWidth[5];

	int	m_UnitSize;	// n value
	double	*m_pPVData;	// Data Array
	double	*m_pVol;	// Data Array
	int	m_StartInx;	// Skipped Data Count
	int	m_iDrawHeight, m_iDrawWidth;	//Drawing Height,Width
	void	FindMinMax(double *pArray = NULL, int Size = 0);	
private:
	bool	m_bCalculated;
};

#endif // !defined(AFX_VPCV_H__798217B4_4C0E_492F_91A9_40B7D8F2378B__INCLUDED_)

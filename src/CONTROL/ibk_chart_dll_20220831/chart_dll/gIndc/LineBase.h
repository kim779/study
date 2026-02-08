#if !defined(AFX_LINEBASE_H__CBDDBDBA_1B0F_45C5_B656_C8C35D3820F1__INCLUDED_)
#define AFX_LINEBASE_H__CBDDBDBA_1B0F_45C5_B656_C8C35D3820F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IndcBase.h"

class CLineBase : public CIndcBase  
{
public:
	CLineBase(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CLineBase();

protected:
	enum {oneSOLID = 0, twoSOLID = 1, oneDOT = 2, twoDOT = 3};
	//** 2005.11.17 macho
	CString m_strDspUnit;

protected:

	CString	GetYposValue(CPoint pt, bool bGrid);
	CString	GetXposValue(CPoint pt);

	void	DrawLine(CDC* pDC, double dMax, double dMin, double adPtValue[], int iStyle, int iWidth, COLORREF color, int iIndex, int iOverFillTickIndex = -1);
	virtual void DrawTickRight(CDC* pDC, double dMax, double dMin);
	virtual void DrawTickLeft(CDC* pDC, double dMax, double dMin);
	virtual bool DrawUserDefinedTick(CDC* pDC, double dMaxValue, double dMinValue);
	bool	CalculateDisplay(double &div, double Max, double Min, double& drawMinValue, int dispTICK,
				double* dispValue, double* logValue, int& dispCount);
	virtual void FormatVariableComma(CString &dispTxt, double dVal, double Max, int maxDigit=10)
				{	FormatSubComma(dispTxt, dVal, m_dYMin, Max, maxDigit);}
	void FormatSubComma(CString &dispTxt, double dVal, double Min, double Max, int maxDigit);
	virtual void DrawTick(CDC* pDC);
	virtual void DrawGraph(CDC* pDC);
	virtual bool CalculateMinMax(bool bForce = false);
	virtual bool IsChangeMinMaxRTM(bool bShift = false, bool bIncrease = false);
	virtual void DrawAid(CDC* pDC);	
	virtual CString MakeLegend(int nIndex) { return _T(""); }
	virtual void MakeTitle(CStringArray& arTitle) {}
	virtual	CString	ExcelData(int idx);
	virtual CString GetEachUnit(double Min, double Max);

	int	m_aiRight[3];
	int	m_iOrgDataKey;
	int	m_iOverBuyY;
	int	m_iOverSellY;

public:
	virtual CString GetDisplayPosData(CPoint pt, bool bGrid = true);	
};

#endif // !defined(AFX_LINEBASE_H__CBDDBDBA_1B0F_45C5_B656_C8C35D3820F1__INCLUDED_)

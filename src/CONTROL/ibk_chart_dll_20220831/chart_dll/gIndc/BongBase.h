// BongBase.h: interface for the CBongBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BONGBASE_H__AD8EAE27_78F2_4163_8B3B_ECC82F7AC6B6__INCLUDED_)
#define AFX_BONGBASE_H__AD8EAE27_78F2_4163_8B3B_ECC82F7AC6B6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IndcBase.h"

class CBongBase : public CIndcBase  
{
public:
	CBongBase(CWnd* pwndView, CWnd* pwndParent, class CDataMgr* pDataMgr, char* pcInfo, char* pcGraph, int iDispDay);
	virtual ~CBongBase();
	
protected:
	int	m_iDispMaxVal;
	int	m_iDispMinVal;
	double	m_dMaxVolValue;
	double	m_dMinVolValue;
	double	m_dMaxVolMav;
	double	m_dMinVolMav;

	CPoint	m_ptMaxDayPos;
	CPoint	m_ptMinDayPos;
	struct _gIndex	m_gindexMaxDay;
	struct _gIndex	m_gindexMinDay;

	int*	m_piVol;
	bool	m_bEqui;
	double	m_dTotal;
	double	m_dUnit;
//	int	m_nLine;

protected:
	CString	GetYposValue(CPoint pt, bool bGrid);
	double	GetYposOrgValue(CPoint pt);
	virtual	CString	GetXposValue(CPoint pt);
	virtual	CPoint	GetAdjustPoint(CPoint pt, CString &sPosVal);
	virtual	CPoint	GetOrderPoint(CPoint pt, CString &sPosVal);

	virtual void DrawGraph(CDC* pDC) { }
	virtual bool CalculateMinMax(bool bForce = false);
	virtual bool IsChangeMinMaxRTM(bool bShift = false, bool bIncrease = false);
	virtual	CString	ExcelData(int idx) { return ""; }
	virtual void SetGrpInfo(struct _graph* pGrp, struct _envInfo* pInfo, struct _CoDraw* pCoDraw);

	virtual void DrawTickRight(CDC* pDC, double dMax, double dMin, bool bDrawMin = true);
	virtual void DrawTickLeft(CDC* pDC, double dMax, double dMin, bool bDrawMin = true);
	void	DrawTickVertical(CDC *pDC);
	bool	CalculateDisplay(double &rdDiv, double dMax, double dMin, double& rdDrawMinValue, 
				int iDispTick, double* pdDispValue, double* pdLogValue, int& riDispCount);
	virtual void DrawTick(CDC* pDC);
	void	DrawLine(CDC *pDC, double Max, double Min, double pValue[], 
		int style, int width, COLORREF color, int index);
	bool	CalculateVolMinMax();
	bool	CalculateVol();
	int	rangeValue(double trn);
	virtual void	GetPtToEndvalPt(CPoint pt, CPoint &ept);
	double	GetOrderPrice(class COrgData *pOrgData, CPoint pt);
};

#endif // !defined(AFX_BONGBASE_H__AD8EAE27_78F2_4163_8B3B_ECC82F7AC6B6__INCLUDED_)

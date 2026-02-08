#if !defined(AFX_BJUGACHART_H__37AD8BEE_6618_4A50_AF26_5ED94B831F61__INCLUDED_)
#define AFX_BJUGACHART_H__37AD8BEE_6618_4A50_AF26_5ED94B831F61__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BongBase.h"

// GK_JPN, GK_BAR, GK_LIN, GK_POLE, GK_CDV, GK_EQV
class CBJugaChart : public CBongBase  
{
public:
	CBJugaChart(CWnd* pwndView, CWnd* pwndParent, class CDataMgr* pDataMgr, char* pcInfo, char* pcGraph, int iDispDay);
	virtual ~CBJugaChart();

	void	DrawGraph(CDC* pDC);
	void	DrawBong(CDC* pDC);
	void	DrawBongJPN(CDC* pDC);
	void	DrawBongUSA(CDC* pDC);
	void	DrawBongLIN(CDC* pDC);
	void	DrawBongPole(CDC* pDC);
	void	DrawMinMaxDay(CDC* pDC);
	bool	CalculateMinMax(bool bForce = false);
	bool	IsChangeMinMaxRTM(bool bShift = false, bool bIncrease = false);
	int	GetDisplayPosIndex(CPoint pt);
	CString	GetDisplayPosHeader(CPoint pt, bool bGrid = true);
	CString GetDisplayPosData(CPoint pt, bool bGrid = true);

	virtual	CString	ExcelData(int idx) { return ""; }

	virtual void	DrawLegend(CDC* pDC);
};

#endif // !defined(AFX_BJUGACHART_H__37AD8BEE_6618_4A50_AF26_5ED94B831F61__INCLUDED_)

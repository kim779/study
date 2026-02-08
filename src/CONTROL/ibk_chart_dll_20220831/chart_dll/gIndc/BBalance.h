#if !defined(AFX_BBALANCE_H__CCD0245F_F003_4D60_874A_B7720B73ED03__INCLUDED_)
#define AFX_BBALANCE_H__CCD0245F_F003_4D60_874A_B7720B73ED03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BongBase.h"

// 일목균형표
/*
	1. 전환선 = (최근 9일간의 최고치 + 최저치) / 2
	2. 기준선 = (최근 26일간의 최고치 + 최저치) / 2
	3. 후행스팬 = 당일의 주가를 26일 전으로 후행시킨 선 
	4. 선행스팬1 = (기준선 + 전환선) / 2 -> 26일 선행
	5. 선행스팬2 = (52일간의 최고치 + 최저치) / 2 -> 26일 선행
*/


class CBBalance : public CBongBase  
{
public:
	CBBalance(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CBBalance();

protected:
	CString	GetXposValue(CPoint pt);

	void	DrawGraph(CDC* pDC);
	void	DrawChart(CDC* pDC);
	bool	CalculateMinMax(bool bForce = false);
	bool	IsChangeMinMaxRTM(bool bShift = false, bool bIncrease = false);
	CString GetDisplayPosData(CPoint pt, bool bGrid = true);
	CString	ExcelData(int idx);
	int	GetValue() { return m_UnitSize4; }
	bool	CalculateLast(bool bShift);
	

private:
	void FindMinMax();
	bool CreateData();
	void RemoveData();
	int GetYPos(double val);
	void SetMember();

	int	m_UnitSize1, m_UnitSize2;	// 기준,전환
	int	m_UnitSize3, m_UnitSize4, m_UnitSize5;	// 후행,선행1,선행2
	int	m_pStyle[5], m_pWidth[5];
	double	*m_Switch;
	double	*m_Pivot;
	double	*m_BSpan, *m_PSpan1, *m_PSpan2;
	struct	_line	m_line[7];
	bool	m_bCalculated;
};

#endif // !defined(AFX_BBALANCE_H__CCD0245F_F003_4D60_874A_B7720B73ED03__INCLUDED_)

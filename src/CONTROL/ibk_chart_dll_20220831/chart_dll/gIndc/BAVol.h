// BAVol.h: interface for the CBAVol class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BAVOL_H__E839575A_FA70_4ADC_98F2_56AF3F500530__INCLUDED_)
#define AFX_BAVOL_H__E839575A_FA70_4ADC_98F2_56AF3F500530__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BongBase.h"

#define POLL_NORMAL	0x00
#define POLL_CURR	0x01
#define POLL_MAX	0x02

#include <afxtempl.h>

class CBAVol : public CBongBase  
{
public:
	CBAVol(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CBAVol();

public:
	struct _local_data
	{
		double		percent;
		double		amount;
	};

private:
	double		m_dMmaxCurr;
	double		m_dMinCurr;
	double		m_dUnit;
	double		m_dVMax;
	int		m_iPoll;
	int		m_iCurr;
	int		m_iMax;
	int		m_iDrawHeight;
	int		m_iDrawWidth;	
	int		m_iViewType;	// 0 : 누적량, 1 : 퍼센트, 2 : 모두
	
	COLORREF	m_rgbNorm;
	COLORREF	m_rgbCurr;
	COLORREF	m_rgbMax;

	CArray<struct _local_data*, struct _local_data*>	m_arData;

	
protected:
	void	DrawGraph(CDC* pDC);
	bool	CalculateMinMax(bool bForce = false);
	bool	IsChangeMinMaxRTM(bool bShift = false, bool bIncrease = false);
	void	RemoveData();

	void	DrawTick(CDC* pDC);

private:
	int	getYPos(double val);
	bool	CreateData();		
	void	SetMember();
	void	DrawChart(CDC* pDC);
	void	FindMinMax();	
	void	DrawHorizontalTick(CDC* pDC, int tkInfo);
	bool	Draw3DBar(CDC* pDC, CRect rc, int Gap, COLORREF orgColor);
	void	Get3DRGB(COLORREF color[3]);


};

#endif // !defined(AFX_BAVOL_H__E839575A_FA70_4ADC_98F2_56AF3F500530__INCLUDED_)

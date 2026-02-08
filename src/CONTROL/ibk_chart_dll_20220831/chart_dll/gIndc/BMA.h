// BMA.h: interface for the CBMA class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BMA_H__1C37C897_F7FF_4927_9F05_BB8DB6FB3A6B__INCLUDED_)
#define AFX_BMA_H__1C37C897_F7FF_4927_9F05_BB8DB6FB3A6B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BongBase.h"

class CBMA : public CBongBase  
{
public:
	CBMA(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CBMA();
protected:
	int	m_dayMAV;
	int	m_gap; 
	int	m_netCnt;

protected:
	void	DrawGraph(CDC *pDC);
	void	DrawNET(CDC *pDC);
	void	DrawMA(CDC *pDC);
	CString	GetDisplayPosData(CPoint pt, bool bGrid = true);
	CString	ExcelData(int idx);
	void	SetGrpInfo(struct _graph *pGrp, struct _envInfo *pInfo, struct _CoDraw *pCoDraw);

	virtual void	DrawLegend(CDC* pDC);


};

#endif // !defined(AFX_BMA_H__1C37C897_F7FF_4927_9F05_BB8DB6FB3A6B__INCLUDED_)

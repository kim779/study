#if !defined(AFX_BDEMARK_H__48F9D9B6_E68F_4C2D_B2D4_A3312C1A5E29__INCLUDED_)
#define AFX_BDEMARK_H__48F9D9B6_E68F_4C2D_B2D4_A3312C1A5E29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BongBase.h"

class CBDemark : public CBongBase  
{
public:
	CBDemark(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CBDemark();

protected:
	void	DrawGraph(CDC* pDC);
	void	drawChart(CDC* pDC);
	CString GetDisplayPosData(CPoint pt, bool bGrid = true);
	CString	ExcelData(int idx);
};

#endif // !defined(AFX_BDEMARK_H__48F9D9B6_E68F_4C2D_B2D4_A3312C1A5E29__INCLUDED_)

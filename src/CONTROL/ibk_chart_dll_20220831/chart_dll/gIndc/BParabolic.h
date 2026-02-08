#if !defined(AFX_BPARABOLIC_H__2262EF05_F17D_413D_9439_B5A0FD17702E__INCLUDED_)
#define AFX_BPARABOLIC_H__2262EF05_F17D_413D_9439_B5A0FD17702E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BongBase.h"

class CBParabolic : public CBongBase  
{
public:
	CBParabolic(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CBParabolic();

	void	DrawGraph(CDC* pDC);
	void	drawChart(CDC* pDC);
	CString GetDisplayPosData(CPoint pt, bool bGrid = true);
	CString	ExcelData(int idx);
};

#endif // !defined(AFX_BPARABOLIC_H__2262EF05_F17D_413D_9439_B5A0FD17702E__INCLUDED_)

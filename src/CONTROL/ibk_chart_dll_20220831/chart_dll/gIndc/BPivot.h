#if !defined(AFX_BPIVOT_H__894D5361_47BF_4186_B6FC_35552BD400B3__INCLUDED_)
#define AFX_BPIVOT_H__894D5361_47BF_4186_B6FC_35552BD400B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BongBase.h"

class CBPivot : public CBongBase  
{
public:
	CBPivot(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CBPivot();
	
protected:
	void DrawGraph(CDC* pDC);
	void drawChart(CDC* pDC);
	CString GetDisplayPosData(CPoint pt, bool bGrid = true);
	CString	ExcelData(int idx);
};

#endif // !defined(AFX_BPIVOT_H__894D5361_47BF_4186_B6FC_35552BD400B3__INCLUDED_)

#if !defined(AFX_BBOLLINGER_H__B8D8FBD7_3570_4503_B1E5_F392EF485306__INCLUDED_)
#define AFX_BBOLLINGER_H__B8D8FBD7_3570_4503_B1E5_F392EF485306__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BongBase.h"

class CBBollinger : public CBongBase  
{
public:
	CBBollinger(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CBBollinger();

protected:
	void DrawGraph(CDC* pDC);
	void drawChart(CDC* pDC);
	CString GetDisplayPosData(CPoint pt, bool bGrid = true);
	CString	ExcelData(int idx);
};

#endif // !defined(AFX_BBOLLINGER_H__B8D8FBD7_3570_4503_B1E5_F392EF485306__INCLUDED_)

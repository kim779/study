// BEnvelope.h: interface for the CBEnvelope class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BENVELOPE_H__B3064D7A_5A94_497E_8D73_E4AAA4CD1F32__INCLUDED_)
#define AFX_BENVELOPE_H__B3064D7A_5A94_497E_8D73_E4AAA4CD1F32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BongBase.h"

class CBEnvelope : public CBongBase  
{
public:
	CBEnvelope(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CBEnvelope();

protected:
	void DrawGraph(CDC* pDC);
	void drawChart(CDC* pDC);
	CString GetDisplayPosData(CPoint pt, bool bGrid = true);
	CString	ExcelData(int idx);
};

#endif // !defined(AFX_BENVELOPE_H__B3064D7A_5A94_497E_8D73_E4AAA4CD1F32__INCLUDED_)

#if !defined(AFX_LTRIX_H__FBE91268_22AB_478D_8033_A2A66F481698__INCLUDED_)
#define AFX_LTRIX_H__FBE91268_22AB_478D_8033_A2A66F481698__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineBase.h"

class CLTrix : public CLineBase  
{
public:
	CLTrix(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CLTrix();

protected:	
	bool CalculateMinMax(bool bForce = false);
	CString MakeLegend(int nIndex);
	void MakeTitle(CStringArray& arTitle);	
	void FormatVariableComma(CString &dispTxt, double dVal, double Max, int maxDigit);
	void DrawTickLeft(CDC* pDC, double Max, double Min);
	void DrawTickRight(CDC* pDC, double Max, double Min);
	CString GetDisplayPosData(CPoint pt, bool bGrid = true);
	CString ExcelData(int idx);
};

#endif // !defined(AFX_LTRIX_H__FBE91268_22AB_478D_8033_A2A66F481698__INCLUDED_)

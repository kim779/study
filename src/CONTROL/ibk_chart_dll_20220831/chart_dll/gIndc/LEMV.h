#if !defined(AFX_LEMV_H__2DD6300C_AF83_40D2_8917_30660393EC8F__INCLUDED_)
#define AFX_LEMV_H__2DD6300C_AF83_40D2_8917_30660393EC8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineBase.h"

class CLEMV : public CLineBase  
{
public:
	CLEMV(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CLEMV();

protected:	
	CString MakeLegend(int nIndex);
	void MakeTitle(CStringArray& arTitle);
	void FormatVariableComma(CString &dispTxt, double dVal, double Max, int maxDigit);
	void DrawTickLeft(CDC* pDC, double Max, double Min);
	void DrawTickRight(CDC* pDC, double Max, double Min);
	CString GetDisplayPosData(CPoint pt, bool bGrid = true);
	CString ExcelData(int idx);
};

#endif // !defined(AFX_LEMV_H__2DD6300C_AF83_40D2_8917_30660393EC8F__INCLUDED_)

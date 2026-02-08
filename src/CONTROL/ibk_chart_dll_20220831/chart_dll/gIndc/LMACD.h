#if !defined(AFX_LMACD_H__A539B9F6_D1C8_4FF1_A85A_9BCD1C1E0FC6__INCLUDED_)
#define AFX_LMACD_H__A539B9F6_D1C8_4FF1_A85A_9BCD1C1E0FC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineBase.h"

class CLMACD : public CLineBase  
{
public:
	CLMACD(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CLMACD();
	
protected:	
	CString MakeLegend(int nIndex);
	void MakeTitle(CStringArray& arTitle);	
	void DrawAid(CDC* pDC);
	void DrawOsc(CDC *pDC, double Max, double Min, double pValue[], int index);

};

#endif // !defined(AFX_LMACD_H__A539B9F6_D1C8_4FF1_A85A_9BCD1C1E0FC6__INCLUDED_)

// LForeignMM.h: interface for the CLForeignMM class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LFOREIGNMM_H__CB32A1A3_D232_4B8B_8311_336F06A56C3A__INCLUDED_)
#define AFX_LFOREIGNMM_H__CB32A1A3_D232_4B8B_8311_336F06A56C3A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineBase.h"

class CLForeignMM : public CLineBase  
{
public:
	CLForeignMM(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CLForeignMM();

protected:
	CString m_title;
	CString MakeLegend(int nIndex);
	void MakeTitle(CStringArray& arTitle);
	CString GetDisplayPosData(CPoint pt, bool bGrid = true);

	void DrawAid(CDC* pDC);
	void DrawOsc(CDC *pDC, double Max, double Min, double pValue[], int index);
};

#endif // !defined(AFX_LFOREIGNMM_H__CB32A1A3_D232_4B8B_8311_336F06A56C3A__INCLUDED_)

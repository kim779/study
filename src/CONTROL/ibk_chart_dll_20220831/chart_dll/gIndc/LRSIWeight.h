// LRSIWeight.h: interface for the CLRSIWeight class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LRSIWEIGHT_H__552E8D8F_3415_4734_86AF_E9CDCC5AEC37__INCLUDED_)
#define AFX_LRSIWEIGHT_H__552E8D8F_3415_4734_86AF_E9CDCC5AEC37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineBase.h"

class CLRSIWeight : public CLineBase  
{
public:
	CLRSIWeight(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CLRSIWeight();

protected:
	void MakeTitle(CStringArray& arTitle);
	CString MakeLegend(int nIndex);
};

#endif // !defined(AFX_LRSIWEIGHT_H__552E8D8F_3415_4734_86AF_E9CDCC5AEC37__INCLUDED_)

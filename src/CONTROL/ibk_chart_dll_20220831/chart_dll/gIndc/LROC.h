#if !defined(AFX_LROC_H__ACDBD110_6A87_4FD2_A931_339129250E72__INCLUDED_)
#define AFX_LROC_H__ACDBD110_6A87_4FD2_A931_339129250E72__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineBase.h"

class CLROC : public CLineBase  
{
public:
	CLROC(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CLROC();

protected:	
	CString MakeLegend(int nIndex);
	void MakeTitle(CStringArray& arTitle);
};

#endif // !defined(AFX_LROC_H__ACDBD110_6A87_4FD2_A931_339129250E72__INCLUDED_)

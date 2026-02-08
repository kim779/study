#if !defined(AFX_LOBV_H__1DAE0F1F_7952_442B_9DEF_5D61E9D1CDEC__INCLUDED_)
#define AFX_LOBV_H__1DAE0F1F_7952_442B_9DEF_5D61E9D1CDEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineBase.h"

class CLOBV : public CLineBase  
{
public:
	CLOBV(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CLOBV();
	
protected:	
	CString MakeLegend(int nIndex);
	void MakeTitle(CStringArray& arTitle);	
};

#endif // !defined(AFX_LOBV_H__1DAE0F1F_7952_442B_9DEF_5D61E9D1CDEC__INCLUDED_)

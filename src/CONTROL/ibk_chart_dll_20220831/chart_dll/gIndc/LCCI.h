#if !defined(AFX_LCCI_H__D69A0072_9BF4_4CF2_A074_71B995B4F847__INCLUDED_)
#define AFX_LCCI_H__D69A0072_9BF4_4CF2_A074_71B995B4F847__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineBase.h"

class CLCCI : public CLineBase  
{
public:
	CLCCI(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CLCCI();
	
protected:
	CString MakeLegend(int nIndex);
	void MakeTitle(CStringArray& arTitle);

};

#endif // !defined(AFX_LCCI_H__D69A0072_9BF4_4CF2_A074_71B995B4F847__INCLUDED_)

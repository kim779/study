#if !defined(AFX_LRATE_H__D69CA73F_AEA5_4EB7_A9F4_41E3782CF1D6__INCLUDED_)
#define AFX_LRATE_H__D69CA73F_AEA5_4EB7_A9F4_41E3782CF1D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineBase.h"

class CLRate : public CLineBase  
{
public:
	CLRate(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CLRate();
	
protected:	
	CString MakeLegend(int nIndex);
	void MakeTitle(CStringArray& arTitle);

};

#endif // !defined(AFX_LRATE_H__D69CA73F_AEA5_4EB7_A9F4_41E3782CF1D6__INCLUDED_)

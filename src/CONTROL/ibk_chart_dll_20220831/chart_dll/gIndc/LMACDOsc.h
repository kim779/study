#if !defined(AFX_LMACDOSC_H__068939AB_0AE4_4941_A009_B299AEFBC84D__INCLUDED_)
#define AFX_LMACDOSC_H__068939AB_0AE4_4941_A009_B299AEFBC84D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineBase.h"

class CLMACDOsc : public CLineBase  
{
public:
	CLMACDOsc(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CLMACDOsc();
	
protected:	
	CString MakeLegend(int nIndex);
	void MakeTitle(CStringArray& arTitle);
};

#endif // !defined(AFX_LMACDOSC_H__068939AB_0AE4_4941_A009_B299AEFBC84D__INCLUDED_)

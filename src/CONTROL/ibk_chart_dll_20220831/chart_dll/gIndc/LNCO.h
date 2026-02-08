#if !defined(AFX_LNCO_H__6D1467D1_920F_4260_A23B_BA8B375FC081__INCLUDED_)
#define AFX_LNCO_H__6D1467D1_920F_4260_A23B_BA8B375FC081__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineBase.h"

class CLNCO : public CLineBase  
{
public:
	CLNCO(CWnd *pView, CWnd *pParent, class CDataMgr *pDataMgr, char *pInfo, char *graph, int dispDay);
	virtual ~CLNCO();
	
protected:	
	CString MakeLegend(int nIndex);
	void MakeTitle(CStringArray& arTitle);

};

#endif // !defined(AFX_LNCO_H__6D1467D1_920F_4260_A23B_BA8B375FC081__INCLUDED_)
